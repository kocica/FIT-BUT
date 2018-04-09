/******************************************************************************
 * Laborator 01 - Zaklady pocitacove grafiky - IZG
 * ihulik@fit.vutbr.cz
 *
 * $Id: $
 * 
 * Popis: Hlavicky funkci pro funkce studentu
 *
 * Opravy a modifikace:
 * - ibobak@fit.vutbr.cz, orderedDithering
 */

#include "student.h"
#include "globals.h"

#include <time.h>

const int M[] = {
    0, 204, 51, 255,
    68, 136, 187, 119,
    34, 238, 17, 221,
    170, 102, 153, 85
};

const int M_SIDE = 4;

/******************************************************************************
 ******************************************************************************
 Funkce vraci pixel z pozice x, y. Je nutne hlidat frame_bufferu, pokud 
 je dana souradnice mimo hranice, funkce vraci barvu (0, 0, 0).
 Ukol za 0.25 bodu */
S_RGBA getPixel(int x, int y)
{
    if (x >= 0 && x < width && y >= 0 && y < height) 
        return frame_buffer[ width * y + x];
    // todo
    
    return COLOR_BLACK; //vraci barvu (0, 0, 0)
}
/******************************************************************************
 ******************************************************************************
 Funkce vlozi pixel na pozici x, y. Je nutne hlidat frame_bufferu, pokud 
 je dana souradnice mimo hranice, funkce neprovadi zadnou zmenu.
 Ukol za 0.25 bodu */
void putPixel(int x, int y, S_RGBA color)
{
    if (x >= 0 && x < width && y >= 0 && y < height) 
        frame_buffer[width * y + x] = color;
    // todo
}
/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na odstiny sedi. Vyuziva funkce GetPixel a PutPixel.
 Ukol za 0.5 bodu */
void grayScale()
{
    // todo
    S_RGBA color;
	unsigned char intensity;
	for (int y = 0; y < height; ++y)
    {
		for (int x = 0; x < width; ++x)
		{
			color = getPixel(x,y);
			intensity = ROUND(color.red * 0.299 + color.green * 0.587 + color.blue * 0.114); // 0.229R + 0.587G + 0.114B
			color.red = intensity;
			color.blue = intensity;
			color.green = intensity;
			putPixel(x, y, color);
		}
    }
}

/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na cernobily pomoci algoritmu maticoveho rozptyleni.
 Ukol za 1 bod */

void orderedDithering()
{
    S_RGBA color; 
    for (int y = 0; y < height; ++y)
    {
        for(int x = 0; x < width; ++x)
        {
            color = getPixel(x,y);
             int i = x % M_SIDE;
             int j = y % M_SIDE;
             if(color.red > M[j*M_SIDE + i])
                putPixel(x,y, COLOR_WHITE);
             
             else
                putPixel(x,y, COLOR_BLACK);
             
        }
    }
}

/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na cernobily pomoci algoritmu distribuce chyby.
 Ukol za 1 bod */

void help_for(int x, int y, float error)   // double ?
{
    S_RGBA color;
    int c;
    if (x  >= 0 && x < width && y >= 0 && y < height)
    {
        color = getPixel(x,y);
         c = ROUND(color.red  + error);
        if(c > 255) {
            putPixel(x,y, COLOR_WHITE);
            return;
        }
        if(c < 0) {
            putPixel(x,y, COLOR_BLACK);
            return;
    }
        else {      
        color.red = c;
        color.green = c;
        color.blue = c;
        putPixel(x,y, color);
        }
    }
}
void errorDistribution()
{   
    // todo
   S_RGBA color;
    int error;
    grayScale();
    for(int y = 0; y < height; ++y)
    {
        for(int x = 0; x < width; ++x)
        {
           color =  getPixel(x ,y);
            int c = color.red;                                 
            //color.red = color.green = color.blue = c;
           if(c <= 127)   // 128 ? 255?
            {
                // putPixel(x,y, COLOR_BLACK);
                color = COLOR_BLACK; 
                error = c;
            }
            else
            {
                //putPixel(x,y, COLOR_WHITE);
                color = COLOR_WHITE;
                error = c - 255;
            }
                //error = c - color.red;
                putPixel(x,y, color);
                help_for(x + 1, y, (3.0/8.0 ) * error);
                help_for(x, y + 1, ( 3.0/8.0) * error);
                help_for(x+1, y + 1,  (2.0/8.0) * error);
        }
    } 
}
/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na cernobily pomoci metody prahovani.
 Demonstracni funkce */
void thresholding(int Threshold)
{
	/* Prevedeme obrazek na grayscale */
	grayScale();

	/* Projdeme vsechny pixely obrazku */
	for (int y = 0; y < height; ++y)
		for (int x = 0; x < width; ++x)
		{
			/* Nacteme soucasnou barvu */
			S_RGBA color = getPixel(x, y);

			/* Porovname hodnotu cervene barevne slozky s prahem.
			   Muzeme vyuzit jakoukoli slozku (R, G, B), protoze
			   obrazek je sedotonovy, takze R=G=B */
			if (color.red > Threshold)
				putPixel(x, y, COLOR_WHITE);
			else
				putPixel(x, y, COLOR_BLACK);
		}
}

/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na cernobily pomoci nahodneho rozptyleni. 
 Vyuziva funkce GetPixel, PutPixel a GrayScale.
 Demonstracni funkce. */
void randomDithering()
{
	/* Prevedeme obrazek na grayscale */
	grayScale();

	/* Inicializace generatoru pseudonahodnych cisel */
	srand((unsigned int)time(NULL));

	/* Projdeme vsechny pixely obrazku */
	for (int y = 0; y < height; ++y)
		for (int x = 0; x < width; ++x)
		{
			/* Nacteme soucasnou barvu */
			S_RGBA color = getPixel(x, y);
			
			/* Porovname hodnotu cervene barevne slozky s nahodnym prahem.
			   Muzeme vyuzit jakoukoli slozku (R, G, B), protoze
			   obrazek je sedotonovy, takze R=G=B */
			if (color.red > rand()%255)
			{
				putPixel(x, y, COLOR_WHITE);
			}
			else
				putPixel(x, y, COLOR_BLACK);
		}
}
/*****************************************************************************/
/*****************************************************************************/

