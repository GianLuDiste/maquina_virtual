basura1 equ "basura re loca jaja"
salto equ "\n"
basura2 equ "basura"

; ESTO NO SE EJECUTA
        stop
        stop
        stop

; ESTO SI SE EJECUTA
main:	Sys 0xF
	MOV EBX, DS   
	MOV [EBX], 10
	STOP