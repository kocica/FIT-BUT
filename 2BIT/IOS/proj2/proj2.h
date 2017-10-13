/// proj2.h
/// Second project to IOS course, 30.3.2017
/// Filip Kocica, xkocic01 AT fit.vutbr.cz
/// Interface and definitions for proj2.c

#ifndef PROJ2_H
#define PROJ2_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <signal.h>
#include <stdlib.h> // atoi
#include <string.h> // memcpy
#include <time.h>   // rand
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

/**
 *  @brief an error has ocurred in function
 */
#define FAILURE           -1
/**
 *  @brief function ended up successfuly
 */
#define OK                0

/**
 *  @brief EC - wrong input
 */
#define INPUT_ERR_CODE    1
/**
 *  @brief EC - sem or shared memory err
 */
#define RESRC_ERR_CODE    1
/**
 *  @brief EC - system call like fork etc error
 */
#define SYS_ERR_CODE      2

/**
 *  @brief how many childs can one adult take care of
 */
#define CHILDS_ONE_ADULT  3

/**
 *  @brief input data check
 */
#define IS_NGEATER(i)   ( (i) <= 0 )
/**
 *  @brief input data range check
 */
#define IS_NRANGE(i)    ( ((i) < 0) || ((i) >= 5001) )

/**
 *  @brief output file
 */
FILE *out;

/**
 *  @brief input data
 */
struct params {
  int A;
  int C;
  int AGT;
  int CGT;
  int AWT;
  int CWT;
};

/**
 *  @brief semaphores
 */
sem_t *sem_c, *sem_a, *sem_counter, *sem_finish, *sem_leave, *sem_a_leave, *sem_c_leave;

/**
 *  @brief main process id
 */
int processId;
/**
 *  @brief shared mem IDs
 */
int id1,
    id2,
    id3,
    id4,
    id5,
    id6,
    id7,
    id8,
    id9,
    id10;

/**
 *  @brief pointers on shared mem
 */
int *child_p_no   = NULL,
    *adult_p_no   = NULL,
    *step_counter = NULL,
    *c_in_center  = NULL,
    *a_in_center  = NULL,
    *a_left       = NULL,
    *deadlock     = NULL,
    *leave        = NULL,
    *end          = NULL,
    *end2         = NULL;


/**
 *  @brief checks valdity of input data and saves them into struct params var
 *
 *  @param argc[in] arguments count
 *  @param argv[in] arguments value
 *  @param p [out]  saved input data
 *  @return OK if succes, or failure
 */
int input_handler(int argc, char *argv[], struct params *p);

/**
 *  @brief generate all child processes
 *
 *  @param p[in] childs count and sleep interval
 */
void child(struct params *p);

/**
 *  @brief generate all adult processes
 *
 *  @param p[in] adults count and sleep interval
 */
void adult(struct params *p);

/**
 *  @brief create semaphores and allocates shared memory
 *
 *  @return OK if success, or failure
 */
int set_resources();

/**
 *  @brief safely kills program, destroy semaphores etc
 */
void kill_prg();

/**
 *  @brief destroys semaphores and frees shared memory
 */
void free_resources();

#endif /* ! PROJ2_H */
