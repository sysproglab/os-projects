[BITS 16]
[ORG 0x7C00]
BS_jmpBoot:
	jmp start0
	nop
	%include "fatTable.asm"
start0:
cli ; запрещение аппаратных прерываний
MOV DH, 0x0 ;head (0=base)
MOV CH, 0x0 ;track/cylinder
MOV CL, 0x02 ;номер сектора (нумерация начинается с 1)
MOV BX, 0x1000 ;адрес RAM для загрузки ядра
MOV ES, BX ;старший адрес RAM 0x1000
MOV BX, 0x0 ;младший адрес RAM 0x0
ReadSector:
	MOV AH, 0x02 ;чтение сектора диска
	MOV AL, 0x02 ;количество секторов для чтения
	INT 0x13

	; Вывод сообщения
	mov	si, message ; сохранение указателя на строку в si
	call	print

	;указатель на адрес RAM (0x1000)
	MOV AX, 0x1000
	MOV DS, AX
	MOV ES, AX
	MOV FS, AX
	MOV GS, AX
	MOV SS, AX
	JMP 0x1000:0x0

message db 'Read of second loader finished!',10,13,0
; Подпрограмма вывода
print:
	cld
	pusha
.PrintChar:
	lodsb ; Считать байт по адресу SI в AL, SI увеличиться на 1
	test	al, al ; проверка, что в регистре AL -  0  (конец строки)
	jz	short .Exit  ; завершение процедуры

	mov	ah, 0x0E ; сообщаем BIOS, что выводим один символ на экран
    mov bh, 0x00 ;нет страниц
	mov	bl, 0x07 ; выбор цвета
	mov	bx, 0x00 ;
	int	0x10 ; вызов прерывания 0x10
	jmp	short .PrintChar ; переходим к печати следующего символа
.Exit:
	popa
	ret ; выход из процедуры

TIMES 510 - ($ - $$) db 0 ;заполняет оставшееся место на секторе 0
DW 0xAA55 ;добавление специальной сигнатуры в конце загрузчика
