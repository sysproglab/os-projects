[BITS 16]	;16-битный код
[ORG 0x7C00]	;указатель на адрес, где будет находиться код, после загрузки системы
MOV SI, HelloString ;сохранение указателя на строку в SI

CALL PrintString	
 JMP $ 		

PrintCharacter:	; процедура для вывода символа на экран
	;необходимое ASCII значение должно быть в регистре AL
MOV AH, 0x0E	;говорит BIOS что выводим один символ на экран
MOV BH, 0x00	;нет страниц
MOV BL, 0x07		;выбор цвета

INT 0x10	;вызов прерывания 0x10
RET		;выход из процедуры

PrintString:	; процедура для вывода строки на экран
	;указатель на строку в регистре SI

next_character:	;
MOV AL, [SI]	;получение и сохранения байта строки в регистре AL 
INC SI		;Инкремент указателя на строку
OR AL, AL	;проверка, что в регистре AL -  0  (конец строки)
JZ exit_function ;завершение процедуры
CALL PrintCharacter ;вывод очередного символа
JMP next_character	;вывести следующий символ

exit_function:	;
RET		;выход из процедуры

;Data
HelloString db 'Hello World!', 0	;строка HelloWorld!, оканчивается 0

TIMES 510 - ($ - $$) db 0	;заполняет оставшееся место на секторе 0
DW 0xAA55			;добавление специальной сигнатуры в конце загрузчика

