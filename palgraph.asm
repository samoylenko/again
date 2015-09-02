bits 32

global _PutSprite
global _Blur
global _IndexedToRGB

;-----------------------------------------------------------------------------------------------

section .data

blur_minus	dd 0x00200020
			dd 0x00200020



;-----------------------------------------------------------------------------------------------

section .code

;-----------------------------------------------------------------------------------------------
;-----------------------------------------------------------------------------------------------
;-----------------------------------------------------------------------------------------------
;-----------------------------------------------------------------------------------------------
;-----------------------------------------------------------------------------------------------
;				0x08		0x0c		0x10		0x14	 0x18			0x1c	 0x20
; PutSprite( void *spr, ULONG sprx, ULONG spry, void *dst, ULONG dstpitch, ULONG x, ULONG y );
_PutSprite:
	push		ebp
	mov			ebp, esp
	push		eax
	push		edi
	push		esi
	push		ecx

	mov			eax, [ebp+0x20]
	mul			dword [ebp+0x18]
	add			eax, [ebp+0x1c]
	add			eax, [ebp+0x14]
	mov			edi, eax
	mov			esi, [ebp+0x08]
	mov			ecx, [ebp+0x10]
.ps1:
	push		ecx

	mov			ecx, [ebp+0x0c]
	shr			ecx, 3

.ps2:
	movq		mm0, [esi]
	paddusb		mm0, [edi]
	movq		[edi], mm0
	add			edi, 8
	add			esi, 8
	loop		.ps2

	add			edi, [ebp+0x18]
	sub			edi, [ebp+0x0c]

	pop			ecx
	loop		.ps1

	emms

	pop			ecx
	pop			esi
	pop			edi
	pop			eax
	pop			ebp
	ret

;-----------------------------------------------------------------------------------------------
; void Blur( void *pSrc, void *pDst, ULONG nBytes )
_Blur:

	push		ebp
	mov			ebp, esp
	push		esi
	push		edi
	push		ecx

	mov			esi, [ebp+0x08]
	mov			edi, [ebp+0x0c]
	mov			eax, 800*598/4

	add			esi, 801
	add			edi, 801

	pxor		mm3, mm3

.blur1:
	movq		mm0, mm3

;	movd		mm1, [esi-801]
;	movq		mm2, mm3
;	punpcklbw	mm2, mm1
;	psrlw		mm2, 8
;	paddw		mm0, mm2
	movd		mm1, [esi-800]
	movq		mm2, mm3
	punpcklbw	mm2, mm1
	psrlw		mm2, 8
	paddw		mm0, mm2
;	movd		mm1, [esi-799]
;	movq		mm2, mm3
;	punpcklbw	mm2, mm1
;	psrlw		mm2, 8
;	paddw		mm0, mm2
	movd		mm1, [esi-1]
	movq		mm2, mm3
	punpcklbw	mm2, mm1
	psrlw		mm2, 8
	paddw		mm0, mm2
	movd		mm1, [esi+1]
	movq		mm2, mm3
	punpcklbw	mm2, mm1
	psrlw		mm2, 8
	paddw		mm0, mm2
;	movd		mm1, [esi]
;	movq		mm2, mm3
;	punpcklbw	mm2, mm1
;	psrlw		mm2, 8
;	paddw		mm0, mm2
;	movd		mm1, [esi+799]
;	movq		mm2, mm3
;	punpcklbw	mm2, mm1
;	psrlw		mm2, 8
;	paddw		mm0, mm2
	movd		mm1, [esi+800]
	movq		mm2, mm3
	punpcklbw	mm2, mm1
	psrlw		mm2, 8
	paddw		mm0, mm2
;	movd		mm1, [esi+801]
;	movq		mm2, mm3
;	punpcklbw	mm2, mm1
;	psrlw		mm2, 8
;	paddw		mm0, mm2
	psubusw		mm0, [blur_minus]

	psrlw		mm0, 2
	packuswb	mm1, mm0
	psrlq		mm1, 32
	movd		[edi], mm1

	add			edi, 4
	add			esi, 4
	dec			eax
	jnz			.blur1

	emms	

	pop			ecx
	pop			edi
	pop			esi
	pop			ebp

	ret

;-----------------------------------------------------------------------------------------------
; void IndexedToRGB( void *src8, void *dst32, void *pal );
_IndexedToRGB:
	push		ebp
	mov			ebp, esp
	push		edi
	push		esi
	push		edx
	push		eax
	push		ecx

	mov			esi, [ebp+0x08]
	mov			edi, [ebp+0x0c]
	mov			edx, [ebp+0x10]
	mov			ecx, 800*600
.i1:
	xor			eax, eax
	lodsb
	mov			eax, [edx+eax*4]
	stosd
	loop		.i1

	pop			ecx
	pop			eax
	pop			edx
	pop			esi
	pop			edi
	pop			ebp

	ret
