
/***************************************************************************************************************

Title:		uart2_tx_char.s

Author:		Inspired Microchip library
			Davis Daidi�
			Michael Bonani
History:
	11/07/05	Start day
	16/09/05	Adaptation uart2->uart1
	22/12/05	Optimisation and change name of function

****************************************************************************************************************/
;to be used with uart_txrx_char.h 

.include "p30f6014A.inc"


.global U2TXPtr
.global U2TXLength
.global U2TXOk

.section  .data, near

U2TXPtr:  .hword 0x0000
U2TXLength: .hword 0x0000
U2TXOk:		.hword 0x0000

.global         __U2TXInterrupt

.section        .text
__U2TXInterrupt:
	bclr    IFS1, #U2TXIF           ;Clear the interrupt flag
        push.d  w0                      ;save context - w0,w1, w2
        push    w2
       

		cp0		U2TXOk
		bra		z, end_transmission

        cp0     U2TXPtr                 ;Check if the pointer is valid (not zero)
        bra     z, exit2_U2TXInt        ;Else exit

		cp0		U2TXLength
		bra		z,exit2_U2TXInt

        mov     U2TXPtr, w1
        mov     #4, w2                  ;Initialize w2=4 as a decrementing counter


load_next_char:
        
	btsc	U2STA,#9	;make sur buffer is not full
	bra 	load_next_char

        mov	U2TXLength, w0			;load lenght of buffer
		cp0     w0                    	;unless end of buffer is encountered
        bra     z, loaded_all_char      
		dec		w0,w0
		mov 	WREG,U2TXLength			;rest of lenght

		mov.b   [w1++], w0              ;Load 4 characters into UART TX queue
        mov.b   WREG, U2TXREG
        dec     w2, w2
        bra     nz, load_next_char
        bra     exit1_U2TXInt           ;Exit ISR after loading 4 chars

loaded_all_char:
        clr     U2TXPtr                 ;Clear pointer if all char send
        bra     end_transmission

exit1_U2TXInt:
        mov     w1, U2TXPtr             ;Save new value of pointer
end_transmission:
 		pop     w2                      ;Restore context - w0, w1, w2
        pop.d   w0
        retfie                          ;Return from Interrupt

exit2_U2TXInt:
        mov		#0,w0
		mov		w0,U2TXOk
		pop     w2                      ;Restore context - w0, w1, w2
        pop.d   w0
        retfie                          ;Return from Interrupt


.global _e_send_uart2_char

; in: w0 pointer on char buffer
; in: w1 lengh of buffert

_e_send_uart2_char:

       
wait_l:							; wait until old buffer sent
		cp0		U2TXOk			; check if pointer is under trsnfer
		bra     nz, wait_l
		bclr    IEC1, #U2TXIE  ;disable interupt
		dec		w1,w1
		mov		w1, U2TXLength   ; move lengh of buffer in U2TXLength
		
		mov.b	[w0++],w1
		mov 	w1,U2TXREG
		mov     w0, U2TXPtr
		mov		#1,w1
		mov		w1,U2TXOk
		bset    IEC1, #U2TXIE	;enable interupt		
        return
       
.global _e_uart2_sending       
        
_e_uart2_sending:
		mov U2TXOk,w0
        return        
      
.end                                    ;EOF



        
