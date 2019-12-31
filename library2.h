/****************************************************************************/
/*                                                                          */
/* This file contains the interface functions                               */
/* you should use for the wet ex 2                                          */
/*                                                                          */
/****************************************************************************/

/****************************************************************************/
/*                                                                          */
/* File Name : library2.h                                                   */
/*                                                                          */
/****************************************************************************/

#ifndef _234218_WET2
#define _234218_WET2

#ifdef __cplusplus
extern "C" {
#endif

/* Return Values
 * ----------------------------------- */
typedef enum {
    SUCCESS = 0,
    FAILURE = -1,
    ALLOCATION_ERROR = -2,
    INVALID_INPUT = -3
} StatusType;


void* Init(int n);

StatusType MergeDataCenters(void *DS, int dataCenter1, int dataCenter2);

StatusType AddServer(void *DS, int dataCenterID, int serverID);

StatusType RemoveServer(void *DS, int serverID);

StatusType SetTraffic(void *DS, int serverID,  int traffic);

StatusType SumHighestTrafficServers(void *DS, int dataCenterID,  int k, int *traffic);

void Quit(void** DS);

#ifdef __cplusplus
}
#endif

#endif    /*_234218_WET1 */