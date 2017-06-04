#ifndef _TIME_H_
#define _TIME_H_


unsigned int ms = 0;		// millisecond
unsigned char sec = 0;		// second
unsigned char mins = 0;		// minutes

unsigned char time_flag = 0;

void timing(void)
{
	ms++;
	
	if (ms == 250)
	
		DP_flag = ~DP_flag;
		
	else if (ms >= 500)
		{
			ms = 0;

			DP_flag = ~DP_flag;

			if (++sec >= 60)
			{
				sec = 0;

				if (++mins >= 60)	mins = 0;
			}

		}
	
	
	if (mins == 0) num_to_dispbuff(sec * 100 + ms * 2 / 10);
	else num_to_dispbuff(mins * 100 + sec);
}

void StopWatch(void)
{
	if(time_flag == 0)		// == 0 表示正处于静止模式，触发后开始计时
	{
		time_flag = 1;
		
		PORTD &= ~0xf0;
	
		TCCR0 = 0x0B; //start timer0
		
	}
	else				// != 0 表示正处于计时模式，触发后进入静止模式
	{
		time_flag = 0;
		
		TCCR0 = 0x00; // stop timer0
	}
	
	//PORTD |= 0xf0;
	//DelayMs(500);		
	
}


void ResetTime(void)
{
	ms = 0;
	
	sec = 0;
	
	mins = 0;
	
	PORTD |= 0xf0;
	
	time_flag = 0;		
	
	TCCR0 = 0x00; // stop timer0
}

#endif