/**
 * \file Ifx_Cfg.h
 * \brief Configuration.
 *
 * \version iLLD_Demos_1_0_0_3_0
 * \copyright Copyright (c) 2014 Infineon Technologies AG. All rights reserved.
 *
 *
 *                                 IMPORTANT NOTICE
 *
 *
 * Infineon Technologies AG (Infineon) is supplying this file for use
 * exclusively with Infineon's microcontroller products. This file can be freely
 * distributed within development tools that are supporting such microcontroller
 * products.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * INFINEON SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 * OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *
 * \defgroup IfxLld_Demo_MulticanBasic_SrcDoc_IlldConfig iLLD configuration
 * \ingroup IfxLld_Demo_MulticanBasic_SrcDoc
 */

#ifndef IFX_CFG_H
#define IFX_CFG_H


/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

/** \addtogroup IfxLld_Demo_MulticanBasic_SrcDoc_IlldConfig
 * \{ */

/*______________________________________________________________________________
** Configuration for IfxScu_cfg.h
**____________________________________________________________________________*/

/**
 * \name Frequency configuration
 * \{ */
#define IFX_CFG_SCU_XTAL_FREQUENCY (20000000)                       /**< \copydoc IFX_CFG_SCU_XTAL_FREQUENCY */
/** \} */
#define IFX_USE_SW_MANAGED_INT                /*Software Managaed Interrupt*/


#   define IFX_CFG_CPU_CSTART_ENABLE_TRICORE0        (1) /**< Cpu0 enabled by default*/


#   define IFX_CFG_CPU_CSTART_ENABLE_TRICORE1        (0) /**< Cpu1 disabled by default*/

#   define IFX_CFG_CPU_CSTART_ENABLE_TRICORE2        (0) /**< CPU2 disabled by default*/


#define __USTACK0			__USTACK
#define __ISTACK0			__ISTACK
#define __INTTAB_CPU0		__inttab
#define __TRAPTAB_CPU0		__traptab
#define __CSA0				__CSA
#define __CSA0_END			__CSA_END
#define __A0_MEM			__sdata
#define __A1_MEM			__sdata2
/** \} */

#ifndef __GNUC__
	#define __GNUC__
#endif

#endif /* IFX_CFG_H */
