#include "Clocks_and_Modes.h"

static uint32_t SOSCDIV1, SIRCDIV1, FIRCDIV1, SPLLDIV1;
uint32_t SOSCDIV2, SIRCDIV2, FIRCDIV2, SPLLDIV2;

static uint32_t V_CO, SPLL_CLK;
static uint32_t DIV_VALUE[8] = {0, 1, 2, 4, 8, 16, 32, 64};

/*******************************************************************************
 * @fn              - SOSC_Init
 *
 * @brief           - This function initializes the System Oscillator (SOSC)
 *
 * @param[in]       - SOSC_Config_t SOSC_Handle: Configuration structure for SOSC
 * @param[in]       - uint8_t DIV: Divider selection (DIV1 or DIV2)
 *
 * @return          - None
 *
 * @Note            - This function configures and enables the SOSC
 */
void SOSC_Init(SOSC_Config_t SOSC_Handle, uint8_t DIV)
{
	uint32_t temp = 0;

	// Configure SOSC divider based on DIV parameter
	if (DIV == DIV1)
	{
		temp |= ((uint32_t)SOSC_Handle.SOSC_DIV1 << SOSCDIV_DIV1);
	}
	else
	{
		temp |= ((uint32_t)SOSC_Handle.SOSC_DIV2 << SOSCDIV_DIV2);
	}
	SCG->SOSCDIV = temp; // Set SOSC divider

	temp = 0;
	// Configure SOSC range and external/crystal selection
	temp |= ((uint32_t)SOSC_Handle.SOSC_Range << SOSCCFG_RANGE);
	temp |= ((uint32_t)SOSC_Handle.SOSC_ExOrX << SOSCCFG_EREFS);
	SCG->SOSCCFG = temp; // Set SOSC configuration

	// Calculate divider values for internal oscillator
	if (SOSC_Handle.SOSC_ExOrX == SOSC_INTERNAL)
	{
		SOSCDIV1 = SOSC_8M / DIV_VALUE[SOSC_Handle.SOSC_DIV1];
		SOSCDIV2 = SOSC_8M / DIV_VALUE[SOSC_Handle.SOSC_DIV2];
	}

	// Wait for SOSC to unlock
	while (SCG->SOSCCSR & SCG_SOSCCSR_LK_MASK)
		;

	SCG_SOSC_EN(); // Enable SOSC

	// Wait for SOSC to become valid
	while (!(SCG->SOSCCSR & SCG_SOSCCSR_SOSCVLD_MASK))
		;
}
/*******************************************************************************
 * @fn              - SIRC_Init
 *
 * @brief           - This function initializes the Slow Internal Reference Clock (SIRC)
 *
 * @param[in]       - SIRC_Config_t SIRC_Handle: Configuration structure for SIRC
 * @param[in]       - uint8_t DIV: Divider selection (DIV1 or DIV2)
 *
 * @return          - None
 *
 * @Note            - This function configures and enables the SIRC
 */
void SIRC_Init(SIRC_Config_t SIRC_Handle, uint8_t DIV)
{
	uint32_t temp = 0;
	SCG_SIRC_DI(); // Disable SIRC before configuration

	// Configure SIRC divider based on DIV parameter
	if (DIV == DIV1)
	{
		temp |= ((uint32_t)SIRC_Handle.SIRC_DIV1 << SIRCDIV_DIV1);
	}
	else
	{
		temp |= ((uint32_t)SIRC_Handle.SIRC_DIV2 << SIRCDIV_DIV2);
	}
	SCG->SIRCDIV = temp; // Set SIRC divider

	// Configure SIRC range
	SCG->SIRCCFG = ((uint32_t)SIRC_Handle.SIRC_Range << SIRCCFG_RANGE);

	// Calculate divider values based on SIRC range
	if (SIRC_Handle.SIRC_Range == SIRC_RANGE_2M)
	{
		SIRCDIV1 = SIRC_2M / DIV_VALUE[SIRC_Handle.SIRC_DIV1]; // Calculate SIRCDIV1 for 2MHz range
		SIRCDIV2 = SIRC_2M / DIV_VALUE[SIRC_Handle.SIRC_DIV2]; // Calculate SIRCDIV2 for 2MHz range
	}
	else
	{
		SIRCDIV1 = SIRC_8M / DIV_VALUE[SIRC_Handle.SIRC_DIV1]; // Calculate SIRCDIV1 for 8MHz range
		SIRCDIV2 = SIRC_8M / DIV_VALUE[SIRC_Handle.SIRC_DIV2]; // Calculate SIRCDIV2 for 8MHz range
	}

	// Wait for SIRC to unlock
	while (SCG->SIRCCSR & SCG_SIRCCSR_LK_MASK)
		;

	SCG_SIRC_EN(); // Enable SIRC
}

