/*************************************************************************************************
 * ROBT4451Lab7.c
 * - top level file for Lab 7
 * Demonstrates use of mcp4921Dac.c. A 12 bit DAC with SPI digital interface to uC
 * Assumes MSP430 is interfaced to mcp4921 through SPI with UCSIA0.
 * dcoFreq and sclkDiv can be set by user
 *
 *
 *  Author: Chris Baitz Greg Scutt
 *  Created on: April, 2017
 **************************************************************************************************/



#include <msp430.h> 
#include <usciSpi.h>
#include "ucsControl.h"
#include "mcp4921Dac.h"
#include "cmdDac.h"
#include "usciUart.h"

#define dcoFreq 20							//MHz.
#define sclkDiv 1							//SPI sclk divide. SCLK MAX to the DAC is 20MHz.
											// sclkDiv will slow down transfer rate to DAC

#define DAC_TST_WORD 0x800

/*
 * main.c
 */

unsigned char rxFlag = 0;
int main(void) {

	//unsigned int *address = (unsigned int*)0x2400;
	unsigned char oscFail = 1;
	//unsigned int dacWord = 0;
//	unsigned int dacCtrl = (~DAC_CFG_WR & ~DAC_CFG_BUF & (DAC_CFG_GA + DAC_CFG_SHDN)) & 0xF000;
								// count direction
    WDTCTL = WDTPW | WDTHOLD;					// Stop watchdog timer

    /*********Set clock frequency*********************************************/
    //unsigned char testPass = 1;
    ucsSelSource(1,1,1,1);
    oscFail = ucsDcoFreqSet (dcoFreq, 2, 1);			//set sclk to dcoFreq
    if (oscFail)
    	return 1;
    /***End***Set clock frequency*********************************************/

    usciA0SpiInit(sclkDiv);
    usciA1UartInit();

    // set-up I/O ports
    P3SEL |= BIT3;             				// SPI A0SIMO P3.3
    P2SEL |= BIT7;             				// SPI A0CLK P2.7
    P4DIR |= CS+LDAC;						// CS 4.0 and LDAC 4.2

    // de-assert serial control signals
    LDAC_B_DEASSERT;
    CS_B_DEASSERT;

    // clear past RXIFG and enable RXIFG interrupt
    UCA0IFG &= ~UCRXIFG;					// clear  RXIFG interrupt flag
    UCA0IE |= (UCRXIE);						// enable RXIFG interrupt

    __enable_interrupt();      				// enable global device interrupts

    CMD dacCmds[MAX_CMDS]; //this is an array of vnh7070Cmds of type CMD
    initDacCmds(dacCmds);

    char rxString[50];

    // command
    int cmdIndex;
        do{
            usciA1UartGets(rxString);
            cmdIndex = parseCmd(dacCmds, rxString);
            if (cmdIndex != -1){
                if (cmdIndex != QUIT_IDX){
                    //displayParsing(scaraCmds, cmdIndex);
                    executeCMD(dacCmds, cmdIndex);
                }
            }
            else
                usciA1UartTxString("Invalid command.\n");
        } while (cmdIndex != QUIT_IDX);


//    while(1){
//    	dacWriteWord(DAC_TST_WORD, dacCtrl);		// just write 0x800 to DAC. over and over and over again ...
//    }

    __disable_interrupt();					// disable global interrupts
	return 0;
}
