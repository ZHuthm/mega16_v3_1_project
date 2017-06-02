#ifndef _SETTINGS_H_
#define _SETTINGS_H_

unsigned char buzz_status = 0;
//unsigned char tc_status = 0;

void ShowSetVolumeInterface(void)
{
	LcdCls();

	LcdWriteEnglishString(0, 0, 0, "VOLUME SETTING");

	LcdWriteEnglishString(0, 1, 0, "==============");

	LcdWriteEnglishString(0, 3, 0, " Status:");

	if (buzz_status)
		LcdWriteEnglishString(48, 3, 0, "Sound");
	else
		LcdWriteEnglishString(48, 3, 0, "Silent");

	LcdWriteEnglishString(6, 5, 0, "SAVE");

	LcdWriteEnglishString(60, 5, 0, "ESC");
}

void SetVolume(void)
{
	unsigned char key_status, buzz_status_tmp;

	ShowSetVolumeInterface();

	buzz_status_tmp = buzz_status;

	while (1)
	{
		
		key_status = ReadAdKey();

		switch (key_status)
		{

		case UP:
			if (!buzz_status_tmp) {
				LcdWriteEnglishString(48, 3, 0, "      ");
				LcdWriteEnglishString(48, 3, 1, "Sound");
				buzz_status_tmp = 1;
			}
			break;


		case DOWN:
			if (buzz_status_tmp) {
				LcdWriteEnglishString(48, 3, 0, "      ");
				LcdWriteEnglishString(48, 3, 1, "Silent");
				buzz_status_tmp = 0;
			}
			break;


		case ENTER:
			buzz_status = buzz_status_tmp;

			if (buzz_status) 
				DDRA |= (1 << 3);
			else 
				DDRA &= ~(1 << 3);

			/*----- Select feedback animation -----*/
			LcdWriteEnglishString(6, 5, 1, "SAVE");;
			DelayMs(500);

			ShowSetVolumeInterface();

			break;


		case ESC:
			/*----- Select feedback animation -----*/
			LcdWriteEnglishString(60, 5, 1, "ESC");
			DelayMs(500);

			return; break;
			
		}
	}
}

void  SetTc(void)//Temperature Coefficient
{	
	
}

#endif