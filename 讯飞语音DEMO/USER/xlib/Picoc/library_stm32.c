/* ͷ�ļ�		--------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "interpreter.h"
#include "BSP.h"
#include "picoc.h"
#include "library_stm32.h"
#include "led.h"
#include "motor.h"
#include "human.h"
#include "servohand.h"
#include "ultrasonic.h"
#include "distance.h"
#include "74h595.h"
#include "digitron.h"
#include "LCD12864G_086P.h"
#include "led_ws2812s.h"
#include "gpio.h"
#include "FIFO.h"


/* ��������     --------------------------------------------------------------*/
/* �궨�� 	    --------------------------------------------------------------*/
#define CMM_LEN 8
#define CCODE_LEN 210
/* ���� 	   	 	--------------------------------------------------------------*/
extern OS_EVENT  	*Str_Q_SendAck ;
extern OS_EVENT     *Str_Q_XFTTS;
extern OS_EVENT     *Str_Q_Digitron;
extern int 			backvalue, motorDebug;
extern u16          lightStatus ,humanStatus;
extern u8           infraredStatus ;
extern u16          micStatus[3] ;
extern double       voltageStatus ;
extern int          wifiConnectStatus  ; 
extern char         wifiSSID[16];
extern int          isOpenAvoidStyle;
extern int          move_adjust;
extern int          turn_adjust;
STRU_Digitron       str_dig ={0};
/* ��������     --------------------------------------------------------------*/


void PlatformSetupFunc()
{ //������������Ҫ���ú궨��   
	//VariableDefinePlatformVar(NULL, "GPIO_Pin_5", &FPType, (union AnyValue *)&M_PI_2Value, FALSE);
}

/**
  * @����: ��ʼ����ִ��C����
  * @����
  * @����ֵ
  */
void CPicoc(char * CSource)
{
	char *pos;
	/**  test picoc excute STM32_C code whether success  **/
	char *SourceStr = CSource ;

	IncludeInit();
	PicocInitialise(HEAP_SIZE);
	PicocIncludeAllSystemHeaders();		//try open there,now
	if (SourceStr)
	{
		for (pos = SourceStr; *pos != 0; pos++)
		{
				if (*pos == 0x1a)
				{
						*pos = 0x20;
				}
		}
	}
	PicocExitBuf[40] = 0;
	setjmp(PicocExitBuf);
	if (PicocExitBuf[40]) 
	{
		printf("Leaving PicoC\n\r");
		PicocCleanup();
//		ReturnValue->Val->Integer = PicocExitValue;
	}
	if (SourceStr)   
		PicocParse("NoFile", SourceStr, strlen(SourceStr), TRUE, TRUE, FALSE);
	
//	PicocParseInteractive();
	PicocCleanup();
//	ReturnValue->Val->Integer = PicocExitValue;
	
}
/**
  * @���ܣ���������Ľű�������ȡC�������C��������ִ��
  */
void ParseStoryScript(char * storyScript)
{
	char *ssch ;
	int tempnum=0 , codeindex = 0 ;
	char cmd[CMM_LEN]={0},content[CCODE_LEN]={0} ;

	ssch = storyScript ;

	while( *(ssch+codeindex) != '\0')
	{		
		codeindex += 1 ;
		if( *(ssch+codeindex-1) == '<')		//
		{
			while(*(ssch+codeindex) != '@' )	// commd endwith @ 
			{
				cmd[tempnum++] = *(ssch+codeindex) ;
				codeindex += 1 ;
			}
			tempnum = 0 ;
		}
		if ( *(ssch+codeindex) == '@' )	//C code startwith @ 
		{
			codeindex += 1 ;
			while( *(ssch+codeindex) != '$')	//C code endwith $
			{	
				content[tempnum++] = *(ssch + codeindex) ;
				codeindex += 1 ;
			}
			if (codeindex >= CCODE_LEN )
			{
				printf("\r\n>-----C Code Length Too Long -----<\r\n");
				memset(cmd , 0 , CMM_LEN);
				memset(content , 0 , CCODE_LEN);
				tempnum = 0 ;	
				break;
			}
			CPicoc(content);		//excute C code use Picoc
			memset(cmd , 0 , CMM_LEN);		//clear data
			memset(content , 0 , CCODE_LEN);
			tempnum = 0 ;			
		}
	}
	codeindex = 0 ;ssch = NULL ;
}
int sound(int flag ,char *cstr)
{
    char strs[260] = {0} ;
    if(flag<=0)
        flag = 0;
    else
        flag = 1;

    memset(strs,0,260);
    strcat(strs,"FFF_");
    
    if(strlen(cstr)>256)
        return -1;
    
    if(*(cstr) > 'A' && *(cstr)<'z')
    {
        strcat(strs,"[h0]");
        strcat(strs,cstr);
    }else{
        strcat(strs,cstr);
    }
	if(strs[4]!='0')
	{	
        while(flag&&GetXFStatus())
            OSTimeDlyHMSM(0, 0, 0, 100);
        //������Ϣ��Ѷ�ɵ���Ϣ������ȥ
		OSQPost(Str_Q_XFTTS,strs);
        //printf("\r\n\n***********3222222**say(%s)***************\r\n",strs);
		return 1;
	}else
		return -1;
}

