/**
 * @file
 *
 * @brief Treti a ctvrty projekt kurzu IZP - Jednoducha shlukova analyza.
 *                                         - Dokumentace zdrojovych kodu.
 *
 * Program nacte objekty z adresare a kazdy objekt zaradi do shluku.
 * Pote hleda dva nejblizsi shluky podle nejvzdalenejsi souseda, ktere
 * spoji do jednoho a druhy uvolni z pameti. Toto provadi az do
 * ciloveho poctu shluku, zadaneho tretim argumentem [implicitne 1].
 * Pote tiskne jednotlive objekty kazdeho shluku
 * a uklidi po sobe - uzavre soubor a uvolni alokovanou pamet.
 *
 * Verze: 1.0
 *
 * Vytvoril Filip Kocica (xkocic01) dne 28.11.2016.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> /// sqrtf
#include <limits.h> /// INT_MAX

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

#define FAILURE		-1	/**< @brief Error return value */

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

/**
 * @brief Heterogenni promenna reprezentujici jeden objekt ve shluku.
 *
 * Struktura popisujici objekt, kazdy objekt ma unikatni 'id' a souradnice
 */
struct obj_t {
    int id;    /**< Unikatni cislo slouzici k identifikaci objektu. */
    float x;   /**< Prvni souradnice popisujici umisteni objektu. */
    float y;   /**< Druha souradnice popisujici umisteni objektu. */
};

/**
 * @brief Heterogenni promenna reprezentujici shluk objektu.
 *
 * Shlukuje pole objektu o 'size' prvcich.
 */
struct cluster_t {
    int size;          /**< Pocet objektu ve shluku. */
    int capacity;      /**< Udava pro kolik objektu je shluk alokovan. */
    struct obj_t *obj; /**< Zde jsou ulozeny jednotlive objekty shluku. */
};

/**
 * @brief Provede inicialiaci clusteru.
 *
 * Funkce alokuje pamet pro 'cap' objektu, a ukazatel uklada do 'obj'.
 * Kapacitu uklada do atributu 'capacity'.
 * Velikost shluku nastavi na 1.
 *
 * @param c [out] Shluk objektu inicializovany pro 'cap' objektu.
 * @param cap [in] Udava pro kolik objektu alokovat shluk.
 * @return
 *   Nic.
 *
 * @pre 'c' nesmi ukazovat na 'NULL' a 'cap' musi byt vetsi rovno nule.
 * @post Pri neuspesne alokaci 'c' ukazuje na NULL a 'capacity' = 0.
 *       Jinak 'c' ukazuje na alokovanou pamet a 'capacity' = 'cap'.
 */
void init_cluster(struct cluster_t *c, int cap)
{
    assert(c != NULL);
    assert(cap >= 0);

   	c->capacity = cap;
	c->size = 1;
	c->obj = malloc( sizeof( struct obj_t ) * cap );
	
	if (c->obj == NULL)
	{
   		c->capacity = 0;

		fprintf(stderr, "\nNepodarilo se alokovat pamet pro c->obj [ERR]\n");
	}
}

/**
 * @brief Vycisti shluk z pameti.
 *
 * Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 *
 * @param c [in] Shluk objektu urceny k uvolneni.
 * @return
 *   Nic.
 *
 * @pre Objekty shluku jeste nesmi byt uvolneny (Kvuli double free).
 * @post Objekty musi byt uvolneny a 'size' = 0.
 */
void clear_cluster(struct cluster_t *c)
{
    free(c->obj);
	c->size = 0;
}

/**
 * @brief Doporuceny pocet pro realokaci.
 *
 * Realokovani po jednom prvku je prilis narocne,
 * proto se pocet navysuje o tuto doporucenou hodnotu.
 */
const int CLUSTER_CHUNK = 10;

