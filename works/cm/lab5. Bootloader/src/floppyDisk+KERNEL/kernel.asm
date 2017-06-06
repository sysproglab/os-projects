MOV SI, HelloString ;Store string pointer to SI
CALL PrintString	;Call print string procedure
JMP $ 		;Infinite loop, hang it here.


PrintCharacter:	;Procedure to print character on screen
	;Assume that ASCII value is in register AL
MOV AH, 0x0E	;Tell BIOS that we need to print one charater on screen.
MOV BH, 0x00	;Page no.
MOV BL, 0x07	;Text attribute 0x07 is lightgrey font on black background

INT 0x10	;Call video interrupt
RET		;Return to calling procedure



PrintString:	;Procedure to print string on screen
	;Assume that string starting pointer is in register SI

next_character:	;Lable to fetch next character from string
MOV AL, [SI]	;Get a byte from string and store in AL register
INC SI		;Increment SI pointer
OR AL, AL	;Check if value in AL is zero (end of string)
JZ exit_function ;If end then return
CALL PrintCharacter ;Else print the character which is in AL register
JMP next_character	;Fetch next character from string

exit_function:	;End label
RET		;Return from procedure


;Data
HelloString db 'Hello World!', 0	;HelloWorld string ending with 0
TIMES 512 - ($ - $$) db 0 ;fill the rest