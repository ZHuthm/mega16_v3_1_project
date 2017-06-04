#ifndef _TEMPERATURE_H_
#define _TEMPERATURE_H_

#include "mega16_v3_1_ds18b20.h"

void ShowTempInterface(void)
{
	LcdCls();

	LcdWriteEnglishString(0, 0, 0, " CURRENT TEMP");

	LcdWriteEnglishString(0, 1, 0, "==============");

	LcdWriteEnglishString(18, 3, 0, "Temp: ");

	LcdWriteEnglishString(12, 5, 0, "hold to ESC");
}

void ShowTemp(void)
{
	unsigned char tmp[2];//保存温度字节

	char tval;    //保存温度值

	ShowTempInterface();

	while (1)
	{

		if ((ds18b20ConvertTemp(tmp) & (1 << 0)) == 0)//如果转换成功
		{
			tval = GetTemperature(tmp);//计算实际温度值

			if (tval >= 0) {
				
				tmp[1] = tval % 10 + '0';
				tmp[0] = tval % 100 / 10 + '0';
				LcdWriteEnglishString(48, 3, 0, "     ");
				LcdWriteEnglishString(54, 3, 0, tmp); //十进制显示温度值
			}
			else 
				//数码管无法显示负数,只能显示0
				//to do...
				LcdWriteEnglishString(48, 3, 1, "ERROR");				
		}
		else
				LcdWriteEnglishString(48, 3, 1, "ERROR");
				

		//DelayMs(1000);
		if(ReadAdKey() == ESC) 
			break;

	}
	
	/*----- Select feedback animation -----*/
	LcdWriteEnglishString(60, 5, 1, "ESC");
	PORTA &= ~(1 << 3);
	DelayMs(15);
	PORTA |= (1 << 3);
	DelayMs(500);
}


#endif