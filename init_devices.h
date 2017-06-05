//ICC-AVR application builder : 5/17/2017 6:20:15 AM
// Target : M16
// Crystal: 7.3728Mhz

#include <iom16v.h>
#include <macros.h>

/*******************************************/

unsigned char time_10ms_counter = 0;

unsigned char time_10ms_ok = 0;

//unsigned char time_5ms_counter = 0;

unsigned char time_5ms_ok = 0;


/*******************************************/

void port_init(void)
{
	PORTA |= (1 << 3);
	DDRA &= ~(1 << 3);
	//PORTB = 0x00;
	//DDRB  = 0x00;
	PORTC = 0xff; //m103 output only
	DDRC = 0xff;
	PORTD = 0x08;
	DDRD = 0x00;
}

//TIMER0 initialize - prescale:64
// WGM: CTC
// desired value: 2mSec
// actual value:  2.005mSec (-0.3%)
void timer0_init(void)
{
	TCCR0 = 0x00; //stop
	TCNT0 = 0x1A; //set count
	OCR0 = 0xE6; //set compare
				 // TCCR0 = 0x0B; //start timer
}

//TIMER2 initialize - prescale:256
// WGM: CTC
// desired value: 5mSec
// actual value:  5.035mSec (0.7%)
void timer2_init(void)
{
	TCCR2 = 0x00; //stop
	ASSR = 0x00; //set async mode
	TCNT2 = 0x70; //setup
	OCR2 = 0x90;
	TCCR2 = 0x0E; //start
}

//call this routine to initialize all peripherals
void InitDevices(void)
{
	//stop errant interrupts until set up
	CLI(); //disable all interrupts
	port_init();
	timer0_init();
	timer2_init();

	MCUCR = 0x08;
	GICR = 0x80;
	TIMSK = 0x82; //timer interrupt sources
	SEI(); //re-enable interrupts
		   //all peripherals are now initialized
}

