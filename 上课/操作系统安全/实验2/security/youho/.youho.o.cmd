cmd_security/youho/youho.o := ld -m elf_x86_64  -z max-page-size=0x200000   -r -o security/youho/youho.o security/youho/youho_lsm.o 
