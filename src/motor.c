/*
 * motor.c
 *
 *  Created on: 2018年4月18日
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
    // 配置GPIO为PWM引脚
    GpioMuxRegs.GPAMUX.bit.PWM1_GPIOA0 = 1; // EVA PWM 1脚
    GpioMuxRegs.GPAMUX.bit.PWM2_GPIOA1 = 1; // EVA PWM 2脚
    EDIS;
    // EVA 配置T1PWM, T2PWM, PWM1.
    // 0-PWM6
    // 初始化定时器
    // 初始化EVA定时器1
    EvaRegs.T1PR = 0x1D4B;       // 开关频率取10k，由f=TCK/(T1PR+1),得到定时器1周期值=7499=0x1D4B
    EvaRegs.T1CMPR = 0x0000;     // 定时器1比较寄存器的值
    EvaRegs.T1CNT = 0x0000;      // 定时器1计数寄存器的值
    // 计数模式为增减计数
    // 定时器使能
    // 定时比较使能
    EvaRegs.T1CON.all = 0x1042;  //连续增计数模式，T1CLK=HSPCLK=75MHZ,使能定时器，使能定时比较操作

    // 初始化PWM1-PWM6相关的比较寄存器1的值
    EvaRegs.CMPR1 = 0x0000;


    // 比较动作控制，当发生比较匹配后动作输出相应电平
    // 输出引脚1，当发生 CMPR1匹配 - 输出低电平
    // 输出引脚2，当发生 CMPR1匹配 - 强制输出低电平

    EvaRegs.ACTRA.all = 0x0001;
    EvaRegs.DBTCONA.all = 0x0000; // 禁止死区
    EvaRegs.COMCONA.all = 0xA600;//使能比较操作；当T1CNT = 0或 T1CNT = T1PR（零匹配或周期匹配）重新加载比较寄存器的值；
    //当T1CNT = 0或 T1CNT = T1PR（零匹配或周期匹配）重新加载动作寄存器的值；
    //PWM1/2/3/4/5/6按比较逻辑输出
    // 清除周期中断标志位
    EvaRegs.EVAIFRA.bit.T1PINT=1;

    // 使能周期中断；
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
    PieCtrlRegs.PIEACK.all|=0x002;       // PIE应答位置1，允许下次中断
    PieCtrlRegs.PIEACK.all|=0x100;       // Issue PIE ack
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
