#include <cmdDac.h>
#include <msp430.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <mcp4921Dac.h>
#include <ctype.h>

/************************************************************************************
* Function: initDacCmds
* Purpose: initialize each index of the array with its correspondent string and nArgs
* arguments:
*   dacCmds   -   CMD
* return:  none
* Author: Marcus Kuhn
* Date: 22/04/2020
* Modified: 22/04/2020
*************************************************************************************/
void initDacCmds(CMD* dacCmds){
    dacCmds[0].name = CMD0;        // initialize the first command name
    dacCmds[0].nArgs = CMD0_NARGS; // initialize num of arguments in first command
    dacCmds[1].name = CMD1;
    dacCmds[1].nArgs = CMD1_NARGS;
    dacCmds[2].name = CMD2;
    dacCmds[2].nArgs = CMD2_NARGS;
}

/************************************************************************************
* Function: parseCmd
* Purpose: splits strings into separate tokens for specific purposes
* arguments:
*   dacCmds   -   CMD
*   cmdLine   -   char*
* return:  if command was valid
* Author: Marcus Kuhn
* Date: 22/04/2020
* Modified: 22/04/2020
*************************************************************************************/
int parseCmd(CMD* dacCmds, char* cmdLine){
    char* token;  // a pointer variable to a string.
    int i = 0;

    // token is assigned the pointer to the token returned by strtok_s
    token = strtok(cmdLine, DELIM);

    int cmdIndex = validateCmd(dacCmds, token);     // finds which index to use for subsequent parsing

//    if (cmdIndex != -1 && dacCmds[cmdIndex].nArgs == 0){
//        token = strtok(NULL, DELIM);
//        if (token)     // if there is any arguments, function will return -1
//            cmdIndex = -1;
//    }
    if (cmdIndex != -1 && dacCmds[cmdIndex].nArgs != 0){
        if (cmdIndex == DACSET_IDX){      // for scaraMotorSpeed, nArgs = 1
                token = strtok(NULL, DELIM);
                // checks if token exists, is of length 1 and is alphabetic
                if (token)
                    DAC_ARG[0] = atof(token);            // casts as integer the value (of a char) token is pointing to
                else
                    cmdIndex = -1;      //
                //if(!token)                 // if token is NULL, command is invalid, function will return -1
                //    cmdIndex = -1;
                token = strtok(NULL, DELIM);
                if (token)     // if there is more than one argument, function will also return -1
                    cmdIndex = -1;
        }
        if (cmdIndex == TRIWAVE_IDX || cmdIndex == SINWAVE_IDX){      // for nokLcdDrawScrnLine, nArgs = 2, for nokLcdDrawLine, nArgs = 4
            for (i = 0; token && i < dacCmds[cmdIndex].nArgs; ++i){      // loops while token is valid and i is less than the nArgs for the command, same for every following for-loop
                token = strtok(NULL, DELIM);
                if (i == CMD2_NARGS - 1){
                    if (token)                                              // every following token here will be a double
                        DAC_ARG[i] = atoi(token);            // converts to int, then stores into the array
                    else if (!token)                // token invalid in the range of nArgs
                        cmdIndex = -1;
                }
                else{
                    if (token)                                              // every following token here will be a double
                        DAC_ARG[i] = atof(token);            // converts to int, then stores into the array
                    else if (!token)                // token invalid in the range of nArgs
                        cmdIndex = -1;
                }
            }
            if (token = strtok(NULL, DELIM))     // looks for more than nArgs arguments, which will also make the function return -1
                cmdIndex = -1;
        }
    }
    return cmdIndex;
}

/************************************************************************************
* Function: validadeCmd
* Purpose: uses cmdIndex to execute a scaraCmds[cmdIndex] Scara command.
* It will perform data type casting (double -> int or char) when necessary.
* arguments:
*   scaraCmds   -   CMD*
*   cmdName     -   char*
* return:  if command was valid
* Author: Marcus Kuhn
* Date: 22/04/2020
* Modified: 22/04/2020
*************************************************************************************/
int validateCmd(CMD* dacCmds, char* cmdName){
    unsigned int i = 0;
    int idx = -1;
    int invalidCmd = 1;
    while (invalidCmd && i < MAX_CMDS)
        invalidCmd = strcmp(cmdName, dacCmds[i++].name);
    if (!invalidCmd)
        idx = i - 1;
    return idx;
}

/************************************************************************************
* Function: executeCMD
* Purpose: uses cmdIndex to execute a scaraCmds[cmdIndex] Scara command.
* It will perform data type casting (double -> int or char) when necessary.
* arguments:
*   scaraCmds   -   CMD*
*   cmdIndex   -   int
*   scaraState   -   SCARA_ROBOT*
* return:  if command was valid
* Author: Marcus Kuhn
* Date: 22/04/2020
* Modified: 22/04/2020
*************************************************************************************/
int executeCMD(CMD* dacCmds, int cmdIndex){
    switch (cmdIndex) {
    case DACSET_IDX:
        dacSet(DAC_ARG[0], VREF, N_BITS);
    case TRIWAVE_IDX:
        triWave(DAC_ARG[0], DAC_ARG[1], DAC_ARG[2]);
    case SINWAVE_IDX:
        sinWave(DAC_ARG[0], DAC_ARG[1], DAC_ARG[2]);
    default:
        break;
    }
    return cmdIndex;
}
