
#include "global.h"

extern  AS608   *as608_p;

void    SCI_INIT( void )
{
    EALLOW;
    GpioMuxRegs.GPFMUX.bit.SCITXDA_GPIOF4 = 1;
    GpioMuxRegs.GPFMUX.bit.SCIRXDA_GPIOF5 = 1;
    GpioMuxRegs.GPGMUX.bit.SCITXDB_GPIOG4 = 1;
    GpioMuxRegs.GPGMUX.bit.SCIRXDB_GPIOG5 = 1;
    EDIS;
    scia_fifo_init();
    scib_fifo_init();
}


void scia_xmit(int a)
{
    while( SciaRegs.SCICTL2.bit.TXRDY != 1 );
    SciaRegs.SCITXBUF=a;

}

void scia_msg(char * msg)
{
    int i;
    i = 0;
    while(msg[i] != '\0')
    {
        scia_xmit(msg[i]);
        i++;

    }
}

void scib_xmit(int a)
{
    while (ScibRegs.SCIFFTX.bit.TXFFST != 0) {}
    ScibRegs.SCITXBUF=a;

}
void scib_msg(char * msg)
{
    int i;
    i = 0;
    while(msg[i] != '\0')
    {
        scib_xmit(msg[i]);
        i++;
    }
}



void error(void)
{
    asm("     ESTOP0"); // Test failed!! Stop!
    for (;;);
}


interrupt void sciaRxFifoIsr(void)
{
    Uint16 i;

    if( as608_uart_process( as608_p ,SciaRegs.SCIRXBUF.all) ) { // Read data
        as608_p->read_flag  =   true;
    }

    SciaRegs.SCIFFRX.bit.RXFFOVRCLR=1;   // Clear Overflow flag
    SciaRegs.SCIFFRX.bit.RXFFINTCLR=1;   // Clear Interrupt flag
    PieCtrlRegs.PIEACK.all|=0x002;       // PIE应答位置1，允许下次中断
    PieCtrlRegs.PIEACK.all|=0x100;       // Issue PIE ack
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

interrupt void scibRxFifoIsr(void)
{
    Uint16 i;
    for(i=0;i<8;i++)
    {
        //rdataB[i]=ScibRegs.SCIRXBUF.all;  // Read data
    }

    ScibRegs.SCIFFRX.bit.RXFFOVRCLR=1;  // Clear Overflow flag
    ScibRegs.SCIFFRX.bit.RXFFINTCLR=1;  // Clear Interrupt flag
    PieCtrlRegs.PIEACK.all|=0x100;      // Issue PIE ack
}


void scia_fifo_init()
{
    SciaRegs.SCICCR.all =0x0007;   // 1 stop bit,  No loopback
    // No parity,8 char bits,
    // async mode, idle-line protocol
    SciaRegs.SCICTL1.all =0x0003;  // enable TX, RX, internal SCICLK,
    // Disable RX ERR, SLEEP, TXWAKE
    SciaRegs.SCICTL2.bit.TXINTENA =1;
    SciaRegs.SCICTL2.bit.RXBKINTENA =1;
    SciaRegs.SCIHBAUD    =0x0000;  // 9600 baud @LSPCLK = 37.5MHz.
    SciaRegs.SCILBAUD    = 0x0050;
    SciaRegs.SCICCR.bit.LOOPBKENA = 0; // Enable loop back
    SciaRegs.SCICTL2.bit.RXBKINTENA =   1;
    SciaRegs.SCICTL2.bit.TXINTENA   =   0;

    SciaRegs.SCICTL1.all =0x0023;     // Relinquish SCI from Reset



}
void scib_fifo_init()
{
    ScibRegs.SCICCR.all =0x0007;    // 1 stop bit,  No loopback
    // No parity,8 char bits,
    // async mode, idle-line protocol
    ScibRegs.SCICTL1.all =0x0003;   // enable TX, RX, internal SCICLK,
    // Disable RX ERR, SLEEP, TXWAKE
    ScibRegs.SCICTL2.bit.TXINTENA =1;
    ScibRegs.SCICTL2.bit.RXBKINTENA =1;
    ScibRegs.SCIHBAUD    =0x0000;  // 9600 baud @LSPCLK = 37.5MHz.
    ScibRegs.SCILBAUD    = 39;
    ScibRegs.SCICCR.bit.LOOPBKENA =0; // Enable loop back
    ScibRegs.SCIFFTX.all=0xC028;
    ScibRegs.SCIFFRX.all=0x0028;
    ScibRegs.SCIFFCT.all=0x00;

    ScibRegs.SCICTL1.all =0x0023;     // Relinquish SCI from Reset
    ScibRegs.SCIFFTX.bit.TXFIFOXRESET=1;
    ScibRegs.SCIFFRX.bit.RXFIFORESET=1;

}


