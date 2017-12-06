/// proj2.c
/// Second project to IOS course, 30.3.2017
/// Filip Kocica, xkocic01 AT fit.vutbr.cz

#include "proj2.h"

int main(int argc, char *argv[])
{
  // try to open output file for reading
  if ( (out = fopen("proj2.out", "w")) == NULL )
  {
    // if error has occured, print error on srderr
    fprintf(stderr, "ERROR: Cannot open file proj2.out\n");
    return INPUT_ERR_CODE;
  }

  // struct with input data
  struct params p;

  // handle signals
	signal(SIGTERM, kill_prg);
	signal(SIGINT,  kill_prg);

  // read input data
  if ( input_handler(argc, argv, &p) )
  {
    fprintf(stderr, "ERROR: Incorrect input.\n");
    return INPUT_ERR_CODE;
  }
  // set semaphores and shared memory
  else if ( set_resources() )
  {
    fprintf(stderr, "ERROR: Cannot set resources.\n");
    return RESRC_ERR_CODE;
  }

  // set buffer for right output and srand for random generator
  setbuf(stdout, NULL);
  srand(time(0));

  // set variables
  *child_p_no = 1;
  *adult_p_no = 1;
  *step_counter = 1;
  *a_left = p.A;
  *leave = *end = *end2 = 0;

  // parent processes buffers
  int adults_pid[ p.A ];
  int childs_pid[ p.C ];

  pid_t processPid = fork();

  if (processPid == 0) // child
  {
    int pid_child;

    // while we havent all childs
    for (int i = 0; i < p.C; i++)
    {
      // sleep for random time from <0, p.CGT>
      if( p.CGT )
        usleep( (random() % (p.CGT)) * 1000 );

      pid_child = fork();
      if ( !pid_child )
        child(&p);
      else if ( pid_child > 0 )
      {
        // save parent pid
        childs_pid[i] = pid_child;
      }
      else
      {
        // fork error
        fprintf(stderr, "ERROR: In function fork(): Process duplication failed.\n");
        free_resources();
        return SYS_ERR_CODE;
      }
    }

    return 0;
  }
  else if (processPid > 0) //parent
  {
    processId = processPid;
    int pid_adult;

    // while we havent all adults
    for (int i = 0; i < p.A; i++)
    {
      // sleep for random time from <0, p.AGT>
      if( p.AGT )
        usleep( (random() % (p.AGT)) * 1000 );

      pid_adult = fork();
      if ( !pid_adult )
        adult(&p);
      else if ( pid_adult > 0 )
      {
        // save parents pid
        adults_pid[i] = pid_adult;
      }
      else
      {
        // fork error
        fprintf(stderr, "ERROR: In function fork(): Process duplication failed.\n");
        free_resources();
        return SYS_ERR_CODE;
      }
    }
  }
  else  // fork error
  {
    fprintf(stderr, "ERROR: In function fork(): Process duplication failed.\n");
    return SYS_ERR_CODE;
  }

  // wair for child processes
  for (int i = 0; i < p.C; i++)
    waitpid(childs_pid[i], NULL, 0);

  // wait for adult processes
  for (int i = 0; i < p.A; i++)
    waitpid(adults_pid[i], NULL, 0);

  // wait for main procces child
  waitpid(processId, NULL, 0);

  // free semaphores and shared mem
  free_resources();

  // return success
  return 0;
}



void child(struct params *p)
{
  // get child process number
  int child_local_num = (*child_p_no)++;

  sem_wait(sem_counter);
  fprintf(out,"%-6d: C %-3d: started\n", (*step_counter)++, child_local_num);
  fflush(out);
  sem_post(sem_counter);

  // entace condition and deadlock detection
  if ( ((*a_in_center) * CHILDS_ONE_ADULT) <= (*c_in_center) && !(*deadlock) )
  {
    // if there are no more adults
    if ( !(*a_left) )
    {
      *deadlock = 1;
    }
    else
    {
      sem_wait(sem_counter);
      fprintf(out,"%-6d: C %-3d: waiting: %d: %d\n", (*step_counter)++, child_local_num,
             *a_in_center, *c_in_center);
      fflush(out);
      sem_post(sem_counter);

      // wait for adult
      sem_wait(sem_a);
      while ( ((*a_in_center) * CHILDS_ONE_ADULT) <= (*c_in_center) && !(*deadlock) )
      {
        sem_wait(sem_a);
      }
    }
  }

  // if child process wanna enter between trying to leave and leave of adult process
  if ( *leave )
  {
    (*leave)++;
    // wait till adult process leaves
    sem_wait(sem_leave);
  }

  sem_wait(sem_counter);
  fprintf(out,"%-6d: C %-3d: enter\n", (*step_counter)++, child_local_num);
  fflush(out);
  sem_post(sem_counter);

  // child enters
  sem_wait(sem_c_leave);
  (*c_in_center)++;
  sem_post(sem_c_leave);
  // sleep for random time from interval
  if( p->CWT )
    usleep( (random() % (p->CWT)) * 1000 );

  sem_wait(sem_counter);
  fprintf(out,"%-6d: C %-3d: trying to leave\n", (*step_counter)++, child_local_num);
  fflush(out);
  sem_post(sem_counter);

  sem_wait(sem_counter);
  fprintf(out,"%-6d: C %-3d: leave\n", (*step_counter)++, child_local_num);
  fflush(out);
  sem_post(sem_counter);

  // child leaves
  sem_wait(sem_c_leave);
  (*c_in_center)--;
  sem_post(sem_c_leave);
  // let adult know child left
  sem_post(sem_c);

  // all processes ended ?
  (*end)++;
  if ( *end != p->C || *end2 != p->A)
    sem_wait(sem_finish);

  sem_wait(sem_counter);
  fprintf(out,"%-6d: C %-3d: finished\n", (*step_counter)++, child_local_num);
  fflush(out);
  sem_post(sem_counter);

  // finish all processes
  for (int c = 0; c < (p->C + p->A); c++)
    sem_post(sem_finish);

  exit(0);
}



