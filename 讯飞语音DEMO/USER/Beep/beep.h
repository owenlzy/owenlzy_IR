#ifndef __BEEP_H
#define __BEEP_H
#include "includes.h"
//�����������	HZ

#define L1 262 
#define L2 294 
#define L3 330 
#define L4 349 
#define L5 392 
#define L6 440 
#define L7 494 
//������������
#define M1 523 
#define M2 587 
#define M3 659 
#define M4 698 
#define M5 784 
#define M6 880 
#define M7 988 
//�����������
#define H1 1047 
#define H2 1175 
#define H3 1319 
#define H4 1397 
#define H5 1568 
#define H6 1760 
#define H7 1976 
//����ʱֵ��λ�����������ٶ� ms
#define TT 2000

typedef struct {
	short mName;	//���� ȡֵL1~L7 , M1~M7 , H1~H7��ȡ0��ʾ��ֹ��
	short mTime;	//ʱֵ ȡֵT��T/2��T/4��T/8��T/16��T/32��ʾȫ�����������������ķ��������˷�������ȡ0��ʾ�������
}tNote;


void playMusic(tNote *Name);
void buzzerSound(unsigned short usFreq);
void buzzerQuiet(void) ;
void beep(unsigned short usFreq);


#endif