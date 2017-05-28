section .text
global  _start
_start:
  mov eax, 2 ; SYS_FORK Op Code
  int 0x80
  cmp eax, 0 ;If the return value is 0, we are in the child process
  jz child

parent:
  mov edx, len ;Move msg length to edx
  mov ecx, msg ;Move msg to ecx
  call print ;Print
  call exit ;Exit

child:
  mov eax,11
  mov ebx, echo
  mov ecx, 0
  xor edx, edx
  int 0x80

print:
  mov     ebx,1               ;first argument: file handle (stdout)
  mov     eax,4               ;system call number (SYS_WRITE)
  int     0x80
  ret

exit:
  mov     ebx,0 ; Exit code
  mov     eax,1 ; SYS_EXIT
  int     0x80

section .data
  echo db '/bin/echo',0
  args db 'echo',0,'lol',0
  msg db      "I AM YOUR FATHER",0xa ;String to be printed by father process
  len equ     $ - msg             ; And it's length