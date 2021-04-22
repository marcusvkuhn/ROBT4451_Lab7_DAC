/*
 * waveformGenerator.c
 *
 *  Created on: Apr 14, 2021
 *      Author: Marcus
 */

#include <msp430.h>
#include <math.h>

#include "waveformGenerator.h"
#include "mcp4921Dac.h"


void triangularWaveDac(void){
    unsigned int dacCtrl = (~DAC_CFG_WR & ~DAC_CFG_BUF & (DAC_CFG_GA + DAC_CFG_SHDN)) & 0xF000;

    // ALL CODES VERSION
    unsigned int i;

    while(1){
        for (i = 0; i < 4096; ++i)
            dacWriteWord(i, dacCtrl);
        for (i = 4095; i != 0; --i)
            dacWriteWord(i, dacCtrl);
    }
}

int triWave(double amp, double freq, int nSamples){

    unsigned int dacCtrl = (~DAC_CFG_WR & ~DAC_CFG_BUF & (DAC_CFG_GA + DAC_CFG_SHDN)) & 0xF000;
    volatile int validTriWave = 1;
    unsigned int i, j;
    volatile double timerSample,
                    nSamplesT,
                    deltaAmp;
    //volatile double nSampleVoltage[150] = {};
    volatile unsigned int nSampleByte[150] = {};

    if (amp > 2.5)
        validTriWave = 0;

    nSamplesT = (1/(freq*nSamples));

    deltaAmp = (amp/nSamples)*2;

    timerSample = nSamplesT / SMCLK_T;

    TA0CCR0 =  (int)timerSample + 1;

    for (i = 0; i <= nSamples/2; ++i){
        //nSampleVoltage[i] = deltaAmp * i;
        nSampleByte[i] = (deltaAmp * i * 4096)/V_REF;
    }
    for (i, j = 1; i < nSamples; ++i, ++j) {
        //nSampleVoltage[i] = amp - (deltaAmp * j);
        nSampleByte[i] = ((amp - (deltaAmp * j)) * 4096)/V_REF;
    }

    i = 0 ;

    TA0CTL |= TACLR | TAIE;

    while(1){
        if (i > nSamples)
            i = 0;
        if (sampleT){
            dacWriteWord(nSampleByte[i++], dacCtrl);
            sampleT = 0;
        }
    }

    return validTriWave;
}
int sinWave(double amp, double freq, int nSamples){

    unsigned int dacCtrl = (~DAC_CFG_WR & ~DAC_CFG_BUF & (DAC_CFG_GA + DAC_CFG_SHDN)) & 0xF000;
    volatile int validSinWave = 1;
    //unsigned int byteAmp;
    unsigned int i;
    volatile double timerSample,
                    nSamplesT,
                    deltaAmp;
    volatile double nSampleVoltage[150] = {};
    volatile unsigned int nSampleByte[150] = {};

    if (amp > 2.5)
        validSinWave = 0;

    nSamplesT = (1/(freq*nSamples));

    deltaAmp = (amp/nSamples)*2;

    timerSample = nSamplesT / SMCLK_T;

    TA0CCR0 =  (int)timerSample + 1;

    for (i = 0; i < nSamples; ++i){
        nSampleVoltage[i] = amp*sin(2*PI*freq*nSamplesT);
        nSampleByte[i] = (nSampleVoltage[i] * 4096)/V_REF;
    }

    i = 0 ;

    TA0CTL |= TACLR | TAIE;

    while(1){
        if (i > nSamples)
            i = 0;
        if (sampleT){
            dacWriteWord(nSampleByte[i++], dacCtrl);
            sampleT = 0;
        }
    }

    return validSinWave;
}
