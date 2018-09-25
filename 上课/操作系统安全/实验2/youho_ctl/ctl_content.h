#include "ctl_file.h"

int add_path(char * path)
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