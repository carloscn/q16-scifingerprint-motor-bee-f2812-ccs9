/*
 * motor.c
 *
 *  Created on: 2018��4��18��
 *      Author: weihaochen
 */
#include "global.h"

Uint32  motor_counter = 0;

uint16  error_count = 0;
void    INIT_MOTOR( void )
{
    init_eva();

    //motor_set_stop();
    motor_set_forward();
}


void init_eva()
{
    EALLOW;
    // ����GPIOΪPWM����
    GpioMuxRegs.GPAMUX.bit.PWM1_GPIOA0 = 1; // EVA PWM 1��
    GpioMuxRegs.GPAMUX.bit.PWM2_GPIOA1 = 1; // EVA PWM 2��
    EDIS;
    // EVA ����T1PWM, T2PWM, PWM1.
    // 0-PWM6
    // ��ʼ����ʱ��
    // ��ʼ��EVA��ʱ��1
    EvaRegs.T1PR = 0x1D4B;       // ����Ƶ��ȡ10k����f=TCK/(T1PR+1),�õ���ʱ��1����ֵ=7499=0x1D4B
    EvaRegs.T1CMPR = 0x0000;     // ��ʱ��1�ȽϼĴ�����ֵ
    EvaRegs.T1CNT = 0x0000;      // ��ʱ��1�����Ĵ�����ֵ
    // ����ģʽΪ��������
    // ��ʱ��ʹ��
    // ��ʱ�Ƚ�ʹ��
    EvaRegs.T1CON.all = 0x1042;  //����������ģʽ��T1CLK=HSPCLK=75MHZ,ʹ�ܶ�ʱ����ʹ�ܶ�ʱ�Ƚϲ���

    // ��ʼ��PWM1-PWM6��صıȽϼĴ���1��ֵ
    EvaRegs.CMPR1 = 0x0000;


    // �Ƚ϶������ƣ��������Ƚ�ƥ����������Ӧ��ƽ
    // �������1�������� CMPR1ƥ�� - ����͵�ƽ
    // �������2�������� CMPR1ƥ�� - ǿ������͵�ƽ

    EvaRegs.ACTRA.all = 0x0001;
    EvaRegs.DBTCONA.all = 0x0000; // ��ֹ����
    EvaRegs.COMCONA.all = 0xA600;//ʹ�ܱȽϲ�������T1CNT = 0�� T1CNT = T1PR����ƥ�������ƥ�䣩���¼��رȽϼĴ�����ֵ��
    //��T1CNT = 0�� T1CNT = T1PR����ƥ�������ƥ�䣩���¼��ض����Ĵ�����ֵ��
    //PWM1/2/3/4/5/6���Ƚ��߼����
    // ��������жϱ�־λ
    EvaRegs.EVAIFRA.bit.T1PINT=1;

    // ʹ�������жϣ�
    EvaRegs.EVAIMRA.bit.T1PINT=1;

}



void    motor_set_stop( void )
{
    EvaRegs.CMPR1 = 0;
}

void    motor_set_forward( void )
{
    EvaRegs.ACTRA.all = 0x0001;
    EvaRegs.CMPR1 = 6500;
    motor_counter = 0;
}


void    motor_set_backward( void )
{
    EvaRegs.ACTRA.all = 0x0004;
    EvaRegs.CMPR1 = 6500;
    motor_counter = 0;
}


interrupt void epwm1_isr(void)
{

    motor_counter   ++;

    if( motor_counter == 45000 ) {
        motor_set_stop();
        motor_counter = 0;
    }

    EvaRegs.EVAIFRA.bit.T1PINT=1;
    PieCtrlRegs.PIEACK.all|=0x002;       // PIEӦ��λ��1�������´��ж�
    PieCtrlRegs.PIEACK.all|=0x100;       // Issue PIE ack
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