/**
*			MOVE API
*			��ֻ��һ������ int speed =100 ȫ�٣�<0 ��ת
*/
void CForward (struct ParseState *Parser, struct Value *ReturnValue, 
									struct Value **Param, int NumArgs)
{
	unsigned char speed = Param[0]->Val->Character ;
	unsigned char distance = Param[1]->Val->Character;
    
    if(motorDebug){
        backvalue = 1;
        return ;   
    }        
	if(speed>=0)
	{   
        if(isOpenAvoidStyle)
        {
            if(infraredStatus < 40)
            {
                car_left_senior(100);
                if(infraredStatus > 40)
                    car_back_senior(speed,distance);
                else{
                     car_right_senior(190);
                     if(infraredStatus > 40)
                        car_back_senior(speed,distance);
                }
            }else
                car_back_senior(speed,distance);
        }else
            car_back_senior(speed,distance);
		backvalue = 1;
		//printf("\r\n>CForward(%d)\r\n",distance);
	}else
		backvalue = -1;
}
void CBack (struct ParseState *Parser, struct Value *ReturnValue, 
									struct Value **Param, int NumArgs)
{
	unsigned char speed = Param[0]->Val->Character ;
	unsigned char distance = Param[1]->Val->Character;
    if(motorDebug){
        backvalue = 1;
        return ;   
    } 
	if(speed>=0)
	{
		
        car_forward_senior(speed,distance);
		backvalue = 1;
		//printf("\r\n>CBack(%d)\r\n",distance);
	}else
		backvalue = -1;
}
void CStop (struct ParseState *Parser, struct Value *ReturnValue, 
									struct Value **Param, int NumArgs)
{
    if(motorDebug){
        backvalue = 1;
        return ;   
    } 
	car_back_stop();
	backvalue = 1;
	//printf("\r\n>stop()\r\n");
}
void CTurn_Left (struct ParseState *Parser, struct Value *ReturnValue, 
									struct Value **Param, int NumArgs)
{
	int degree = Param[0]->Val->Integer;
    if(motorDebug){
        backvalue = 1;
        return ;   
    } 
	if(degree>=0){
		car_left_senior(degree);
		backvalue = 1;
		//printf("\r\n>turn_left(%d)\r\n",degree);
	}else
		backvalue = -1;
}
void CTurn_Right (struct ParseState *Parser, struct Value *ReturnValue, 
									struct Value **Param, int NumArgs)
{
	int degree = Param[0]->Val->Integer;
    if(motorDebug){
        backvalue = 1;
        return ;   
    } 
	if(degree>=0){
		car_right_senior(degree);
		backvalue = 1;
		//printf("\r\n>CTurn_Right(%d)\r\n",degree);
	}else
		backvalue = -1;
}
void CMotor_Speed (struct ParseState *Parser, struct Value *ReturnValue, 
									struct Value **Param, int NumArgs)
{
	int m_num = Param[0]->Val->Integer;
    int m_speed = Param[1]->Val->Integer;
    if(motorDebug){
        backvalue = 1;
        return ;   
    } 
	if(m_speed>=0){
		_42_step_move_forward();
		backvalue = 1;
		//printf("\r\n> void motor_speed_left( speed = %d )\r\n",speed);
	}else
		backvalue = -1;
}
void CMotor_Forward (struct ParseState *Parser, struct Value *ReturnValue, 
									struct Value **Param, int NumArgs)
{
	int m_distn = Param[0]->Val->Integer;
    if(motorDebug){
        backvalue = 1;
        return ;   
    } 
	if(m_distn>=0){
		_42_step_move_forward();
		backvalue = 1;
		//printf("\r\n> void motor_speed_left( speed = %d )\r\n",speed);
	}else
		backvalue = -1;
}
void CMotor_Back (struct ParseState *Parser, struct Value *ReturnValue, 
									struct Value **Param, int NumArgs)
{
	int m_distn = Param[0]->Val->Integer;
    if(motorDebug){
        backvalue = 1;
        return ;   
    } 
	if(m_distn>=0){
		_42_step_move_back();
		backvalue = 1;
		//printf("\r\n> void motor_speed_left( speed = %d )\r\n",speed);
	}else
		backvalue = -1;
}
void CMotor_Right (struct ParseState *Parser, struct Value *ReturnValue, 
									struct Value **Param, int NumArgs)
{
	int m_distn = Param[0]->Val->Integer;
    if(motorDebug){
        backvalue = 1;
        return ;   
    } 
	if(m_distn>=0){
		_42_step_move_left();
		backvalue = 1;
		//printf("\r\n> void motor_speed_left( speed = %d )\r\n",speed);
	}else
		backvalue = -1;
}
void CMotor_Left (struct ParseState *Parser, struct Value *ReturnValue, 
									struct Value **Param, int NumArgs)
{
	int m_distn = Param[0]->Val->Integer;
    if(motorDebug){
        backvalue = 1;
        return ;   
    } 
	if(m_distn>=0){
		_42_step_move_right();
		backvalue = 1;
		//printf("\r\n> void motor_speed_left( speed = %d )\r\n",speed);
	}else
		backvalue = -1;
}

