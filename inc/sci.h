/*
 * sci.h
 *
 *  Created on: 2018Äê4ÔÂ16ÈÕ
 *      Author: weihaochen
 */

#ifndef INC_SCI_H_
#define INC_SCI_H_

#define                     CPU_FREQ     150E6
#define                     LSPCLK_FREQ  CPU_FREQ/4
#define                     SCI_FREQ     100E3
#define                     SCI_PRD      (LSPCLK_FREQ/(SCI_FREQ*8))-1
#define                     SCI_RX_LENGTH_MAX           256

void scia_fifo_init();
void scib_fifo_init();
void error(void);

extern void    SCI_INIT( void );
extern void scib_msg(char * msg);
extern void scib_xmit(int a);
extern void scia_msg(char * msg);
extern void scia_xmit(int a);
extern interrupt void scibRxFifoIsr(void);
extern interrupt void sciaRxFifoIsr(void);
#endif /* INC_SCI_H_ */
