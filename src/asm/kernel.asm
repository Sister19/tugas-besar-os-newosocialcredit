;kernel.asm
;Michael Black, 2007
;Modified by Asisten Sister, 2021

;kernel.asm contains assembly functions that you can use in your kernel

global _getCursorPos
global _putInMemory
global _makeInterrupt21
global _exec
extern _handleInterrupt21

_exec:
	; calling conv
	mov bp,sp
	; get segment to bx
	mov bx,[bp+2]
	; get current segment
	mov ax,cs
	mov ds,ax
	; change jump segment
	mov si,launch
	mov [si+3],bx
	; setup segment registers
	mov ds,bx
	mov ss,bx
	mov es,bx
	; the stack start at segment:fff0
	mov ax,0xfff0
	mov sp,ax
	mov bp,ax
launch: jmp 0:0

;void putInMemory (int segment, int address, byte b)
_putInMemory:
	push bp
	mov bp,sp
	push ds
	mov ax,[bp+4]
	mov si,[bp+6]
	mov cl,[bp+8]
	mov ds,ax
	mov [si],cl
	pop ds
	pop bp
	ret

;int getCursorPos()
_getCursorPos:
	push bp
	mov bp,sp
	push dx
	push cx
	mov ax,0x0300
	mov bx,0
	; don't care for the rest
	int 0x10
	; return it to ax
	mov ax,dx
	pop cx
	pop dx
	pop bp
	ret

;void makeInterrupt21()
;this sets up the interrupt 0x21 vector
;when an interrupt 0x21 is called in the future,
;_interrupt21ServiceRoutine will run

_makeInterrupt21:
	;get the address of the service routine
	mov dx,_interrupt21ServiceRoutine
	push ds
	mov ax, 0	;interrupts are in lowest memory
	mov ds,ax
	mov si,0x84	;interrupt 0x21 vector (21 * 4 = 84)
	mov ax,cs	;have interrupt go to the current segment
	mov [si+2],ax
	mov [si],dx	;set up our vector
	pop ds
	ret

;this is called when interrupt 21 happens
;it will call your function:
;void handleInterrupt21 (int AX, int BX, int CX, int DX)
_interrupt21ServiceRoutine:
	push dx
	push cx
	push bx
	push ax
	call _handleInterrupt21
	pop ax
	pop bx
	pop cx
	pop dx

	iret
