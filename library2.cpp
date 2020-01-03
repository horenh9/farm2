#include <iostream>
#include "DataCenterManager.h"

void *Init(int n) {
    try {
        DataCenterManager *DS = new DataCenterManager(n);
        return (void *) DS;
    }
    catch (...) {
        return NULL;
    }

}

StatusType MergeDataCenters(void *DS, int dataCenter1, int dataCenter2) {
    try {
        return ((DataCenterManager *) DS)->MergeDataCenters(dataCenter1, dataCenter2);
    }
    catch (...) {
        return ALLOCATION_ERROR;
    }
}

StatusType AddServer(void *DS, int dataCenterID, int serverID) {
    try {
        return ((DataCenterManager *) DS)->AddServer(dataCenterID, serverID);
    }
    catch (...) {
        return ALLOCATION_ERROR;
    }
}

StatusType RemoveServer(void *DS, int serverID){
    try {
        return ((DataCenterManager *) DS)->RemoveServer(serverID);
    }
    catch (...) {
        return ALLOCATION_ERROR;
    }
}

StatusType SetTraffic(void *DS, int serverID,  int traffic){
    try {
        return ((DataCenterManager *) DS)->SetTraffic(serverID, traffic);
    }
    catch (...) {
        return ALLOCATION_ERROR;
    }
}

StatusType SumHighestTrafficServers(void *DS, int dataCenterID,  int k, int *traffic){
    try {
        return ((DataCenterManager *) DS)->SumHighestTrafficServers(dataCenterID, k, traffic);
    }
    catch (...) {
        return ALLOCATION_ERROR;
    }
}

void Quit(void **DS) {
    try {
        delete *reinterpret_cast<DataCenterManager **>(DS);
        *DS = nullptr;
    }
    catch (...) {}
}
