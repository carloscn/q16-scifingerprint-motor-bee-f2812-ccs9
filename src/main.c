//###########################################################################
// $TI Release: DSP281x C/C++ Header Files V1.20 $
// $Release Date: July 27, 2009 $
//###########################################################################

#include "global.h"
#include "main.h"

//#define             FLASH
// Functions that will be run from RAM need to be assigned to
// a different section.  This section will then be mapped using
// the linker cmd file.
// #pragma CODE_SECTION(eva_timer1_isr, "ramfuncs");
// These are defined by the linker (see F2812.cmd)
#ifdef  FLASH

extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;

#endif
#define                     INPUT             0

Uint16 sdataA[8];    // Send data for SCI-A
Uint16 sdataB[8];    // Send data for SCI-B
Uint16 rdataA[8];    // Received data for SCI-A
Uint16 rdataB[8];    // Received data for SCI-A
Uint16 rdata_pointA; // Used for checking the received data
Uint16 rdata_pointB;
AS608   *as608_p, as608;
bool    open_flag = false;
bool    id_flag =   false;

void main(void)
{
    Uint16 k;
    Uint16 i, n,ensure, msg_str[40];
    Uint16 count = 0;
    Uint16 addr=0;
    as608_p  =    &as608;
    // Step 1. Initialize System Control:-
    // PLL, WatchDog, enable Peripheral Clocks
    // This example function is found in the DSP281x_SysCtrl.c file.
    InitSysCtrl();

    // Step 2. Initalize GPIO:
    // This example function is found in the DSP281x_Gpio.c file and
    // illustrates how to set the GPIO to it's default state.
    InitGpio();  // Skipped for this example

    // Step 3. Clear all interrupts and initialize PIE vector table:
    // Disable CPU interrupts
    DINT;

    // Initialize PIE control registers to their default state.
    // The default state is all PIE interrupts disabled and flags
    // are cleared.
    // This function is found in the DSP281x_PieCtrl.c file.
    InitPieCtrl();

    // Disable CPU interrupts and clear all CPU interrupt flags:
    IER = 0x0000;
    IFR = 0x0000;

    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    // This will populate the entire table, even if the interrupt
    // is not used in this example.  This is useful for debug purposes.
    // The shell ISR routines are found in DSP281x_DefaultIsr.c.
    // This function is found in DSP281x_PieVect.c.
    InitPieVectTable();

    // Interrupts that are used in this example are re-mapped to
    // ISR functions found within this file.
    EALLOW;  // This is needed to write to EALLOW protected registers
    PieVectTable.RXAINT = &sciaRxFifoIsr;
    PieVectTable.RXBINT = &scibRxFifoIsr;
    PieVectTable.T1PINT = &epwm1_isr;
    PieVectTable.TINT0 = &cpu_timer0_isr;
    EDIS;   // This is needed to disable write to EALLOW protected registers
    BELL_DAT=0;
    Init_Bell(); //���÷������˿����

    // Step 4. Initialize all the Device Peripherals:
    // This function is found in DSP281x_InitPeripherals.c
    // InitPeripherals(); // Not required for this example
    SCI_INIT();
    INIT_MOTOR();
    motor_set_stop();
    AS608_INIT( as608_p );
    // Step 5. User specific code, enable interrupts:

    // Init send data.  After each transmission this data
    // will be updated for the next transmission
    // ���� 4.��ʼ��Ƭ������:
    InitCpuTimers();   // ��������Ҫ��ʼ��CPU��ʱ��

    // Copy time critical code and Flash setup code to RAM
    // This includes the following ISR functions: EvaTimer1(), EvaTimer2()
    // EvbTimer3 and and InitFlash();
    // The  RamfuncsLoadStart, RamfuncsLoadEnd, and RamfuncsRunStart
    // symbols are created by the linker. Refer to the F2812.cmd file.
#ifdef  FLASH
    MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);

    // Call Flash Initialization to setup flash waitstates
    // This function must reside in RAM
    InitFlash();
