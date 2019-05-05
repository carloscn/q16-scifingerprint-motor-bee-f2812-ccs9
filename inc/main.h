/*
 * main.h
 *
 *  Created on: 2018年4月16日
 *      Author: weihaochen
 */

#ifndef INC_MAIN_H_
#define INC_MAIN_H_

// Prototype statements for functions found within this file.
void    alarm_voice( float sec );
interrupt void cpu_timer0_isr(void);
void Delay(Uint16 t)  ;
void Init_Bell(void);
void    alarm_voice( float sec );

#define BUZZ_CLK_GENER  GpioDataRegs.GPATOGGLE.bit.GPIOA9 = 1;  //蜂鸣器控制IO，IO电平翻转，                                                                //产生控制脉冲
#define BELL_DIR GpioMuxRegs.GPADIR.bit.GPIOA9
#define BELL_DAT GpioDataRegs.GPADAT.bit.GPIOA9
#define DISABLE_TIMER0_INT      IER &= 0xFFFE;
#define ENABLE_TIMER0_INT       IER |= M_INT1;
#define BUZZ_OFF                GpioDataRegs.GPACLEAR.bit.GPIOA9 = 1;

void Init_Bell(void);
interrupt void cpu_timer0_isr(void);
void Delay(Uint16 t);

Uint16 Song[]={1,2,3,4,5,6,7}; //乐谱：do,re,mi,fa,so,la,xi
Uint16 Musi[23]={       //单位 us，不同频率下，蜂鸣器发出不同音调的声音
                        0,
                        3816,   //L_do
                        3496,   //L_re
                        3215,   //L_mi
                        2865,   //L_fa
                        2551,   //L_so
                        2272,   //L_la
                        2024,   //L_xi
                        1912,   //do
                        1703,   //re
                        1517,   //mi
                        1432,   //fa
                        1275,   //so
                        1136,   //la
                        1013,   //xi
                        956,    //H_do
                        851,    //H_re
                        758,    //H_mi
                        716,    //H_fa
                        638,    //H_so
                        568,    //H_la
                        506,    //H_xi
                        0xFF    //STOP
};
// Global variables
#endif /* INC_MAIN_H_ */
