/**
 * @file
 *
 * @mainpage
 *
 * Ctvrty projekt kurzu IZP - Dokumentace zdrojovych kodu.
 *
 * <br>Program nacte objekty z adresare a kazdy objekt zaradi do shluku.
 * <br>Pote hleda dva nejblizsi shluky podle nejvzdalenejsi souseda, ktere
 * spoji do jednoho a druhy uvolni z pameti. <br>Toto provadi az do
 * ciloveho poctu shluku, zadaneho tretim argumentem [implicitne 1].
 * <br>Pote tiskne jednotlive objekty kazdeho shluku
 * a uklidi po sobe - uzavre soubor a uvolni alokovanou pamet.
 *
 * Verze: 1.0
 *
 * Vytvoril Filip Kocica (xkocic01@fit.vutbr.cz) dne 28.11.2016.
 */

/**
 * @defgroup DeklaraceStruktur Definice objektu a shluku.
 *
 * @brief Definice struktur reprezentujici shluky a objekty.
 *
 * @{
 */

/**
 * @brief Reprezentace jednoho objektu ve shluku.
 *
 * Struktura popisujici objekt, kazdy objekt ma unikatni 'id' a souradnice
 * 'x', 'y', na jejich zaklade se provadi shlukovani.
 */
struct obj_t {
    int id;    /**< Unikatni cislo slouzici k identifikaci objektu. */
    float x;   /**< Prvni souradnice popisujici umisteni objektu. */
    float y;   /**< Druha souradnice popisujici umisteni objektu. */
};

/**
 * @brief Reprezentace shluku objektu.
 *
 * Shlukuje pole objektu o 'size' prvcich.
 */
struct cluster_t {
    int size;          /**< Pocet objektu ve shluku. */
    int capacity;      /**< Udava pro kolik objektu je shluk alokovan. */
    struct obj_t *obj; /**< Zde jsou ulozeny jednotlive objekty shluku. */
};

/** @} */

/**
 * @defgroup  DynamickaPamet Prace s dynamickou pameti.
 *
 * @brief Zajistuje alokaci, realokaci a uvolneni pameti jednotlivych shluku.
 *
 * @{
 */

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
void init_cluster(struct cluster_t *c, int cap);

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
void clear_cluster(struct cluster_t *c);

/**
 * @brief Doporuceny pocet pro realokaci.
 *
 * Realokovani po jednom prvku je prilis narocne,
 * proto se pocet navysuje o tuto doporucenou hodnotu.
 */
extern const int CLUSTER_CHUNK;

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
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap);

/** @} */

/**
 * @defgroup Objekty Prace s objekty shluku.
 *
 * @brief Zajistuje nacitani, razeni, pridavani a odebirani objektu.
 *
 * @{
 */

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
void append_cluster(struct cluster_t *c, struct obj_t obj);

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
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2);

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
int remove_cluster(struct cluster_t *carr, int narr, int idx);

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
int load_clusters(char *filename, struct cluster_t **arr);

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

/** @} */

/**
 * @defgroup Shlukovani Shlukovani objektu.
 *
 * @brief Pocitani vzdalenosti a hledani nejvhodnejsiho souseda.
 *
 * @{
 */

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
float obj_distance(struct obj_t *o1, struct obj_t *o2);

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
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2);

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
 * @param c1 [out] Zde se uklada index prvniho souseda (shluku).
 * @param c2 [out] Zde se uklada index druheho souseda (shluku).
 * @return
 *   Nic.
 *
 * @pre 'carr' nesmi ukazovat na 'NULL' a velikost 'narr' musi byt > 0.
 * @post 'c1' a 'c2' musi mit kladne ruzne hodnoty.
 */
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2);

/** @} */

/**
 * @defgroup Tisk Tisk shluku.
 *
 * @brief Na stdout vypise veskere shluky i jejich objekty.
 *
 * @{
 */

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
void print_cluster(struct cluster_t *c);

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
void print_clusters(struct cluster_t *carr, int narr);

/** @} */
