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
	FILE* last=fopen("result.txt","w");
	FILE* ma=fopen("machine.txt","w");
	
	symbolLoc middle_sym[100];
	int exe=-1;
	int T[10];
	
	for(int index=0;index<100;index++)
	{
		middle_sym[index].symbol=malloc(sizeof(char)*100);
		middle_sym[index].loc=-1;
		middle_sym[index].rlink=NULL;
	}
	
	for(int index=0;fscanf(SYMTAB0,"%X%s",&middle_sym[index].loc,middle_sym[index].symbol)==2;index++)
	{
		printf("sym:%04X\t%s\n",middle_sym[index].loc,middle_sym[index].symbol);
	}
	int tindex=0;
	for(fgets(temp,100,source0);;fgets(temp,100,source0))
	{
		 effect_read2(command,temp);
		 if(strcmp(command->opcode,"START")==0)
		 {
			 fprintf(ma,"H%-6s%06X%06X\n",command->label,strtol(command->parameter,NULL,16),section_index[0]->segement.pc-strtol(command->parameter,NULL,16));
			continue;
		 }
		 if(strcmp(command->opcode,"END")==0)
		break;
		 
		printf("%s\t",command->opcode);
		printf("0:\n");
		int comma=0;
		int X=0;
		int destination=0;
		char temp[100]={'\0'};
		for(int index=0;command->parameter[index]!='\0';index++)
		{
			for(;command->parameter[index]!='\0';index++)
			{
				printf("LOOP:%c\n",command->parameter[index]);
				temp[index]= command->parameter[index];
				if(command->parameter[index]==','&&command->parameter[index+1]=='X')
				{
					X=1;
				}
			}
		}
		printf("1:\n");
		
		if(strcmp(command->parameter,"\0")==0)
		{
			destination=0;
		}
		else if(format_search(command->opcode)==5)
		{
			fprintf(last,"%4X\t%s\t%s\t%s\t",command->loc,command->label,command->opcode,command->parameter);
			if(command->parameter[0]=='X')
			{
				for(int index=2;command->parameter[index]!='\'';index++)
				{
					fprintf(last,"%c",command->parameter[index]);
					printf("%c",command->parameter[index]);
				}
				fprintf(last,"\n");
			}
			else if(command->parameter[0]=='C')
			{
				char a;
				for(int index=2;command->parameter[index]!='\'';index++)
				{
					a=command->parameter[index];
					fprintf(last,"%2X",(int)a);
					printf("%2X",(int)a);
				}
				fprintf(last,"\n");
			}
			else
			{
				fprintf(last,"%06X\n",strtol(command->parameter,NULL,10));
			}
			if(exe==-1)
			exe=command->loc;
			continue;
		}
		else if(format_search(command->opcode)==6)
		{
			fprintf(last,"%4X\t%s\t%s\t%s\n",command->loc,command->label,command->opcode,command->parameter);
			continue;
		}
		else
		{
			for(int index=0;index<100;index++)
			{
				if(strcmp(temp,middle_sym[index].symbol)==0)
				{
					destination=middle_sym[index].loc;
					printf("%4X\n",middle_sym[index].loc);
					break;
				}
			}
		}
		printf("2:\n");
		fprintf(last,"%4X\t%s\t%s\t%s\t",command->loc,command->label,command->opcode,command->parameter);
		if(X==1)
		{
			destination+=2*2*2*2*2*2*2*2*2*2*2*2*2*2*2;
			fprintf(last,"%02X%04X\n",opcode_search(command->opcode),destination);
			if(exe==-1)
			exe=command->loc;
		}
		else
		{
			fprintf(last,"%02X%04X\n",opcode_search(command->opcode),destination);
			if(exe==-1)
			exe=command->loc;
		}
	}
	fprintf(ma,"E%06X\n",exe);
	return 0;
}