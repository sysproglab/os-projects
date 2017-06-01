        section .text           ; Code section.

        global main		; the standard gcc entry point
main:				; the program label for the entry point
		mov eax, 57 ; SYS_FORK Op Code
    	syscall
		cmp eax, 0 ;If the return value is 0, we are in the child process
		jz child

parent:
		mov edx, len ;Move msg length to edx
		mov ecx, msg ;Move msg to ecx
		call print ;Print
		call exit ;Exit
		jmp end

child:
		mov edx, clen ;Move msg length to edx
		mov ecx, cmsg ;Move msg to ecx
		call print ;Print
		call exit ;Exit
		jmp end

print:
		mov ebx,1 ;first argument: file handle (stdout)
		mov eax,4 ;system call number (SYS_WRITE)
		int 0x80
		ret

exit:
		mov ebx,0 ; Exit code
		mov eax,60 ; SYS_EXIT
		syscall

end:	
		mov edx, lnotexit
		mov ecx, notexit
		mov ebx,1 ;first argument: file handle (stdout)
		mov eax,4 ;system call number (SYS_WRITE)
		int 0x80

section .data
timeval:
    	tv_sec  dd 0
    	tv_usec dd 0
		msg db "I AM YOUR FATHER",0xa
		len equ $ - msg
		cmsg db "I AM YOUR CHILD",0xa
		clen equ $ - cmsg
		notexit db "EXIT NOT EXITED",0xa
		lnotexit equ $ - notexit