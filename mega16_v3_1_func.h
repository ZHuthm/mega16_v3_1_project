#ifndef _MEGA16_V3_1_FUNC_H_
#define _MEGA16_V3_1_FUNC_H_

void DelayMs(unsigned int);

    #include "time.h"

    #include "led.h"

    #include "temperature.h"
	
	#include "settings.h"


void DelayMs(unsigned int mSec)
{
   int i;
   while(mSec--)
       for(i=0; i<(187*2); i++);
}

void AboutSystem(void)
{
	LcdCls();

	LcdWriteEnglishString(0,1,0,"#============#");
	LcdWriteEnglishString(0,2,0,"# Author:CZH #");
	LcdWriteEnglishString(0,3,0,"#Date:2017/05#");
	LcdWriteEnglishString(0,4,0,"#============#");
		
	while(ReadAdKey()!=ESC);
	DelayMs(50);
}

/*--------------------------------

								  

--------------------------------*/

#endif