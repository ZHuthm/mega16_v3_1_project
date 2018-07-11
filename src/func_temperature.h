#ifndef _TEMPERATURE_H_
#define _TEMPERATURE_H_

#include "mega16_v3_1_ds18b20.h"

void ShowTempInterface(void)
{
	LcdCls();

	LcdWriteEnglishString(0, 0, 0, " CURRENT TEMP");

	LcdWriteEnglishString(0, 1, 0, "==============");

	LcdWriteEnglishString(18, 3, 0, "Temp: ");

	LcdWriteEnglishString(12, 5, 0, "KEY5 to ESC");
}

void ShowTemp(void)
{
	unsigned char tmp[2];//保存温度字节

	unsigned char show_tmp[3];

	char tval;    //保存温度值

	ShowTempInterface();

	if ((InitDs18b20() & (1 << 0)) != 0)
	{
		LcdWriteEnglishString(0, 3, 1, " INIT ERROR!!");
		DelayMs(5000);
		return;
	}

	while (1)
	{
		if (time_1s_ok)
		{
			time_1s_ok = 0;

			if ((ds18b20ConvertTemp(tmp) & (1 << 0)) == 0)//如果转换成功
			{
				tval = GetTemperature(tmp);//计算实际温度值

				TempToStr(tval, show_tmp);

				LcdWriteEnglishString(48, 3, 0, "     ");

				LcdWriteEnglishString(54, 3, 0, show_tmp); //十进制显示温度值
			}
			else
				LcdWriteEnglishString(48, 3, 1, "ERROR");
		}
		if (ReadAdKey() == ESC)
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