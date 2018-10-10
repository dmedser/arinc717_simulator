#include "scu_clk_cfg.h"
#include <IfxScuCcu.h>

/* fPLL = 200 MHz */
void scu_clk_init(void) {
	/** \brief Default configuration for the PLL initial steps
	 */
	static const IfxScuCcu_PllStepsConfig IfxScuCcu_testPllConfigSteps[] = {
	    IFXSCU_CFG_PLL_STEPS
	};

	/** \brief Default configuration for the Clock Configuration
	 */
	IfxScuCcu_Config IfxScuCcu_testClockConfig = {
	    {
	        sizeof(IfxScuCcu_testPllConfigSteps) / sizeof(IfxScuCcu_PllStepsConfig),
	        (IfxScuCcu_PllStepsConfig *)IfxScuCcu_testPllConfigSteps,
	        IFXSCU_CFG_PLL_INITIAL_STEP,
		},
	    IFXSCU_CFG_CLK_DISTRIBUTION,
	    IFXSCU_CFG_FLASH_WAITSTATE
	};
    /* standard PLL initialisation */
	IfxScuCcu_init(&IfxScuCcu_testClockConfig);
}
