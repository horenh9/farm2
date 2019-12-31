/***************************************************************************/
/*                                                                         */
/* 234218 Data DSs 1, Winter 2019-2020                                     */
/* Homework : Wet 1                                                        */
/*                                                                         */
/***************************************************************************/

/***************************************************************************/
/*                                                                         */
/* File Name : main1.cpp                                                   */
/*                                                                         */
/* Holds the "int main()" function and the parser of the shell's           */
/* command line.                                                           */
/***************************************************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library1.h"

#ifdef __cplusplus
extern "C" {
#endif

/* The command's strings */
typedef enum {
    NONE_CMD = -2,
    COMMENT_CMD = -1,
    INIT_CMD = 0,
	MERGEDATACENTERS_CMD = 1,
	ADDSERVER_CMD = 2,
	REMOVESERVER_CMD = 3,
	SETTRAFFIC_CMD = 4,
	SUMHIGHEST_CMD = 5,
    QUIT_CMD = 6
} commandType;

static const int numActions = 9;
static const char *commandStr[] = {
        "Init",
        "MergeDataCenters",
        "AddServer",
        "RemoveServer",
        "SetTraffic",
        "SumHighestTrafficServers",
        "Quit" };

static const char* ReturnValToStr(int val) {
    switch (val) {
        case SUCCESS:
            return "SUCCESS";
        case ALLOCATION_ERROR:
            return "ALLOCATION_ERROR";
        case FAILURE:
            return "FAILURE";
        case INVALID_INPUT:
            return "INVALID_INPUT";
        default:
            return "";
    }
}

/* we assume maximum string size is not longer than 256  */
#define MAX_STRING_INPUT_SIZE (255)
#define MAX_BUFFER_SIZE       (255)

#define StrCmp(Src1,Src2) ( strncmp((Src1),(Src2),strlen(Src1)) == 0 )

typedef enum {
    error_free, error
} errorType;
static errorType parser(const char* const command);

#define ValidateRead(read_parameters,required_parameters,ErrorString,ErrorParams) \
if ( (read_parameters)!=(required_parameters) ) { printf(ErrorString, ErrorParams); return error; }

static bool isInit = false;

/***************************************************************************/
/* main                                                                    */
/***************************************************************************/

int main(int argc, const char**argv) {

    char buffer[MAX_STRING_INPUT_SIZE];

    // Reading commands
    while (fgets(buffer, MAX_STRING_INPUT_SIZE, stdin) != NULL) {
        fflush(stdout);
        if (parser(buffer) == error)
            break;
    };
    return 0;
}

/***************************************************************************/
/* Command Checker                                                         */
/***************************************************************************/

static commandType CheckCommand(const char* const command,
                                const char** const command_arg) {
    if (command == NULL || strlen(command) == 0 || StrCmp("\n", command))
        return (NONE_CMD);
    if (StrCmp("#", command)) {
        if (strlen(command) > 1)
            printf("%s", command);
        return (COMMENT_CMD);
    };
    for (int index = 0; index < numActions; index++) {
        if (StrCmp(commandStr[index], command)) {
            *command_arg = command + strlen(commandStr[index]) + 1;
            return ((commandType)index);
        };
    };
    return (NONE_CMD);
}

/***************************************************************************/
/* Commands Functions                                                      */
/***************************************************************************/

static errorType OnInit(void** DS, const char* const command);
static errorType OnMergeDataCenters(void* DS, const char* const command);
static errorType OnAddServer(void* DS, const char* const command);
static errorType OnRemoveServer(void* DS, const char* const command);
static errorType OnSetTraffic(void* DS, const char* const command);
static errorType OnSumHighestTrafficServers(void* DS, const char* const command);
static errorType OnQuit(void** DS, const char* const command);

/***************************************************************************/
/* Parser                                                                  */
/***************************************************************************/

static errorType parser(const char* const command) {
    static void *DS = NULL; /* The general data structure */
    const char* command_args = NULL;
    errorType rtn_val = error;

    commandType command_val = CheckCommand(command, &command_args);

    switch (command_val) {

        case (INIT_CMD):
            rtn_val = OnInit(&DS, command_args);
            break;
        case (MERGEDATACENTERS_CMD):
            rtn_val = OnMergeDataCenters(DS, command_args);
            break;
        case (ADDSERVER_CMD):
            rtn_val = OnAddServer(DS, command_args);
            break;
        case (REMOVESERVER_CMD):
            rtn_val = OnRemoveServer(DS, command_args);
            break;
        case (SETTRAFFIC_CMD):
            rtn_val = OnSetTraffic(DS, command_args);
            break;
        case (SUMHIGHEST_CMD):
            rtn_val = OnSumHighestTrafficServers(DS, command_args);
            break;
        case (QUIT_CMD):
            rtn_val = OnQuit(&DS, command_args);
            break;

        case (COMMENT_CMD):
            rtn_val = error_free;
            break;
        case (NONE_CMD):
            rtn_val = error;
            break;
        default:
            assert(false);
            break;
    };
    return (rtn_val);
}

