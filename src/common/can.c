/* Author: t.me/dmedser */

#include "can.h"
#include <stdint.h>
#include <Platform_Types.h>

/* CAN handle */
static IfxMultican_Can can;

/* Node handles */
static IfxMultican_Can_Node can_src_node;
static IfxMultican_Can_Node can_dst_node;

/* Message Object handles */
static IfxMultican_Can_MsgObj can_src_mo;
static IfxMultican_Can_MsgObj can_dst_mo;

void can_init(void) {
	/* CAN */
	IfxMultican_Can_Config can_cfg;
	/* Default settings */
	IfxMultican_Can_initModuleConfig(&can_cfg, &MODULE_CAN);
	IfxMultican_Can_initModule(&can, &can_cfg);


	/* Source & destination nodes */
	IfxMultican_Can_NodeConfig can_node_cfg;
	/* Default settings */
	IfxMultican_Can_Node_initConfig(&can_node_cfg, &can);
	can_node_cfg.baudrate 	= 500000; 						/* 500 kBit/sec */
	can_node_cfg.nodeId		= IfxMultican_NodeId_0;
	can_node_cfg.rxPin 		= &IfxMultican_RXD0A_P02_1_IN;
	can_node_cfg.txPin 		= &IfxMultican_TXD0_P02_0_OUT;
	can_node_cfg.rxPinMode	= IfxPort_InputMode_pullUp;
	can_node_cfg.txPinMode 	= IfxPort_OutputMode_pushPull;
	IfxMultican_Can_Node_init(&can_src_node, &can_node_cfg);
	IfxMultican_Can_Node_init(&can_dst_node, &can_node_cfg);


	/* Source MO, TX */
	{
		IfxMultican_Can_MsgObjConfig can_src_mo_cfg;
		/* Default settings */
		IfxMultican_Can_MsgObj_initConfig(&can_src_mo_cfg, &can_src_node);
		can_src_mo_cfg.msgObjId = 0;
		can_src_mo_cfg.frame 	= IfxMultican_Frame_transmit;
		IfxMultican_Can_MsgObj_init(&can_src_mo, &can_src_mo_cfg);
	}

	/* Destination MO, RX */
	{
		IfxMultican_Can_MsgObjConfig can_dst_mo_cfg;
		/* Default settings */
		IfxMultican_Can_MsgObj_initConfig(&can_dst_mo_cfg, &can_dst_node);
		can_dst_mo_cfg.msgObjId = 1;
		can_dst_mo_cfg.frame 	= IfxMultican_Frame_receive;
		IfxMultican_Can_MsgObj_init(&can_dst_mo, &can_dst_mo_cfg);
	}
}


uint32_t swap_endianness(uint32_t value) {
	return ((value & ((uint32_t)0xFF << 0))  << 24) |
		   ((value & ((uint32_t)0xFF << 8))  << 8)  |
		   ((value & ((uint32_t)0xFF << 16)) >> 8)  |
		   ((value & ((uint32_t)0xFF << 24)) >> 24);
}


void can_tx(uint32_t id, uint64_t data) {
	uint32_t data_low  = (uint32_t)data;
	data_low = swap_endianness(data_low);

	uint32_t data_high = (uint32_t)(data >> 32);
	data_high = swap_endianness(data_high);

	IfxMultican_Message tx_msg;
	IfxMultican_Message_init(&tx_msg, id, data_low, data_high, IfxMultican_DataLengthCode_8);

	IfxMultican_Can_MsgObj_sendMessage(&can_src_mo, &tx_msg);
	while( IfxMultican_Can_MsgObj_sendMessage(&can_src_mo, &tx_msg) == IfxMultican_Status_notSentBusy );
}


IfxMultican_Status can_test_rx(void) {
	IfxMultican_Message rx_msg;

	/* Initialize the message structure with dummy values, will be replaced by the received values */
	IfxMultican_Message_init(&rx_msg, 0xBEEF, 0xBA5EBA11, 0xC0CAC01A, IfxMultican_DataLengthCode_8);

	/* Wait until MultiCAN received a new message */
	while( IfxMultican_Can_MsgObj_isRxPending(&can_dst_mo) == FALSE);

	return IfxMultican_Can_MsgObj_readMessage(&can_dst_mo, &rx_msg);
}

