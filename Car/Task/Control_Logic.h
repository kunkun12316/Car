#ifndef __CONTROL_LOGIC_H_
#define __CONTROL_LOGIC_H_

#include "../Bsp/Bsp.h"

extern uint8_t Car_Task_Data_0;
extern int16_t Car_Task_Data_1;
extern int16_t Car_Task_Data_2;
extern uint8_t Running_Task_Flag;

void Tack_mode_Proc(void);

void move_to_qr(void);
void qr_task(void);
void move_to_material(void);
void material_first_times_task(void);
void move_to_temp(void);
void temp_first_times_task(void);
void move_to_store(void);
void store_first_times_task(void);
void back_to_material(void);
void material_second_times_task(void);
void temp_second_times_task(void);
void store_second_times_task(void);
void back_to_begin(void);

void Tack_mode_Proc_Che(void);

#endif