static errorType OnInit(void** DS, const char* const command) {
    if (isInit) {
        printf("init was already called.\n");
        return (error_free);
    };
    isInit = true;
    int numOfDataCenters;
    ValidateRead(sscanf(command, "%d", &numOfDataCenters), 1, "%s failed.\n", commandStr[INIT_CMD]);
    *DS = Init(numOfDataCenters);

    if (*DS == NULL) {
        printf("init failed.\n");
        return error;
    };

    printf("init done.\n");
    return error_free;
}

static errorType OnMergeDataCenters(void* DS, const char* const command) {
    int dataCenter1, dataCenter2;
    ValidateRead(sscanf(command, "%d %d", &dataCenter1, &dataCenter2), 2, "%s failed.\n", commandStr[MERGEDATACENTERS_CMD]);
    StatusType res = MergeDataCenters(DS, dataCenter1, dataCenter2);

    if (res != SUCCESS) {
        printf("%s: %s\n", commandStr[MERGEDATACENTERS_CMD], ReturnValToStr(res));
        return error_free;
    }

    printf("%s: %s\n", commandStr[MERGEDATACENTERS_CMD], ReturnValToStr(res));
    return error_free;
}

static errorType OnAddServer(void* DS, const char* const command) {
    int dataCenterID,serverID;
    ValidateRead(sscanf(command, "%d %d", &dataCenterID, &serverID), 2, "%s failed.\n", commandStr[ADDSERVER_CMD]);
    StatusType res = AddServer(DS, dataCenterID, serverID);

    if (res != SUCCESS) {
        printf("%s: %s\n", commandStr[ADDSERVER_CMD], ReturnValToStr(res));
        return error_free;
    }

    printf("%s: %s\n", commandStr[ADDSERVER_CMD], ReturnValToStr(res));
    return error_free;
}

static errorType OnRemoveServer(void* DS, const char* const command) {
    int serverID;
    ValidateRead(sscanf(command, "%d", &serverID), 1, "%s failed.\n", commandStr[REMOVESERVER_CMD]);
    StatusType res = RemoveServer(DS, serverID);

    if (res != SUCCESS) {
        printf("%s: %s\n", commandStr[REMOVESERVER_CMD], ReturnValToStr(res));
        return error_free;
    }

    printf("%s: %s\n", commandStr[REMOVESERVER_CMD], ReturnValToStr(res));
    return error_free;
}

static errorType OnSetTraffic(void* DS, const char* const command) {
    int serverID, traffic;
    ValidateRead(sscanf(command, "%d %d", &serverID, &traffic), 2, "%s failed.\n", commandStr[SETTRAFFIC_CMD]);
    StatusType res = SetTraffic(DS, serverID, traffic);

    if (res != SUCCESS) {
        printf("%s: %s\n", commandStr[SETTRAFFIC_CMD], ReturnValToStr(res));
        return error_free;
    }

    printf("%s: %s\n", commandStr[SETTRAFFIC_CMD], ReturnValToStr(res));
    return error_free;
}

static errorType OnSumHighestTrafficServers(void* DS, const char* const command) {
    int dataCenterID, k, traffic;
    ValidateRead(sscanf(command, "%d %d", &dataCenterID, &k), 2, "%s failed.\n", commandStr[SUMHIGHEST_CMD]);
    StatusType res = SumHighestTrafficServers(DS, dataCenterID, k, &traffic);

    if (res != SUCCESS) {
        printf("%s: %s\n", commandStr[SUMHIGHEST_CMD], ReturnValToStr(res));
        return error_free;
    }

    printf("%s: %d\n", commandStr[SUMHIGHEST_CMD], traffic);

    return error_free;
}

static errorType OnQuit(void** DS, const char* const command) {
    Quit(DS);
    if (*DS != NULL) {
        printf("quit failed.\n");
        return error;
    };

    isInit = false;
    printf("quit done.\n");
    return error_free;
}

#ifdef __cplusplus
}
#endif