#ifndef __LIBRARY_STM32_H__
#define __LIBRARY_STM32_H__
/******* One Function is for Test , anthoer is parser*************/

//say(\"i am ugev\");
/*
                        "distns=humansensor();printf(\"\\r\\nhuman=%d\\r\\n\",distns);"\
                        "if(distns<100)say(\"防贼\");"\
*/

#define CCODE_SAY_TEST1 "say(\"大家端午节快乐\");say(\"大家上午好\");void main(){}"
#define CCODE_SAY_TEST2 "say(\"我是优唯思教育机器人\");void main(){}"
#define CCODE_SAY_TEST3 "say(\"大家端午节快乐\");void main(){}"

#define CCODE_LAST_TEST "double svalue=0.00;int distns=0;"\
                        "say(\"我是优唯思教育机器人\");forward(1,24);motor_speed(1,1);"\
                        "distns=lightsensor();"\
                        "if(distns>3750)say(\"晃眼\");"\
                        "distns=humansensor();printf(\"\\r\\nhuman=%d\\r\\n\",distns);"\
                        "if(distns==0)say(\"防贼\");"\
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