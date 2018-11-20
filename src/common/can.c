/* Author: t.me/dmedser */

#include "can.h"
#include "hbp_tx.h"
#include "hbp_rx.h"
#include "isr_priorities.h"
#include <IfxMultican_Can.h>
#include <IfxSrc_reg.h>
#include <stdint.h>
#include <Platform_Types.h>
#include <machine/cint.h>

#if(OP_MODE == TRANSMITTER)
	#define CAN_DST_MO_MSG_ID   TRANSMITTER_ID
#else
	#define CAN_DST_MO_MSG_ID	RECEIVER_ID
#endif

/* CAN handle */
static IfxMultican_Can can;

/* Node handle */
static IfxMultican_Can_Node can_node;

/* Message Object handles */
#if(OP_MODE == RECEIVER)
static IfxMultican_Can_MsgObj can_src_mo;
#endif
static IfxMultican_Can_MsgObj can_dst_mo;

#if(OP_MODE == RECEIVER)
static uint16_t can_tx_msg_id = 0;
#endif

void can_init(void) {
	/* CAN */
	IfxMultican_Can_Config can_cfg;

	/* Default settings */
	IfxMultican_Can_initModuleConfig(&can_cfg, &MODULE_CAN);

	IfxMultican_Can_initModule(&can, &can_cfg);

	/* CAN node */
	IfxMultican_Can_NodeConfig can_node_cfg;

	/* Default settings */
	IfxMultican_Can_Node_initConfig(&can_node_cfg, &can);

	can_node_cfg.baudrate 					= 500000; 						/* 500 kBit/sec */
	can_node_cfg.nodeId						= IfxMultican_NodeId_0;
	can_node_cfg.rxPin 						= &IfxMultican_RXD0A_P02_1_IN;
	can_node_cfg.txPin 						= &IfxMultican_TXD0_P02_0_OUT;
	can_node_cfg.rxPinMode					= IfxPort_InputMode_pullUp;
	can_node_cfg.txPinMode 					= IfxPort_OutputMode_pushPull;
	can_node_cfg.transferInterrupt.enabled	= 0b1;
	can_node_cfg.transferInterrupt.srcId	= IfxMultican_SrcId_0;

	IfxMultican_Can_Node_init(&can_node, &can_node_cfg);

	#if(OP_MODE == RECEIVER)
	/* Source MO, TX */
	{
		IfxMultican_Can_MsgObjConfig can_src_mo_cfg;

		/* Default settings */
		IfxMultican_Can_MsgObj_initConfig(&can_src_mo_cfg, &can_node);

		can_src_mo_cfg.msgObjId = 0;
		can_src_mo_cfg.frame 	= IfxMultican_Frame_transmit;

		IfxMultican_Can_MsgObj_init(&can_src_mo, &can_src_mo_cfg);
	}
	#endif

	/* Destination MO, RX */
	{
		IfxMultican_Can_MsgObjConfig can_dst_mo_cfg;

		/* Default settings */
		IfxMultican_Can_MsgObj_initConfig(&can_dst_mo_cfg, &can_node);

		can_dst_mo_cfg.msgObjId				= 1;
		can_dst_mo_cfg.messageId			= CAN_DST_MO_MSG_ID;
		can_dst_mo_cfg.frame				= IfxMultican_Frame_receive;
		can_dst_mo_cfg.rxInterrupt.srcId	= IfxMultican_SrcId_0;
		can_dst_mo_cfg.rxInterrupt.enabled  = TRUE;

		IfxMultican_Can_MsgObj_init(&can_dst_mo, &can_dst_mo_cfg);
	}

	/* CAN receive interrupt */
	/* Service request priority number */
	MODULE_SRC.CAN.CAN[0].INT[0].B.SRPN = ISR_PN_CAN_RX;
	/* Enable service request */
	MODULE_SRC.CAN.CAN[0].INT[0].B.SRE = 0b1;
	_install_int_handler(ISR_PN_CAN_RX, (void (*) (int))ISR_can_rx, 0);

	#if(OP_MODE == RECEIVER)
	/* CAN transmit interrupt */
	/* Service request priority number */
	MODULE_SRC.GPSR.GPSR[0].SR1.B.SRPN = ISR_PN_CAN_TX;
	/* Enable service request */
	MODULE_SRC.GPSR.GPSR[0].SR1.B.SRE = 0b1;
	_install_int_handler(ISR_PN_CAN_TX, (void (*) (int))ISR_can_tx, 0);
	#endif
}


static uint32_t swap_endianness(uint32_t value) {
	return ((value & ((uint32_t)0xFF << 0))  << 24) |
		   ((value & ((uint32_t)0xFF << 8))  << 8)  |
		   ((value & ((uint32_t)0xFF << 16)) >> 8)  |
		   ((value & ((uint32_t)0xFF << 24)) >> 24);
}


#if(OP_MODE == RECEIVER)
static void can_tx(uint32_t id, uint64_t data) {
	uint32_t data_low  = (uint32_t)data;
	uint32_t data_high = (uint32_t)(data >> 32);

	IfxMultican_Message tx_msg;
	IfxMultican_Message_init(&tx_msg, id, data_low, data_high, IfxMultican_DataLengthCode_8);

	while( IfxMultican_Can_MsgObj_sendMessage(&can_src_mo, &tx_msg) == IfxMultican_Status_notSentBusy );
}
#endif


void ISR_can_rx(void) {
	IfxMultican_Message rx_msg;

	IfxMultican_Can_MsgObj_readMessage(&can_dst_mo, &rx_msg);

	if(rx_msg.id == CAN_DST_MO_MSG_ID) {
		#if(OP_MODE == TRANSMITTER)
		uint32_t data_high = swap_endianness(rx_msg.data[0]);

		uint8_t received_opcode = (uint8_t)(data_high >> 24);

		#define START	0xFF
		#define STOP	0x01

		switch(received_opcode) {
		case START:
			start_hbp_tx();

			/* TEST */
			/* Disable CAN RX interrupts for transmitter */
			MODULE_SRC.CAN.CAN[0].INT[0].B.SRE = 0b0;

			break;
		case STOP:
			stop_hbp_tx();
			break;
		}
		#else

		/* Receiver CAN RX interrupt routine */

		#endif
	}
}


#if(OP_MODE == RECEIVER)
void ISR_can_tx(void) {

	/* Interrupts shouldn't be disabled here! */

	#define SUBFRAME_IS_TRANSMITTED	 (idx_of_word_to_tx == SUBFRAME_LEN)

	if(SUBFRAME_IS_TRANSMITTED) {
		idx_of_word_to_tx = 0;
		idx_of_subframe_to_tx = idx_of_subframe_to_rx;
	}
	else {
		uint64_t can_tx_msg_data = frame.get_8_bytes_from(&subframe_to_tx);
		uint64_t can_tx_msg_id_splitted = (uint64_t)(can_tx_msg_id & (0xF << 0))  << (12 - 0) |
							   	   	   	  (uint64_t)(can_tx_msg_id & (0xF << 4))  << (28 - 4) |
							   	   	   	  (uint64_t)(can_tx_msg_id & (0xF << 8))  << (44 - 8) |
							   	   	   	  (uint64_t)(can_tx_msg_id & (0xF << 12)) << (60 - 12);
		can_tx_msg_data |= can_tx_msg_id_splitted;
		can_tx(RECEIVER_ID, can_tx_msg_data);
		idx_of_word_to_tx += 4;
		can_tx_msg_id++;
	}

}
#endif

