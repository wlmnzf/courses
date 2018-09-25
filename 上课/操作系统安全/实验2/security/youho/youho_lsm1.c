//demo_lsm.c
#include <linux/lsm_hooks.h>
#include<linux/syscalls.h>
#include <linux/sysctl.h>
#include <linux/kernel.h> 
#include <linux/module.h> 
#include <linux/fs.h> 
#include <asm/uaccess.h> 
#include <linux/mm.h> 
#include <linux/integrity.h>

#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/rbtree.h>
#include <linux/file.h>
#include <linux/uaccess.h>
// #include "integrity.h"


// #include <linux/moduleparam.h>
// #include <linux/ratelimit.h>
// #include <linux/crypto.h>
// #include <linux/scatterlist.h>
// #include <linux/err.h>
// #include <linux/slab.h>
// #include <crypto/hash.h>

int my_integrity_kernel_read(struct file *file, loff_t offset,
			  char *addr, unsigned long count)
{
	mm_segment_t old_fs;
	char __user *buf = (char __user *)addr;
	ssize_t ret;

	if (!(file->f_mode & FMODE_READ))
		return -EBADF;

	old_fs = get_fs();
	set_fs(get_ds());
	ret = __vfs_read(file, buf, count, &offset);
	set_fs(old_fs);

	return ret;
}

/*
 * integrity_read_file - read entire file content into the buffer
 *
 * This is function opens a file, allocates the buffer of required
 * size, read entire file content to the buffer and closes the file
 *
 * It is used only by init code.
 *
 */
int __init my_integrity_read_file(const char *path, char **data)
{
	struct file *file;
	loff_t size;
	char *buf;
	int rc = -EINVAL;

	if (!path || !*path)
		return -EINVAL;

	file = filp_open(path, O_RDONLY, 0);
	if (IS_ERR(file)) {
		rc = PTR_ERR(file);
		pr_err("Unable to open file: %s (%d)", path, rc);
		return rc;
	}

	size = i_size_read(file_inode(file));
	if (size <= 0)
		goto out;

	buf = kmalloc(size, GFP_KERNEL);
	if (!buf) {
		rc = -ENOMEM;
		goto out;
	}

	rc = my_integrity_kernel_read(file, 0, buf, size);
	if (rc == size) {
		*data = buf;
	} else {
		kfree(buf);
		if (rc >= 0)
			rc = -EIO;
	}
out:
	fput(file);
	return rc;
}


 static unsigned long long count = 0;

// static char *read_line(char *buf, int buf_len, struct file *fp)
// {
//         int ret;
//         int i = 0;
//         mm_segment_t fs;

//         fs=get_fs();
//         set_fs(KERNEL_DS);
//         ret = fp->f_op->read(fp, buf, buf_len, &(fp->f_pos));
//         set_fs(fs);

//         if (ret <= 0)
//                 return NULL;

//         while(buf[i++] != '\n' && i < ret);

//         if(i < ret) {
//                 fp->f_pos += i-ret;
//         }

//         if(i < buf_len) {
//                 buf[i] = 0;
//         }
//         return buf;
// }


int demo_task_create(unsigned long clone_flags)
{
    printk("[+geek] call task_create(). count=%llu\n", ++count);    

    return 0;
}

static int youho_file_open(struct file *file, const struct cred *cred)
{
	//struct file * filp=filp_open("/etc/youho/file_list",O_RDONLY,0); 
	//char buffer[200];
	//int index=1;
	//while(
	//read_line(buffer,200,filp);//&&buffer!=NULL)
       // printk("%d %s\n",index++,buffer);
	//printk("buffer:%s",buffer);
    //filp_close(filp,NULL); 
    char buffer[100];
    char * buf1;
	loff_t pos;
	mm_segment_t fs;

    char *path= d_path(&(file->f_path),buffer,100);
    printk("file:%s",path);
    // if(strcmp(path,"/etc/youho/file_list")==0)
    // {
    // 	printk("hahahaha");
    	
    //    // buf1 = kzalloc(500, GFP_KERNEL);

	   //  //fs =get_fs();
	   // // set_fs(KERNEL_DS);
	   //  //pos =0;
	   //  //vfs_read(file,buf1, sizeof(buf1), &pos);
	   // // printk("Write contet=%s\n",buffer);
	   // // set_fs(fs);

	   // // kfree(buf1);
    // }

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

    // char buf1[32];
    // struct file *fp;
    // mm_segment_t fs;
    // loff_t pos;
    // printk("test enter\n");
    // fp =filp_open("/etc/youho/file_list",O_RDWR | O_CREAT,0644);
    // if (IS_ERR(fp)){
    //     printk("create file error\n");
    //     return -1;
    // }
    // fs =get_fs();
    // set_fs(KERNEL_DS);
    // // pos =0;
    // // vfs_write(fp,buf, sizeof(buf), &pos);
    // pos =0;
    // vfs_read(fp,buf1, sizeof(buf1), &pos);
    // printk("Write contet=%s\n",buf1);
    // filp_close(fp,NULL);
    // set_fs(fs);
}

static __init int demo_init(void){
    demo_add_hooks();

  // int fd;
  // char buf[1];

  // mm_segment_t old_fs = get_fs();
  // set_fs(KERNEL_DS);

  // fd = sys_open("/etc/youho/file_list", O_RDONLY, 0);
  // if (fd >= 0) {
  //   printk(KERN_DEBUG);
  //   while (sys_read(fd, buf, 1) == 1)
  //     printk("%c", buf[0]);
  //   printk("\n");
  //   sys_close(fd);
  // }
  // set_fs(old_fs);

 //    printk("fssss");
	// char **bufff;
	// *bufff=(char *)kmalloc(1024,GFP_KERNEL);
	// int rc = my_integrity_read_file("file_list",  bufff);
	// printk("%d   %s",rc,*bufff);

	return 0;

}

security_initcall(demo_init);