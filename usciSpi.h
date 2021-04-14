/*
 * usciSpi.h
 *
 *  Created on: Feb 19, 2017
 *      Author: Greg Scutt
 */

#ifndef USCISPI_H_
#define USCISPI_H_


// extern allows the usciSpi.c to be compiled on its own.
// Let's linker know that there is a global variable rxFlag "defined" externally in the .c file.
extern unsigned char rxFlag;

void usciA0SpiInit(unsigned int sclkDiv);
void usciA0SpiClkDiv(unsigned int sclkDiv);
void usciB1SpiInit(unsigned int sclkDiv);
void usciB1SpiClkDiv(unsigned int sclkDiv);

#endif /* USCISPI_H_ */
