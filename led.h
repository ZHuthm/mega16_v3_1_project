#ifndef _LED_H_
#define _LED_H_

unsigned char led_state[8] = { 1,1,1,1,1,1,1,1 };

void LightenAll(void)
{
	unsigned char i;
	PORTC = 0x00;
	for (i = 0; i < 8; i++)
		led_state[i] = 0;
}

void LightOffAll(void)
{
	unsigned char i;
	PORTC = 0xff;
	for (i = 0; i < 8; i++)
		led_state[i] = 1;
}

void LeftShift(void)
{
	unsigned char i;

	LcdCls();

	LcdWriteEnglishString(18, 2, 0, "SHOWING...");

	for (i = 0; i < 8; i++)
	{
		PORTC = ~(1 << i);
		DelayMs(500);
	}

	PORTC = 0xff;
	for (i = 0; i < 8; i++)
		if (!led_state[i])  PORTC &= ~(1 << (7 - i));

}


void RightShift(void)
{
	unsigned char i;

	LcdCls();

	LcdWriteEnglishString(18, 2, 0, "SHOWING...");

	for (i = 7; i != 255; i--)
	{
		PORTC = ~(1 << i);
		DelayMs(500);
	}

	PORTC = 0xff;
	for (i = 0; i < 8; i++)
		if (!led_state[i])  PORTC &= ~(1 << (7 - i));
}


void CustomizeLed(void)
{
	unsigned key_state, cursor = 0;
	unsigned i, j, k;

	LcdCls();
	
	LcdWriteEnglishString(0,0,0," Customize Led");

	while (1)
	{
		if (time_10ms_ok)
		{
			time_10ms_ok = 0;

			key_state = ReadAdKey();

			switch (key_state)
			{
			case ESC:
				return;
				break;

			case UP:
				led_state[cursor] = 0;
				PORTC &= ~(1 << (7 - cursor));
				LcdCls();
				break;

			case DOWN:
				led_state[cursor] = 1;
				PORTC |= 1 << (7 - cursor);
				break;

			case LEFT:
				if (--cursor >= 8) cursor = 7;
				break;

			case RIGHT:
				if (++cursor >= 8) cursor = 0;
				break;
			}

			for (i = 3; i < 11; i++)
			{
				j = i * 6;
				k = !(cursor - (i - 3));
				
				if (led_state[i - 3])
				{
					LcdWriteEnglishString(j, 2, k, "O");
					LcdWriteEnglishString(j, 3, k, "F");
					LcdWriteEnglishString(j, 4, k, "F");
				}
				else
				{
					LcdWriteEnglishString(j, 2, k, "O");
					LcdWriteEnglishString(j, 3, k, "N");
				}
			}
			
		}
	}
	ReadAdKey();
}


#endif