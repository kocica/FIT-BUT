// tail2.cc
// Řešení IJC-DU2, příklad b), 29.3.2017
// Autor: Filip Kočica (xkocic01@fit.vutbr.cz), FIT
// Přeloženo: gcc 5.4
// Popis: Funkce orezava vystup na poslednich N (implicitne 10)
//        radku a ty tiskne na stdout. Data bere ze souboru zadaneho
//        jako parametr nebo std::cin v pripade absence souboru.
//        Pocet tisknutych radku se da upravit prepinacem -n N. N > 0.

#include <iostream>
#include <string>
#include <fstream>
#include <queue>

/** @brief Ve funkci vznikla chyba */
#define FAILURE        -1
/** @brief Funkce probehla v poradku */
#define OK             0
/** @brief Implicitni pocet poslednich radku k tisknuti */
#define NO_ROWS_IMP    10

/**
 * @brief Kontroluje platnost vstupnich dat a nacita parametry.
 *
 * Funkce v zavislosti na poctu parametru (argc) kontroluje
 * platnost vstupnich dat. Pri zadanem souboru umisti ukazatel
 * na prislusny argv do filename.
 *
 * @param argc[in] Pocet parametru predanych programu.
 * @param argv[in] Parametry predane programu.
 * @param filename[out] Ukazatel na jmeno souboru (argv[{1|3}]) nebo nullptr.
 * @return Pocet radku k tisknuti nebo FAILURE(-1) pri chybe.
 */
int input_handler(int argc, char *argv[], std::string *filename);

/**
 * @brief Provede cely program tail.
 *
 * @param in[in] Vstupni proud dat.
 * @param row_len[in] Pocet radku k tisknuti.
 */
void tail(std::istream *in, int row_len);



int input_handler(int argc, char *argv[], std::string *filename)
{
	if (argc > 4)
		return FAILURE;
	else if (argc == 1)
		return NO_ROWS_IMP;

  std::string param = argv[1];

  switch (argc)
  {
		case 4:  if ( !param.compare("-n") &&
								  atoi (argv[2]) > 0 )
             {
               *filename = argv[3];
               return atoi (argv[2]);
             }
             break;

		case 3:	 if ( !param.compare("-n") &&
									atoi (argv[2]) > 0 )
					   return atoi (argv[2]);
             break;

    case 2:  *filename = argv[1];
             return NO_ROWS_IMP;
	}

	return FAILURE;
}



void tail(std::istream *in, int row_len)
{
	int stack_size = 0;
	std::string row;
	std::queue<std::string> myqueue;

	while ( std::getline(*in, row) )
		myqueue.push(row);

	stack_size = myqueue.size();
	int counter = 0;
  /// Pokud musime preskocit nejake radky
	if ( row_len < stack_size )
	{
    /// Preskocime (vyhodime) radky ktere nepotrebujeme
		while ( !myqueue.empty() && counter < (stack_size - row_len) )
		{
			myqueue.pop();
			counter++;
		}
	}

  /// Tiskneme poslednich N radku.
  while ( !myqueue.empty() )
  {
    std::cout << myqueue.front() << '\n';
    myqueue.pop();
	}
}



int main(int argc, char *argv[])
{
  std::ios::sync_with_stdio(false);

	std::string filename = "";
	int row_len = input_handler(argc, argv, &filename);

  /// Implicitne cteme z std::cin
  std::istream *in = &std::cin;
  std::ifstream ifn;

  /// Spatny vstup
	if ( row_len < 0 )
	{
	  std::cerr << "ERROR #1: Insufficient input.\n";
    return EXIT_FAILURE;
	}

  /// Byl zadan soubor, cteme z nej
  if ( filename != "" )
  {
    ifn.open (filename.c_str());
    if ( !ifn.is_open() )
    {
	    std::cerr << "ERROR #2: Cannot open file.\n";
      return EXIT_FAILURE;
    }

    in=&ifn;
  }

  tail(in, row_len);

	return 0;
}