/** 	Say Something 	*/
void CSayMode1 (struct ParseState *Parser, struct Value *ReturnValue, 
									struct Value **Param, int NumArgs)
{
    backvalue = sound(1,Param[0]->Val->Identifier);
}
void CSayMode2 (struct ParseState *Parser, struct Value *ReturnValue, 
									struct Value **Param, int NumArgs)
{
    backvalue = sound(Param[0]->Val->Integer,Param[1]->Val->Identifier);
}
void CSayMode3 (struct ParseState *Parser, struct Value *ReturnValue, 
									struct Value **Param, int NumArgs)
{
    sound(1,Param[0]->Val->Identifier);
    backvalue = 0;
}

void CPlay (struct ParseState *Parser, struct Value *ReturnValue, 
									struct Value **Param, int NumArgs)
{
	char cont[256] = {0};
	strncpy(cont , Param[0]->Val->Identifier,strlen(Param[0]->Val->Identifier));//�ȱ������ַ���������
	if(cont[0]!='\0')
	{
		printf("\r\n>play(%s)\r\n",cont);
		backvalue = 1;
	}else
		backvalue = -1;
}
/**		 Waiting Several Seconds wait(int seconds)	*/
void CWait (struct ParseState *Parser, struct Value *ReturnValue, 
									struct Value **Param, int NumArgs)
{
    int waitl = Param[0]->Val->Integer ;
    if(waitl <0)    waitl = 0;
	OSTimeDlyHMSM(0, 0,  Param[0]->Val->Integer, 0);
    backvalue = 1;
	printf("\r\n>wait(%d)\r\n",Param[0]->Val->Integer);
}
/**		������		*/
void CLight_Sensor (struct ParseState *Parser, struct Value *ReturnValue, 
									struct Value **Param, int NumArgs)
{
    char cvalue[8] = {0};
    int clen= 0;
    sprintf(cvalue,"%d",lightStatus);
    clen = strlen(cvalue);
    cvalue[clen]='\0';
    printf("\r\n**************C Library Light=%d**********************\r\n",atoi(cvalue));
    ReturnValue->Val->Integer = atoi(cvalue) ;
    backvalue = 1;
}
void CVoltage_Sensor (struct ParseState *Parser, struct Value *ReturnValue, 
									struct Value **Param, int NumArgs)
{
    ReturnValue->Val->FP = voltageStatus ;
    backvalue = 1;
}
void CInfrared_Sensor (struct ParseState *Parser, struct Value *ReturnValue, 
									struct Value **Param, int NumArgs)
{
    char ivalue[6] = {0};
    sprintf(ivalue,"%d",infraredStatus);
    //printf("\r\n**************C Library Infrared=%d**********************\r\n",atoi(ivalue));
    ReturnValue->Val->Integer = atoi(ivalue) ; 
    backvalue = 1;
}
void CHuman_Sensor (struct ParseState *Parser, struct Value *ReturnValue, 
									struct Value **Param, int NumArgs)
{
    char cvalue[10] = {0};
    int clen= 0;
    sprintf(cvalue,"%d",humanStatus);
    clen = strlen(cvalue);
    cvalue[clen]='\0';
    printf("\r\n**************C Library Human=%d**********************\r\n",atoi(cvalue));
    ReturnValue->Val->Integer = atoi(cvalue) ;
    backvalue = 1;
}
void CMic1_Sensor (struct ParseState *Parser, struct Value *ReturnValue, 
									struct Value **Param, int NumArgs)
{
    char mvalue[6] = {0};
    sprintf(mvalue,"%d",micStatus[0]);
    mvalue[strlen(mvalue)] = '\0' ;
    ReturnValue->Val->Integer = atoi(mvalue) ; 
    backvalue = 1;
    //ReturnValue->Val->UnsignedShortInteger = micStatus[0] ;
}
void CMic2_Sensor (struct ParseState *Parser, struct Value *ReturnValue, 
									struct Value **Param, int NumArgs)
{
    char mvalue[6] = {0};
    sprintf(mvalue,"%d",micStatus[1]);
    mvalue[strlen(mvalue)] = '\0' ;
    ReturnValue->Val->Integer = atoi(mvalue) ;
    backvalue = 1;
    //ReturnValue->Val->UnsignedShortInteger = micStatus[1] ;
}
void CMic3_Sensor (struct ParseState *Parser, struct Value *ReturnValue, 
									struct Value **Param, int NumArgs)
{
    char mvalue[6] = {0};
    sprintf(mvalue,"%d",micStatus[2]);
    mvalue[strlen(mvalue)] = '\0' ;
    ReturnValue->Val->Integer = atoi(mvalue) ;
    backvalue = 1;
    //ReturnValue->Val->UnsignedShortInteger = micStatus[2] ;
}
/**		��е�ֱ۵�API	*/
void CServo (struct ParseState *Parser, struct Value *ReturnValue, 
									struct Value **Param, int NumArgs)
{
	//	servo1_turn(Param[0]->Val->Integer);
	//	void servo(int , int*)
	int num = Param[0]->Val->Integer;
	int dof = Param[1]->Val->Integer;
	if(num>0 && num <3)
	{
		switch(num)
		{
		case 1:
			servo1_turn(dof);
			break;
		case 2:
			servo2_turn(dof);
			break;
		case 3:
			servo3_turn(dof);
			break;	
		}
		printf("\r\n>servo-num:%d:dof:%d",num,dof);
		backvalue = 1;
	}else
		backvalue = -1;
}
/**   other math function */
void CRandom (struct ParseState *Parser, struct Value *ReturnValue, 
									struct Value **Param, int NumArgs)
{	// ����ԭ�� int random(int)
	int seed = Param[0]->Val->Integer;
	char revalue[32] = {0};
	srand(seed);
	backvalue = rand();
    backvalue = backvalue==0?backvalue+1:backvalue ;
	printf("\r\n>random(%d) \r\n",seed);
	ReturnValue->Val->Integer = backvalue ;
}
/**   save_data_log  */
void CWrite_Flash (struct ParseState *Parser, struct Value *ReturnValue, 
									struct Value **Param, int NumArgs)
{
	//����ԭ�� void save_data_log(char*) 
	char cont[256] = {0};
	strncpy(cont , Param[0]->Val->Identifier,strlen(Param[0]->Val->Identifier));//�ȱ������ַ���������
	if(cont[0]!='\0')
	{
		printf("\r\n>save_data_log(%s)\r\n",cont);
		backvalue = 1;
	}else
		backvalue = -1;
}
void CRead_Flash (struct ParseState *Parser, struct Value *ReturnValue, 
									struct Value **Param, int NumArgs)
{
	//����ԭ�� void save_data_log(char*) 
	char cont[256] = {0} , *temp = NULL;
	//����API ��ȡFlash����

	memcpy(cont , temp , strlen(temp)+1);
	
    backvalue = -1 ;
	ReturnValue->Val->Identifier = cont ;
}
/********	��ʾ����ص�API		*******/
/**		8 * 8 ����	*/
void CExELed (struct ParseState *Parser, struct Value *ReturnValue, 
									struct Value **Param, int NumArgs)
{
	//void led(int,int)
    int is_light = Param[0]->Val->Integer;
	int x = Param[1]->Val->Integer;
	int y = Param[2]->Val->Integer;
    
    if(is_light<0)
        is_light = 0 ;
    else
        is_light = 1;
	if(x<=0 || y<= 0 || y>8 || x>8)
		backvalue = -1;
	else{
		//���ú�������������
		display_8x8(x,y);
        OSTimeDlyHMSM(0, 0, 0, 100);
		backvalue = 1 ;
	}
	//printf("\r\n> CLed%d,%d\r\n",x,y);
}

/**		 12866��ʾ����ʾ����   */
void CLCD_Display (struct ParseState *Parser, struct Value *ReturnValue, 
									struct Value **Param, int NumArgs)
{
	//void show_chinese(int ,int ,char*)
	int x = Param[0]->Val->Integer;
	int y = Param[1]->Val->Integer;
	unsigned char cont[256] = {0};
	memcpy(cont, Param[2]->Val->Pointer, strlen(Param[2]->Val->Pointer));
	//ֱ�ӵ���д�õ� ������ʾ API
 	LCD12864G_086P_Clear_Screen();			 //LCD ����	
 	display_string_8x16(x, y, (char*)cont, 0); 
	backvalue = 1 ;
//	printf("\r\n> Show_Chinese(x=%d , y=%d , cont=%s) \r\n",x,y,cont);
}
void CDigital_Display (struct ParseState *Parser, struct Value *ReturnValue, 
									struct Value **Param, int NumArgs)
{
	//void digital_display(int ,int); [param:index , num]
    int index = Param[0]->Val->Integer;
	int num = Param[1]->Val->Integer; 
    if (index <0 || index >3 || num <0 || num > 16)
    {
       backvalue = -1 ;
       return ;
    }
    switch(index)
    {
     case 1:
         digitron_display( CS_1 , num);
         break;
     case 2:
         digitron_display( CS_2 , num);
         break;
     case 3:
         digitron_display( CS_3 , num);
        break;
    }
    backvalue = 1 ;
}
void CLCD_Display2 (struct ParseState *Parser, struct Value *ReturnValue, 
									struct Value **Param, int NumArgs)
{   
    char cont[64] = {0};
    char cvalue[32] = {0};
    int dvalue = *(int*)Param[1]->Val->Pointer ;
    memcpy(cvalue, Param[0]->Val->Identifier, strlen(Param[0]->Val->Identifier)+1);