/**
 * @brief Zmeni velikost shluku.
 *
 * Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 *
 * @param c [out] Ukazatel na shluk objektu jehoz velikost jsme zmenili.
 * @param new_cap [in] Nova kapacita (max pocet objektu) shluku.
 * @return
 *   Shluk se zmenenou velikosti.
 *
 * @pre 'c' nesmi ukazovat na 'NULL', 'new_cap' musi byt
 *   vetsi rovno nule, stejne jako kapacita shluku. 
 * @post Pri chybne realokaci 'c' ukazuje na NULL, v opacnem pripade
 *   na realokovanou pamet. Do 'capacity' se ulozi 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
    // TUTO FUNKCI NEMENTE
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)
        return c;

    size_t size = sizeof(struct obj_t) * new_cap;

    void *arr = realloc(c->obj, size);
    if (arr == NULL)
    	return NULL;

    c->obj = arr;
    c->capacity = new_cap;
    return c;
}

/**
 * @brief Prida objekt do shluku.
 *
 * Prida objekt 'obj' na konec shluku 'c'.
 * Pokud se do shluku objekt nevejde, rozsiri shluk.
 *
 * @param c [out] Shluk objektu s objektem pridanym na konec.
 * @param obj [in] Objekt, ktery chceme do shluku pridat.
 * @return
 *   Nic.
 *
 * @pre 'size' musi byt mensi rovno 'capacity'.
 * @post Na pozici 'size'+1 musi byt ulozen objekt 'obj'.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
    if(c->size == c->capacity)
	{
		resize_cluster(c, c->size + CLUSTER_CHUNK);
	}

	c->obj[c->size++] = obj;
}

/**
 * @brief Seradi objekty v shluku.
 *
 * Seradi objekty ve shluku 'c' vzestupne podle jejich 'id'.
 *
 * @param c [out] Ukazatel na serazeny shluk objektu.
 * @return
 *   Nic.
 *
 * @pre 'c' je ruzne od 'NULL'.
 * @post Objekty shluku 'c' musi byt serazeny podle 'id'.
 */
void sort_cluster(struct cluster_t *c);

/**
 * @brief Spoji dva shluky do jednoho a pote je seradi.
 *
 * Do shluku 'c1' prida objekty 'c2'.
 * Shluk 'c1' bude v pripade nutnosti rozsiren.
 * Objekty ve shluku 'c1' budou serazny vzestupne podle id.
 * Shluk 'c2' zustava nezmenen.
 *
 * @param c1 [out] Shluk objektu zvetseny o 'c2' objektu a serazeny.
 * @param c2 [in] Ukazatel na shluk, ktery chceme pridat k 'c1'
 * @return
 *   Nic.
 *
 * @pre 'c1' ani 'c2' nesmi ukazovat na NULL.
 * @post 'c1' musi obsahovat prvky 'c2', 'c2' musi zustat nezmenen,
 *   'c1->size' se musi rovnat novemu poctu prvku a 'c1->obj'
 *   musi byt serazeno.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c2 != NULL);

	int dif = c2->size - (c1->capacity - c1->size) + CLUSTER_CHUNK;

	if ( dif > 0 )
	{
		resize_cluster(c1, c1->capacity + dif);
	}

	for(int i = c1->size, idx = 0; i < c1->capacity - CLUSTER_CHUNK;
    ++i, ++idx)
	{
		append_cluster(c1, c2->obj[idx]);
	}

	sort_cluster(c1);
}


/**
 * Pomocna funkce slouzici pro razeni objektu ve shluku na zaklade id.
 * Ukazatel na tuto funkci se predava funkci qsort.
 *
 * @param a [in] ukazatel na void, ktery se pretypuje na objekt
 * @param b [in] ukazatel na void, ktery se pretypuje na objekt
*/
static int obj_sort_compar(const void *a, const void *b)
{
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = a;
    const struct obj_t *o2 = b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/**
 * @brief Seradi objekty v shluku.
 *
 * Seradi objekty ve shluku 'c' vzestupne podle jejich 'id'.
 *
 * @param c [out] Ukazatel na serazeny shluk objektu.
 * @return
 *   Nic.
 *
 * @pre 'c' je ruzne od 'NULL'.
 * @post Objekty shluku 'c' musi byt serazeny podle 'id'.
 */
void sort_cluster(struct cluster_t *c)
{
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/**
 * @brief Odstrani shluk z pole shluku.
 *
 * Funkce odstrani shluk nachazejici se na indexu 'idx' z pole shluku 'carr'.
 *
 * @param carr [out] Zmensene pole shluku.
 * @param narr [in] Pocet shluku v poli.
 * @param idx [in] Index shluku, jez chceme uvolnit z pameti.
 * @return
 *   Novy pocet shluku v poli.
 *
 * @pre Index 'idx' musi byt mensi nez pocet prvku 'narr' a pocet prvku musi
 *   byt vetsi nez 0.
 * @post Prvek na indexu 'idx' musi byt odstranen a prvky jez jsou za timto
 *   indexem musi byt posunuty na index o jedna mensi.
 */
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
    assert(idx < narr);
    assert(narr > 0);

	struct obj_t *tmp = carr[idx].obj;

    for(int i = idx; i < narr-1; ++i)
	{
	    carr[i] = carr[i+1];
	}

	carr[narr-1].obj = tmp;

	return (narr - 1);
}

/**
 * @brief Zjisti vzdalenost dvou objektu.
 *
 * Na zaklade souradnic 'x' a 'y' objektu vypocte eukleidovskou vzdalenost
 * mezi dvema objekty.
 *
 * @param o1 [in] Ukazatel na prvni objekt.
 * @param o2 [in] Ukazatel na druhy objekt.
 * @return
 *   Vzdalenost mezi dvema objekty.
 *
 * @pre Ani jeden z objektu nesmi ukazovat na 'NULL'.
 * @post Ani jeden z objektu nesmi ukazovat na 'NULL'.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);

	float x = o1->x - o2->x;
	float y = o1->y - o2->y;

    float dist = sqrtf( (x*x) + (y*y) );

	return dist;
}

/**
 * @brief Hleda dva nejvzdalenejsi objekty.
 *
 * Funkce pomoci 'obj_distance' najde dva nejvzdalenejsi objekty
 * ze dvou shluku a vraci jejich vzdalenost.
 *
 * @param c1 [in] Ukazatel na prvni shluk.
 * @param c2 [in] Ukazatel na druhy shluk.
 * @return
 *   Vzdalenost dvou nejvzdalenejsich objektu z obou shluku.
 *
 * @pre Ani jeden shluk nesmi ukazovat na 'NULL' a jejich velikost
 *   'size' musi byt vetsi nez nula.
 * @post Nic.
 */
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);

	float max = obj_distance(&c1->obj[0], &c2->obj[0]);
	float act;

	for(int n = 0; n < c1->size; n++)
	{
		for(int m = 0; m < c2->size; m++)
		{
			if(n == m)
				continue;

			act = obj_distance(&c1->obj[n], &c2->obj[m]);

			if(act > max)
			{
				max = act;
			}
		}
	}
	return max;
}

