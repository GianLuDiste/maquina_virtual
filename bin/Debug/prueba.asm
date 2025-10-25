basura1 equ "basura re loca jaja"
salto equ "\n"
basura2 equ "basura"

; ESTO NO SE EJECUTA
        stop
        stop
        stop

; ESTO SI SE EJECUTA
main:	Sys 0xF
	Sys 0xF
	MOV EAX, 10
	MOV EBX, DS
	MOV AX, 5
	MOV CL, 3
	MOV EDX, EAX
	STOP