    //������Ϣ���з��͸�LCD��ʾ������
    sprintf(cont,"%s%d(cm)",cvalue,dvalue);
    LCD12864G_086P_Clear_Screen();			 //LCD ����	
 	display_string_8x16(1, 1, cont, 1); 
	backvalue = 1 ;    
    printf("\r\n**********C_Pico Get Double Value:[%s]**********************\r\n",cont);
}
void CDigital_Display2 (struct ParseState *Parser, struct Value *ReturnValue, 
									struct Value **Param, int NumArgs)
{
    int     index = 0 ,num = 1;
    char    cvalue[8] = {0};
    double  dvalue = *(double*)Param[0]->Val->Pointer ;
    char    temp[2] = {0};
    memset(cvalue,0,8);
    sprintf(cvalue,"%lf",dvalue);
    for(index = 0 ; index < 4;index ++)
    {
        if(cvalue[index] == '.'){
            //printf("dot_index=[%d];\n",index);
            switch(index)
            {
            case 1: str_dig.first += 16; break;
            case 2: str_dig.second += 16; break;
            case 3: str_dig.third += 16; break;
            }
        }
        else{
            temp[0] = cvalue[index];    temp[1]='\0';
            switch(num)
            {
            case 1: str_dig.first = atoi(temp); break;
            case 2: str_dig.second = atoi(temp); break;
            case 0: str_dig.third = atoi(temp); break;
            }
            num = (num+1)%3 ;
        }
    }
    //������Ϣ���з��͸����������
    OSQPost(Str_Q_Digitron, &str_dig); 
//     printf("\r\n*****************C_Pico Get Vlotage Value:[%d:%d:%d:%s]********************\r\n",
//                     str_dig.first,str_dig.second,str_dig.third,cvalue);
    
}
/**		 С���м�6��LED ��ʾʣ�����   */
void CSix_Led (struct ParseState *Parser, struct Value *ReturnValue, 
									struct Value **Param, int NumArgs)
{	//void battery_energy(int)
	int i = 1 ;
	int energy = Param[0]->Val->Integer;
	if(energy<0)	energy = 0 ;
	if(energy>6)	energy = 6 ;
	//ֱ�ӵ���д�õ�  API
	//for(i =1 ; i <= energy ; i++)
	flash_6xled(energy);
	