/**
 * @brief Nalezne dva nejvhodnejsi sousedy (shluky).
 *
 * Funkce hleda v poli shluku 'carr' o velikosti 'narr' dva nejblizsi shluky
 * podle metody nejvzdalenejsiho souseda.
 * Nalezene shluky identifikuje jejich indexy v poli 'carr' a ulozi na adresy
 * 'c1' a 'c2'.
 *
 * @param carr [in] Ukazatel na prvni prvek pole shluku.
 * @param narr [in] Pocet prvku pole shluku.
 * @param c1[out] Zde se uklada index prvniho souseda (shluku).
 * @param c2[out] Zde se uklada index druheho souseda (shluku).
 * @return
 *   Nic.
 *
 * @pre 'carr' nesmi ukazovat na 'NULL' a velikost 'narr' musi byt > 0.
 * @post 'c1' a 'c2' musi mit kladne ruzne hodnoty.
 */
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
    assert(narr > 0);

	float min = cluster_distance(&carr[0], &carr[1]);
	*c1 = 0;
	*c2 = 1;
	float act;

	for(int m = 0; m < narr; m++)
	{
		for (int n = m + 1; n < narr; n++)
		{
			act = cluster_distance(&carr[m], &carr[n]);

			if(act < min)
			{
				min = act;
				*c1 = m;
				*c2 = n;
			}
		}	
	}
}

/**
 * @brief Tiskne jeden shluk.
 *
 * Tisk jednotlivych objektu shluku 'c' na stdout.
 *
 * @param c [in] Ukazatel na shluk.
 * @return
 *   Nic.
 *
 * @pre 'c' je ruzne od 'NULL'.
 * @post Obsah 'c' musi zustat nezmenen.
 */
void print_cluster(struct cluster_t *c)
{
    for (int i = 0; i < c->size; i++)
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}

/**
 * @brief Tiskne jednotlive shluky.
 *
 * Tisk pole shluku o 'narr' prvcich.
 *
 * @param carr [in] Ukazatel na prvni prvek pole shluku.
 * @param narr [in] Pocet shluku v poli.
 * @return
 *   Nic.
 *
 * @pre 'carr' je ruzne od 'NULL'.
 * @post Obsah 'carr' musi zustat nezmenen.
 */
void print_clusters(struct cluster_t *carr, int narr)
{
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++)
    {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}

/**
 * Tiskne napovedu pri nedefinovanem vstupu nebo chybe.
*/
void pHelp(void)
{
	printf("Program runs in the following form:\n\n"
           "   ./proj3 FILE [N]\n\n"
           "- where FILE is name of text file, where object are defined.\n"
           "N is number of clusters we want to make (implicit: 1)\n");
}

