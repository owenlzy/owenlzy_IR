#ifndef __BEEP_H
#define __BEEP_H
#include "includes.h"
//定义低音部分	HZ

#define L1 262 
#define L2 294 
#define L3 330 
#define L4 349 
#define L5 392 
#define L6 440 
#define L7 494 
//定义中音部分
#define M1 523 
#define M2 587 
#define M3 659 
#define M4 698 
#define M5 784 
#define M6 880 
#define M7 988 
//定义高音部分
#define H1 1047 
#define H2 1175 
#define H3 1319 
#define H4 1397 
#define H5 1568 
#define H6 1760 
#define H7 1976 
//定义时值单位，决定演奏速度 ms
#define TT 2000

typedef struct {
	short mName;	//音名 取值L1~L7 , M1~M7 , H1~H7，取0表示休止符
	short mTime;	//时值 取值T、T/2、T/4、T/8、T/16、T/32表示全音符、二分音符、四分音符、八分音符，取0表示演奏结束
}tNote;


void playMusic(tNote *Name);
void buzzerSound(unsigned short usFreq);
void buzzerQuiet(void) ;
void beep(unsigned short usFreq);


#endif