	backvalue = 1 ;
	
	//printf("\r\n> void battery_energy(energy = %d ) \r\n", energy);
}
/**		 С���߽�4��LED  ���������   */
void CFour_Led (struct ParseState *Parser, struct Value *ReturnValue, 
									struct Value **Param, int NumArgs)
{
	//void blink_led(unsigned char )
	int num = Param[0]->Val->Integer;
	//ֱ�ӵ���д�õ�  API
	if(num >=1 && num <=4)
	{
		flash_4xled(num);
		backvalue =  1;
	}
	else if(num ==0)
	{
		flash_4xleds();
		backvalue =  1;
	}else
	{
		backvalue =  -1;
		return;
	}
	//printf("\r\n> void blink_led( cdata = %d ) \r\n",num);
}
/**		 С���м�4��ȫ��LED   */
void CColorful_LED (struct ParseState *Parser, struct Value *ReturnValue, 
									struct Value **Param, int NumArgs)
{
	//colorful_led(int,unsigned char*)
	int index = Param[0]->Val->Integer;
	int color = Param[1]->Val->Integer;
	//ֱ�ӵ���д�õ�  API
	if(index <0)    index = 0 ;
    else if(index >4) index = 4;
    
    if (color<0)    color = 0;
    else if(color>2)    color = 2;
    
	flash_ws2812s(index,color);
	backvalue = 1;
	//printf("\r\n> colorful_led(index = %d , rgb = %p ) \r\n",index ,RGB);
}
/**		�ܽ���ص�API */
void CPinMode (struct ParseState *Parser, struct Value *ReturnValue, 
									struct Value **Param, int NumArgs)
{	//����ԭ�� pinMode(int mode)
	int pin = Param[0]->Val->Integer;
	int mode = Param[1]->Val->Integer;
	//���� �Ѷ���õĺ���
	
	backvalue = 1;
	printf("\r\n> pinMode( mode = %d ) \r\n",mode);
}
void CDigitalWrite (struct ParseState *Parser, struct Value *ReturnValue, 
									struct Value **Param, int NumArgs)
{
	//����ԭ�� digitalWrite(pin , value)
	int pin = Param[0]->Val->Integer;
	int value = Param[1]->Val->Integer;
	if(pin<0)
	{
		backvalue = -1;
		return ;
	}
	//���� �Ѷ���õĺ���
	
	backvalue = 1;
	printf("\r\n> digitalWrite(pin = %d , value = %d ) \r\n",pin,value);
}
void CDigitalRead (struct ParseState *Parser, struct Value *ReturnValue, 
									struct Value **Param, int NumArgs)
{
	//����ԭ�� float digitalRead(pin)
	int pin = Param[0]->Val->Integer;
	char revalue[32]={0};
	
	if(pin<0)
	{
		backvalue = -1;
		return ;
	}
	//���� �Ѷ���õĺ���
	