void adult(struct params *p)
{
  // get actual adult number
  int adult_local_num = (*adult_p_no)++;

  sem_wait(sem_counter);
  fprintf(out,"%-6d: A %-3d: started\n", (*step_counter)++, adult_local_num);
  fflush(out);
  sem_post(sem_counter);

  sem_wait(sem_counter);
  fprintf(out,"%-6d: A %-3d: enter\n", (*step_counter)++, adult_local_num);
  fflush(out);
  sem_post(sem_counter);

  // adult entrace
  sem_wait(sem_a_leave);
  (*a_in_center)++;
  sem_post(sem_a_leave);
  // let the childs know new adult is in the town
  sem_post(sem_a);
  // sleep
  if( p->AWT )
    usleep( (random() % (p->AWT)) * 1000 );

  *leave = 1;
  sem_wait(sem_counter);
  fprintf(out,"%-6d: A %-3d: trying to leave\n", (*step_counter)++, adult_local_num);
  fflush(out);
  sem_post(sem_counter);

  // entrace condition and deadlock spot
  if ( (((*a_in_center) - 1) * CHILDS_ONE_ADULT) < (*c_in_center) && !(*deadlock) )
  {
    sem_wait(sem_counter);
    fprintf(out,"%-6d: A %-3d: waiting: %d: %d\n", (*step_counter)++, adult_local_num,
           *a_in_center, *c_in_center);
    fflush(out);
    sem_post(sem_counter);

    // while there are too many childs in center
    sem_wait(sem_c);
    while ( (((*a_in_center) - 1) * CHILDS_ONE_ADULT) < (*c_in_center) && !(*deadlock) )
    {
      sem_wait(sem_c);
    }
  }

  sem_wait(sem_counter);
  fprintf(out,"%-6d: A %-3d: leave\n", (*step_counter)++, adult_local_num);
  fflush(out);
  sem_post(sem_counter);

  // adult left
  sem_wait(sem_a_leave);
  (*a_in_center)--;
  sem_post(sem_a_leave);
  (*a_left)--;

  // if it was last adult, let all child end
  if ( *a_left == 0 )
  {
    for (int c = 0; c < p->C; c++)
    {
      sem_post(sem_a);
      sem_post(sem_leave);
    }
  }

  for (int c = *leave; c > 1; c--)
    sem_post(sem_leave);
  *leave = 0;

  // all processes left?
  (*end2)++;
  if ( *end != p->C || *end2 != p->A)
    sem_wait(sem_finish);

  sem_wait(sem_counter);
  fprintf(out,"%-6d: A %-3d: finished\n", (*step_counter)++, adult_local_num);
  fflush(out);
  sem_post(sem_counter);

  // let all process finish
  for (int c = 0; c < (p->C + p->A); c++)
    sem_post(sem_finish);

  exit(0);
}



void kill_prg()
{
  free_resources();
  kill(processId, SIGTERM);
  kill(getpid(), SIGTERM);
  exit(EXIT_FAILURE);
}



void free_resources()
{
  sem_destroy(sem_c);
  sem_destroy(sem_a);
  sem_destroy(sem_counter);
  sem_destroy(sem_finish);
  sem_destroy(sem_leave);
  sem_destroy(sem_a_leave);
  sem_destroy(sem_c_leave);

  shmctl(id1, IPC_RMID, NULL);
  shmctl(id2, IPC_RMID, NULL);
  shmctl(id3, IPC_RMID, NULL);
  shmctl(id4, IPC_RMID, NULL);
  shmctl(id5, IPC_RMID, NULL);
  shmctl(id6, IPC_RMID, NULL);
  shmctl(id7, IPC_RMID, NULL);
  shmctl(id8, IPC_RMID, NULL);
  shmctl(id9, IPC_RMID, NULL);
  shmctl(id10, IPC_RMID, NULL);
}



