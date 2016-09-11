/**
  ******************************************************************************
  * @�ļ���    
  * @���� 
  * @�汾
  * @����
  * @��Ҫ
  ******************************************************************************
  * @ע������
  *
  *
  *
  ******************************************************************************
  */  

/* ͷ�ļ�				--------------------------------------------------------------*/
#include <ctype.h>

#include "xCLI.h"

/* ��������     --------------------------------------------------------------*/
/* �궨�� 	    --------------------------------------------------------------*/
/* ���� 	    	--------------------------------------------------------------*/
static bool xCLI_State = true;
static xCLI_BufTypedef xcli_buf = {0,{NULL}};

/* ��������     --------------------------------------------------------------*/

/**
  * @����
  * @����
  * @����ֵ
  */
int32_t xCLI_GetLine(void)
{
	bool state;
	signed char ch;
	
	xcli_buf.index = 0;
	xcli_buf.data[0] = NULL;
	
	
	for( state = true; state;){
		ch = xCLI_getchar();
		if(ch == EOF) continue; /* ����� */
		
		if(ch >= ' '){
			xcli_buf.data[xcli_buf.index++] = ch;
			xCLI_putchar(ch);
		}else{
			switch(ch){		/* �����ַ������� */
				case '\b':  if(xcli_buf.index > 0){
											xcli_buf.data[xcli_buf.index--] = NULL;
											xCLI_printf("\b \b");
										}else{
											xCLI_putchar('\a');
										}
										break;
				case '\r':	xcli_buf.data[xcli_buf.index] = NULL;
										state = false;		
										break;		
// 				case '\n':	state = false;	break;													
			}
		}		
	}
	
	return (xcli_buf.index);
}

/**
  * @����
  * @����
  * @����ֵ
  */
int32_t xCLI_process_cmd(uint8_t *pdata, uint32_t len)
{
	int32_t n, res = -1;
	xCLI_CmdTypedef *p;
	
	for(p = &CLI_CmdTable[0]; p -> cmd != NULL; p++){
		if(strstr((char *)pdata, p -> name)){
			n = strlen(p -> name);
			res = p -> cmd(&pdata[n], len);			
			break;
		}
	}
	
	return res;
}

/**
  * @����
  * @����
  * @����ֵ
  */
int32_t xCLI(void)
{
	/* ��¼ */

	memset(xcli_buf.data, NULL, sizeof(xcli_buf.data));
// 	xCLI_process_cmd((uint8_t*)"Wellcom", 7);
//	xCLI_process_cmd((uint8_t*)"picoc", 7);
	
	do{
		xCLI_printf("\r\nxCLI >");
		
		if(xCLI_GetLine()){
			xCLI_process_cmd(xcli_buf.data, xcli_buf.index);
		}
	}while(xCLI_State);
	
	return 0;
}

/**
  * @}
  */

/**
 ******************************************************************************
 * �ļ�ĩβ
 ******************************************************************************
 */