/**
 * @brief Nacita objekty ze souboru a uklada je do shluku, jez naalokuje.
 *
 * Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 * jej do pole shluku. Alokuje prostor pro pole vsech shluku, ukazatel na prvni
 * polozku pole (ukazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 * kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych shluku.
 * V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr' hodnotu NULL.
 *
 * @param filename [in] Jmeno souboru, ve kterem jsou ulozeny definice objektu.
 * @param arr [out] Ukazatel na ukazatel na alokovany shluk, kam byly
 *   objekty ulozeny.
 * @return
 *   Pocet objektu jez nacte.
 *
 * @pre 'arr' nesmi ukazovat na 'NULL'.
 * @post Pri vzniku chyby se musi uvolnit doposud alokovana pamet, uzavrit
 *   soubor se kterym se pracovalo, vypsat ERR na stderr a vratit error kod.
 *   'arr' musi pri chybe ukazovat na 'NULL'.
 */
int load_clusters(char *filename, struct cluster_t **arr)
{
    assert(arr != NULL);
	int num_clusters;

    FILE *fr = fopen(filename, "r");
	if (fr == NULL)
	{
		*arr = NULL;
		return FAILURE;
	}

	if (fscanf(fr, "count=%d", &num_clusters) != 1)
	{
		fclose(fr);
		fprintf(stderr, "ERROR: Wrong format of data in text file.\n");
		*arr = NULL;
		return FAILURE;
	}

	*arr = malloc(sizeof(struct cluster_t) * num_clusters);

	if (arr == NULL)
	{
		fclose(fr);
		fprintf(stderr, "ERROR: Error in memory allocation.");
		*arr = NULL;
		return FAILURE;
	}

	for (int i = 0; i < num_clusters; ++i)
	{
		init_cluster(&arr[0][i], 1);
		if(!arr[0][i].capacity)
		{
			for (int j = 0; j <= i; j++)
			{
				clear_cluster(&arr[0][j]);
			}
			free(*arr);
			fclose(fr);
			fprintf(stderr, "ERROR: Error in memory allocation.\n");
			*arr = NULL;
			return FAILURE;
		}

		if (fscanf(fr, " %d %f %f",
			&arr[0][i].obj->id,
			&arr[0][i].obj->x,
			&arr[0][i].obj->y) != 3)
		{
			for (int j = 0; j <= i; j++)
			{
				clear_cluster(&arr[0][j]);
			}
			free(*arr);
			fclose(fr);
			fprintf(stderr, "ERROR: Wrong format of data in text file.\n");
			*arr = NULL;
			return FAILURE;
		}
		else if (arr[0][i].obj->x < 0 || arr[0][i].obj->x > 1000 ||
                 arr[0][i].obj->y < 0 || arr[0][i].obj->y > 1000 ||
                 arr[0][i].obj->id >= INT_MAX)
		{
			for (int j = 0; j <= i; j++)
			{
				clear_cluster(&arr[0][j]);
			}
			free(*arr);
			fclose(fr);
			fprintf(stderr, "ERROR: Wrong format of data in text file.\n");
			*arr = NULL;
			return FAILURE;
		}
	}

	fclose(fr);
	return num_clusters;
}

int main (int argc, char **argv)
{
	if (argc < 2)
	{
		fprintf(stderr, "ERROR: Name of text file required.\n");
		pHelp();
		return EXIT_FAILURE;
	}

	int N = 1;

	if (argc == 3)
	{
		if (atoi(argv[2]) <= 0)
		{
			fprintf(stderr, "ERROR: ./test FILE.txt [N]; N > 0.\n");
			pHelp();
			return EXIT_FAILURE;
		}
		N = atoi(argv[2]);
	}
	
	struct cluster_t *clusters;
	int num_clusters = load_clusters(argv[1], &clusters);

	if(num_clusters == FAILURE)
	{
		fprintf(stderr, "ERROR: Memory allocation failed.\n");
		pHelp();
		return EXIT_FAILURE;
	}

	int a, b, i = num_clusters;

	for(; i > N; i--)
	{
		find_neighbours(clusters, i, &a, &b);

		merge_clusters(&clusters[a], &clusters[b]);

		remove_cluster(clusters, i, b);
	}

	print_clusters(clusters, i);
	
	/* MEM FREE*/

	/**************************************************************/

	for(int i = 0; i < num_clusters; ++i)
		if(&clusters[i] != NULL)
			clear_cluster(&clusters[i]);

	if(clusters != NULL)
		free(clusters);
	

	return EXIT_SUCCESS;
}
