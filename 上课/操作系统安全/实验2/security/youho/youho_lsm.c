#include <linux/lsm_hooks.h>
#include <linux/sysctl.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>

#include<linux/module.h>
#include<linux/sched.h>
#include<linux/fs.h>
#include<linux/seq_file.h>
#include<linux/slab.h>
#include <linux/limits.h>
static unsigned long long count = 0;
// static char buffer[20];


#define FILE_NUM 10
#define ROLE_NUM 10
#define RELA_NUM 10

static char *paths[FILE_NUM];
static int switch_my;
static int filecnt;
static int rolecnt;
static int relationcnt;

struct myrole
{
	char *name;
	int uid;
	int file_list[FILE_NUM];
};
struct relation  
{  
    char * name; /* Username, POSIX.1 */  
    int uid; /* User ID, POSIX.1 */  
    int gid; /* Group ID, POSIX.1 */  
    int role_id;
};  

static struct myrole roles[ROLE_NUM];
static struct relation relations[RELA_NUM];



static char *str = NULL;

static int youho_show(struct seq_file *m,void *v){
	//seq_printf(m,"hello from proc file\n");
	seq_printf(m,"%s\n",str);
	return 0;
}

static int youho_open(struct inode *inode,struct file *file){
	return single_open(file,youho_show,NULL);
}


// static ssize_t my_proc_write(struct file *file, const char __user *buffer, size_t count, loff_t *f_pos)
// {
// 	//分配临时缓冲区
// 	char *tmp = kzalloc((count+1), GFP_KERNEL);
// 	if (!tmp)
// 		return -ENOMEM;
//     printk(KERN_DEBUG "user buffer : 0x%lx %s\n", buffer, buffer);
// 	//将用户态write的字符串拷贝到内核空间
// 	//copy_to|from_user(to,from,cnt)
// 	if (copy_from_user(tmp, buffer, count)) {
// 		kfree(tmp);
// 		return -EFAULT;
// 	}

// 	//将str的旧空间释放，然后将tmp赋值给str
// 	kfree(str);
// 	str = tmp;

// 	return count;
// }
// file_operations -> write
static ssize_t youho_file_list(struct file *file, const char __user *buffer, size_t count, loff_t *f_pos)
{
	//分配临时缓冲区
	// int x=0;
	// filecnt=0;
 //    for(x=0;i<FILE_NUM;x++)
	// 	path[x] =kmalloc(PATH_MAX, 0);



	char *tmp = kzalloc((count+1), GFP_KERNEL);
	if (!tmp)
		return -ENOMEM;
   // printk(KERN_DEBUG "file_list : 0x%lx %s\n", buffer, buffer);

     char *token, *cur = buffer;  
     int i=0;
     // char *p=*path;
	
    // char *path= d_path(&(file->f_path),buffer,PATH_MAX);
    // printk("file:%s",path);
    // kfree(buffer);
  // while (token = strsep(&cur, "\n")&&token!= NULL) {  
  //   printk(KERN_DEBUG "file_list :%d %s\n",i++, token);
  // }  
        char delim[] = "\n";   
     for(token = strsep(&cur, delim); token != NULL; token = strsep(&cur, delim)) {    
        if(strcmp("",token)==0)
        	break;

        	strcpy(paths[i++],token);
       // p=token;

        //printk(KERN_DEBUG "file_list :%d %s\n",i++, p);   
       // p++;
    }  

filecnt=i;

    for(i=0;i<4;i++)
    	printk(KERN_DEBUG " %s\n", paths[i]);  
	//将用户态write的字符串拷贝到内核空间
	//copy_to|from_user(to,from,cnt)
	if (copy_from_user(tmp, buffer, count)) {
		kfree(tmp);
		return -EFAULT;
	}

	//将str的旧空间释放，然后将tmp赋值给str
	kfree(str);
	str = tmp;

	return count;
}

