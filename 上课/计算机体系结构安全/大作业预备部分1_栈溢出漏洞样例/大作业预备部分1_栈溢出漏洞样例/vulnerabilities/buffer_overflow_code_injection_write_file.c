

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
FILE* g_fp; 

void read_file(){
    char buf[100];
    int v,length=0;           
    g_fp=fopen("../input/buffer_overflow_code_injection_write_file.txt","r"); 
    if(g_fp==NULL)
    {
        printf("open file failed!\n");
        exit(1);
    }
    while (fscanf(g_fp, "\\x%02x", &v) == 1)
    {
        buf[length++] = v;
    } 
    fclose(g_fp);
}

int main(int argc,char *argv[]){
    read_file(); 
    return 0;
}
