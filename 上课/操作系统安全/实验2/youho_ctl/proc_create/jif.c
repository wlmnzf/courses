#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/types.h>

#include <linux/fs.h>		// for basic filesystem
#include <linux/proc_fs.h>	// for the proc filesystem
#include <linux/seq_file.h>	// for sequence files
#include <linux/jiffies.h>	// for youhofies
#include <linux/slab.h>		// for kzalloc, kfree
#include <linux/uaccess.h>	// for copy_from_user

#include <linux/lsm_hooks.h>
#include <linux/sysctl.h>
#include <linux/limits.h>

#define BUF_SIZE 128
static char *str = NULL;

// global var


// struct myuser  
// {  
//     char * pw_name;  Username, POSIX.1   
//     __uid_t pw_uid; /* User ID, POSIX.1 */  
//     __gid_t pw_gid; /* Group ID, POSIX.1 */  
//     int role_id;
// };  

#define FILE_NUM 10
#define ROLE_NUM 10
#define RELA_NUM 10
static char *path[FILE_NUM];
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

// seq_operations -> show
static int youho_show(struct seq_file *m, void *v)
{
	char buf[BUF_SIZE];
	int ret = 0;
	ret = sprintf(buf, "current kernel time is %llu\n", (unsigned long long) get_jiffies_64());
	ret += sprintf(buf + ret, "str is %s\n", str);

	seq_printf(m, "%s", buf);

	return 0; //!! must be 0, or will show nothing T.T
}

// file_operations -> write
static ssize_t youho_file_list(struct file *file, const char __user *buffer, size_t count, loff_t *f_pos)
{
	//分配临时缓冲区
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

        	strcpy(path[i++],token);
       // p=token;

        //printk(KERN_DEBUG "file_list :%d %s\n",i++, p);   
       // p++;
    }  

filecnt=i;

    for(i=0;i<4;i++)
    	printk(KERN_DEBUG " %s\n", path[i]);  
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

// seq_operations -> open
static int youho_open(struct inode *inode, struct file *file)
{
	return single_open(file, youho_show, NULL);
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
// module init
static int __init youho_init(void)
{
	// path =kmalloc(PATH_MAX*10, 0);
	filecnt=0;
	rolecnt=0;

	int i=0;
	int j=0;
	for(i=0;i<FILE_NUM;i++)
		path[i] =kmalloc(PATH_MAX, 0);

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
	    return -ENOMEM;
	}

	youho_switch = proc_create("switch", 0666, NULL, &switch_fops);
	if (NULL == youho_switch)
	{
	    return -ENOMEM;
	}

	youho_role_list = proc_create("role_list", 0666, NULL, &role_list_fops);
	if (NULL == youho_role_list)
	{
	    return -ENOMEM;
	}

	youho_relation = proc_create("relation", 0666, NULL, &relation_fops);
	if (NULL == youho_relation)
	{
	    return -ENOMEM;
	}


	return 0;
}

// module exit
static void __exit youho_exit(void)
{
	remove_proc_entry("switch", NULL);
	remove_proc_entry("file_list", NULL);
	remove_proc_entry("role_list", NULL);
	remove_proc_entry("relation", NULL);
	kfree(str);
	int i=0;
	for(i=0;i<FILE_NUM;i++)
		kfree(path[i]);

}

module_init(youho_init);
module_exit(youho_exit);

MODULE_AUTHOR("aran");
MODULE_LICENSE("GPL");
