#ifndef _CHINESE_12x12_H_
#define	_CHINESE_12x12_H_

typedef struct 
{
	unsigned char value[2];
	unsigned char matrix[24];
}Unicode_12;

extern const Unicode_12 chinese_12[];
extern const unsigned char chinese_12_len;


#endif // !_CHINESE_12x12_H_