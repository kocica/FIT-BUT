/**
 *	Projekt c. 1 z predmetu IZP - Prace s textem
 *	Autor: Filip Kocica (xkocic01)
 *	Verze: 1
 *	Datum zacatku prace: 13.10.2016
 *	Datum posledni zmeny: 25. 10. 2016
*/

#include <stdio.h>
#include <ctype.h>					// Hlavickovy s. zahrnut kvuli fci isprint

#define ZNAK_NULA			'0'
#define ZNAK_DEVET			'9'
#define MAX_DELKA_S			200		// Definice konstant pouzitych v prg
#define MAX_BYTU			16
#define MAX_DELKA_ADRESY	8

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS		0
#endif
#ifndef EXIT_FAILURE
#define EXIT_FAILURE		1
#endif

enum
{
	BIN_NA_TEXT = 1,
	BIN_NA_TEXT_S = 2,
	BIN_NA_TEXT_N = 3,
	BIN_NA_TEXT_S_N = 4,
	BIN_NA_TEXT_N_S = 5,			// Vyctovy typ reprezentujici typy vstupu
	TEXT_NA_HEXA = 6,
	HEXA_NA_TEXT = 7,
	TISK_RETEZCE = 8,
	ERR_NEDEF_VSTUP = -1
};

int jeCislo(char *s);
int porovnejRetezce(char *s1, char *s2);
int vstupniData(int argc, char *argv[]);
int delkaRetezce(char *s);
int charNaInt(char *s);
int hexNaInt(char c);
int hexNaBinarni(void);
void binarniNaText(int zac, int max);
void vypisMezeru(int pocet, char *s);
void tiskniRetezce(int min);
void binarniNaHex(void);
void vytiskniVystup(char *tiskData, int index, int prectenoBytu);
void tiskNapovedy(void);
void chyba(int kod);

int main(int argc, char *argv[])
{
	switch (vstupniData(argc,argv))
	{
		case BIN_NA_TEXT:			// Bez argumentu
			binarniNaText(0,0);
			break;
		case BIN_NA_TEXT_S:			// Pouze s argumentem -s (skip)
			binarniNaText(charNaInt(argv[2]),0);
			break;
		case BIN_NA_TEXT_N:			// Pouze s argumentem -n (number of chars)
			binarniNaText(0,charNaInt(argv[2]));
			break;
		case BIN_NA_TEXT_S_N:		// S obema argumenty -s & -n
			binarniNaText(charNaInt(argv[2]),charNaInt(argv[4]));
			break;
		case BIN_NA_TEXT_N_S:		// Otoceni volitelnych argumentu -n & -a
			binarniNaText(charNaInt(argv[4]),charNaInt(argv[2]));
			break;
		case TISK_RETEZCE:			// Argument -S
			tiskniRetezce(charNaInt(argv[2]));
			break;
		case TEXT_NA_HEXA:			// Argument -x (hexa-print)
			binarniNaHex();
			break;
		case HEXA_NA_TEXT:			// Argument -r (reverse)
			if(hexNaBinarni())	chyba(HEXA_NA_TEXT);
			return EXIT_FAILURE;
			break;
		case ERR_NEDEF_VSTUP:		// Spatne zadana vstupni data
			tiskNapovedy();
			return EXIT_FAILURE;
			break;
	}
    return EXIT_SUCCESS;
}

/**
 *	@param s - pole znaku jez kontrolujeme, zda je cislo
 *	
 *	@return - 1 v poli jsou pouze cisla, jinak 0
*/
int jeCislo(char *s)
{
	int i=0, delka = delkaRetezce(s);				// Zjisti delku retezce

	if(s[0] == '-')									// Prvni znak minus, preskoc jej
		i++;

	for(; i < delka; ++i)
	{
		if(ZNAK_NULA > s[i] || s[i] > ZNAK_DEVET)	//  znak < 0 nebo znak > 9
			return 0;
	}

	return 1;										// Pouze cisla -> vrat 1
}

/**
 *	@param - s1 pole znaku
 *	@param - s2 pole znaku ze vstupniho souboru
 *
 *	@return - 0 pokud se retezce rovnaji, 1 pokud ne
*/
int porovnejRetezce(char *s1, char *s2)
{
	int delka = delkaRetezce(s1);

	for(int i = 0; i < delka; i++)  				// Dokud nejsi na konci 1. retezce
	{ 
		if(s1[i] != s2[i])							// Pokud se znaky nerovnaji
			return 1;
	}

	return 0;										// Pole jsou stejna -> vrat 0
}

