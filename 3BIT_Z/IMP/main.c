/**
 *
 * @file main.c
 * @author Filip Kocica <xkocic01@stud.fit.vutbr.cz>
 * @brief IMP projekt -- Mereni vzdalenosti pomoci ultazvukov. merice
 *
 */

#include "MK60D10.h"
#include <string.h>
#include <stdio.h>

/* Macros for bit-level registers manipulation */
#define GPIO_PIN_MASK	0x1Fu
#define GPIO_PIN(x)		(((1)<<(x & GPIO_PIN_MASK)))

#define FIRST_NUM  0x100
#define SECOND_NUM 0x200
#define THIRD_NUM  0x1000
#define FOURTH_NUM 0x2000

#define LEFT_DOWN  0x40
#define LEFT_UP    0x80
#define DOT        0x100
#define RIGHT_UP   0x200
#define BOTTOM     0x400
#define UP         0x800
#define RIGHT_DOWN 0x4000
#define CENTER     0x8000

#define TRIG_PIN   0x1000000
#define ECHO_PIN   0x2000000

int pressed_up = 0, pressed_down = 0, indexDisplay = 0;
unsigned int compare = 0x200;

/* A delay function */
void delay(long long bound)
{
  long long i;

  for(i = 0; i < bound; i++)
  {

  }
}

/* Initialize the MCU - basic clock settings, turning the watchdog off */
void MCUInit(void)
{
    MCG_C4       |= ( MCG_C4_DMX32_MASK | MCG_C4_DRST_DRS(0x01) );
    SIM_CLKDIV1  |= SIM_CLKDIV1_OUTDIV1(0x00);
    WDOG_STCTRLH &= ~WDOG_STCTRLH_WDOGEN_MASK;
}

void PortsInit(void)
{
    /* Turn on all port clocks */
    SIM->SCGC5 = SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTD_MASK;

	PORTA->PCR[25] = ( PORT_PCR_ISF(0x01)  /* Nuluj ISF (Interrupt Status Flag)   */
					 | PORT_PCR_IRQC(0x0A) /* Interrupt enable on falling edge == A, rising == 9 */
					 | PORT_PCR_MUX(0x01)  /* Pin Mux Control to GPIO             */
					 | PORT_PCR_PE(0x01)   /* Pull resistor enable...             */
					 | PORT_PCR_PS(0x01)); /* ...select Pull-Up == 0x01, pulldown == 0x00 */

	NVIC_ClearPendingIRQ(PORTA_IRQn);      /* Nuluj priznak preruseni od portu A  */
	NVIC_EnableIRQ(PORTA_IRQn);            /* Povol preruseni od portu A          */

    PORTA->PCR[6]  = PORT_PCR_MUX(0x01);
	PORTA->PCR[7]  = PORT_PCR_MUX(0x01);
	PORTA->PCR[8]  = PORT_PCR_MUX(0x01);
	PORTA->PCR[9]  = PORT_PCR_MUX(0x01);
	PORTA->PCR[10] = PORT_PCR_MUX(0x01);
	PORTA->PCR[11] = PORT_PCR_MUX(0x01);
	PORTA->PCR[24] = PORT_PCR_MUX(0x01);

	PORTD->PCR[8]  = PORT_PCR_MUX(0x01);
	PORTD->PCR[9]  = PORT_PCR_MUX(0x01);
	PORTD->PCR[12] = PORT_PCR_MUX(0x01);
	PORTD->PCR[13] = PORT_PCR_MUX(0x01);
	PORTD->PCR[14] = PORT_PCR_MUX(0x01);
	PORTD->PCR[15] = PORT_PCR_MUX(0x01);

    /* Change corresponding PT{A | D} port pins as outputs */
	PTA->PDDR = GPIO_PDDR_PDD( 0x1FC0 | TRIG_PIN );
	PTD->PDDR = GPIO_PDDR_PDD( 0xF300 );

	PTA->PDOR &= ~TRIG_PIN;
}

