; x86_16 real-mode implementation of the Bresenham's line drawing algorithm
; Also contains a macro that puts a pixel on the 13h screen



; puts a pixel in mode 13h
%macro putpixel 5 ; assume %4 = 0xa000 ; %1 - x %2 - y  %3 - color %5 - memory reg
	shl %2, 6
	mov %5, %2   ; calculate the memory address of the beginning of the raw of pixels(raw number or y is in %2)
	shl %5, 2    ; and put it into the effective address combatible register (%5)
	add %5, %2   ; same as %5 = %2 * 320

	add %5, %1   ; add the x coordinate to %5 to get pixel's address
	mov byte [%4:%5], %3

%endmacro ; affects %5, %2 (%2 is shifted left by 6 bits)(can be restored is value was <= 0000001111111111b)


; args are on stack as following:
; word [sp+2] - x0
; word [sp+4] - y0
; word [sp+6] - x1
; word [sp+8] - y1
; word [sp+10] - color
;
; for mode 13h => 320x200 256  colors
; assume es = 0xa000
bresh_ln:
	mov bp, sp
	mov ax, word [bp+2]  ; ax = x0
	mov bx, word [bp+6]  ; bx = x1
	mov di, ax 			 ; save ax in di = x0

	sub ax, bx
	mov dx, ax
	sar ax, 15
	xor dx, ax
	sub dx, ax    ; dx = abs(x0-x1)

	mov ax, word [bp+4]   ; ax = y0
	mov bx, word [bp+8]   ; bx = y1
	mov si, ax 			  ; save ax in si = y0

	sub ax, bx
	mov cx, ax
	sar ax, 15
	xor cx, ax
	sub cx, ax    ; cx = abs(y0-y1)
	neg cx 		  ; cx = -asb(y0-y1)

    cmp si, bx
    jae ._decry
	._incry:
	cmp di, word [bp+6]
	jae ._incry_decrx
	._incry_incrx:  ; di = x0 si = y0 ax - err, bx and bp - for usage
		mov ax, dx
		add ax, cx    ; ax = err
		mov bp, sp
		mov bl, byte [bp+10]
		._incry_incrx_loop:	
			putpixel di, si, bl, es, bp
			shr si, 6
			mov bp, sp
			cmp di, word [bp+6]
			je ._incry_incrx_loop_xeq
			._incry_incrx_loop_endeqchk:
				mov bp, ax   ; bp is now e2
				add bp, bp
				cmp bp, cx   ; cmp e2, dy
				jge ._incry_incrx_loop_chx
				._incry_incrx_loop_endchx:
				cmp bp, dx
				jle ._incry_incrx_loop_chy
				._incry_incrx_loop_endchy:
		jmp ._incry_incrx_loop 

		._incry_incrx_loop_chx:
			add ax, cx
			inc di
		jmp ._incry_incrx_loop_endchx

		._incry_incrx_loop_chy:
			add ax, dx
			inc si
		jmp ._incry_incrx_loop_endchy

		._incry_incrx_loop_xeq:
			cmp si, word [bp+8]
			jne ._incry_incrx_loop_endeqchk
		ret

	._decry:
		cmp di, word [bp+6]
		jae ._decry_decrx
		._decry_incrx:
			mov ax, dx
			add ax, cx  ; ax = err
			mov bp, sp
			mov bl, byte [bp+10]
		._decry_incrx_loop:	
			putpixel di, si, bl, es, bp
			shr si, 6
			mov bp, sp
			cmp di, word [bp+6]
			je ._decry_incrx_loop_xeq
			._decry_incrx_loop_endeqchk:
				mov bp, ax   ; bp is now e2
				add bp, bp
				cmp bp, cx   ; cmp e2, dy
				jge ._decry_incrx_loop_chx
				._decry_incrx_loop_endchx:
				cmp bp, dx
				jle ._decry_incrx_loop_chy
				._decry_incrx_loop_endchy:
		jmp ._decry_incrx_loop

		._decry_incrx_loop_chx:
			add ax, cx
			inc di
		jmp ._decry_incrx_loop_endchx

		._decry_incrx_loop_chy:
			add ax, dx
			dec si
		jmp ._decry_incrx_loop_endchy

		._decry_incrx_loop_xeq:
			cmp si, word [bp+8]
			jne ._decry_incrx_loop_endeqchk
		ret

		._incry_decrx:
			mov ax, dx
			add ax, cx  ; ax = err
			mov bp, sp
			mov bl, byte [bp+10]
		._incry_decrx_loop:	
			putpixel di, si, bl, es, bp
			shr si, 6
			mov bp, sp
			cmp di, word [bp+6]
			je ._incry_decrx_loop_xeq
			._incry_decrx_loop_endeqchk:
				mov bp, ax   ; bp is now e2
				add bp, bp
				cmp bp, cx   ; cmp e2, dy
				jge ._incry_decrx_loop_chx
				._incry_decrx_loop_endchx:
				cmp bp, dx
				jle ._incry_decrx_loop_chy
				._incry_decrx_loop_endchy:
		jmp ._incry_decrx_loop

		._incry_decrx_loop_chx:
			add ax, cx
			dec di
		jmp ._incry_decrx_loop_endchx

		._incry_decrx_loop_chy:
			add ax, dx
			inc si
		jmp ._incry_decrx_loop_endchy

		._incry_decrx_loop_xeq:
			cmp si, word [bp+8]
			jne ._incry_decrx_loop_endeqchk
		ret

		._decry_decrx:
			mov ax, dx
			add ax, cx  ; ax = err
			mov bp, sp
			mov bl, byte [bp+10]
		._decry_decrx_loop:	
			putpixel di, si, bl, es, bp
			shr si, 6
			mov bp, sp
			cmp di, word [bp+6]
			je ._decry_decrx_loop_xeq
			._decry_decrx_loop_endeqchk:
				mov bp, ax   ; bp is now e2
				add bp, bp
				cmp bp, cx   ; cmp e2, dy
				jge ._decry_decrx_loop_chx
				._decry_decrx_loop_endchx:
				cmp bp, dx
				jle ._decry_decrx_loop_chy
				._decry_decrx_loop_endchy:
		jmp ._decry_decrx_loop

		._decry_decrx_loop_chx:
			add ax, cx
			dec di
		jmp ._decry_decrx_loop_endchx

		._decry_decrx_loop_chy:
			add ax, dx
			dec si
		jmp ._decry_decrx_loop_endchy

		._decry_decrx_loop_xeq:
			cmp si, word [bp+8]
			jne ._decry_decrx_loop_endeqchk
		ret