int set_resources()
{
  if( (sem_c = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
       MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED ) { return FAILURE; }

  else if( (sem_a = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
           MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED ) { return FAILURE; }

  else if( (sem_counter = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
           MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED ) { return FAILURE; }

  else if( (sem_finish = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
           MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED ) { return FAILURE; }

  else if( (sem_leave = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
           MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED ) { return FAILURE; }

  else if( (sem_a_leave = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
           MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED ) { return FAILURE; }

  else if( (sem_c_leave = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
           MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED ) { return FAILURE; }

  else if ( ( sem_init(sem_c,       1, 0) == -1 )
         || ( sem_init(sem_a,       1, 0) == -1 )
         || ( sem_init(sem_counter, 1, 1) == -1 )
         || ( sem_init(sem_finish,  1, 0) == -1 )
         || ( sem_init(sem_leave,   1, 0) == -1 )
         || ( sem_init(sem_a_leave, 1, 1) == -1 )
         || ( sem_init(sem_c_leave, 1, 1) == -1 ) )
  {
    sem_destroy(sem_c);
    sem_destroy(sem_a);
    sem_destroy(sem_counter);
    sem_destroy(sem_finish);
    sem_destroy(sem_leave);
    sem_destroy(sem_a_leave);
    sem_destroy(sem_c_leave);
    return FAILURE;
  }

  if( (id1 = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0666)) == -1 )
  { return FAILURE; }
  else if( (child_p_no = (int *) shmat(id1, NULL, 0)) == NULL )
  { return FAILURE; }

  else if( (id2 = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0666)) == -1 )
  { return FAILURE; }
  else if( (adult_p_no = (int *) shmat(id2, NULL, 0)) == NULL )
  { return FAILURE; }

  else if( (id3 = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0666)) == -1 )
  { return FAILURE; }
  else if( (step_counter = (int *) shmat(id3, NULL, 0)) == NULL )
  { return FAILURE; }

  else if( (id4 = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0666)) == -1 )
  { return FAILURE; }
  else if( (c_in_center = (int *) shmat(id4, NULL, 0)) == NULL )
  { return FAILURE; }

  else if( (id5 = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0666)) == -1 )
  { return FAILURE; }
  else if( (a_in_center = (int *) shmat(id5, NULL, 0)) == NULL )
  { return FAILURE; }

  else if( (id6 = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0666)) == -1 )
  { return FAILURE; }
  else if( (a_left = (int *) shmat(id6, NULL, 0)) == NULL )
  { return FAILURE; }

  else if( (id7 = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0666)) == -1 )
  { return FAILURE; }
  else if( (deadlock = (int *) shmat(id7, NULL, 0)) == NULL )
  { return FAILURE; }

  else if( (id8 = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0666)) == -1 )
  { return FAILURE; }
  else if( (leave = (int *) shmat(id8, NULL, 0)) == NULL )
  { return FAILURE; }

  else if( (id9 = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0666)) == -1 )
  { return FAILURE; }
  else if( (end = (int *) shmat(id9, NULL, 0)) == NULL )
  { return FAILURE; }

  else if( (id10 = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0666)) == -1 )
  { return FAILURE; }
  else if( (end2 = (int *) shmat(id10, NULL, 0)) == NULL )
  { return FAILURE; }

  return OK;
}



int input_handler(int argc, char *argv[], struct params *p)
{
  if ( argc != 7 )
    return FAILURE;
  char *c;

  // atoi - POSIX standad says
  // "if value cannot be represented, behaviout is undefined"
  // so i use strtol instead

  p->A   = strtol( argv[1], &c, 10 );
  if ( c == argv[1] )
    return FAILURE;

  p->C   = strtol( argv[2], &c, 10 );
  if ( c == argv[2] )
    return FAILURE;

  p->AGT = strtol( argv[3], &c, 10 );
  if ( c == argv[3] )
    return FAILURE;

  p->CGT = strtol( argv[4], &c, 10 );
  if ( c == argv[4] )
    return FAILURE;

  p->AWT = strtol( argv[5], &c, 10 );
  if ( c == argv[5] )
    return FAILURE;

  p->CWT = strtol( argv[6], &c, 10 );
  if ( c == argv[6] )
    return FAILURE;

  if ( ( IS_NGEATER( p->A ) ) ||
       ( IS_NGEATER( p->C ) ) ||
       ( IS_NRANGE ( p->AGT ) )  ||
       ( IS_NRANGE ( p->CGT ) )  ||
       ( IS_NRANGE ( p->AWT ) )  ||
       ( IS_NRANGE ( p->CWT ) ) )
    return FAILURE;

  return OK;
}