void showNumber(int index)
{
	switch(index)
	{
	case 0:
		PTD->PDOR &= ~FIRST_NUM;
		PTD->PDOR |= SECOND_NUM;
		PTD->PDOR |= THIRD_NUM;
		PTD->PDOR |= FOURTH_NUM;
		break;
	case 1:
		PTD->PDOR |= FIRST_NUM;
		PTD->PDOR |= SECOND_NUM;
		PTD->PDOR |= THIRD_NUM;
		PTD->PDOR &= ~FOURTH_NUM;
		break;
	case 2:
		PTD->PDOR |= FIRST_NUM;
		PTD->PDOR |= SECOND_NUM;
		PTD->PDOR &= ~THIRD_NUM;
		PTD->PDOR |= FOURTH_NUM;
		break;
	case 3:
		PTD->PDOR |= FIRST_NUM;
		PTD->PDOR &= ~SECOND_NUM;
		PTD->PDOR |= THIRD_NUM;
		PTD->PDOR |= FOURTH_NUM;
		break;
	}
}

void nulNumber()
{
	PTA->PDOR &= ~LEFT_DOWN;
	PTA->PDOR &= ~LEFT_UP;
	PTA->PDOR &= ~DOT;
	PTA->PDOR &= ~RIGHT_UP;
	PTA->PDOR &= ~BOTTOM;
	PTA->PDOR &= ~UP;
	PTD->PDOR &= ~RIGHT_DOWN;
	PTD->PDOR &= ~CENTER;
}

void printNumber(int num, int dot)
{
	nulNumber();

	/* dot == 0 -> 0-9       */
	/* dot == 1 -> 0.-9.     */
	/* -1       -> - (minus) */

	switch(num)
	{
	case -1:
		PTD->PDOR |= CENTER;
		break;
	case 0:
		PTA->PDOR |= UP;
		PTA->PDOR |= RIGHT_UP;
		PTD->PDOR |= RIGHT_DOWN;
		PTA->PDOR |= LEFT_DOWN;
		PTA->PDOR |= LEFT_UP;
		PTA->PDOR |= BOTTOM;
		break;
	case 1:
		PTA->PDOR |= RIGHT_UP;
		PTD->PDOR |= RIGHT_DOWN;
		break;
	case 2:
		PTA->PDOR |= UP;
		PTA->PDOR |= RIGHT_UP;
		PTD->PDOR |= CENTER;
		PTA->PDOR |= LEFT_DOWN;
		PTA->PDOR |= BOTTOM;
		break;
	case 3:
		PTA->PDOR |= UP;
		PTA->PDOR |= RIGHT_UP;
		PTD->PDOR |= CENTER;
		PTD->PDOR |= RIGHT_DOWN;
		PTA->PDOR |= BOTTOM;
		break;
	case 4:
		PTA->PDOR |= LEFT_UP;
		PTD->PDOR |= CENTER;
		PTA->PDOR |= RIGHT_UP;
		PTD->PDOR |= RIGHT_DOWN;
		break;
	case 5:
		PTA->PDOR |= UP;
		PTA->PDOR |= LEFT_UP;
		PTD->PDOR |= CENTER;
		PTD->PDOR |= RIGHT_DOWN;
		PTA->PDOR |= BOTTOM;
		break;
	case 6:
		PTA->PDOR |= LEFT_DOWN;
		PTA->PDOR |= LEFT_UP;
		PTA->PDOR |= BOTTOM;
		PTA->PDOR |= UP;
		PTD->PDOR |= RIGHT_DOWN;
		PTD->PDOR |= CENTER;
		break;
	case 7:
		PTA->PDOR |= UP;
		PTA->PDOR |= RIGHT_UP;
		PTD->PDOR |= RIGHT_DOWN;
		break;
	case 8:
		PTA->PDOR |= LEFT_DOWN;
		PTA->PDOR |= LEFT_UP;
		PTA->PDOR |= RIGHT_UP;
		PTA->PDOR |= BOTTOM;
		PTA->PDOR |= UP;
		PTD->PDOR |= RIGHT_DOWN;
		PTD->PDOR |= CENTER;
		break;
	case 9:
		PTA->PDOR |= LEFT_UP;
		PTA->PDOR |= RIGHT_UP;
		PTA->PDOR |= BOTTOM;
		PTA->PDOR |= UP;
		PTD->PDOR |= RIGHT_DOWN;
		PTD->PDOR |= CENTER;
		break;
	default:
		/* Unknown value */
		break;
	}

	/* Print dot */
	if (dot)
	{
		PTA->PDOR |= DOT;
	}
}