/**
 *	@param pocet - cele cislo reprezentujici, kolik odsazeni ma vytisknout
 *	(napr. doplneni do 16 Bytu mezerama)
 *	@param s - retezec obsahujici odsazeni (napr. 3 mezery "   ")
 *
 *	@return - nevraci nic
*/
void vypisMezeru(int pocet, char *s)
{
	for(int i = 0; i < pocet; ++i)
		printf("%s",s);
}

/**
 *	@param s - retezec jez chceme konvertovat na cislo
 *
 *	@return vraci cele cislo, ktere bylo predano jako pole znaku
 *
 *	Od kazdeho znaku odecte 48 (0 je v ASCII t. na 48. miste)
*/
int charNaInt(char *s)
{
	int soucet = 0,delka = delkaRetezce(s);
	
	for(int i = 0; i < delka; ++i)
		soucet = ((s[i]) - ZNAK_NULA) + soucet * 10;

	return soucet;
}

/**
 *	@param s - retezec jehoz delku chceme zjistit
 *
 *	@return - vraci celociselnou delku retezce
 *
 *	Prochazi retezec s dokud nenarazi na nulovy znak, pri kazde iteraci 
 *	inkrementuje promennou delka, kterou pote vraci.
*/
int delkaRetezce(char *s)
{
	int delka = 0;

	while (s[delka] != '\0')
		delka++;

	return delka;
}


/**
 *	@param zac - pokud je zadan prepinac -s preskoc M Bytu, jinak 0
 *	@param max - pokud je zadan prepinac -n tiskni N Bytu, jinak vsechny
 *
 *	@return - funkce nic nevraci
 *
 *	Cteme vstup az do EOF, nebo do omezeni dane -n, pri 16 Bytech
 *  nebo EOF vola fci tisknuti. Mod -n -> NOCH (number of chars)
 *  udava pocet Bytu k tisknuti
*/
void binarniNaText(int zac, int max)
{
	char nactenaData[MAX_BYTU];
    int c, prectenoBytu = 0, index = 0, omezeni_NOCH = 0;


	while((c = getchar()) != EOF && (omezeni_NOCH < max || !max))	
	{
		if(prectenoBytu >= zac)	// Pri zadani -s vypisuje az od M-teho Bytu
		{	if(!c) c--;
			nactenaData[index++] = c;
			omezeni_NOCH++;
		}
		prectenoBytu++;
		if(index == MAX_BYTU)	// Pokud je nacteno 16 Bytu, tiskni
		{
			nactenaData[index] = '\0';
			vytiskniVystup(nactenaData,index,prectenoBytu);	
			index = 0;
		}
	}

	nactenaData[index] = '\0';
	if(nactenaData[0] != '\0')
		vytiskniVystup(nactenaData,index,prectenoBytu);
}

/**
 *	@param min - tisk retezcu delsich-rovno N
 *	
 *	@return fce nic nevraci
 *
 *	Nacita z stdin po EOF, kdyz narazi na \0 nebo \n, zjisti, zda
 *  delka retezce >= min, pokud ano tiskne retezec.
*/
void tiskniRetezce(int min)
{
	char nactenaData[MAX_DELKA_S];
	int i = 0, c;

	while((c = getchar()) != EOF)
	{
		nactenaData[i++] = c;
		if(nactenaData[i - 1] == '\n' || nactenaData[i - 1] == '\0')
		{
			nactenaData[i - 1] = '\0';

			if((i - 1) >= min)
				printf("%s\n",nactenaData);

			i = 0;
		}
	}
}

/**
 *	@param funkce neprebira zadne parametry
 *
 *	@return funkce nic nevraci
 *
 *	Binarni data z stdin tiskne pomoci specifikatoru %x v hexa podobe
*/
void binarniNaHex(void)
{
	int c;

	while((c = getchar()) != EOF)
	{
		printf("%02x", c);		
	}

	printf("\n");
}

