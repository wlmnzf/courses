#undef _FORTIFY_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <unistd.h>
#include <dlfcn.h>

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
	 read(STDIN_FILENO, buf, 512);
}

int main(int argc, char** argv) {
      write(STDOUT_FILENO, "Hello, World\n", 13);
      vulnerable_function();
}

