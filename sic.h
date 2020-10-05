#ifndef SIC_H
#define SIC_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE 50
#define OP_MAX_SIZE 59
#define DATA_MAX_SIZE 5
#define AS_MAX_SIZE 6
#define MAX_SECTION_NUMBER 10

struct block{
	char name[MAX_SIZE];
	int num;
	long pc;
	long loc;	
	struct block* rlink;
	struct block* llink;
};

struct section{
	char name[MAX_SIZE];
	struct block segement;
};

struct op{
	char* Mnemonic;
	int code;
	int format;
};

struct directive{
	char* as_directive;
};

struct data{
	char* data_size_name;
	int data_byte;
};

struct instruction{
	int loc;
	char label[MAX_SIZE+1];	
	char opcode[MAX_SIZE+1];
	char parameter[MAX_SIZE+1];
};

struct symbolLoc{
	char* symbol;
	int loc;
	struct symbolLoc* rlink;
};

typedef struct block block;
typedef struct block* block_ptr;
typedef struct section section;
typedef struct block* block_ptr;
typedef struct section* section_ptr;
typedef struct op op;
typedef struct data data;
typedef struct instruction instruction;
typedef struct directive directive;
typedef struct op* op_ptr;
typedef struct data* data_ptr;
typedef struct instruction* instruction_ptr;
typedef struct directive* directive_ptr;
typedef struct symbolLoc symbolLoc;
typedef struct symbolLoc* symbolLoc_ptr;

data DTAB[DATA_MAX_SIZE]=
{
	{"BYTE",1},{"RESB",1},{"WORD",3},{"RESW",3},{"*",0}
};

directive DIRECTIEVE[AS_MAX_SIZE]=
{
	{"START"},{"END"},{"LTORG"},{"EQU"},{"USE"},{"CSECT"}
};

op OPTAB[OP_MAX_SIZE] = 
{
	{"ADD", 	0X18, 	3},{"ADDF", 	0X58, 	3},
	{"ADDR", 	0X90, 	2},{"AND", 	0X40, 	3},
	{"CLEAR", 	0XB4, 	2},{"HIO", 	0XF4, 	1},
	{"COMP", 	0X28, 	3},{"COMPF", 	0X88, 	3},
	{"COMPR", 	0XA0, 	2},{"DIV", 	0X24, 	3},
	{"DIVF", 	0X64, 	3},{"DIVR", 	0X9C, 	2},
	{"FIX", 	0XC4, 	1},{"FLOAT", 	0XC0, 	1},
	{"J", 		0X3C, 	3},{"JEQ", 	0X30, 	3},
	{"JGT", 	0X34, 	3},{"JLT", 	0X38, 	3},
	{"JSUB", 	0X48, 	3},{"LDA", 	0X00, 	3},
	{"LDB", 	0X68, 	3},{"LDCH", 	0X50, 	3},
	{"LDF", 	0X70, 	3},{"LDL", 	0X08, 	3},
	{"LDS", 	0X6C, 	3},{"LDT", 	0X74, 	3},
	{"LDX", 	0X04, 	3},{"LPS", 	0XD0, 	3},
	{"MUL", 	0X20, 	3},{"MULF", 	0X60, 	3},
	{"MULR", 	0X98, 	2},{"NORM", 	0XC8, 	1},
	{"OR", 		0X44, 	3},{"RD", 	0XD8, 	3},
	{"RMO", 	0XAC, 	2},{"RSUB", 	0X4C, 	3},
	{"SHIFT", 	0XA4, 	2},{"SHIFTR", 	0XA8, 	2},
	{"SIO", 	0XF0, 	1},{"SSK", 	0XEC, 	3},
	{"STA", 	0X0C, 	3},{"STB", 	0X78, 	3},
	{"STCH", 	0X54, 	3},{"STF", 	0X80, 	3},
	{"STI", 	0XD4, 	3},{"STL", 	0X14, 	3},
	{"STS", 	0X7C, 	3},{"STSW", 	0XE8, 	3},
	{"STT", 	0X84, 	3},{"STX", 	0X10, 	3},
	{"SUB", 	0X1C, 	3},{"SUBF", 	0X5C, 	3},
	{"SUBR", 	0X94, 	2},{"SVC", 	0XB0, 	2},
	{"TD",		0XE0, 	3},{"TIO", 	0XF8, 	1},
	{"TIX", 	0X2C, 	3},{"TIXR", 	0XB8, 	2},
	{"WD", 		0XDC, 	3}
};

