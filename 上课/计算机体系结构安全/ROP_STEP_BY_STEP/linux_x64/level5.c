#undef _FORTIFY_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <unistd.h>
#include <dlfcn.h>

void systemaddr()
{
    void* handle = dlopen("libc.so.6", RTLD_LAZY);
    printf("%p\n",dlsym(handle,"system"));
    fflush(stdout);
}

void dummy()
{
	__asm__ __volatile__(
			"add %rdi,%rsi\n\t"
			"jmp *(%rsi)\n\t"

		);
}

void vulnerable_function() {
	char buf[128];
	printf("%p\n",&buf[0]);
	// gets(buf);
	// write(STDIN_FILENO,buf,64);
	 read(STDIN_FILENO, buf, 512);
}

int main(int argc, char** argv) {
	// write(STDOUT_FILENO, "Hello, World\n", 13);
	systemaddr();
	vulnerable_function();
}

