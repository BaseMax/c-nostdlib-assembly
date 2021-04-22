// Linux x86_64
#ifdef __x86_64__
asm(
		".global _start\n"
		"_start:\n"
		"   xorl %ebp,%ebp\n"       // mark outermost stack frame
		"   movq 0(%rsp),%rdi\n"    // get argc
		"   lea 8(%rsp),%rsi\n"     // the arguments are pushed just below, so argv = %rbp + 8
		"   call onelang_main\n"       // call our onelang_main
		"   movq %rax,%rdi\n"       // take the main return code and use it as first argument for...
		"   movl $60,%eax\n"        // ... the exit syscall
		"   syscall\n"
		"   int3\n");               // just in case

asm(
		"onelang_write:\n"             // write syscall wrapper; the calling convention is pretty much ok as is
		"   movq $1,%rax\n"         // 1 = write syscall on x86_64
		"   syscall\n"
		"   ret\n");
#endif

// Linux i386
#ifdef __i386__
asm(
		".global _start\n"
		"_start:\n"
		"   xorl %ebp,%ebp\n"       // mark outermost stack frame
		"   movl 0(%esp),%edi\n"    // argc is on the top of the stack
		"   lea 4(%esp),%esi\n"     // as above, but with 4-byte pointers
		"   sub $8,%esp\n"          // the start starts 16-byte aligned, we have to push 2*4 bytes; "waste" 8 bytes
		"   pushl %esi\n"           // to keep it aligned after pushing our arguments
		"   pushl %edi\n"
		"   call onelang_main\n"       // call our onelang_main
		"   add $8,%esp\n"          // fix the stack after call (actually useless here)
		"   movl %eax,%ebx\n"       // take the main return code and use it as first argument for...
		"   movl $1,%eax\n"         // ... the exit syscall
		"   int $0x80\n"
		"   int3\n");               // just in case

asm(
		"onelang_write:\n"             // write syscall wrapper; convert the user-mode calling convention to the syscall convention
		"   pushl %ebx\n"           // ebx is callee-preserved
		"   movl 8(%esp),%ebx\n"    // just move stuff from the stack to the correct registers
		"   movl 12(%esp),%ecx\n"
		"   movl 16(%esp),%edx\n"
		"   mov $4,%eax\n"          // 4 = write syscall on i386
		"   int $0x80\n"
		"   popl %ebx\n"            // restore ebx
		"   ret\n");                // notice: the return value is already ok in %eax
#endif

int onelang_write(int fd, const void *buf, unsigned count);

unsigned my_strlen(const char *ch) {
  const char *ptr;
  for(ptr = ch; *ptr; ++ptr);
  return ptr-ch;
}

int onelang_main(int argc, char *argv[]) {

  for(int i = 0; i < argc; ++i) {
		int len = my_strlen(argv[i]);
		onelang_write(1, argv[i], len);
		onelang_write(1, "\n", 1);
	}

  return argc;
}
