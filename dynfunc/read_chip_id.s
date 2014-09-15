   0:	24f0      	subi	r0, r0, 16
   2:	9820      	st	r8, (r0, 8)
   4:	1208      	mov	r8, r0
   6:	6007      	movi	r7, 0
   8:	9718      	st	r7, (r8, 4)
   a:	6007      	movi	r7, 0
   c:	9708      	st	r7, (r8, 0)
   e:	33d7      	bgeni	r7, 29
  10:	35f7      	bseti	r7, r7, 31
  12:	9708      	st	r7, (r8, 0)
  14:	8708      	ld	r7, (r8, 0)
  16:	3547      	bseti	r7, r7, 20
  18:	3557      	bseti	r7, r7, 21
  1a:	9708      	st	r7, (r8, 0)
  1c:	8708      	ld	r7, (r8, 0)
  1e:	34d7      	bseti	r7, r7, 13
  20:	34f7      	bseti	r7, r7, 15
  22:	9708      	st	r7, (r8, 0)
  24:	8708      	ld	r7, (r8, 0)
  26:	3477      	bseti	r7, r7, 7
  28:	3487      	bseti	r7, r7, 8
  2a:	9708      	st	r7, (r8, 0)
  2c:	8708      	ld	r7, (r8, 0)
  2e:	8707      	ld	r7, (r7, 0)
  30:	3f07      	lsri	r7, r7, 16
  32:	2e37      	andi	r7, r7, 3
  34:	9718      	st	r7, (r8, 4)
  36:	8718      	ld	r7, (r8, 4)
  38:	1272      	mov	r2, r7
  3a:	1280      	mov	r0, r8
  3c:	8820      	ld	r8, (r0, 8)
  3e:	20f0      	addi	r0, r0, 16
  40:	00cf      	jmp	r15
