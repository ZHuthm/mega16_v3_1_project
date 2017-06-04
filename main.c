#include "init_devices.h"
#include "mega16_v3_1_ad_key.h"
#include "mega16_v3_1_lcd3310.h"
#include "mega16_v3_1_7seg.h"
#include "mega16_v3_1_menu.h"


//# define Reset    INT1 

/*******************************************/

const struct MenuItem* menu_point = main_menu;

unsigned char user_choice = 0;

unsigned char disp_start = 0;

unsigned char disp_point = 0;

unsigned char max_items;


/*******************************************/

void InitSystem(void);

void ShowMenu(void);



/*******************************************/

#pragma interrupt_handler timer0_comp_isr:iv_TIM0_COMP
void timer0_comp_isr(void)
{
	/*----- compare occured TCNT0=OCR0 -----*/
	timing();

	if (menu_point == stopwatch_menu) // 如果在stopwatch菜单里，则每隔1s响铃
	{
		if (ms == 0) PORTA &= ~(1 << 3);
		if (ms == 20) PORTA |= (1 << 3);
	}


}

#pragma interrupt_handler timer2_comp_isr:iv_TIM2_COMP
void timer2_comp_isr(void)
{

	/*----- compare occured TCNT2=OCR2 -----*/

	if (++time_10ms_counter >= 2) {
		time_10ms_counter = 0;
		time_10ms_ok = 1;  	 // 10ms is come 	 	
	}

	if ((PORTD & 0xf0) != 0xf0)	// 只要有数码管在亮，则一直扫描
	{
		Disp_1_digit(posit, DP_flag);
		if (++posit >= 4) posit = 0;
	}

}

#pragma interrupt_handler int1_isr:iv_INT1
void int1_isr(void)
{
	/*----- external interupt on INT1 -----*/

	// !! need to be finished
	/*
	menu_point = main_menu;

	user_choice = 0;

	disp_start = 0;

	ShowMenu();
	*/
}



/*******************************************/

void InitSystem(void)
{

	InitDevices();

	InitAdKeyDevices();

	InitLcdDevices();

	InitSegDevices();

}

void ShowWelcomeInterface(void)
{
	LcdWriteEnglishString(6, 0, 0, "UP   > KEY 3 ");
	LcdWriteEnglishString(6, 1, 0, "DOWN > KEY 2 ");
	LcdWriteEnglishString(6, 2, 0, "LEFT > KEY 4 ");
	LcdWriteEnglishString(6, 3, 0, "RIGHT> KEY 1 ");
	LcdWriteEnglishString(6, 4, 0, "ENTER> KEY 6 ");
	LcdWriteEnglishString(6, 5, 0, "ESC  > KEY 5 ");
}

void UtilityWizards(void)
{
	unsigned char u, d, l, r, en, es;
	u = d = l = r = en = es = 0;

	ShowWelcomeInterface();
	while (!(u&d&l&r&en&es))
	{
		if (time_10ms_ok)
		{
			time_10ms_ok = 0;

			switch (ReadAdKey())
			{
			case UP:
				u = 1;
				LcdWriteEnglishString(42, 0, 1, " KEY 3 ");
				break;

			case DOWN:
				d = 1;
				LcdWriteEnglishString(42, 1, 1, " KEY 2 ");
				break;

			case LEFT:
				l = 1;
				LcdWriteEnglishString(42, 2, 1, " KEY 4 ");
				break;

			case RIGHT:
				r = 1;
				LcdWriteEnglishString(42, 3, 1, " KEY 1 ");
				break;

			case ENTER:
				en = 1;
				LcdWriteEnglishString(42, 4, 1, " KEY 6 ");
				break;

			case ESC:
				es = 1;
				LcdWriteEnglishString(42, 5, 1, " KEY 5 ");
				break;

			default:
				break;
			}

		}
	}
	
	DelayMs(1000);
	
	LcdCls();
	
	LcdWriteEnglishString(18, 2, 0, "Welcome!");
	
	DelayMs(3000);
	
}



void ShowMenu(void)
{
	unsigned char i = 0, times = 0;


	LcdCls();

	LcdWriteEnglishString(0, 0, 0, menu_point[0].disp_title); // display TITLE


	disp_point = disp_start;

	max_items = menu_point[0].menu_count;

	if (max_items >= DISPLAY_ITEMS)
		times = 4;
	else
		times = max_items;

	for (i = 1; i <= times; i++)
	{

		if (disp_point == user_choice)
			LcdWriteEnglishString(0, i, 1, menu_point[disp_point].disp_str);
		else
			LcdWriteEnglishString(0, i, 0, menu_point[disp_point].disp_str);

		++disp_point;

	}


	LcdWriteEnglishString(2, 5, 0, "ENTER");
	LcdWriteEnglishString(60, 5, 0, "ESC");

}



/*****************************************************/

void main(void)
{
	unsigned char refresh_flag = 1;		// 刷新标志位，有显示变化为1

	unsigned char action_flag = 0;

	InitSystem();

	UtilityWizards();

	ShowMenu();

	while (1)
	{
		if (time_10ms_ok)
		{
			time_10ms_ok = 0;

			action_flag = 0;

			refresh_flag = 1;

			switch (ReadAdKey())
			{

			case NO_ACTION:
				refresh_flag = 0;
				break;

			case UP:
				--user_choice;
				if (user_choice < disp_start) disp_start--;		// 用户选项小于窗口指针
				if (user_choice >= max_items) {					// 反向溢出
					user_choice = max_items - 1;				// 用户选项回到最后一个
					if (max_items > DISPLAY_ITEMS) disp_start = max_items - DISPLAY_ITEMS;
					else disp_start = 0;
				}
				action_flag = 1;
				break;

			case DOWN:
				++user_choice;
				if (user_choice >= disp_start + DISPLAY_ITEMS) disp_start++;
				if (user_choice >= max_items) {
					user_choice = 0;
					disp_start = 0;
				}
				action_flag = 1;
				break;

			case LEFT:
				// to do ...
				//user_choice -= 4;
				//if (user_choice >= max_items) user_choice = max_items - 1;
				break;

			case RIGHT:
				// to do ... 
				//user_choice += 4;
				//if (user_choice >= max_items) user_choice = 0;
				break;

			case ENTER:

				/*----- Select feedback animation -----*/
				LcdWriteEnglishString(2, 5, 1, "ENTER");
				DelayMs(500);


				if ((menu_point[user_choice].Subs) != (NullSubs))
				{
					PORTA &= ~(1 << 3);
					DelayMs(25);
					PORTA |= (1 << 3);

					(*menu_point[user_choice].Subs)();

				}
				else if (menu_point[user_choice].children_menus != NULL)
				{
					menu_point = menu_point[user_choice].children_menus;
					user_choice = 0;
					disp_start = 0;
					action_flag = 1;
				}


				break;

			case ESC:

				if (menu_point[0].parent_menus != NULL)
				{
					menu_point = menu_point[0].parent_menus;
					user_choice = 0;
					disp_start = 0;
					action_flag = 1;
				}

				/*----- Select feedback animation -----*/
				LcdWriteEnglishString(60, 5, 1, "ESC");
				DelayMs(500);

				break;

			default: break;

			}

			if (refresh_flag)		
				ShowMenu();

			if ( action_flag ) 
			{
				PORTA &= ~(1 << 3);
				DelayMs(15); 
				PORTA |= (1 << 3);
			}
			//else if ((PORTA & (1 << 3)) == 0 ) PORTA |= (1 << 3);

		}
	}

	return;
}