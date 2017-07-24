.model tiny
.stack	10000
.data
a 	dd	?
b 	dd	?
a 	dd	?
tempString1	db	100 dup($)
tempString2	db	100 dup($)
tempString3	db	100 dup($)
tempString4	db	100 dup($)
sourceStringAddress	dw	?
destinationStringAddress	dw	?

.code
.startup

MOV	EAX,100
PUSH	EAX
POP	EAX
MOV	a,EAX

MOV	EAX,20
PUSH	EAX
POP	EAX
MOV	b,EAX

LEA	ESI,	tempString1
MOV	[ESI +  0 ],	' a '
MOV	[ESI +  1 ],	'$'
EDI,	str
destinationStringAddress,	EDI
ESI,	tempString1
MOV	sourceStringAddress,	ESI
CALL	CopyString

.exit
CopyString	PROC	NEAR

LEA	ESI,	sourceStringAddress
LEA	EDI,	destinationStringAddress
CopyStringLoop_1:
CMP	BYTE PTR[ESI],	'$'
JE	CopyStringEnd_1
MOV	EAX,	BYTE PTR[ESI]
MOV	BYTE PTR[EDI],	EAX
INC	ESI
INC	EDI
JMP	CopyStringLoop_1
CopyStringEnd_1:
MOV	BYTE PTR[EDI],	'$'
RET
CopyString	ENDP

end
