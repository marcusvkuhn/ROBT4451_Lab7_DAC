/*
 * waveformGenerator.c
 *
 *  Created on: Apr 14, 2021
 *      Author: Marcus
 */

#include <msp430.h>

#include "waveformGenerator.h"
#include "mcp4921Dac.h"


void triangularWaveDac(void){
    unsigned int dacCtrl = (~DAC_CFG_WR & ~DAC_CFG_BUF & (DAC_CFG_GA + DAC_CFG_SHDN)) & 0xF000;

    // ALL CODES VERSION
    unsigned int i;

    while(1){
        for (i = 0; i < 4096; ++i) {
            dacWriteWord(i, dacCtrl);
        }
        for (i = 4095; i != 0; --i) {
            dacWriteWord(i, dacCtrl);
        }
    }
}

int triWave(double amp, double freq, int nSamples){

    unsigned int dacCtrl = (~DAC_CFG_WR & ~DAC_CFG_BUF & (DAC_CFG_GA + DAC_CFG_SHDN)) & 0xF000;
    volatile int validTriWave = 1;
    unsigned int byteAmp;
    unsigned int i;
    volatile double timerSample;

    if (amp > 2.5)
        validTriWave = 0;

    byteAmp = (amp * 4096)/V_REF;

    timerSample = 1/(nSamples*freq);

    timerSample = timerSample / SMCLK_T;

    TA0CCR0 =  (int)timerSample + 1;
    TA0CTL |= TACLR | TAIE;

    while(1){
        for (i = 0; i < byteAmp; ++i) {
            if (sampleT){
                dacWriteWord(i, dacCtrl);
                sampleT = 0;
            }
        }
        for (i = byteAmp; i != 0; --i) {
            if (sampleT){
                dacWriteWord(i, dacCtrl);
                sampleT = 0;
            }
        }
    }
}
