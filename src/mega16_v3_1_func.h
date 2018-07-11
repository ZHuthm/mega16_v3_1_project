#ifndef _MEGA16_V3_1_FUNC_H_
#define _MEGA16_V3_1_FUNC_H_

void DelayMs(unsigned int);

    #include "func_time.h"

    #include "func_led.h"

    #include "func_temperature.h"
	
	#include "func_settings.h"


void DelayMs(unsigned int mSec)
{
   int i;
   while(mSec--)
       for(i=0; i<(187*2); i++);
}

void AboutSystem(void)
{
	LcdCls();

	LcdWriteEnglishString(0, 1, 0, "#============#");
	LcdWriteEnglishString(0, 2, 0, "# Author:CZH #");
	LcdWriteEnglishString(0, 3, 0, "#Date:2017/05#");
	LcdWriteEnglishString(0, 4, 0, "#============#");
	LcdWriteEnglishString(12, 5, 0, "KEY5 to ESC");
		
	while(ReadAdKey()!=ESC);
	
	/*----- Select feedback animation -----*/
	LcdWriteEnglishString(60, 5, 1, "ESC");
	PORTA &= ~(1 << 3);
	DelayMs(15);
	PORTA |= (1 << 3);
	DelayMs(500);
}

/*--------------------------------

								  

--------------------------------*/

#endif