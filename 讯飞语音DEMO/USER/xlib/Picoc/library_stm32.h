#ifndef __LIBRARY_STM32_H__
#define __LIBRARY_STM32_H__
/******* One Function is for Test , anthoer is parser*************/

//say(\"i am ugev\");
/*
                        "distns=humansensor();printf(\"\\r\\nhuman=%d\\r\\n\",distns);"\
                        "if(distns<100)say(\"����\");"\
*/

#define CCODE_SAY_TEST1 "say(\"��Ҷ���ڿ���\");say(\"��������\");void main(){}"
#define CCODE_SAY_TEST2 "say(\"������Ψ˼����������\");void main(){}"
#define CCODE_SAY_TEST3 "say(\"��Ҷ���ڿ���\");void main(){}"

#define CCODE_LAST_TEST "double svalue=0.00;int distns=0;"\
                        "say(\"������Ψ˼����������\");forward(1,24);motor_speed(1,1);"\
                        "distns=lightsensor();"\
                        "if(distns>3750)say(\"����\");"\
                        "distns=humansensor();printf(\"\\r\\nhuman=%d\\r\\n\",distns);"\
                        "if(distns==0)say(\"����\");"\
                        "distns = infraredsensor();lcd_display2(\"ultro:\",&distns);"\
                        "svalue = voltagesensor();digital_display2(&svalue);"\
                        "printf(\"\\r\\n\\r\\n%d\\r\\n\",mic1sensor());"\
                        "for(distns=1;distns<=6;distns++)six_led(distns);"\
                        "for(distns=1;distns<=4;distns++)four_led(distns);"\
                        "void main(){}" 


                        

void CPicoc(char * CSource);
void ParseStoryScript(char * storyScript);
int sound(int flag ,char *cstr);
#endif