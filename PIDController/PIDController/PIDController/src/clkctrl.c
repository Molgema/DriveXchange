/**
 * \file
 *
 * \brief CLKCTRL related functionality implementation.
 *
 (c) 2020 Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms,you may use this software and
    any derivatives exclusively with Microchip products.It is your responsibility
    to comply with third party license terms applicable to your use of third party
    software (including open source software) that may accompany Microchip software.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 */

/**
 * \defgroup doc_driver_system_clkctrl Clock Controller
 * \ingroup doc_driver_system
 *
 * \section doc_driver_clockctrl_rev Revision History
 * - v0.0.0.1 Initial Commit
 *
 *@{
 */
#include <clkctrl.h>
#include <ccp.h>
/**
 * \brief Initialize clkctrl interface
 *
 * \return Initialization status.
 */
int8_t CLKCTRL_init()
{

	// ccp_write_io((void*)&(CLKCTRL.XOSC32KCTRLA),CLKCTRL_CSUT_1K_gc /* 1k cycles */
	//		 | 0 << CLKCTRL_ENABLE_bp /* Enable: disabled */
	//		 | 0 << CLKCTRL_RUNSTDBY_bp /* Run standby: disabled */
	//		 | 0 << CLKCTRL_SEL_bp /* Select: disabled */
	//		 | 0 << CLKCTRL_LPMODE_bp /* Low-Power Mode: disabled */);

	// ccp_write_io((void*)&(CLKCTRL.MCLKCTRLB),CLKCTRL_PDIV_2X_gc /* 2 */
	//		 | 0 << CLKCTRL_PEN_bp /* Prescaler enable: disabled */);

	// ccp_write_io((void*)&(CLKCTRL.PLLCTRLA),0 << CLKCTRL_RUNSTDBY_bp /* Run Standby: disabled */
	//		 | CLKCTRL_MULFAC_DISABLE_gc /* 1 */
	//		 | 0 << CLKCTRL_SOURCE_bp /* Select Source for PLL: disabled */);

	// ccp_write_io((void*)&(CLKCTRL.MCLKCTRLC),CLKCTRL_CFDSRC_CLKMAIN_gc /* Main Clock */
	//		 | 0 << CLKCTRL_CFDEN_bp /* Setting: disabled */);

	// ccp_write_io((void*)&(CLKCTRL.MCLKINTCTRL),CLKCTRL_INTTYPE_INT_gc /* Regular Interrupt */
	//		 | 0 << CLKCTRL_CFD_bp /* Clock Failure Detection Interrupt Enable: disabled */);

	// ccp_write_io((void*)&(CLKCTRL.XOSCHFCTRLA),0 << CLKCTRL_RUNSTDBY_bp /* Run standby: disabled */
	//		 | CLKCTRL_CSUTHF_256_gc /* 256 XOSCHF cycles */
	//		 | CLKCTRL_FRQRANGE_8M_gc /* Max 8 MHz XTAL Frequency */
	//		 | 0 << CLKCTRL_SEL_bp /* Source Select: disabled */
	//		 | 0 << CLKCTRL_ENABLE_bp /* External high-frequency Oscillator: disabled */);

	ccp_write_io((void *)&(CLKCTRL.OSCHFCTRLA),
	             CLKCTRL_FREQSEL_16M_gc         /* 16 */
	                 | 0 << CLKCTRL_AUTOTUNE_bp /* Auto-Tune enable: disabled */
	                 | 0 << CLKCTRL_RUNSTDBY_bp /* Run standby: disabled */);

	// ccp_write_io((void*)&(CLKCTRL.MCLKCTRLA),CLKCTRL_CLKSEL_OSCHF_gc /* Internal high-frequency oscillator */
	//		 | 0 << CLKCTRL_CLKOUT_bp /* System clock out: disabled */);

	return 0;
}
