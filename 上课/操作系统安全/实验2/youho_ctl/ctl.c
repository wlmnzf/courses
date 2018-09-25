#include<stdio.h>
// #include "ctl_file.h"
// #include "ctl_content.h"
#include<getopt.h>
#include "ctl_role.h"


int switch_on();
int switch_off();
int add_path(char * path);
void file_list();
int add_role();
void printf_roles();
int apply_role();


static const struct option options[] = {
        {"switch",         1, NULL, 's'},
        {"add",            0, NULL, 'a'},
        {"delete",         0, NULL, 'd'},
        {"edit",           0, NULL, 'e'},
        {"file",           1, NULL, 'f'},
        {"list",           1, NULL, 'l'},
        {NULL,             0, 0,     0 }
};

static const char *opt_string = "s:adef:l:";

int main(int argc, char* argv[])
{
   int  next_option = getopt_long(argc, argv, opt_string,options, NULL);
      
       switch(next_option)
        {
            case 's':
                {
                   // printf("s received! Para:%s\n", optarg);
                	if(strcmp("1",optarg)==0)
                	{
                		switch_on();
                	}
                	else
                	{
                		switch_off();
                	}
                }break;
            case 'a':
                {
                    add_role();
                }break;
            case 'd':
                {
                    del_role();
                }break;
            case 'e':
	            {
	            	edit_role();
	            }break;
	        case 'f':
	            {
	            	//printf("%s\n",optarg );
	            	add_file(optarg);
	            }break;
	        case 'l':
	            {
	            	if(strcmp("role",optarg)==0)
	            		printf_roles();
	            	else
	            		file_list();
	            	//printf("%s\n",optarg );
	            	//add_file(optarg);
	            }break;
	        default:
	        {
 				//printf("welcome!\n");
	        	apply_role();
	        }
	       
	        break;

          }
system("/bin/sh  ./init.sh");   // add_path("/etc/");
   // add_path("/etc/int/");
	//file_list();
   // switch_on();
	//add_role();
// printf_roles();
	return 0;
}