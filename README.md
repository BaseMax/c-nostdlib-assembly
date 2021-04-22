# c-nostdlib-assembly

We define own `_start` point without using glibc runtime library using **ASM**.

```bash
$ gcc asm.c -o asm -nostdlib
```

This program dates back to probably 2019.
I remember getting help from several people to prepare this. But unfortunately I do not remember their names.

## Extra Draft

```assembly
.data
msg: .ascii "Hello World\n"

.text
.global _start


_start:
    movl $4, %eax   ; use the write syscall
    movl $1, %ebx   ; write to stdout
    movl $msg, %ecx ; use string "Hello World"
    movl $12, %edx  ; write 12 characters
    int $0x80       ; make syscall
    
    movl $1, %eax   ; use the _exit syscall
    movl $0, %ebx   ; error code 0
    int $0x80       ; make syscall
```

As defined in $build/usr/include/asm/unistd_32.h, the syscall numbers for write and _exit are:
```c
#define __NR_exit 1
#define __NR_write 4
```

In $build/usr/include/asm/unistd_64.h, the syscall numbers are defined as following:
```c
#define __NR_write 1
#define __NR_exit 60
```

Thanks
