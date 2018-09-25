#include "ctl_file.h"
#include <string.h>
#include <stdlib.h>
#include <pwd.h>  
#include <sys/types.h>  
#include <stddef.h>  

struct role
{
	char name[200];
	int uid;
	int file_list[200];
};

struct user  
{  
    char * pw_name; /* Username, POSIX.1 */  
    __uid_t pw_uid; /* User ID, POSIX.1 */  
    __gid_t pw_gid; /* Group ID, POSIX.1 */  
    int role_id;
};  

struct role roles [100];
struct user users[200];
struct user relations[200];
int usercnt;
int relationcnt;
int curuid=0;

int write_role()
{
   char buffer[200];
   FILE *fp=common_file_open("youho/role_list","w");
   if(fp==NULL)
	{
		printf("open failed\n");
		return -1;
	}
   int j=0;
   int i=0;
   int offset=0;
   //strcpy(buffer,"sss");
   // printf("%d\n",curuid );
   for(i=0;i<curuid;i++)
	{
       offset+= sprintf(buffer+offset,"%s:%d",roles[i].name,roles[i].uid);
        for(j=0;j<200;j++)
        {
        	if(roles[i].file_list[j]>0)
        		offset+=sprintf(buffer+offset,":%d",roles[i].file_list[j]);
        }
        offset+=sprintf(buffer+offset,"\n");
	}

   fputs(buffer,fp);
   fclose(fp);
   return 0;
}

int read_roles()
{
	int i=-1;
	int j=-1;
	for(i=0;i<100;i++)
	{
		roles[i].uid=-1;
		for(j=0;j<200;j++)
		{
			roles[i].file_list[j]=-1;
		}
	}

	FILE *fp=common_file_open("youho/role_list","r");
	if(fp==NULL)
	{
		printf("open failed\n");
		return -1;
	}
	char buffer[200];
	int index=0;
	int fid=-1;
	// int uid=-1;
	// while(fscanf(fp,"%s:%d:%s",roles[index].name,&roles[index].uid,buffer)==3)
 //        {
 //          //  if(roles[index].uid<0)
 //        		// continue;
 //        	char*temp = strtok(buffer,":");
 //        	i=0;
	// 	    while(temp)
	// 	    {
		       
	// 	    	fid=atoi(temp); printf("%d",fid);
	// 	    	roles[index].file_list[i++]=fid;
	// 	        temp = strtok(NULL,":");
	// 	    }
	// 	    index++;
 //        }
	
	while(fgets(buffer,200,fp)!=NULL)
		{
			int xx=0;
			char*temp = strtok(buffer,":");
        	i=0;
		    while(temp)
		    {
		         if(xx==0)
		         	{
		         		strcpy(roles[index].name,temp);
		         		// printf("%s",temp);
		         		xx++;
		         	}
		         else if(xx==1)
		         {

		         	roles[index].uid=atoi(temp); 
		         	xx++;
		         }
		    	else
		    	{
		    		fid=atoi(temp); //printf("%d",fid);
		    		roles[index].file_list[i++]=fid;
		    	}
		        temp = strtok(NULL,":");
		    }

		    index++;
		}
    fclose(fp);

    // int t_uid=-1;
    // for(;index>0;index--)
    // {
    //    t_uid=(roles[index-1].uid);
    //    if(t_uid)>0
    //    {
    //    	curuid=
    //    	break;
    //    }
    // }
     // printf("%d",roles[index-1].uid);
     curuid=roles[index-1].uid>0?roles[index-1].uid:-roles[index-1].uid;
     // printf("%d",curuid);
  return curuid;
}

void printf_roles()
{
	int u=read_roles();
	   // printf("%d",u);
	int i=0;
	int index=1;
	for(i=0;i<u;i++)
	{
		//printf("++++  uid: %d  ,name: %s\n", roles[i].uid,roles[i].name);
		if(roles[i].uid<0)
			continue;

		printf("sn: %d\n",index++);
		printf("uid: %d  ,name: %s\n", roles[i].uid,roles[i].name);
		printf("file_list: \n");
		int j=0;
		while(roles[i].file_list[j]>0)
		{
			printf("%d\n",roles[i].file_list[j]);
			j++;
		}
		printf("\n");
	}

}


int add_role()
{
	// printf_roles();
	int uid=read_roles();
	//printf_roles();
	// struct role eg_role;
	printf("input name\n");
    scanf("%s",roles[uid].name);
    roles[uid].uid=uid+1;

    printf("choose file\n");
    file_list();

    int fid=-1;
    int j=0;
    while(scanf("%d",&fid)&&fid>0)
    {
    	//printf("%d",fid);
       roles[uid].file_list[j++]=fid;
    }

    curuid=uid+1;
    if(write_role(roles,uid)<0)
    {
    	printf("write error");
    	return -1;
    }
    
    // uid++;
    printf_roles();
    return 0;

}

int del_role()
{
   printf_roles();
   printf("\ninput uid\n");
   int uid,i=0;
   scanf("%d",&uid);

   for(i=0;i<curuid;i++)
   {
   	 if(roles[i].uid==uid)
   	 {
   	 	roles[i].uid=-uid;
   	 	return write_role();
   	 }
   }
   return -1;
}

