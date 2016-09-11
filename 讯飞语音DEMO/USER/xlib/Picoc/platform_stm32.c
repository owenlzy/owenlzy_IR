#include "picoc.h"
#include "interpreter.h"
#include "BSP.h"

#define Picoc_getchar() 	USART1_getchar()
#define Picoc_putchar(c) 	USART1_putchar(c)
#define Picoc_printf			Printf

/* mark where to end the program for platforms which require this */
jmp_buf PicocExitBuf;

/* platform stm32 */

// void PlatformInit(Picoc *pc)
// {
// }

void PlatformCleanup(void)
{
	
}

/* exit the program */
void PlatformExit(int ExitVal)
{
	
}

/* get a character of interactive input */
int PlatformGetCharacter(void)
{
//     return Picoc_getchar();
	  fflush(stdout);
    return fgetc(stdin);
}

/* write a character to the console */
void PlatformPutc(unsigned char OutCh, union OutputStreamInfo *Stream)
{
//     Picoc_putchar(OutCh);
	 fputc(OutCh, stdout);
}

/* get a line of interactive input */
char *PlatformGetLine(char *Buf, int MaxLen, const char *Prompt)
{
	bool state;
	int index, ch;
	if (Prompt != NULL)
			Printf("%s", Prompt);
	
	for( state = true, index = 0; state;){
		ch = Picoc_getchar();
		if(ch == EOF) continue; /* »º´æ¿Õ */
		
		if(ch >= ' '){
			Buf[index++] = ch;
			Picoc_putchar(ch);
		}else{
			switch(ch){		/* ¿ØÖÆ×Ö·û´®´¦Àí */
				case '\b':  if(index > 0){
											Buf[index--] = NULL;
											Picoc_printf("\b \b");
										}else{
											Picoc_putchar('\a');
										}
										break;
				case '\r':	Buf[index] = NULL;
										state = false;		
										break;		
// 				case '\n':	state = false;	break;
				case '\x04': return NULL; /* ctrl + d*/
			}
		}
	}		
		
	return Buf;

//     if (Prompt != NULL)
//         printf("%s", Prompt);
//         
//     fflush(stdout);
//     return fgets(Buf, MaxLen, stdin)
}

/* read a file into memory */
char *PlatformReadFile( const char *FileName)
{

    return NULL;    
}

/* read and scan a file for definitions */
void PicocPlatformScanFile(const char *FileName)
{	
	char *SourceStr = NULL;
	
// 	char *SourceStr = PlatformReadFile(pc, FileName);
	PicocParse(FileName, SourceStr, strlen(SourceStr), TRUE, FALSE, FALSE);	
}


int PicocPlatformSetExitPoint(void)
{
	
	return 0;
}
