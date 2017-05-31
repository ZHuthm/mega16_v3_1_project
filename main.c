#include "init_devices.h"
#include "mega16_v3_1_ad_key.h"
#include "mega16_v3_1_lcd3310.h"
#include "mega16_v3_1_7seg.h"
#include "mega16_v3_1_menu.h"


//# define Reset    INT1 

/*******************************************/

const struct MenuItem* menu_point = main_menu ;

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
	
	if(menu_point == stopwatch_menu ) // �����stopwatch�˵����ÿ��1s����
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
	
	if( (PORTD & 0xf0) != 0xf0 )	// ֻҪ���������������һֱɨ��
	{
		Disp_1_digit(posit,DP_flag);
		if(++posit>=4) posit = 0;
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

void ShowMenu(void)
{
	unsigned char i = 0, times = 0;

	
	LcdCls();
	
	LcdWriteEnglishString(0, 0, 0, menu_point[0].disp_title); // display TITLE
	//LcdWriteEnglishString(0, 1, 0, "=============="); // display TITLE
	
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
	unsigned char refresh_flag = 1;

	InitSystem();

	ShowMenu();

	while (1)
	{
		if (time_10ms_ok)
		{
			time_10ms_ok = 0;

			refresh_flag = 1;
			switch (ReadAdKey())
			{
			
			case NO_ACTION:
				refresh_flag = 0;
				break;

			case UP:
				--user_choice;
				if (user_choice < disp_start) disp_start--;		// �û�ѡ��С�ڴ���ָ��
				if (user_choice >= max_items) {					// �������
					user_choice = max_items - 1;				// �û�ѡ��ص����һ��
					if(max_items > DISPLAY_ITEMS) disp_start = max_items - DISPLAY_ITEMS;
					else disp_start = 0;
				}
				break;

			case DOWN:
				++user_choice;
				if (user_choice >= disp_start + DISPLAY_ITEMS) disp_start++;
				if (user_choice >= max_items) {
					user_choice = 0;
					disp_start = 0;
				}
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
					(*menu_point[user_choice].Subs)();
				else if (menu_point[user_choice].children_menus != NULL)
				{
					menu_point = menu_point[user_choice].children_menus;
					user_choice = 0;
					disp_start = 0;
				}
				
								
				break;

			case ESC:
				if (menu_point[0].parent_menus != NULL)
				{
					menu_point = menu_point[0].parent_menus;
					user_choice = 0;
					disp_start = 0;
					
					/*----- Select feedback animation -----*/
					LcdWriteEnglishString(60, 5, 1, "ESC");
					DelayMs(500);
				}
				
				break;
				
			default : break;

			}
			if (refresh_flag)	ShowMenu();
		}
	}

	return ;
}