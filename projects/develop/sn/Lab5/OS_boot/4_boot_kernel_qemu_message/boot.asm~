[BITS 16]
[ORG 0x7C00]
MOV DH, 0x0 ;head (0=base)
MOV CH, 0x0 ;track/cylinder
MOV CL, 0x02 ;номер сектора (нумерация начинается с 1)
MOV BX, 0x1000 ;адрес RAM для загрузки ядра
MOV ES, BX ;старший адрес RAM 0x1000
MOV BX, 0x0 ;младший адрес RAM 0x0
ReadFloppy:
MOV AH, 0x02 ;чтение сектора диска
MOV AL, 0x01 ;количество секторов для чтения
INT 0x13
;указатель на адрес RAM (0x1000)
MOV AX, 0x1000
MOV DS, AX
MOV ES, AX
MOV FS, AX
MOV GS, AX
MOV SS, AX
JMP 0x1000:0x0
TIMES 510 - ($ - $$) db 0 ;заполняет оставшееся место на секторе 0
DW 0xAA55 ;добавление специальной сигнатуры в конце загрузчика
