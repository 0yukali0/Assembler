#include "sic.h"

int main(char argc,char* argv[])
{
	section_ptr section_index[MAX_SECTION_NUMBER] ={NULL};
	symbolLoc_ptr symtab_head[MAX_SECTION_NUMBER]={NULL};
	FILE* source = fopen(argv[1],"r");
	FILE* destination = fopen("middle.txt","w");
	FILE* SYMTAB = fopen("symtab0.txt","w");
	char temp[100];
	
	section_index[0] = malloc(sizeof(section));
	instruction_ptr command = malloc(sizeof(instruction));
	
	for(int section_number=0;section_index[section_number]!=NULL&&section_number<MAX_SECTION_NUMBER;section_number++)
	{
		symtab_head[section_number]=malloc(sizeof(symbolLoc));
		symbolLoc_ptr sym_ptr=symtab_head[section_number];
		block_ptr data=&(section_index[section_number]->segement);
		
		if(section_number==0)
			strcpy(data->name,"");
		data->num=0;	data->pc = 0x0;	data->loc = data->pc;	data->llink=NULL;	data->rlink=NULL;
		
		int isLTORG=0,isStart=0;
		for(int block_num=0;fgets(temp,100,source)!=NULL || !(strcmp(command->opcode,"END")==0);)
		{
			effect_read(command,temp);
			/**************************************************************
			Start check
			**************************************************************/
			if(strcmp(command->opcode,"START")==0)
			{
				data->pc = strtol(command->parameter,NULL,16);
				isStart=1;
			}
			else if(isStart==0)
			{
				continue;
			}
			/*********************************************
			CSERT check and break
			*********************************************/
			if(strcmp(command->opcode,"CSET")==0)
			{
				section_index[section_number+1]=malloc(sizeof(section));
				symtab_head[section_number+1]=malloc(sizeof(symbolLoc));
				strcpy(section_index[section_number+1]->name,command->label);
				break;
			}
			/*********************************************
			USE check and block
			*********************************************/
			if(strcmp(command->opcode,"USE")==0)
			{
				int isUse=0;
				block_ptr temp=data;
				for(;temp!=NULL;temp=temp->rlink)
				{
					if(strcmp(temp->name,command->parameter)==0)
					{
						isUse=1;
					}
				}
				
				if(isUse==1)
				{
					data=temp;
				}
				else
				{
					data->rlink=malloc(sizeof(block));
					data=data->rlink;
					block_num++;
					data->num=block_num;
					strcpy(data->name,command->parameter);
				}
			}
			/**************************************************
			literal
			**************************************************/
			if(strcmp(command->opcode,"LTORG")==0)
			{
				isLTORG=1;
				continue;
			}
			else if(isLTORG==1&&strcmp(command->label,"*")==0)
			{
				fprintf(SYMTAB,"%04X\t%s\t0\n",data->loc,command->opcode);
				fprintf(destination,"%04X\t%s\t%s\t%s\n",data->loc,command->label,command->opcode,command->parameter);
				
				data->loc=data->pc;
				data->pc+=code_length(command->opcode,command->parameter);
				sym_ptr->symbol=command->opcode;
				sym_ptr->rlink=malloc(sizeof(symbolLoc));
				sym_ptr=sym_ptr->rlink;
				continue;
			}
			/******************************************
			EQU
			******************************************/
			if(strcmp(command->opcode,"EQU")==0)
			{
				char equ[100];
				if(strcmp(command->parameter,"*")==0)
				{
					fprintf(SYMTAB,"%04X\t%s\t0\n",data->pc,command->label);
					sym_ptr->symbol=command->label;
					sym_ptr->loc=data->pc;
				}
				else
				{
					char t=command->parameter[0];
					int result=0x0;
					for(int index=0;t!='\0';)
					{
						for(int t_index=0;command->parameter[index]!='-';index++)
						{
							equ[t_index]=command->parameter[index];
							t_index++;
						}
						for(symbolLoc_ptr temp=sym_ptr;;temp=temp->rlink)
						{
							if(strcmp(equ,temp->symbol)==0)
							{
								result=temp->loc;
								break;
							}
						}
						for(int t_index=0;command->parameter[index]!='\0';index++)
						{
							equ[t_index]=command->parameter[index];
							t_index++;
						}
						for(symbolLoc_ptr temp=sym_ptr;;temp=temp->rlink)
						{
							if(strcmp(equ,temp->symbol)==0)
							{
								result-=temp->loc;
								break;
							}
						}
					}
				}
			}
			
			data->loc=data->pc;
			data->pc+=code_length(command->opcode,command->parameter);
			
			if(!(strcmp(command->opcode,".")==0))
			{
				fprintf(destination,"%04X\t%s\t%s\t%s\n",data->loc,command->label,command->opcode,command->parameter);
				if(command->label[0]!='\0' && command->label[0]!='*')
				{
					sym_ptr->symbol=command->label;
					sym_ptr->loc=data->loc;
					sym_ptr->rlink=malloc(sizeof(symbolLoc));
					sym_ptr=sym_ptr->rlink;
					fprintf(SYMTAB,"%04X\t%s\n",data->loc,command->label);
				}
			}
			
		}
	}
	fclose(source);
	fclose(SYMTAB);
	fclose(destination);
	
	
	FILE* source0 = fopen("middle.txt","r");
	FILE* SYMTAB0 = fopen("symtab0.txt","r");
	symbolLoc middle_sym[100];
	
	for(int index=0;index<100;index++)
	{
		middle_sym[index].symbol=malloc(sizeof(char)*100);
		middle_sym[index].loc=-1;
		middle_sym[index].rlink=NULL;
	}
	
	for(int index=0;fscanf(SYMTAB0,"%X%s",&middle_sym[index].loc,middle_sym[index].symbol)==2;index++);

	int base=-1;
	/*
	int n = 0X2;
	int i = 0X1;
	int x = 0X8;
	int b = 0X4;
	int p = 0X2;
	int e = 0X1;
	*/
	int n = 0X0;
	int i = 0X0;
	int x = 0X0;
	int b = 0X0;
	int p = 0X0;
	int e = 0X0;
	for(fgets(temp,100,source0);;fgets(temp,100,source0))
	{
		int disp=0x0;
		int opnum=0x0;
		int belong=0x0;
		effect_read2(command,temp);
		if(strcmp(command->opcode,"BASE")==0&&base==-1)
		{
			for(int index=0;index<100;index++)
			{
				if(strcmp(command->parameter,middle_sym[index].symbol)==0)
				{
					base=middle_sym[index].loc;
					continue;
				}
			}
		}
		
		printf("%04X\t%s\t%s\t%s\t",command->loc,command->label,command->opcode,command->parameter);
		if(format_search(command->opcode)==5)
		{
			if(command->parameter[0]=='X')
			{
				for(int p=0,index=0;p!=2;index++)
				{
					if(command->parameter[index]=='\'')
					{
						p++;
					}
					if(p==1)
					{
						printf("%c",command->parameter[index]);
					}
				}
				printf("\n");
			}
			if(command->parameter[0]=='C')
			{
				char c=command->parameter[0];
				for(int p=0,index=0;p!=2;index++)
				{
					if(command->parameter[index]=='\'')
					{
						p++;
					}
					if(p==1)
					{
						c=command->parameter[index];
						printf("%02X",(int)c);
					}
				}
				printf("\n");
			}
		}
		else if(format_search(command->opcode)==2)
		{
			int X=0X1,A=0X0,T=0X5,S=0X4;
			int sum=0;
			printf("%02X",opcode_search(command->opcode));
			for(int index=0;index<2;)
			{
				switch(command->parameter[index])
				{
					case 'X':
					index++;
					sum+=X;
					break;
					case 'A':
					index++;
					sum+=A;
					break;
					case 'T':
					index++;
					sum+=T;
					break;
					case 'S':
					index++;
					sum+=S;
					break;
					default:
					index++;
					break;
				}
				if(index==1)
				{
					sum*=16;
				}
				if(command->parameter[index]=='\0')
				{
					printf("%2X\n",sum);
					break;
				}
			}
		}
		else if(format_search(command->opcode)==1)
		{
			printf("%2X\n",opcode_search(command->opcode));
		}
		else if(format_search(command->opcode)<5 && format_search(command->opcode)>=3)
		{
			/******************
			@,#
			********************/
			if(command->parameter[0]!='@' && command->parameter[0]!='#')
			{
				n=0x2;
				i=0x1;
			}
			else if(command->parameter[0]!='@')
			{
				n=0x2;
			}
			else if(command->parameter[0]!='#')
			{
				i=0x1;
			}
			/******************
			+
			********************/
			if(command->opcode[0]=='+')
			{
				e=0x1;
			}
			/******************
			X
			********************/
			for(int isx=0;command->parameter[isx];isx++)
			{
				if(command->parameter[isx]=='X' && command->parameter[isx-1]==',' &&command->parameter[isx+1]=='\0')
				{
					isx=-1;
				}
				if(isx==-1)
				{
					x=0x8;
				}
			}
			char use_c[10]={'\0'};
			for(int index=0;command->parameter[index]!='\0' || command->parameter[index]!=',';index++)
			{
				use_c[index]=command->parameter[index];
			}
			for(int index=0;index<100;index++)
			{
				if(strcmp(use_c,middle_sym[index].symbol)==0)
				{
					disp=middle_sym[index].loc;
					break;
				}
			}
			
			/************************
			b,p and result
			************************/
			
			belong=n+p+x+e;
			opnum=opcode_search(command->opcode)+n+i;
			if(format_search(command->opcode)==4)
			{
				printf("%02X%0X%04X\n",opnum,belong,disp);
			}
			else
			{
				if(i>0)
				{
					printf("%02X%0X%03X\n",opnum,belong,disp);
					continue;
				}
				else if(disp-(command->loc+format_search(command->opcode))>=-2048&&disp-(command->loc+format_search(command->opcode))<2048)
				{
					p=0x2;
					belong+=b+p;
				}
				else if(disp>base && disp-base<4096)
				{
					b=0x4;
					belong+=b+p;
				}
				printf("%02X%0X%03X\n",opnum,belong,disp);
			}
		}
		
		if(strcmp(command->opcode,"END")==0)
		{
			break;
		}
	}
	return 0;
}