/*******************************************************************************
 * @fn              - FIRC_Init
 *
 * @brief           - This function initializes the Fast Internal Reference Clock (FIRC)
 *
 * @param[in]       - FIRC_Config_t FIRC_Handle: Configuration structure for FIRC
 * @param[in]       - uint8_t DIV: Divider selection (DIV1 or DIV2)
 *
 * @return          - None
 *
 * @Note            - This function configures and enables the FIRC
 */
void FIRC_Init(FIRC_Config_t FIRC_Handle, uint8_t DIV)
{
	uint32_t temp = 0;

	SCG_FIRC_DI(); // Disable FIRC before configuration

	// Configure FIRC divider based on DIV parameter
	if (DIV == DIV1)
	{
		temp |= ((uint32_t)FIRC_Handle.FIRC_DIV1 << FIRCDIV_DIV1);
	}
	else
	{
		temp |= ((uint32_t)FIRC_Handle.FIRC_DIV2 << FIRCDIV_DIV2);
	}
	SCG->FIRCDIV = temp; // Set FIRC divider

	// Calculate divider values
	FIRCDIV1 = FIRC_48M / DIV_VALUE[FIRC_Handle.FIRC_DIV1]; // Calculate FIRCDIV1
	FIRCDIV2 = FIRC_48M / DIV_VALUE[FIRC_Handle.FIRC_DIV2]; // Calculate FIRCDIV2

	// Wait for FIRC to unlock
	while (SCG->FIRCCSR & SCG_FIRCCSR_LK_MASK)
		; // Check if FIRC is locked, wait until it's unlocked

	SCG_FIRC_EN(); // Enable FIRC
}

/*******************************************************************************
 * @fn              - SPLL_Init
 *
 * @brief           - This function initializes and configures the System Phase-Locked Loop (SPLL)
 *
 * @param[in]       - SPLL_Config_t SPLL_Handle: Configuration structure for SPLL
 * @param[in]       - uint8_t DIV: Divider selection (DIV1 or DIV2)
 *
 * @return          - None
 *
 * @Note            - This function configures and enables the SPLL
 *                  - SPLL can be sourced from either SOSC (System Oscillator) or FIRC (Fast Internal Reference Clock)
 *                  - The function calculates VCO (Voltage Controlled Oscillator) and SPLL clock based on the source
 *                  - SPLL_DIV1 and SPLL_DIV2 are calculated for clock distribution to different peripherals
 *                  - The function waits for SPLL to unlock and become valid before returning
 */
