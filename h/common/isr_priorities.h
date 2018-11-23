/* Author: t.me/dmedser */

#ifndef ISR_PRIORITIES_H_
#define ISR_PRIORITIES_H_

/* Service request priority number
 * (0 - lowest, 0xFF - highest priority)
 */

#define ISR_PN_EDGE_CAPTURE             250
#define ISR_PN_BIT_TX_TIMEOUT           249
#define ISR_PN_BIT_TX_PWM               250
#define ISR_PN_CAN_RX                   100
#define ISR_PN_CAN_TX                   99
#define ISR_PN_RTOS_100_US              200
#define ISR_PN_RTOS_1_MS                199
#define ISR_PN_GLITCH                   101

#endif /* ISR_PRIORITIES_H_ */
