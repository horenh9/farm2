#ifndef DATACENTERMANAGER_DATACENTERMANAGER_H
#define DATACENTERMANAGER_DATACENTERMANAGER_H

#include "AVLtree.h"
#include "UpTree.h"

class DataCenter;

class Server {
public:
    const int id;
    int traffic;
    DataCenter *home;
};

class DataCenter {
public:
    int id;
    int servers;
    AVLtree<int, Server*> servers_by_traffic;
    DataCenter *root;
};


class DataCenterManager {
public:
    UpTree farms;
    Hush_Table *hush_Servers;
    AVLtree<int, Server*> all_servers_by_traffic;
    int servers;


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
