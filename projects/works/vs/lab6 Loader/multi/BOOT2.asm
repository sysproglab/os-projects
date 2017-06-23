bits 16
org 0x100

mov si, HelloString	; ���������� � ���� ��������� �� ������
	call PrintString	; ����� ��������� ������ ������
	jmp $				; ����������� ����

PrintCharacter:		; ��������� ������ �������
	mov ah, 0x0E		; ���� ����, ��� ��� ����� ������� �� ����� ���� ������
	mov bh, 0x00		; ����� ��������
	mov bl, 0x07		; ���� ����, ��� ��������� ������� ����� �� ������ ����

	int 0x10			; ����� ���������� �����
	ret					; ���������� � ��������� ���������

PrintString:	; ��������� ������ ������

next_character:
	mov al, [si]	; ����� ���� ���� �� ������ � ���������� ��� � ������� AL
	inc si		; ����������� ��������� SI
	or al, al	; �������� ����� ������
	jz exit_function
	call PrintCharacter ; �������� ������
	jmp next_character
	exit_function:
	ret

;Data
HelloString db 'OS2 starting...', 0