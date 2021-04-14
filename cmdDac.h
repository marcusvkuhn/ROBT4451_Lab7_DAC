/*
 * cmdVNH7070.h
 *
 *  Created on: Feb 23, 2021
 *      Author: Raymond
 */

#ifndef CMDDAC_H_
#define CMDDAC_H_

#define     MAX_CMDS             3

#define     CMD0                 "DacSet"
#define     CMD0_NARGS           1
#define     DACSET_IDX         0

#define     DELIM                " ,\t"
#define     NULL                 '\0'          // null char
#define     DAC_ARG              dacCmds[cmdIndex].args
#define     MAX_ARGS             4

#define     QUIT_IDX             6

#define     VREF                2.5
#define     N_BITS              12

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