int containsDot(const char* s, int n)
{
	for (int i = 0; i <= n; i++)
	{
		if (s[i] == '.')
		{
			return 1;
		}
	}

	return 0;
}

void str2flt(char* s, float f)
{
	int tmpInt1   = f;
	float tmpFrac = f - tmpInt1;
	int tmpInt2   = tmpFrac * 1000;

	sprintf(s, "%d.%03d\0\n", tmpInt1, tmpInt2);
}

void printVal(float val)
{
	/* Can not display this number */
	if (val < 0. || val > 9999.)
	{
		// Don't print anything (Unprintable value)
		nulNumber();
		return;
	}

	/* We're able to display number */
	char valStr[10] = {};
	str2flt(valStr, val);

	if ((indexDisplay < strlen(valStr)) || (strlen(valStr) == 0))
	{
		showNumber(indexDisplay);
	}

	if (strlen(valStr) == 0) /* Value = 0, then print "0.0" */
	{
		if (indexDisplay == 0)
		{
			printNumber(0, 1); /* 0. */
		}
		else if (indexDisplay == 1)
		{
			printNumber(0, 0); /* 0  */
		}
	}
	else if (indexDisplay < strlen(valStr))
	{
		int dot = 0;

		if ((indexDisplay < 3) && (valStr[indexDisplay + 1] == '.'))
		{
			dot = 1;
		}

		if (containsDot(valStr, indexDisplay))
		{
			printNumber(valStr[indexDisplay + 1] - 48, dot);
		}
		else
		{
			printNumber(valStr[indexDisplay] - 48, dot);
		}
	}
}

void trigPulse()
{
	PTA->PDOR |= TRIG_PIN;

	delay(30);                // Vysli ultrazvuk. signal

	PTA->PDOR &= ~TRIG_PIN;
}

unsigned long long distance = 0;
int loop = 1;
float cm = 0.;

void echoPulse()
{
	loop = 1;
	distance = 0;

	while (loop)
	{
		distance++;           // Pocitej tiky procesoru dokud nedojde echo zpet
	}

	float us = distance * 0.36;  // Spocitej dobu nez se signal vratil zpet
	cm = us / 58;                // Spocitej vzdalenost
}

void PORTA_IRQHandler(void)
{
	if ((PORTA->ISFR & ECHO_PIN) && !(GPIOA_PDIR & ECHO_PIN))
	{
		loop = 0;             // Vypni cekani na echo
		PORTA->ISFR = ~0;     // Nuluj priznak preruseni
	}
}


//unsigned int counter = 1;

void LPTMR0_IRQHandler(void)
{
    LPTMR0_CMR = compare / 100;
    LPTMR0_CSR |=  LPTMR_CSR_TCF_MASK;

    printVal(cm);                // Tiskni na display
    indexDisplay++;

    if (indexDisplay == 4)
    {
    	indexDisplay = 0;
    }
}

void LPTMR0Init(int count)
{
    SIM_SCGC5 |= SIM_SCGC5_LPTIMER_MASK;

    LPTMR0_CSR &= ~LPTMR_CSR_TEN_MASK;     // Turn OFF LPTMR to perform setup

    LPTMR0_PSR = ( LPTMR_PSR_PRESCALE(0) // 0000 is div 2
                 | LPTMR_PSR_PBYP_MASK  // LPO feeds directly to LPT
                 | LPTMR_PSR_PCS(1)) ; // use the choice of clock

    LPTMR0_CMR = count;  // Set compare value

    LPTMR0_CSR =(  LPTMR_CSR_TCF_MASK   // Clear any pending interrupt (now)
                 | LPTMR_CSR_TIE_MASK   // LPT interrupt enabled
                );

    NVIC_EnableIRQ(LPTMR0_IRQn);

    LPTMR0_CSR |= LPTMR_CSR_TEN_MASK;   // Turn ON LPTMR0 and start counting
}



int main(void)
{
    MCUInit();
    PortsInit();
    LPTMR0Init(compare);

    while (1)
    {
		trigPulse();      // Vysli signal
		echoPulse();      // Prijmi signal
		delay(300000);    // Chvilku pockej
    }

    return 0;
}