	backvalue = 1;
	printf("\r\n> digitalWrite(pin = %d ) \r\n",pin);
	ReturnValue->Val->Integer = 110 ;
	
}
void CAnalogReference (struct ParseState *Parser, struct Value *ReturnValue, 
									struct Value **Param, int NumArgs)
{
	//����ԭ�� analogReference(ref)
	int ref = Param[0]->Val->Integer;
	if(ref<0)
	{
		backvalue = -1;
		return ;
	}
	//���� �Ѷ���õĺ���
	
	backvalue = 1;
	printf("\r\n> analogReference(ref = %d ) \r\n",ref);
}
void CAnalogRead (struct ParseState *Parser, struct Value *ReturnValue, 
									struct Value **Param, int NumArgs)
{
	//����ԭ�� float analogRead(pin)
	int pin = Param[0]->Val->Integer;
	char revalue[32]={0};

	if(pin<0)
	{
		backvalue = -1;
		return ;
	}
	//���� �Ѷ���õĺ���
    backvalue = 1;
	printf("\r\n> analogRead(pin = %d ) \r\n",pin);
	ReturnValue->Val->Integer = 120 ;
}
void CAnalogWrite (struct ParseState *Parser, struct Value *ReturnValue, 
									struct Value **Param, int NumArgs)
{
	//����ԭ�� analogWrite(pin , value)
	int pin = Param[0]->Val->Integer;
	int value = Param[1]->Val->Integer;
	if(pin<0)
	{
		backvalue = -1;
		return ;
	}
	//���� �Ѷ���õĺ���
    backvalue = 1;	
	printf("\r\n> analogWrite(pin = %d , value = %d ) \r\n",pin,value);
}
/**		������ص�API	*/
void CPress_Key (struct ParseState *Parser, struct Value *ReturnValue, 
									struct Value **Param, int NumArgs)
{
    extern QUEUE key_Q ;
    char pdata;
    void * temp ;
	//�����Ƿ��а������¡�û�а�������0���а�������1��
	if(!IsEmptyQueue(&key_Q))
    {
        Dequeue(&key_Q, &pdata);
        temp = (void *)&pdata ;
        backvalue = *(int*)temp;
    //    printf("\r\n>press_key()\r\n");
    }else{
        backvalue = -1 ;
    }
	ReturnValue->Val->Integer = backvalue;
}
void CGetch (struct ParseState *Parser, struct Value *ReturnValue, 
									struct Value **Param, int NumArgs)
{
	//���ذ�����ASCII�롣���û�а�������һֱ�ȴ����а���Ϊֹ��
	
	backvalue = 1;
	printf("\r\n>getch_key()\r\n");
}
void CForCheck (struct ParseState *Parser, struct Value *ReturnValue, 
									struct Value **Param, int NumArgs)
{
	backvalue = 1;
}
void CMotorDebug (struct ParseState *Parser, struct Value *ReturnValue, 
									struct Value **Param, int NumArgs)
{
	motorDebug = Param[0]->Val->Integer ;
    if(motorDebug<0)
        motorDebug = 0;
    backvalue = 1;
}
void CDebug (struct ParseState *Parser, struct Value *ReturnValue, 
									struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = motorDebug;
    backvalue = 1;
}
void CWIFI_Status (struct ParseState *Parser, struct Value *ReturnValue, 
									struct Value **Param, int NumArgs)
{
    