/**
 *	@param funkce neprebira zadne parametry
 *
 *	@return funkce nic nevraci
 *
 *	Funkce nacita dvojice znaku, oba znaky prevede na cele cislo a prvni
 *	vynasobi 16, pote cisla secte a tiskne znak. Pri lichem poctu znaku
 *	zjisti, zda je > 15 (g), pokud ano error, pokud ne tiskne jej.
*/
int hexNaBinarni(void)
{
	char nactenaData[2];
	int c, index = 0;

	while((c = getchar()) != EOF)
	{
		if((!isblank(c)) && c != '\n')
		{
			nactenaData[index++] = c;
		}
		if(index == 2)
		{
        	printf("%c",16 * (hexNaInt(nactenaData[0])) + hexNaInt(nactenaData[1]));	
			index = 0;
		}
	}

	if(index == 1)
	{
		if((c = hexNaInt(nactenaData[0])) > 15)
			return 1;
		else
			printf("%c",c);
	}

	return 0;
}

/**
 *	@param c - znak, ktery prevadime z hexa na int
 *
 *	@return celociselnou reprezentaci hexa znaku
 *
 *	Pokud je na vstupu male pismeno, prevede jej na velke.
 *	Pote provede prevod na cele cislo a vrati jej.
*/
int hexNaInt(char c)
{
	if(c >= 'a' && c <= 'z')
    	c = c - ('a' - 'A');

	int soucet;

	soucet = c / 16 - 3;
	soucet *= 10;
	soucet += c % 16;

	return (soucet > 9) ? soucet - 1 : soucet;
}

/**
 *	@param dataKTisknuti - pole znaku, ve kterem jsou ulozena data k tisknuti
 *	@param index - aktualni pocet Bytu v poli
 *  @param prectenoBytu - pocet Bytu nactenych od spusteni programu
 *
 *	@return funkce nic nevraci
 *
 *	Funkce z predanych parametru tiskne vystup ve formatu:
 *	AAAAAAAA  xx xx xx xx xx xx xx xx  xx xx xx xx xx xx xx xx  |bbbbbbbbbbbbbbbb|
 *
 *	Pri mensim poctu Bytu vse doplni mezerama v zajmu zachovani formatovani
 *  V pripade adresy kratsi nez 8 Bytu doplni nulami zleva
*/
void vytiskniVystup(char *tiskData, int index, int prectenoBytu)
{
	char hexaAdresa[MAX_DELKA_ADRESY];	// 8-Bytova hexa adresa prvniho znaku
	int delka = delkaRetezce(tiskData);


	sprintf(hexaAdresa,"%x",prectenoBytu - index);
	vypisMezeru(MAX_DELKA_ADRESY - delkaRetezce(hexaAdresa), "0");
	printf("%s  ",hexaAdresa);

	for(int i = 0; i < delka; ++i)
	{	
		if(tiskData[i] == -1) tiskData[i] = 0;
    	printf("%x ", tiskData[i]);
		if(i == 7) printf(" ");
	}
	if(index < 8) printf(" ");


	vypisMezeru(MAX_BYTU - delka,"   "); printf(" |");


	for(int i = 0; i < delka; ++i)
	{
		if( isprint(tiskData[i]) )
   			printf("%c",tiskData[i]);
   		else
			printf(".");
	}

	vypisMezeru(MAX_BYTU - delka," ");
	printf("|\n");
}

/**
 *	@param kod - Urcuje chybovy vypis
 *
 *	@return - fce nic nevraci
 *
 *	Fce na zaklade kodu tiskne chybove hlasky
*/
void chyba(int kod)
{
	switch(kod)
	{
		case HEXA_NA_TEXT:
			fprintf(stderr,"\nERROR: Hodnota Bytu neni v rozsahu 0-15.\n");
			tiskNapovedy();
			break;
		case BIN_NA_TEXT:
			fprintf(stderr,"\nERROR: "
			" [-s M] [-n N] M musi byt vetsi rovno 0 a N vetsi nez 0.\n");
			break;
		case TISK_RETEZCE:
			fprintf(stderr,"\nERROR: N musi byt v rozsahu 0 < N < 200.\n");
			break;
	}
}

