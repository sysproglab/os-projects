org		0x7C00
bits	16

jmp	short	start ; ������� � ������������ ����
nop

%include "fatTable.asm"	; ����������� ������� � FAT-��������

start:
	cld	; ���������� ����������
	
	; ������ ������������ ��������� � ������� ����
	mov si, SelectMessage	;���������� � ���� ��������� �� ������
  call PrintString	;����� ��������� ������ ������
	
	; ���������� �������
	mov ah, 10h
	int 16h
	mov ah, 0
  mov bx, si ; ��������� ��������� �� ���� � bx
	pusha
	
	; ���������� ��������� ������
	cmp al,'1'
	je select_first
	cmp al,'2'
	je select_second

; � ��������� ������ ������� ��������� �� ������
  mov si, SelectErrorMessage	;���������� � ���� ��������� �� ������ � ���������� �� ������
  call PrintString ; ������� ������
  int 18h ; ����������, ��������������� � ��������� ��������

; ���� ������ ������ ����������� ����
select_first:
  mov si, ProgramName1 ; ��������� ��������� ����� �� ������ � ������ ������� �����
  jmp write_name ; ��������� � ����������� �����

; ���� ������ ������ ����������� ����
select_second:
	mov si, ProgramName2 ; ��������� ��������� ����� �� ������ � ������ ������� �����
	
write_name:
  mov di, ProgramName ; ���������� � di �����, ���� ����� �������� ��� ����������� ���������
	mov cx, 11 ; ���������� � cx ������ �����
write_char:
  mov ax, [si] ; ����� ������ ������ �� ����� ���������
	mov [di], ax ; ���������� �� � ������� ������ ��� ����������� ���������
	inc si ; ����������� si 
	inc di ; � di ��� �������� � ���������� �������
	dec cx ; ��������� �������
	jnz write_char ; ���� �� ����, �� ��������� � ����������� ���������� �������
	mov si, bx ; ��������������� ��������� �� ����
	popa
	
  push cs
  pop ds
  mov [var_logical_drive_number], dl ; ���������� ������ ������������ ����� BIOS
  and byte [var_cluster_number+3], 0Fh ; ���������� �������� �������� 
  mov esi, [var_cluster_number] ; ���������� � esi ����� �������� �������� ����������	

; ������ �������� ����������
RootDirReadContinue:
  push 60h ; 60h - �������� ������������ ������ BIOS
  pop es ; ������� �������� �� ����� � �������� ��� � es
  xor bx, bx ; ��������� bx
  call ReadCluster ; ����� ������� ������ ������ �������� �� �������� ����������
  push esi ; ���������� ������ ���������� �������� � esi
  pushf ; ���������� ���� � ����

; ����� � ������ ������������ �����
  push 60h
  pop  es ; ������� �������� �� ����� � �������� ��� � es
  xor  di, di ; �������� di
  mov  si, ProgramName ; ���������� � ���� ��� ��������� ��� �������
       
  mov al, [var_sector_by_claster] ; ���������� � al ���������� �������� � ��������
  cbw ; ����������� ���������� al � �������� ����� (�����) � ax 
  mul word [var_byte_per_sector]; �������� ���������� �������� � �������� �� ���������� ���� � ������� � �������� ���������� ���� � ��������
  shr ax, 5 ; ����� ������ �������� ax �� 5 ��� � �������� ���������� ������� ��������
  mov dx, ax ; ���������� ���������� � ������� dx

; ����� ����� �����
; � ���� �������� ��� �����
; dx - ���������� ����� �����
; � ���������� � esi ����� ������� ����� ��������
FindName:
  mov cx, 11 ; ���������� � cx ����� ����� (11 ����)

; ���� ������ �����
FindNameCycle:
  cmp byte [es:di], ch ; ���������� ����� ������� ������ � ���, ��� ����� �����
  jne FindNameNotEnd ; ���� �� �����, �� ���������� ������
  jmp ErrFind ; ���� ����� �������� ���������� (������� NULL ������) �� ������� ��������� �� ������

; ����������� ������ �����
FindNameNotEnd:
  pusha ; ��������� � ����� ���������� ��������� AX, CX, DX, BX, SP, BP, SI, DI
  repe cmpsb ; ���������� ������
  popa ; ��������������� ���������� ��������� AX, CX, DX, BX, SP, BP, SI, DI
  je FindNameFound ; ���� �����, �� ���� ������ � ��������� �� ����� FindNameFound
  add di, 32 ; ���������� � di 32 (������� � ��������� ������)
  dec dx ; ��������� �������
  jnz FindNameCycle	; ��������� �������� ��� ����� ������
  popf	; ��������������� �����
  pop esi	; � esi ������������ ����� ���������� �������� �������� ����������
  jc RootDirReadContinue ; ���� �������� ��������, �� �������� ������ ���������� ��������
  jmp ErrFind ; ���� �� ������, ������� ��������� �� ������

; ��� ����� �������
FindNameFound:
  push word [es:di+14h]
  push word [es:di+1Ah]
  pop esi	; ���������� ����� �������� � si

; �������� �����
  push 60h
  pop es
  xor bx, bx

; ���� ������ �����
FileReadContinue:
  call ReadCluster ; ������ ������ �������� � �������� ����������
  jc FileReadContinue ; ���� �� ������� ���, �� ���������� ������

; ������ ����������� ���������
  push 60h
  pop ds ; ���������� � ds �������� 60h
  mov ax, ds ; � ax � ds ����� ��������, ���� ����� �������� ����������� ����
       
  sub ax, 10h 
  mov es, ax
  mov ds, ax
  mov ss, ax
  xor sp, sp ; ������� �����
  push es
  push word 100h ; ���������� � ���� �����, �� �������� ����� �������� ����
  mov  dl, [cs:var_logical_drive_number] 
  retf