// file_operations -> write
static ssize_t youho_switch(struct file *file, const char __user *buffer, size_t count, loff_t *f_pos)
{
	//分配临时缓冲区
	char *tmp = kzalloc((count+1), GFP_KERNEL);
	if (!tmp)
		return -ENOMEM;
    printk(KERN_DEBUG "switch : 0x%lx %s\n", buffer, buffer);

    if(strcmp(buffer,"1")==0)
    	switch_my=1;
    else
    	switch_my=0;

    printk(KERN_DEBUG "switch_var : %d\n", switch_my);

	//将用户态write的字符串拷贝到内核空间
	//copy_to|from_user(to,from,cnt)
	if (copy_from_user(tmp, buffer, count)) {
		kfree(tmp);
		return -EFAULT;
	}

	//将str的旧空间释放，然后将tmp赋值给str
	kfree(str);
	str = tmp;

	return count;
}


// file_operations -> write
static ssize_t youho_role_list(struct file *file, const char __user *buffer, size_t count, loff_t *f_pos)
{
	//分配临时缓冲区
	char *tmp = kzalloc((count+1), GFP_KERNEL);
	if (!tmp)
		return -ENOMEM;
    //printk(KERN_DEBUG "role_list : 0x%lx %s\n", buffer, buffer);

     char *token, *cur = buffer;  
     int i=0,j=0;
     char delim[] = "\n";   

     for(token = strsep(&cur, delim); token != NULL; token = strsep(&cur, delim)) {    
        if(strcmp("",token)==0)
        	break;

// printk("%s ",token);
       char *content;
       int k=0;j=0;
       for(content = strsep(&token, ":"); content != NULL; content = strsep(&token, ":")) {    
        if(strcmp("",content)==0)
        	break;
         // printk("%s ",content);
	        	if(k==0)
	        	{
	        		strcpy(roles[i].name,content);
	        	    // printk("%s ",roles[k].name);
	        		k++;
	        	}
	        	else if(k==1)
	        	{
	        		roles[i].uid = simple_strtol(content, NULL, 10);
	        		int z=0;
			     	while(roles[i].file_list[z]!=-1)
			     	{
			     		roles[i].file_list[z]=-1;
						//printk("  %d",roles[j].file_list[jjj]);
						z++;
						if(z==ROLE_NUM)
							break;
			     	}
				      k++;
	        	}
	        	else
	        	{
	        		roles[i].file_list[j++]=simple_strtol(content, NULL, 10);

	        	}
	        	// k++;
        }
        i++;

    }  

    rolecnt=i;
   // printk("     %d",i);

     for (j = 0; j <i; ++j)
     {
     	printk("%s  %d",roles[j].name,roles[j].uid);
     	int jjj=0;
     	while(roles[j].file_list[jjj]!=-1)
     	{
			printk("  %d",roles[j].file_list[jjj]);
			jjj++;
     	}
     	printk("\n");
     	/* code */
     }
	//将用户态write的字符串拷贝到内核空间
	//copy_to|from_user(to,from,cnt)
	if (copy_from_user(tmp, buffer, count)) {
		kfree(tmp);
		return -EFAULT;
	}

	//将str的旧空间释放，然后将tmp赋值给str
	kfree(str);
	str = tmp;

	return count;
}