/**
 *	@param - fce neprebira parametry
 *
 *	@return - fce nic nevraci
 *
 *	Fce tiskne napovedu pri neplatnem vstupu
*/
void tiskNapovedy()
{
	fprintf(stderr,"\nNejsou definovany nutne parametry, nebo"
	" jsou definovany spatne.\n\nProgram se spousti v nasledujici podobe:\n"
   	"\n	./proj1 [-s M] [-n N] 		0 <= M, 0 < N\n"
   	"	./proj1 -S N			0 < N < 200\n"
   	"	./proj1 -x\n"
   	"	./proj1 -r\n"
   	"\nSpusteni bez argumentu zpusobi tisknuti posloupnosti 16 Bytu ze vstupniho "
	"souboru na radek ve formatu: \nAAAAAAAA  xx xx xx xx xx xx xx xx  xx xx "
	"xx xx xx xx xx xx  |bbbbbbbbbbbbbbbb|,\nkde 'AAAAAAAA' je adresa prvniho Bytu,"
	" 'xx' je hexadecimalni reprezentace znaku a 'b' je znak samotny.\n"
	"\nVolitelne prepinace: [-s M] preskoci M bytu, [-n N] vytiskne pouze N Bytu.\n"
	"-S N tiskne retezce ukoncene '\\n' nebo '\\0', jez jsou delsi nebo rovno N.\n"
	"-x prevadi vstupni binarni Byty na hexadecimalni reprezentaci.\n-r tiskne"
	" textovou reprezentaci hexadecimalnich dat ze vstupniho souboru.\n\n");
}

/**
 *	@param argc - pocet parametru predanych programu
 *	@param argv	- ukazatel na pole ukazatelu na znak predaných programu
 *
 *	@return - promennou z vyctoveho typu
 *
 *	Funkce zjistuje, co uzivatel zadal a na zaklade toho vrati promennou
 *  z vyctoveho typu reprezentujici funkci, ktera se ma spustit.
*/
int vstupniData(int argc, char *argv[])
{
	switch (argc)
	{
		case 1:
			return BIN_NA_TEXT;
			break;

		case 2:
			if( porovnejRetezce(argv[1], "-x") == 0 )
				return TEXT_NA_HEXA;

			else if( porovnejRetezce(argv[1], "-r") == 0 )
				return HEXA_NA_TEXT;

			break;

		case 3:
			if( jeCislo(argv[2]) )					// je N cislo ?
			{
				if( porovnejRetezce(argv[1], "-S") == 0 )
				{
					if( charNaInt(argv[2]) > 0 && charNaInt(argv[2]) < MAX_DELKA_S )
						return TISK_RETEZCE;
					else
					{
						chyba(TISK_RETEZCE);
						return ERR_NEDEF_VSTUP;		// N < 0 nebo N > 199 -> ERR
					}
				}

				else if( porovnejRetezce(argv[1], "-s") == 0 )
				{
                    if ( charNaInt(argv[2]) >= 0 )             
						return BIN_NA_TEXT_S;
					else
					{
						chyba(BIN_NA_TEXT);
						return ERR_NEDEF_VSTUP;		// M < 0 -> ERR
					}
                }

				else if( porovnejRetezce(argv[1], "-n") == 0 )
                {
                    if ( charNaInt(argv[2]) > 0 )             
						return BIN_NA_TEXT_N;
					else
					{
						chyba(BIN_NA_TEXT);
						return ERR_NEDEF_VSTUP;		// N < 1 -> ERR
					}
                }
			}
			break;

		case 5:
			if( jeCislo(argv[2]) && jeCislo(argv[4]) )	// jsou N a M cisla ?
			{
				if( (porovnejRetezce(argv[1], "-s") == 0) && (porovnejRetezce(argv[3], "-n") == 0) )
				{
                    if ( charNaInt(argv[2]) >= 0 && charNaInt(argv[4]) > 0 )               
				    	return BIN_NA_TEXT_S_N;
					else
					{
						chyba(BIN_NA_TEXT);
						return ERR_NEDEF_VSTUP;		// N < 1 nebo M < 0 -> ERR
					}
                }

				else if( (porovnejRetezce(argv[1], "-n") == 0) && (porovnejRetezce(argv[3], "-s") == 0) )
				{									// Zamena volitelnych argumentu
                    if ( charNaInt(argv[4]) >= 0 && charNaInt(argv[2]) > 0 ) 
						return BIN_NA_TEXT_N_S;
					else
					{
						chyba(BIN_NA_TEXT);
						return ERR_NEDEF_VSTUP;		// N < 1 nebo M < 0 -> ERR
					}
                }
			}
			break;
	}

	return ERR_NEDEF_VSTUP;
}




