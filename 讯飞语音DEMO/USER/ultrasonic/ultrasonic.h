#ifndef _ULTRASONIC_H
#define _ULTRASONIC_H
#include "includes.h"
void  time_init(void);
u32 UltrasonicWave_StartMeasure(void);
extern u16 UltrasonicWave_Distance; 
#endif