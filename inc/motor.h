/*
 * motor.h
 *
 *  Created on: 2018Äê4ÔÂ18ÈÕ
 *      Author: weihaochen
 */

#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

void init_eva();
void    motor_set_stop( void );
void    motor_set_forward( void );
void    motor_set_backward( void );
extern interrupt void epwm1_isr(void);
extern void    INIT_MOTOR( void );

#endif /* INC_MOTOR_H_ */