	wifiConnectStatus = Param[1]->Val->Integer ;
    memcpy(wifiSSID, Param[0]->Val->Identifier ,strlen(Param[0]->Val->Identifier)+1);
    backvalue = 0;
}
void CSwitchAvoidStyle (struct ParseState *Parser, struct Value *ReturnValue, 
									struct Value **Param, int NumArgs)
{
	isOpenAvoidStyle = Param[0]->Val->Integer ;
    if(isOpenAvoidStyle<0)
        isOpenAvoidStyle = 0 ;
    backvalue = 1;
}
void CSetMoveAdjust (struct ParseState *Parser, struct Value *ReturnValue, 
									struct Value **Param, int NumArgs)
{
	move_adjust = Param[0]->Val->Integer ;
    backvalue = 1;
}
void CSetTurnAdjust (struct ParseState *Parser, struct Value *ReturnValue, 
									struct Value **Param, int NumArgs)
{
	turn_adjust = Param[0]->Val->Integer ;
    backvalue = 1;
}
/***/
void PlatformLibraryInit(void)
{
	LibraryAdd(&GlobalTable, "platform library", PlatformLibrary);
	/*********** ע�������ͷ�ļ� ************/
	IncludeRegister("stm32f10x_gpio.h" , NULL , &PlatformLibrary[50] , NULL);

}
/* list of all library functions and their prototypes */
struct LibraryFunction PlatformLibrary[] =
{
		//for auto check   1
		{	CForCheck,					"void forcheck();"},
		//===========move=======   10
		{	CForward,					"void forward(int,int);"},
		{	CBack,						"void back(int,int);"},
		{	CStop,						"void stop();"},
		{	CTurn_Left,					"void turn_left(int);"},
		{	CTurn_Right,				"void turn_right(int);"},
		{	CMotor_Speed,				"void motor_speed(int,int);"},
        {	CMotor_Forward,				"void motor_forward(int);"},
        {	CMotor_Back,				"void motor_back(int);"},
        {	CMotor_Right,				"void motor_right(int);"},
        {	CMotor_Left,				"void motor_left(int);"},
		//=========time ======   1
		{	CWait,						"void wait(int);"},
		//======= sensor =======   7
		{	CLight_Sensor,				"int   lightsensor();"},
        {	CVoltage_Sensor,			"double  voltagesensor();"},
        {	CInfrared_Sensor,			"int  infraredsensor();"},
        {	CHuman_Sensor,				"int  humansensor();"},
        {	CMic1_Sensor,				"int  mic1sensor();"},
        {	CMic2_Sensor,				"int  mic2tsensor();"},
        {	CMic3_Sensor,				"int  mic3tsensor();"},
		//====== other     1
		{	CRandom,					"int random(int);"},
		//======= sound    4
		{	CSayMode1,					"void say(void *);"},
        {	CSayMode2,					"void gpp_say(int,void *);"},
        {	CSayMode3,					"void wifi_say(void *);"},
		{	CPlay ,						"void play(char*);"},
		//======== data collection   2
		{	CWrite_Flash ,				"void 	wr_flash(char*);"},
		{	CRead_Flash ,				"char* 	rd_flash(char*);"},
		//======== 8 * 8 LED ��ʾ��   1
		{	CExELed ,					"void EXELed(int,int,int);"},
		//========  TFT 12866��ʾ��   4
		{	CLCD_Display, 				"void lcd_display(int ,int ,char*);"},
		{	CDigital_Display, 			"void digital_display(int ,int);"},
        {	CLCD_Display2, 				"void lcd_display2(char*,void *);"},
        {	CDigital_Display2, 			"void digital_display2(void *);"},
		//===== 6+4 ���ƶ�����      2
		{	CSix_Led,					"void six_led(int);"},
		{	CFour_Led,					"void four_led(int);"},
		//===== ȫ��LED           1
		{	CColorful_LED ,				"void colorful_led(int,int);"},
		//===== ��е�ֱ� ����      1
		{	CServo ,					"void servo(int , int);"},
		//===== �ܽ���ص�API      6
		{	CPinMode,						"void   pinMode(int,int);"},
		{	CDigitalWrite,					"void   digitalWrite(int,int);"},
		{	CDigitalRead,					"float  digitalRead(int);"},
		{	CAnalogReference,				"void   analogReference(int);"},
		{	CAnalogRead,					"float  analogRead(int);"},
		{	CAnalogWrite,					"void   analogWrite(int,int);"},
		//===== ������ص�API       9
		{	CPress_Key,							"char press_key();"},
		{	CGetch,								"int  getch();"},
        {	CMotorDebug,						"void motordebug(int);"},
        {	CDebug,						        "int  debug();"},
        {	CWIFI_Status,						"void wifi_statue(char*,int);"},
        {	CSwitchAvoidStyle,					"void avoid_mode(int);"},
        {	CSetMoveAdjust,					    "void setmove_adjust(int);"},
        {	CSetTurnAdjust,					    "void setturn_adjust(int);"},
		{ 	NULL,        			 	NULL 						}
};
/********************************************************************************
	END File
*********************************************************************************/







