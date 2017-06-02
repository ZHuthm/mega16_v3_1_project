#ifndef _MEGA16_V3_1_DS18B20_H_
#define _MEGA16_V3_1_DS18B20_H_

#define INPUT_DQ()		 DDRA &= ~(1 << 2)		
#define OUTPUT_DQ()		 DDRA |= (1 << 2)		
#define CLR_DQ()		 PORTA &= ~(1 << 2)		
#define SET_DQ()		 PORTA |= (1 << 2)		
#define READ_DQ()		 PINA & (1 << 2)		

#define DS18B20_READ_ROM			0x33
#define DS18B20_MATCH_ROM			0X55
#define DS18B20_SKIP_ROM			0XCC
#define DS18B20_SEARCH_ROM			0XF0
#define DS18B20_ALARM_SEARCH_ROM	0XEC
#define DS18B20_WRITE_RAM			0X40
#define DS18B20_READ_RAM			0XBE
#define DS18B20_COPY_RAM			0X48 
#define DS18B20_CONVERT_TEM			0X44
#define DS18B20_EECALL_EEPROM		0XB8
#define DS18B20_READ_POWER_SUPPLY	0XB4


/******************************************************/
// Set the 1-Wire timing to 'standard' or 'overdrive'
// Adjust tick values depending on speed

// Standard Speed
const unsigned int  A = 6;
const unsigned int	B = 64;
const unsigned int  C = 60;
const unsigned int  D = 10;
const unsigned int  E = 9;
const unsigned int  F = 55;
const unsigned int  G = 0;
const unsigned int	H = 480;
const unsigned int	I = 70;
const unsigned int	J = 410;

// Overdrive Speed
/*
const unsigned char A = 1.5 ;
const unsigned char B = 7.5 ;
const unsigned char C = 7.5 ;
const unsigned char D = 2.5 ;
const unsigned char E = 0.75;
const unsigned char F = 7	;
const unsigned char G = 2.5 ;
const unsigned int	H = 70	;
const unsigned char I = 8.5 ;
const unsigned char J = 40	;
*/
/******************************************************/

// Pause for exactly ‘tick’ number of ticks = 3us 
// Implementation is platform specific 
void tickDelay(unsigned int tick)
{
	while (tick--){NOP();NOP();NOP();NOP();}
}

unsigned char InitDs18b20(void)
{
	unsigned char status;
	
	OUTPUT_DQ();
	
	SET_DQ();	  		
	
	tickDelay(G);
	
	CLR_DQ();	 		 // Bus master active low
	
	tickDelay(H);
	
	SET_DQ();	 		 // release BUS	
	
	INPUT_DQ();	
	
	tickDelay(I);
	
	status = READ_DQ();	 
	
	tickDelay(J);
	
	OUTPUT_DQ();
	
	SET_DQ();
	
	return status;	// Successful initialization returns 0
}


//-----------------------------------------------------------------------------
// Send a 1-Wire write byte. Provide 10us recovery time.
//
void ds18b20WriteByte(unsigned char byte)
{
	unsigned char i;

	for(i=0;i<8;i++)
	{		
		if(byte & (1<<i)) 
		{
			// write 1
			OUTPUT_DQ();
			CLR_DQ();		// Drives DQ low
			tickDelay(A);
			
			INPUT_DQ();
			SET_DQ();		// Releases the bus
			tickDelay(B);	// Complete the time slot and 10us recovery
		}
		else
		{
			 // write 0
			 OUTPUT_DQ();
			 CLR_DQ();
			 tickDelay(C);
			 
			 INPUT_DQ();
			 SET_DQ();
			 tickDelay(D);			 
		}
	}
}

unsigned char ds18b20ReadBit(void)
{
		unsigned char result;
		
		OUTPUT_DQ();
		CLR_DQ();
		tickDelay(A);
		
		INPUT_DQ();
		SET_DQ();
		tickDelay(E);
		
		result = READ_DQ();
		
		tickDelay(F);
		
		return result;
}

unsigned char ds18b20ReadByte(void)
{
	unsigned char i,result;
	for(i=0;i<8;i++)
	{
		result >>= 1;
		
		if(ds18b20ReadBit())
			result |= 0x80;
	}
	return result;
}

void Delay_ms(unsigned int mSec)
{
	int i;
	while (mSec--)
		for (i = 0; i<(187 * 2); i++);
}

unsigned char ds18b20ConvertTemp(unsigned char *temp)
{

	if (InitDs18b20()!=0x00) return 1; // initialize failed

	ds18b20WriteByte(DS18B20_SKIP_ROM);

	ds18b20WriteByte(DS18B20_CONVERT_TEM);

	Delay_ms(800);

	if (InitDs18b20()!=0x00) return 1; // initialize failed

	ds18b20WriteByte(DS18B20_SKIP_ROM);

	ds18b20WriteByte(DS18B20_READ_RAM);
	
	temp[0] = ds18b20ReadByte();

	temp[1] = ds18b20ReadByte();
	
	return 0; // successfully
}

char GetTemperature(unsigned char *t)
{	

	char ret;
	unsigned long val;
	unsigned int temp = (t[1] * 256) + t[0];
	unsigned char sflag = 0;

	if ((t[1] & 0xf8) == 0xf8)
	{
		sflag = 1;
		temp = ~temp;
		temp++;
	}
	temp &= 0x07ff;

	val = ((unsigned long)temp) * 625;
	val /= 10000;

	ret = (char)val;

	if (sflag)
		ret |= 0x80;
	
	return ret;
	
}

/*
unsigned char tmp[2];//保存温度字节
char tval;    //保存温度值

if((ds18b20ConvertTemp(tmp) & (1<<0)) == 0)//如果转换成功
    {
       tval=GetTemperature(tmp);//计算实际温度值
      if(tval>=0)
        //to do...
		//十进制显示温度值
      else
        //to do...
		//数码管无法显示负数,只能显示0
    }
*/

#endif