// file_operations -> write
static ssize_t youho_relation(struct file *file, const char __user *buffer, size_t count, loff_t *f_pos)
{
	//分配临时缓冲区
	char *tmp = kzalloc((count+1), GFP_KERNEL);
	if (!tmp)
		return -ENOMEM;
    // printk(KERN_DEBUG "relation : 0x%lx %s\n", buffer, buffer);



     char *token, *cur = buffer;  
     int i=0,j=0;
     char delim[] = "\n";   

     for(token = strsep(&cur, delim); token != NULL; token = strsep(&cur, delim)) {    
        if(strcmp("",token)==0)
        	break;

// printk("%s ",token);
       char *content;
       int k=0;j=0;
       for(content = strsep(&token, ":"); content != NULL; content = strsep(&token, ":")) {    
        if(strcmp("",content)==0)
        	break;
         // printk("%s ",content);
	        	if(k==0)
	        	{
	        		strcpy(relations[i].name,content);
	        	     //printk("%s ",relations[k].name);
	        		k++;
	        	}
	        	else if(k==1)
	        	{
	        		relations[i].uid = simple_strtol(content, NULL, 10);

	        		k++;
	        	}
	        	else if(k==2)
	        	{
	        		relations[i].gid = simple_strtol(content, NULL, 10);
	        		k++;

	        	}
	        	else
	        	{
	        		relations[i].role_id = simple_strtol(content, NULL, 10);
	        		k++;
	        	}
	        	// k++;
        }
        i++;

    }  
relationcnt=i;


for(i=0;i<relationcnt;i++)
	printk("%s  %d  %d  %d",relations[i].name,relations[i].uid,relations[i].gid,relations[i].role_id);



	//将用户态write的字符串拷贝到内核空间
	//copy_to|from_user(to,from,cnt)
	if (copy_from_user(tmp, buffer, count)) {
		kfree(tmp);
		return -EFAULT;
	}

	//将str的旧空间释放，然后将tmp赋值给str
	kfree(str);
	str = tmp;

	return count;
}


static const struct file_operations file_list_fops =
{
	.owner		= THIS_MODULE,
	.open		= youho_open,
	.read		= seq_read,
	.write 		= youho_file_list,
	.llseek		= seq_lseek,
	.release	= single_release,
};


