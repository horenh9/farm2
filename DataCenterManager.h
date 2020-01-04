#ifndef DATACENTERMANAGER_DATACENTERMANAGER_H
#define DATACENTERMANAGER_DATACENTERMANAGER_H

#include <stdint.h>
#include "AVLtree.h"
#include "UpTree.h"
#include "HashTable.h"

class DataCenter;

class Server {
public:
    const int id;
    int traffic;
    DataCenter *home;

    Server(const int id, int traffic, DataCenter *home);

    bool operator>(const Server &) const;

    bool operator<(const Server &) const;

};

class DataCenter {
public:
    int id;
    int servers;
    AVLtree<Server *, int> servers_by_traffic;
    DataCenter *root;

    explicit DataCenter(int id);
};

class DataCenterManager {
public:
    UpTree<DataCenter> farms;
    DynamicHashTable<Server*> *hush_Servers;
    AVLtree<Server *, int> all_servers_by_traffic;
    int servers;
    int size;

    explicit DataCenterManager(int n);

    StatusType MergeDataCenters(int dataCenter1, int dataCenter2);

    StatusType AddServer(int dataCenterID, int serverID);

    StatusType RemoveServer(int serverID);

    StatusType SetTraffic(int serverID, int traffic);

    StatusType SumHighestTrafficServers(int dataCenterID, int k, int *traffic);

    virtual ~DataCenterManager();



    void SetServerArray(Server **pServer, DataCenter *pCenter);
};


#endif //DATACENTERMANAGER_DATACENTERMANAGER_H
