org 0x7C00
bits 16

BS_jmpBoot:
	jmp start
	nop

%include "fatTable.asm"

start:

	mov si, HelloString2	;���������� � ���� ��������� �� ������
	call PrintString	;����� ��������� ������ ������
	jmp $				;����������� ����

PrintCharacter:		;��������� ������ �������
	mov ah, 0x0E		;���� ����, ��� ��� ����� ������� �� ����� ���� ������
	mov bh, 0x00		;����� ��������
	mov bl, 0x07		;���� ����, ��� ��������� ������� ����� �� ������ ����
	int 0x10			;����� ���������� �����
	ret					;���������� � ��������� ���������

PrintString:	;��������� ������ ������

next_character:
	mov al, [si]	;����� ���� ���� �� ������ � ���������� ��� � ������� AL
	inc si		;����������� ��������� SI
	or al, al	;�������� ����� ������
	jz exit_function
	call PrintCharacter ;�������� ������
	jmp next_character
exit_function:
	ret

HelloString: db "second bootloader hello", 13, 10, 0
end:
	times  510 - (end - BS_jmpBoot)	db 0	; �������� ������ �� 512 ����
	db 0xAA,0x55

start_second:	
HelloString2: db "second bootloader hello", 13, 10, 0
end_second:
	times  510 - (end_second - start_second)	db 0	; �������� ������ �� 512 ����
	db 0xAA,0x55
