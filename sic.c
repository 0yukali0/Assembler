#include "sic.h"

int main(char argc,char* argv[])
{
	FILE* source = fopen(argv[1],"r");
	FILE* destination = fopen("middle.txt","w");
	FILE* SYMTAB = fopen("symtab0.txt","w");

	char temp[100];
	section_index[0] = malloc(sizeof(section));
	instruction_ptr command_ptr = malloc(sizeof(instruction));
	block block_data = section_index[0]->segement;
	block_data.name="";	block_data.num=0;	block_data.pc = 0x0;	block_data.loc = block_data.pc;	block_data.llink=NULL;	block_data.rlink=NULL;

	for(int index=0;section_index[index]!=NULL;index++)
	{
		symtab_head[index].llink=NULL; symtab_head[index].rlink=NULL;
		int isLTORG = 0;

		for(int block_num=0;fgets(temp,100,source)!=NULL || !(strcmp(command_ptr->opcode,"END")==0);)
		{
			
			effect_read(command_ptr,temp);

			if(strcmp(command_ptr->opcode,"START")==0)
			{
				block_data.pc = strtol(command_ptr->parameter,NULL,16);
			}
			else if(strcmp(command_ptr->opcode,"USE")==0)
			{
				/****************
				USE if not be use
				*****************/
				int isuse=0;
				block search=section_index[index]->segement;
				for(;;)
				{
					if(strcmp(search.name,command_ptr->parameter)==0)
					{
						isuse=1;
					}
					if(search.rlink==NULL)
					{
						break;
					}
					else
					{
						search=*search.rlink;
					}
				}
			
				if(isuse==1)
				{
					block_data=search;
				}
				else
				{
					block_num++;
					block_data.rlink=malloc(sizeof(block));
					block btemp=*block_data.rlink;
					btemp.llink = &block_data;	btemp.rlink = NULL; btemp.name = command_ptr->parameter;	btemp.num = block_num; btemp.pc = 0x0;	btemp.loc = btemp.pc;
					block_data = btemp;
				}
				continue;
			}
			else if(strcmp(command_ptr->opcode,"CSET")==0)
			{
				/***************************
				a section end and to next
				***************************/
				section_index[index+1]=malloc(sizeof(section));
				block_data=section_index[index+1]->segement;
				block_data.name=command_ptr->label;	block_data.num=index+1;	block_data.pc = 0x0;	block_data.loc = block_data.pc;	block_data.llink=NULL;	block_data.rlink=NULL;
				break;
			}
			
			block_data.loc = block_data.pc;
			block_data.pc+=code_length(command_ptr->opcode,command_ptr->parameter);
			
			/**************************
			literal
			***************************/
			if(strcmp(command_ptr->label,"*")==0 && isLTORG==1)
			{
				fprintf(SYMTAB,"%04X\t%s\t0\n",block_data.loc,command_ptr->opcode);
				fprintf(destination,"%04X\t%s\t%s\t%s\n",block_data.loc,command_ptr->label,command_ptr->opcode,command_ptr->parameter);
				continue;
			}
			else if(strcmp(command_ptr->opcode,"LTORG")==0)
			{
				isLTORG=1;
				continue;
			}
			
			if(strcmp(command_ptr->opcode,"EQU")==0)
			{
				if(strcmp(command_ptr->parameter,"*")==0)
				{
					fprintf(SYMTAB,"%04X\t%s\t1\n",block_data.pc,command_ptr->label);
				}
			}
			else if(!(strcmp(command_ptr->opcode,".")==0))
			{	
				fprintf(destination,"%04X\t%s\t%s\t%s\n",block_data.loc,command_ptr->label,command_ptr->opcode,command_ptr->parameter);
				if(command_ptr->label[0]!='\0' && command_ptr->label[0]!='*')
				{
					int duplicate=0;
					fprintf(SYMTAB,"%04X\t%s\t2\n",block_data.loc,command_ptr->label);
				}
				else if(command_ptr->label[0]=='*')
				{
					fprintf(SYMTAB,"%04X\t%s\t3\n",block_data.loc,command_ptr->opcode);
				}
			}
		}
	}
	fclose(source);
	fclose(SYMTAB);

	return 0;
}