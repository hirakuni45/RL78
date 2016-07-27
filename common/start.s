
	.text

	.global _start
	.type	_start, @function
_start:
	movw	sp, #__stack


;; block move to initialize .data

	;; we're copying from 00:[_romdatastart] to 0F:[_datastart]
	;; and our data is not in the mirrored area.

	mov		es, #0

	sel		rb0		; bank 0
	movw	hl, #__datastart
	movw	de, #__romdatastart
	sel		rb1		; bank 1
	movw	ax, #__romdatacopysize
	shrw	ax,1
L15:
	cmpw	ax, #0
	bz		$L28
	decw	ax
	sel		rb0		; bank 0
	movw	ax, es:[de]
	movw	[hl], ax
	incw	de
	incw	de
	incw	hl
	incw	hl
	sel		rb1
	br		$L15
L28:
	sel		rb0		; bank 0



;; block fill to .bss

	sel		rb0		; bank 0
	movw	hl, #__bssstart
	movw	ax, #0
	sel		rb1		; bank 1
	movw	ax, #__bsssize
	shrw	ax,1
L39:
	cmpw	ax, #0
	bz		$L48
	decw	ax
	sel		rb0		; bank 0
	movw	[hl], ax
	incw	hl
	incw	hl
	sel		rb1
	br		$L39
L48:
	sel		rb0		; bank 0


	call	!!_init


	movw	ax, #0
	push	ax	/* envp */
	push	ax	/* argv */
	push	ax	/* argc */
	call	!!_main
	
    movw	ax, 0xffef0				 ; Save return code.
	push	ax

#ifdef PROFILE_SUPPORT
;;	call	!!__mcleanup
#endif

;	call	!!_exit

	.global _halt
_halt:
	br		$_halt

	.size	_start, . - _start

	.global	_rl78_run_ctor
	.type	_rl78_run_ctor,@function
_rl78_run_ctor:
	movw	hl, #__ctor_start
	movw	de, #__ctor_end
	movw	bc, #2
	br	$_rl78_run_inilist


	.global	_rl78_run_preinit_array
	.type	_rl78_run_preinit_array,@function
_rl78_run_preinit_array:
	movw	hl, #__preinit_array_start
	movw	de, #__preinit_array_end
;;	movw	bc, #-2
	movw	bc, #2
	br	$_rl78_run_inilist



	.global	_rl78_run_init_array
	.type	_rl78_run_init_array,@function
_rl78_run_init_array:
	movw	hl, #__init_array_start
	movw	de, #__init_array_end
	movw	bc, #2
	br	$_rl78_run_inilist



	.global	_rl78_run_fini_array
	.type	_rl78_run_fini_array,@function
_rl78_run_fini_array:
	movw	hl, #__fini_array_start
	movw	de, #__fini_array_end
;;	movw	bc, #-2
	movw	bc, #2

	;; HL = start of list
	;; DE = end of list
	;; BC = step direction (+2 or -2)
_rl78_run_inilist:
next_inilist:
	movw	ax, hl
	cmpw	ax, de
	bz		$done_inilist
	movw	ax, [hl]
	cmpw	ax, #-1
	bz		$skip_inilist
	cmpw	ax, #0
	bz		$skip_inilist
	push	ax
	push	bc
	push	de
	push	hl
	call	ax
	pop		hl
	pop		de
	pop		bc
	pop		ax

skip_inilist:
	movw	ax, hl
	addw	ax, bc
	movw	hl, ax
	br		$next_inilist

done_inilist:
	ret
