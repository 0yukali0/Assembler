#include "sic.h"

int main(char argc,char* argv[])
{
	FILE* source = fopen(argv[1],"r");
	FILE* destination = fopen("middle.txt","w");
	FILE* SYMTAB = fopen("symtab0.txt","w");

	char temp[100];
	section_index[0] = malloc(sizeof(section));
	instruction_ptr command_ptr = malloc(sizeof(instruction));

	for(int index=0;section_index[index]!=NULL;index++)
	{
		block block_data = &(section_index[index]->segement);
		block_data.name="";	block_data.num=0;	block_data.pc = 0x0;	block_data.loc = block_data.pc;	block_data.llink=null;	block_data.rlink=null;
		symtab_head[0]->llink=null; symtab_head[0]->rlink=null;
		int isLTORG = 0;
		/*******************************************************************************************************
		section create automatically is  waiting!!!!!!!!!!!!!! :)
		Here just for sic or sic/xe in basis way for block in single section,not for multiple sections version
		********************************************************************************************/

		for(int block_num=0;fgets(temp,100,source)!=NULL || !(strcmp(command_ptr->opcode,"END")==0);)
		{
			
			effect_read(command_ptr,temp);

			if(strcmp(command_ptr->opcode,"START")==0)
			{
				block_data.pc = strtol(command_ptr->parameter,NULL,16);
				block_data.loc = block_data.pc;
			}
			else if(strcmp(command_ptr->opcode,"START")!=0)
			{
				block_data.loc = block_data.pc;
			}
			else if(strcmp(command_ptr->opcode,"USE")==0)
			{
				block_num++;
				block_data.rlink=malloc(sizeof(block));
				block btemp=*block_data.rlink;
				btemp.llink = &block_data;	btemp.rlink = null; btemp.name = command_ptr->parameter;	btemp.num = block_num; btemp.pc = 0x0;	btemp.loc = btemp.pc;
				block_data = btemp;
			}else if(strcmp(command_ptr->opcode,"CSET")==0)
			{
				
			}
			
		
			block_data.pc+=code_length(command_ptr->opcode,command_ptr->parameter);
			
			/**************************
			literal
			***************************/
			if(strcmp(command_ptr->label,"*")==0 && isLTORG==1)
			{
				fprintf(SYMTAB,"%04X\t%s\n",block_data.loc,command_ptr->opcode);
			}
			else if(strcmp(command_ptr->opcode,"LTORG")==0)
			{
				isLTORG=1;
				continue;
			}
			else if((strcmp(command_ptr->opcode,"EQU")==0)
			{
				if(strcmp(command_ptr->parameter,"*")==0)
				{
					fprintf(SYMTAB,"%04X\t%s\n",block_data.pc,command_ptr->opcode);
				}
				else
				{
					for(;;)
				}
			}
			else if(!(strcmp(command_ptr->opcode,".")==0))
			{	
				fprintf(destination,"%04X\t%s\t%s\t%s\n",block_data.loc,command_ptr->label,command_ptr->opcode,command_ptr->parameter);
				if(command_ptr->label[0]!='\0' && command_ptr->label[0]!='*')
				{
					int duplicate=0;
					fprintf(SYMTAB,"%04X\t%s\n",block_data.loc,command_ptr->label);
				}
				else if(command_ptr->label[0]=='*')
				{
					fprintf(SYMTAB,"%04X\t%s\n",block_data.loc,command_ptr->opcode);
				}
			}
		}
	}
	fclose(source);
	fclose(SYMTAB);

	return 0;
}