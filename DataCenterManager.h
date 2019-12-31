#ifndef DATACENTERMANAGER_DATACENTERMANAGER_H
#define DATACENTERMANAGER_DATACENTERMANAGER_H

#include "AVLtree.h"
class DataCenter {
public:
    int id;
    int size;
    AVLtree<int, int> servers_by_traffic;
};

class Server {
public:
    const int id;
    int traffic;
    DataCenter *home;
};

class DataCenterManager {
public:
    DataCenter *farms;
    Server *hush_Servers;

    void *Init();

    StatusType MergeDataCenters(void *DS, int dataCenter1, int dataCenter2);

    StatusType AddServer(void *DS, int dataCenterID, int serverID);

    StatusType RemoveServer(void *DS, int serverID);

    StatusType SetTraffic(void *DS, int serverID, int traffic);

    StatusType SumHighestTrafficServers(void *DS, int dataCenterID, int k, int *traffic);

    void Quit(void **DS);

    DataCenterManager();

    ~DataCenterManager();
};


#endif //DATACENTERMANAGER_DATACENTERMANAGER_H