static const struct file_operations switch_fops =
{
	.owner		= THIS_MODULE,
	.open		= youho_open,
	.read		= seq_read,
	.write 		= youho_switch,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static const struct file_operations role_list_fops =
{
	.owner		= THIS_MODULE,
	.open		= youho_open,
	.read		= seq_read,
	.write 		= youho_role_list,
	.llseek		= seq_lseek,
	.release	= single_release,
};


static const struct file_operations relation_fops =
{
	.owner		= THIS_MODULE,
	.open		= youho_open,
	.read		= seq_read,
	.write 		= youho_relation,
	.llseek		= seq_lseek,
	.release	= single_release,
};




// static struct file_operations my_fops={
// 	.owner = THIS_MODULE,
// 	.open = my_proc_open,
// 	.release = single_release,
// 	.read = seq_read,
// 	.llseek = seq_lseek,
// 	.write = my_proc_write
// };

// static int __init hello_init(void){
// 	struct proc_dir_entry *entry;
// 	entry = proc_create("helloproc",0777,NULL,&my_fops);
// 	if(!entry){
// 		return -1;	
// 	}else{
// 		printk(KERN_INFO "create proc file successfully\n");
// 	}
// 	return 0;
// }

// static void __exit hello_exit(void){
// 	remove_proc_entry("helloproc",NULL);
// 	printk(KERN_INFO "Goodbye world!\n");
// }





int demo_task_create(unsigned long clone_flags)
{
   // printk("[+geek] call task_create(). count=%llu\n", ++count);    
    return 0;
}

static int youho_file_open(struct file *file, const struct cred *cred)
{
	if(switch_my==1)
	{
		char * buffer;
		buffer =kmalloc(PATH_MAX, 0);
	    char *path= d_path(&(file->f_path),buffer,PATH_MAX);
	    //printk("file:%s",path);
	    kfree(buffer);

	    int fileindex=-1;
	    int userindex=-1;
	    int uid=(cred->uid).val;
	    int gid=(cred->gid).val;

		int i=0;
	    for(i=0;i<filecnt;i++)
	    {
	    	if(strcmp(paths[i],path)==0)
	    	{
	    		fileindex=i;  
	    		printk("fileindex:%d",fileindex);
	    		break;
	    	}
	    }


	    for(i=0;i<relationcnt;i++)
	    {
	    	if(uid==relations[i].uid)
	    	{
	    		userindex=i;   
printk("userindex:%d",userindex);

printk("relations[userindex].role_id:%d",relations[userindex].role_id);	    		
break;
	    	}
	    }


	    if(userindex!=-1&&fileindex!=-1)
	    {
	    	int roleindex=-1;
	    	for(i=0;i<rolecnt;i++)
	    	{
	    		if(roles[i].uid==relations[userindex].role_id)
	    		{
	    			printk("roles[i].uid=:%d",roles[i].uid);
	    			int x=0;
	    			while(roles[i].file_list[x]!=-1&&x<FILE_NUM)
	    			{
	    				printk("roles[i].file_list[%d]:%d",x,roles[i].file_list[x]);
	    				if(roles[i].file_list[x]==fileindex+1)
	    					return 0;

	    				x++;
	    			}
	    				    			
	    			return -13;
	    		}
	    	}
	    }


    }
   // printk("uid:%d,gid:%d",cred->uid,cred->gid);
   
	return 0;
}


static struct security_hook_list demo_hooks[] = {
    LSM_HOOK_INIT(task_create,demo_task_create),
    LSM_HOOK_INIT(file_open, youho_file_open),
};
void __init demo_add_hooks(void)
{
    pr_info("Demo: becoming mindful.\n");        //打印相关信息，可以通过dmesg |  grep Yama:查看
    security_add_hooks(demo_hooks, ARRAY_SIZE(demo_hooks));   //添加安全模块函数
}



static __init int youho_init(void){
      security_add_hooks(demo_hooks, ARRAY_SIZE(demo_hooks));

 //     printk(KERN_INFO "my_module_init called.  Module is now loaded.\n");

 //     memset(kernel_buf,0x00,128);
 // my_proc_fs=proc_create(PROC_NAME,0777,NULL,&myTest_proc_fops);  //create_proc_entry(PROC_NAME,0666,NULL);
 // my_proc_fs->read_proc=read_proc;
 // my_proc_fs->write_proc=write_proc;

	// struct proc_dir_entry *entry;
	// entry = proc_create("helloproc",0777,NULL,&my_fops);
	// if(!entry){
	// 	printk("create proc file failed\n");
	// 	return -1;	
	// }else{
	// 	printk("create proc file successfully\n");
	// }
    filecnt=0;
	rolecnt=0;
	relationcnt=0;

	int i=0;
	int j=0;

	for(i=0;i<FILE_NUM;i++)
		paths[i] =kmalloc(PATH_MAX, 0);

	for(i=0;i<ROLE_NUM;i++)
	{
		roles[i].name=kmalloc(10, 0);
		roles[i].uid=-1;
		
		for(j=0;j<FILE_NUM;j++)
		{
			roles[i].file_list[j]=-1;
		}
	}

	for(i=0;i<RELA_NUM;i++)
	{
		relations[i].name=kmalloc(10, 0);
		relations[i].uid=-1;
		relations[i].gid=-1;
		relations[i].role_id=-1;
	}

	struct proc_dir_entry* youho_file_list;
	struct proc_dir_entry* youho_switch;
	struct proc_dir_entry* youho_relation;
	struct proc_dir_entry* youho_role_list;

	youho_file_list = proc_create("file_list", 0666, NULL, &file_list_fops);
	if (NULL == youho_file_list)
	{
		printk("create proc [file_list] failed\n");
	    return -ENOMEM;
	}

	youho_switch = proc_create("switch", 0666, NULL, &switch_fops);
	if (NULL == youho_switch)
	{
		printk("create proc [switch] failed\n");
	    return -ENOMEM;
	}

	youho_role_list = proc_create("role_list", 0666, NULL, &role_list_fops);
	if (NULL == youho_role_list)
	{
		printk("create proc role_list failed\n");
	    return -ENOMEM;
	}

	youho_relation = proc_create("relation", 0666, NULL, &relation_fops);
	if (NULL == youho_relation)
	{
		printk("create proc relation failed\n");
	    return -ENOMEM;
	}


    return 0;
}

security_initcall(demo_init);