/*
 * cmdVNH7070.h
 *
 *  Created on: Feb 23, 2021
 *      Author: Raymond
 */

#ifndef CMDDAC_H_
#define CMDDAC_H_

#define     MAX_CMDS             3

#define     CMD0               "DACSET"
#define     CMD0_NARGS         1
#define     DACSET_IDX         0

#define     CMD1               "TRIWAVE"
#define     CMD1_NARGS         3
#define     TRIWAVE_IDX        1

#define     CMD2               "SINWAVE"
#define     CMD2_NARGS         3
#define     SINWAVE_IDX        2

#define     DELIM              " ,\t"
#define     NULL               '\0'          // null char
#define     DAC_ARG            dacCmds[cmdIndex].args
#define     MAX_ARGS           3

#define     QUIT_IDX           6

#define     VREF               2.5
#define     N_BITS             12

typedef struct CMD {
    const char *name; // command name
    int nArgs; // number of input arguments for a command
    double args[MAX_ARGS]; // arguments
}CMD;

//-------------- func prototypes-------------

void initDacCmds(CMD* vnh7070Cmds);
int parseCmd(CMD* vnh7070Cmds, char* cmdLine);
int validateCmd(CMD* scaraCmds, char* cmdName);
int executeCMD(CMD* vnh7070Cmds, int cmdIndex);

#endif /* CMDDAC_H_ */
