/*
 * waveformGenerator.h
 *
 *  Created on: Apr 14, 2021
 *      Author: Raymond
 */

#ifndef WAVEFORMGENERATOR_H_
#define WAVEFORMGENERATOR_H_

#define V_REF 2.50061
#define SMCLK_20_F 20000000
#define SMCLK_T 0.000000954
#define SMCLK_20_T 0.0000005
#define PI 3.14159265359

void triangularWaveDac(void);

int triWave(double amp, double freq, int nSamples);
int sinWave(double amp, double freq, int nSamples);

volatile int sampleT;

#endif /* WAVEFORMGENERATOR_H_ */