; ������� ������ ��������
ReadCluster:
  mov ax, [var_byte_per_sector]
  shr ax, 2	; �������� � ax ���������� ������� � �������
  cwde
  mov ebp, esi	; ���������� � ebp ����� ��������
  xchg  eax, esi ; ������ �������� � eax � esi
  cdq
  div esi	; � eax ������� ����� �������
  movzx edi, word [var_reserved_sector_counter] ; ���������� � edi ���������� ����������������� ��������
  add edi, [var_number_of_hidden_sectors] ; ���������� � ����� ���������� ���������� ��������
  add eax, edi	; ���������� ����� �������

  push dx	; ���������� � ���� dx (����� �������)
  mov cx, 1
  call ReadSector	; ������� ������ ������ �������

  pop si	; ���������� �������� ������ �� si. ������ �� ������� ����� ����� ������� ������ ��������
  add si, si ; ����������� si � 4 ����
  add si, si ; ��� �� �������� ����� ���������� ��������
  and byte [es:si+3], 0Fh	; ����� �������� ��������
  mov esi, [es:si]	; � esi ���������� ����� ���������� ��������

  lea eax, [ebp-2]
  movzx ecx, byte [var_sector_by_claster] ; ���������� � ecx ���������� �������� � ��������
  mul ecx ; �������� � �������
  mov ebp, eax ; ���������� � ebp �������

  movzx eax, byte [var_num_fat_copies] ; ���������� � ax ���������� �����
  mul dword [var_number_of_sectors_per_fat] ; �������� �� ���������� �������� � FAT-�������
      
  add eax, ebp ; ���������� ������� �� ���������� � �������
  add eax, edi ; �������� ����� ������� �������
      
  call ReadSector
        
  mov ax, [var_byte_per_sector] ; ���������� � ax ���������� ���� � �������
  shr ax, 4 ; ���������� ax ���������� ���� � ������� (����� ����� �� 4)
  mul cx ; �������� �� ���������� �������� (������� � cx ����� ������� ReadSector)
      
  mov cx, es ; ���������� � cx ����� �������� ��������
  add cx, ax ; �������� ����� ���������� ��������, �������� � ������ ������� �������� ������ �������� ��������
  mov es, cx ; ������ � es:bx ����� ���������� ��������
      
  cmp esi, 0FFFFFF8h ; ���� ��������� �������, �� ������� ����� ����� 0, ����� 1
  ret 

; ������ �������
ReadSector:
  pushad ; ��������� �������� ������ ���������� � ����

; ������ ���������� �������
ReadSectorNext:
  pusha ; ��������� �������� � �����
	
; ������ ������ � ����
  push byte 0 ; ���������� � ���� 0
  push byte 0 ; ���������� � ���� 0
  push eax ; ���������� � ���� 1 ��� ������������� LBA
  push es ; ���������� � ���� es
  push bx ; ���������� � ���� ��������
  push byte 1 ; ����� �������� 1 ������
  push byte 16 ; ������ ������ 16 ����, ����������������x 0 ����
       
  mov ah, 42h
  mov dl, [var_logical_drive_number] ; ���������� � dl ����� ����������
  mov si, sp ; ���������� � si ��������� �����
  push ss ; ���������� � ���� ss
  pop ds ; ���������� �������� ss � ������� ds
  int 13h ; ���������� ��������� ����� - ������
  push cs ; ������ �������� � �������� cs
  pop ds ; � ������� ds
       
  jc short ErrRead
  add  sp, 16 ; ��� ������� �������� ������� ����� �� ������������ ���� ��������
       
  popa ; ��������������� ��������
  dec cx ; ��������� �������
  jz ReadSectorDone ; ��������� ������
       
  add  bx, [var_byte_per_sector] ; ������������� �������� ��� ���������� �������
  add  eax, byte 1 ; ������������� LBA ��� ���������� �������
  jmp  short ReadSectorNext ; ������ ��������� ������

ReadSectorDone:
  popad ; ��������� �������� � �����
  ret

PrintString:	;��������� ������ ������
	mov ax, si ; ��������� ����� �����
next_character:
	mov al, [si]	;����� ���� ���� �� ������ � ���������� ��� � ������� AL
	inc si	;����������� ��������� SI
	or al, al	;�������� ����� ������
	jz exit_function
	call PrintCharacter ;�������� ������
	jmp next_character
exit_function:
	mov si, ax ; ��������������� ����
  ret

PrintCharacter:	;��������� ������ �������
	mov ah, 0x0E	;���� ����, ��� ��� ����� ������� �� ����� ���� ������
	mov bh, 0x00	;����� ��������
	mov bl, 0x07	;���� ����, ��� ��������� ������� ����� �� ������ ����

	int 0x10	;����� ���������� �����
	ret	;���������� � ��������� ���������
	
; ��������� �� ������

ErrRead:
ErrFind:
  mov si, ErrorMessage	;���������� � ���� ��������� �� ������
  call PrintString
exit_err:
 jmp short $

SelectMessage db "1.BOOT1.BIN", 13,10, "2.BOOT2.BIN", 13, 10, 0
ProgramName1	db	"BOOT1   BIN"   ; ��� ����������� ������ ���������
ProgramName2	db	"BOOT2   BIN"   ; ��� ����������� ������ ���������
ProgramName	db	"NNNNN   BIN"   ; ��� ����������� ���������
ErrorMessage db "ERROR", 13, 10, 0
SelectErrorMessage db "SELECT ERROR", 13, 10, 0

times (1024-2-($-$$)) db 0 ; ��������� ���������� ������������ ������
		
dw	0AA55h ; ��������� ����������
