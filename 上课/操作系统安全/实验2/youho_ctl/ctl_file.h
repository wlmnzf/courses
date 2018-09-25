#include<stdio.h>
// char *file_list[1000]
FILE * common_file_open(char * path,char * mode)
{
        FILE *fp=NULL;
        if((fp=fopen(path,mode))== NULL)
		{
		 printf("The file can not be opened.\n"); 
		   return NULL;
		}
		return fp;
}

int add_file(char * path)
{
   FILE *fp=common_file_open("youho/file_list","a");
   	if(fp==NULL)
	{
		return 1;
	}

   fputs(path,fp);
   fputc('\n',fp);
   fclose(fp);
   return 0;
    
}


void file_list()
{
	FILE *fp=common_file_open("youho/file_list","r");
	if(fp==NULL)
	{
		printf("open failed\n");
	}
	char buffer[200];
	int index=1;
	while(fgets(buffer,200,fp)!=NULL)
        printf("%d %s\n",index++,buffer);
    fclose(fp);
}

int switch_op(char state)
{
	FILE* fp=common_file_open("youho/switch","w");
	if(fp==NULL)
	{
		return 1;
	}

    fputc(state,fp);
     // char buffer[200];
	 // while(fgets(buffer,SIZE,fp)!=NULL)
  //       printf("%s\n",buffer);

	fclose(fp);
	return 0;
}
int switch_on()
{
	if(switch_op('1')==1)
	{
		printf("on error\n");
	}
	else
	{
		printf("on success\n");
	}

	return 0;
}

int switch_off()
{
	if(switch_op('0')==1)
	{
		printf("off error\n");
	}
	else
	{
		printf("off success\n");
	}

    return 0;
}