int edit_role()
{
    printf_roles();
    printf("\ninput uid\n");
    int uid,i=0;
    scanf("%d",&uid);

  for(i=0;i<curuid;i++)
   {
   	 if(roles[i].uid==uid)
   	 {
   	 	 printf("\ninput name\n");
   	 	 scanf("%s",roles[i].name);
   	 	 file_list();
   	 	 printf("choose file\n");
   	 	  int fid=-1;
		    int j=0;
		    while(scanf("%d",&fid)&&fid>0)
		    {
		    	//printf("%d",fid);
		       roles[i].file_list[j++]=fid;
		    }
		    for(;j<200;j++)
		    	roles[i].file_list[j]=-1;
   	 	return write_role();
   	 }
   }
   
   return -1;
}

int write_relation()
{
  FILE *fp=common_file_open("youho/relation","w");
  if(fp==NULL)
  	return -1;
  char buffer[100];
  int i=0;
  for(i=0;i<usercnt;i++)
	{
		if(users[i].role_id!=-1)
		{
			sprintf(buffer,"%s:%d:%d:%d\n",users[i].pw_name,users[i].pw_uid,users[i].pw_gid,users[i].role_id);
			fputs(buffer,fp);
		}
	}

  fclose(fp);
}

int read_relation()
{
  FILE *fp=common_file_open("youho/relation","r");
  if(fp==NULL)
  	return -1;
  int i=0;
  char buffer[100];
 //  for(i=0;i<usercnt;i++)
	// {
	// 	if(users[i].role_id!=-1)
	// 	{
			//sprintf(buffer,"%s:%d:%d:%d",users[i].pw_name,users[i].pw_uid,users[i].pw_gid,users[i].role_id);
  
 //      // fgets(buffer,100,fp);
 //      // printf("-----%s",buffer);
 //  relations[0].pw_name=(char*)malloc(400);
 // printf("%d",fscanf(fp,"%s/:%d/:%d/:%d",relations[i].pw_name,&relations[i].pw_uid,&relations[i].pw_gid,&relations[i].role_id));
 //   // {
 //  	     printf("-----%d",relations[i].pw_uid);
 //   // 	  i++;
 //   // }

  while(fgets(buffer,100,fp))
  {
		  	char*temp = strtok(buffer,":");
		  	int xx=0;
        	
		    while(temp)
		    {
		         if(xx==0)
		         {
		         		relations[i].pw_name=(char*)malloc(400);
		         		strcpy(relations[0].pw_name,temp);
		         		// printf("%s",temp);
		         		xx++;
		         }
		         else if(xx==1)
		         {
		         	relations[i].pw_uid=atoi(temp); 
		         	//printf("%d\n",relations[i].pw_uid);
		         	xx++;
		         }
		    	else if(xx==2)
		    	{
		    		relations[i].pw_gid=atoi(temp); 
		         	xx++;
		    	}
		    	else
		    	{
		    	    relations[i].role_id=atoi(temp); 
		         	xx++;
		    	}
		        temp = strtok(NULL,":");
		    }
		    i++;
  }

   relationcnt=i;
   //printf("%d\n",i);
	// 	}
	// }

  fclose(fp);

}

int get_user()
{
    int res=read_relation();
    if(res<0)
    {
    	printf("%d\n",res);
    	return -1;
    }
    struct passwd* pstPwd = NULL;  

   // printf("+_++%d",relationcnt);
    //setpwent();  
    int i=0;
    int j=0;
  
    while ( (pstPwd = getpwent()) != NULL )  
    {  
    	char name[100];
    	users[i].pw_name=(char *)malloc(400);
        strcpy(users[i].pw_name,pstPwd->pw_name);
        users[i].pw_uid=pstPwd->pw_uid;
        users[i].pw_gid=pstPwd->pw_uid;

        users[i].role_id=-1;
        for(j=0;j<relationcnt;j++)
        {
        	if(users[i].pw_uid==relations[j].pw_uid)
        	{
        		users[i].role_id=relations[j].role_id;
        	}
        }
        //printf("%s\n",users[i].pw_name);
        i++;
       // printf("%s\n",users[i-1]->pw_name);
    }  
    usercnt=i;
  
  //   for(i=0;i<usercnt;i++)
  // {
  // 	printf("%s\n",users[i]->pw_name);
  // }   
    endpwent();  
    printf("%s\n",users[1].pw_name);
}  

int show_user()
{
	get_user();
  int i=0;
  for(;i<usercnt;i++)
  {
  	printf("uid:%i \t\t  name:%s \t\t  role:%i\t\n",users[i].pw_uid,users[i].pw_name,users[i].role_id);
  }     
	   //  struct passwd *user;
    // while((user = getpwent()) != 0)
    // {
    //      printf("%s:%d:%d:%s:%s:%s\n", user->pw_name, user->pw_uid, user->pw_gid,
    //     user->pw_gecos, user->pw_dir, user->pw_shell);
    // }
    // endpwent();
}



int apply_role()
{
    show_user();
    printf("input uid\n");
    int uid;
    scanf("%d",&uid);  

    printf_roles();
    printf("choose role\n");
    int rid;
    scanf("%d",&rid);

    int i=0;
    for(i=0;i<usercnt;i++)
	{
		if(users[i].pw_uid==uid)
		{
			users[i].role_id=rid;
		}
	}
	write_relation();
}

// gcc -o ctl ctl.c ctl_file.h ctl_content.h ctl_role.h
// ./ctl