void SPLL_Init(SPLL_Config_t SPLL_Handle, uint8_t DIV)
{
	uint32_t temp = 0;

	// Wait for SPLL to unlock
	while (SCG->SPLLCSR & SCG_SPLLCSR_LK_MASK)
		;

	SCG_SPLL_DI(); // Disable SPLL before configuration

	// Configure SPLL divider based on DIV parameter
	if (DIV == DIV1)
	{
		temp |= ((uint32_t)SPLL_Handle.SPLL_DIV1 << SPLLDIV_DIV1);
	}
	else
	{
		temp |= ((uint32_t)SPLL_Handle.SPLL_DIV2 << SPLLDIV_DIV2);
	}
	SCG->SPLLDIV = temp; // Set SPLL divider

	temp = 0;
	// Configure SPLL multiplier, pre-divider, and source
	temp |= ((uint32_t)SPLL_Handle.SPLL_MULT << SPLLCFG_MULT);
	temp |= ((uint32_t)SPLL_Handle.SPLL_PreDiv << SPLLCFG_PREDIV);
	temp |= ((uint32_t)SPLL_Handle.SPLL_Source << SPLLCFG_SOURCE);
	SCG->SPLLCFG = temp;

	// Configure and enable SPLL based on the source
	if (SPLL_Handle.SPLL_Source == SPLL_SOURCE_SOSC)
	{
		// Configure SOSC if it's the source
		temp = (1 << SOSCCFG_EREFS) | (2 << SOSCCFG_RANGE);
		SCG->SOSCCFG = temp;
		SCG_SOSC_EN(); // Enable SOSC

		// Calculate VCO, SPLL_CLK, and divider values for SOSC source
		V_CO = ((SPLL_SOSC_8M / (1 + SPLL_Handle.SPLL_PreDiv)) * (SPLL_Handle.SPLL_MULT + 16));
		SPLL_CLK = (V_CO / 2);
		SPLLDIV1 = SPLL_CLK / DIV_VALUE[SPLL_Handle.SPLL_DIV1];
		SPLLDIV2 = SPLL_CLK / DIV_VALUE[SPLL_Handle.SPLL_DIV2];
	}
	else if (SPLL_Handle.SPLL_Source == SPLL_SOURCE_FIRC)
	{
		// Calculate VCO, SPLL_CLK, and divider values for FIRC source
		V_CO = ((SPLL_FIRC_48M / (1 + SPLL_Handle.SPLL_PreDiv)) * (SPLL_Handle.SPLL_MULT + 16));
		SPLL_CLK = (V_CO / 2);
		SPLLDIV1 = SPLL_CLK / DIV_VALUE[SPLL_Handle.SPLL_DIV1];
		SPLLDIV2 = SPLL_CLK / DIV_VALUE[SPLL_Handle.SPLL_DIV2];
	}

	// Wait for SPLL to unlock again
	while (SCG->SPLLCSR & SCG_SPLLCSR_LK_MASK)
		;

	SCG_SPLL_EN(); // Enable SPLL

	// Wait for SPLL to become valid
	while (!(SCG->SPLLCSR & SCG_SPLLCSR_SPLLVLD_MASK))
		;
}
/*******************************************************************************
 * @fn				- SCG_Init
 *
 * @brief			- This function initializes the System Clock Generator (SCG) based on the provided source clock configuration
 *
 * @param[in]		- Source_Handle: Configuration structure for the desired clock source
 *
 * @return			- None
 *
 * @Note			- This function selects and initializes one of the four possible clock sources: SOSC, SIRC, FIRC, or SPLL
 */
void SCG_Init(SourceClock_Config_t Source_Handle)
{
	// Check the selected clock source and initialize accordingly
	if (Source_Handle.Soure == Source_SOSC)
	{
		// Initialize System Oscillator (SOSC)
		SOSC_Init(Source_Handle.SOSC_Config, Source_Handle.DIV);
	}
	else if (Source_Handle.Soure == Source_SIRC)
	{
		// Initialize Slow Internal Reference Clock (SIRC)
		SIRC_Init(Source_Handle.SIRC_Config, Source_Handle.DIV);
	}
	else if (Source_Handle.Soure == Source_FIRC)
	{
		// Initialize Fast Internal Reference Clock (FIRC)
		FIRC_Init(Source_Handle.FIRC_Config, Source_Handle.DIV);
	}
	else if (Source_Handle.Soure == Source_SPLL)
	{
		// Initialize System PLL (SPLL)
		SPLL_Init(Source_Handle.SPLL_Config, Source_Handle.DIV);
	}
	// Note: If an invalid source is provided, no action is taken
}

void RunNormalMode_80MHz(void)
{
	/* Run mode */
	SCG->RCCR = ((SCS_PLL << RCCR_SCS) | (1 << RCCR_DIVCORE) | (1 << RCCR_DIVBUS) | (2 << RCCR_DIVSLOW));
	while (((SCG->CSR & 0xF000000) >> 24) != 6)
		;
}
