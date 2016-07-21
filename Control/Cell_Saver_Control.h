
#include "stm32f10x.h"
#include "../User/user_conf.h"
extern void Cell_Saver_Control(void);
extern void Pump_set_ARR(uint16 speed);

extern uint16 CRC_CHK(uint8 *tx_data, uint8 length);