void effect_read2(instruction_ptr three,char* in)
{

	char temp;	int index = 0;	temp=*in;
	char number[5];
	for(;temp!='\t';index++)
	{
		number[index]=temp;
		in++;	temp=*in;
	}
	number[4]='\0';
	three->loc=strtol(number,NULL,16);
	in+=1; index=0; temp=*in;

	for(;temp!='\t';index++)
	{
		three->label[index]=temp;
		in++;	temp = *in;
	}
	for(;index<=MAX_SIZE+1;index++)	{three->label[index]='\0';}

	in += 1;	index = 0;	temp=*in;

	for(;temp!='\t';index++)
	{
		three->opcode[index]=temp;
		in++;	temp=*in;
	}
	for(;index<=MAX_SIZE+1;index++)	{three->opcode[index]='\0';}

	in += 1;	index = 0;	temp=*in;

	for(;temp!='\n';index++)
	{
		three->parameter[index]=temp;
		in++;	temp=*in;
	};
	for(;index<=MAX_SIZE+1;index++)	{three->parameter[index]='\0';}
}

void effect_read(instruction_ptr three,char* in)
{

	char temp;	int index = 0;	temp=*in;

	for(;temp!='\t';index++)
	{
		three->label[index]=temp;
		in++;	temp = *in;
	}
	for(;index<=MAX_SIZE+1;index++)	{three->label[index]='\0';}

	in += 1;	index = 0;	temp=*in;

	for(;temp!='\t';index++)
	{
		three->opcode[index]=temp;
		in++;	temp=*in;
	}
	for(;index<=MAX_SIZE+1;index++)	{three->opcode[index]='\0';}

	in += 1;	index = 0;	temp=*in;

	for(;temp!='\n';index++)
	{
		three->parameter[index]=temp;
		in++;	temp=*in;
	};
	for(;index<=MAX_SIZE+1;index++)	{three->parameter[index]='\0';}
}

int X_length(char* X)
{
	int length = 0;
	if(*X=='=')
	{
		X+=3;
	}
	else
	{
		X+=2;
	}

	for(;*X != '\'';X++)	length++;

	return length/2;
}

int C_length(char* C)
{
	int length = 0;
	if(*C=='=')
	{
		C+=3;
	}
	else
	{
		C+=2;
	}

	for(;*C != '\'';C++)	length++;

	return length;
}

int code_length(char* opcode,char* parameter)
{

	/*{"BYTE",1},{"RESB",1},{"WORD",3},{"RESW",3}*/

	char temp_o[MAX_SIZE+1];
	int e=0;

	/*
	literal 
	*/
	if(*opcode=='=')
	{
		char* test= opcode;
		test++;
		switch(*test)
		{
			case 'X':
			printf("Xin: %d\t%s\n",X_length(opcode),opcode);
			return X_length(opcode);
			break;
			case 'C':
			printf("Cin: %d\t%s\n",C_length(opcode),opcode);
			return C_length(opcode);
			break;
		}
	}

	/*
	usaul operation
	*/
	if((*opcode)=='+')
	{
		e=1;
		char* temp = opcode;
		temp++;
		for(int index=0;index<MAX_SIZE;temp++)
		{
			temp_o[index]=*temp;
			index++;
		}
	}
	else
	{
		strcpy(temp_o,opcode);
	}

	for(int index=0;index<DATA_MAX_SIZE;index++)
	{
		if(strcmp(opcode,DTAB[index].data_size_name)==0)
		{
			switch(index)
			{	
				case 5:
				case 0:
				switch(*parameter)
				{
					case 'X':
					return X_length(parameter);
					break;
					case 'C':
					return C_length(parameter);
					break;
				}
				break;
				case 1:
				return strtol(parameter,NULL,10); 
				break;
				case 2:
				return 3;
				break;
				case 3:
				return 3*strtol(parameter,NULL,10);
				break;
			}
		}
	}

	for(int index=0;index<OP_MAX_SIZE;index++)
	{
		if(strcmp(temp_o,OPTAB[index].Mnemonic)==0)
		{
			return OPTAB[index].format+e;
		}
	}
	return 0;
}
int opcode_search(char* o)
{
	for(int index=0;index<OP_MAX_SIZE;index++)
	{
		if(strcmp(o,OPTAB[index].Mnemonic)==0)
		{
			return OPTAB[index].code;
		}
	}
	return 0;
}

int format_search(char* o)
{
	for(int index=0;index<OP_MAX_SIZE;index++)
	{
		if(strcmp(o,OPTAB[index].Mnemonic)==0)
		{
			return OPTAB[index].format;
		}
	}
	if(strcmp(o,"BYTE")==0)
	{
		return 5;
	}
	if(strcmp(o,"WORD")==0)
	{
		return 5;
	}
	if(strcmp(o,"RESB")==0)
	{
		return 6;
	}
	if(strcmp(o,"RESW")==0)
	{
		return 6;
	}
	return 0;
}
#endif