#endif
    // Enable interrupts required for this example
    PieCtrlRegs.PIECRTL.bit.ENPIE = 1;   // Enable the PIE block
    PieCtrlRegs.PIEIER9.bit.INTx1   =1;     // PIE Group 9, INT1
    PieCtrlRegs.PIEIER9.bit.INTx3   =1;     // PIE Group 9, INT3
    PieCtrlRegs.PIEIER2.bit.INTx4=1;     // PIE �� 2, INT4��T1PINT
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
    IER |= M_INT9 | M_INT2 | M_INT1; // Enable CPU INT

    ERTM;   // Enable Global realtime interrupt DBGM
    // Enable global Interrupts and higher priority real-time debug events:
    EINT;   // Enable Global interrupt INTM
    scib_msg("scib: �豸���ڳ�ʼ�����������������.... \n\r");

    DELAY_MS(5000);
    DELAY_MS(2000);

    alarm_voice(100);
    // Step 6. IDLE loop. Just sit and loop forever:

    scib_msg("scib: ��ʼ���������ź�.... \n\r");
    scib_msg("scib: ������������źų�ʱ��û��Ӧ���밴�����ϵĺ�ɫ��λ���������� \n\r");
    while( !as608_p->hand_shake(as608_p, as608_p->device_address )  );
    //printf("debug:�豸�������\n");
    DELAY_MS(100);

    scib_msg("scib: ��ȡ�豸ָ�Ƹ���....\n\r");
    ensure = as608_p->valid_templete_num( as608_p, &n );
    if( ensure != 0x00 ) {
        as608_p->ensure_message( as608_p, ensure );
    }
    DELAY_MS(100);
    scib_msg("scib: ��ȡ�豸����....\n\r");
    ensure = as608_p->read_sys_para( as608_p, &as608_p->system_para );

    if ( ensure == 0x00 ) {
        memset(msg_str, 0,50);
        sprintf( msg_str, "scib: current num: %d, level: %d\n\r" ,as608_p->system_para.PS_max, as608_p->system_para.PS_level);
        scib_msg( msg_str );
    }else {
        as608_p->ensure_message( as608_p, ensure );
    }
#if INPUT
    DELAY_MS(1000);
    as608_p->add_finger( as608_p );
#endif

    ConfigCpuTimer(&CpuTimer0, 150, 1000000);
    CpuTimer0Regs.TCR.bit.TSS   =   0;
    while(1) {
#if !INPUT
        if( TOUCH ==  as608_p->read_state( as608_p ) ) {
            as608_p->press_finger( as608_p );
        }
#endif

    }
}

/*------------------------------------------*/
/*��ʽ������void                            */
/*����ֵ:void                              */
/*��������:��ʼ���������˿�Ϊ���           */
/*------------------------------------------*/

void Init_Bell(void)
{
    EALLOW;    //����Ĵ�������
    GpioMuxRegs.GPAMUX.bit.CAP2Q2_GPIOA9 = 0;
    BELL_DIR=1;//Bell�˿����
    EDIS;      //�Ĵ�������
}
/*------------------------------------------*/
/*��ʽ������void                            */
/*����ֵ:void                              */
/*��������:��ʱ��CPU0�жϷ����ӳ���         */
/*------------------------------------------*/

interrupt void cpu_timer0_isr(void)
{
    CpuTimer0.InterruptCount++;
    if( CpuTimer0.InterruptCount >= 15 ) {
        if( id_flag == true ) {
            id_flag = false;
            scib_msg( "scib: no any operation, close the door!!\n" );
            motor_set_backward();
        }
        CpuTimer0.InterruptCount = 0;
    }
    // Acknowledge this interrupt to receive more interrupts from group 1
    PieCtrlRegs.PIEACK.all|=0x002;       // PIEӦ��λ��1�������´��ж�
    PieCtrlRegs.PIEACK.all|=0x100;       // Issue PIE ack
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

void Delay(Uint16 t)                                                        //��ʱ����
{
    Uint32 i=0;
    Uint32 gain = 300000;                                                   //��ʱ����
    Uint32 base=0;
    base=gain*t;
    for(i=0;i<=base;i++);
}

void    alarm_voice( float sec )
{
    int i;
    for(i = 0; i < sec; i ++) {
        BUZZ_CLK_GENER;
        DELAY_MS(1);
    }
}




//===========================================================================
// No more.
//===========================================================================
