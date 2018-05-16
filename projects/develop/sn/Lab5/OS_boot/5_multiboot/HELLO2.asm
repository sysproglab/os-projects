bits	16
[ORG 0x0100]
  cli
; Вывод приветственного сообщения
	mov	si, mHello ; сохранение указателя на строку в si
	call	print
; Зависаем в бесконечном цикле
die:	jmp	short die

mHello db 'Hello world 2!',10,13,0

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
	int	0x10 ; вызов прерывания 0x10
	jmp	short .PrintChar ; переходим к печати следующего символа
.Exit:
	popa
	ret ; выход из процедуры

TIMES 512 - ($ - $$) db 0	;заполняет оставшееся место на секторе 0
