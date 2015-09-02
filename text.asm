bits 32

;-----------------------------------------------------------------------------------------------

global _g_pText
global _g_lTextPitch

global _g_dwTextPrnX
global _g_dwTextPrnY
global _g_dwTextPrnC
global _g_dwTextPrnM
global _g_dwTextCutLf
global _g_dwTextCutRt

global _PrintL
global _CalcLen

;-----------------------------------------------------------------------------------------------

section .data

;-----------------------------------------------------------------------------------------------

_g_pText		dd 0
_g_lTextPitch	dd 0

_g_dwTextPrnX	dd 0
_g_dwTextPrnY	dd 0
_g_dwTextPrnC	dd 0x00ffffff
_g_dwTextPrnM	dd 0	  
_g_dwTextCutLf	dd 0
_g_dwTextCutRt	dd 799

mask			times 16 dw 0
font			incbin "res\skoobwin.fnt"

;-----------------------------------------------------------------------------------------------

section .code

;-----------------------------------------------------------------------------------------------

; void PrintL( char )
_PrintL:
	push		ebp
	mov			ebp, esp
	push		esi
	push		edi
	push		ebx
	push		ecx
	push		edx

	mov			edi, mask
	mov			ecx, 8
	xor			eax, eax
	rep			stosd

	mov			al, [ebp + 8]
	shl			eax, 5
	add			eax, font
	push		eax
	mov			esi, eax
	add			esi, 2
	mov			edi, mask
	mov			ecx, 14
.genmask:
	lodsw
	xchg		al, ah
	or			[edi + 0], ax
	or			[edi + 2], ax
	or			[edi + 4], ax
	shl			ax, 1
	or			[edi + 0], ax
	or			[edi + 2], ax
	or			[edi + 4], ax
	shr			ax, 2
	or			[edi + 0], ax
	or			[edi + 2], ax
	or			[edi + 4], ax
	add			edi, 2
	loop		.genmask

	mov			esi, mask
	mov			eax, [_g_lTextPitch]
	mul			dword [_g_dwTextPrnY]
	add			eax, [_g_pText]
	mov			ebx, [_g_dwTextPrnX]
	shl			ebx, 2
	add			eax, ebx
	push		eax

	mov			edi, eax
	mov			edx, [_g_dwTextPrnM]
	mov			ecx, 16
.putmask:
	lodsw
	mov			ebx, [_g_dwTextPrnX]
	push		ecx
	mov			ecx, 16
.pm1:   
	rcl			ax, 1
	jnc			.pm2
	cmp			ebx, [_g_dwTextCutLf]
	jl			.pm2
	cmp			ebx, [_g_dwTextCutRt]
	jg			.pm2
	mov			[edi], edx
.pm2:
	inc			ebx
	add			edi, 4
	loop		.pm1
	add			edi, [_g_lTextPitch]
	sub			edi, 16*4
	pop			ecx
	loop		.putmask
	pop			edi
	pop	esi
	mov	edx, [_g_dwTextPrnC]
	mov	ecx, 16
.putletter:     
	lodsw
	xchg	al, ah
	mov		ebx, [_g_dwTextPrnX]
	push	ecx
	mov		ecx, 16
.pl1:   
	rcl		ax, 1
	jnc		.pl2
	cmp		ebx, [_g_dwTextCutLf]
	jl		.pl2
	cmp		ebx, [_g_dwTextCutRt]
	jg		.pl2
	mov		[edi], edx
.pl2:
	inc		ebx
	add		edi, 4
	loop	.pl1
	add		edi, [_g_lTextPitch]
	sub		edi, 16*4
	pop		ecx
	loop	.putletter

	xor		eax, eax
	mov		al, [ebp + 8]
	mov		al, [font + 0x2000 + eax]
	add		[_g_dwTextPrnX], eax

	pop		edx
	pop		ecx
	pop		ebx
	pop		edi
	pop		esi
	pop		ebp
	ret

;-----------------------------------------------------------------------------------------------

; DWORD CalcLen( char * )
_CalcLen:
	push	ebp
	mov		ebp, esp
	push	esi
	push	ebx
	push	ecx

	mov		esi, [ebp + 8]
	xor		ebx, ebx
	xor		eax, eax
	xor		ecx, ecx
.cl1:
	lodsb
	and		al, al
	jz		.cl2
	mov		cl, [font + 0x2000 + eax]
	add		ebx, ecx
	jmp		.cl1
.cl2:
	mov		eax, ebx

	pop		ecx
	pop		ebx
	pop		esi
	pop		ebp
	ret

;-----------------------------------------------------------------------------------------------
