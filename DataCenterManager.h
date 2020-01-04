#ifndef DATACENTERMANAGER_DATACENTERMANAGER_H
#define DATACENTERMANAGER_DATACENTERMANAGER_H

#include <stdint.h>
#include "AVLtree.h"
#include "UpTree.h"
#include "HashTable.h"

class DataCenter;

class Server {
public:
    int id;
    int traffic;
    DataCenter *home;

    Server(int id, int traffic, DataCenter *home);

    bool operator>(const Server *) const;

    bool operator<(const Server *) const;

    Server &operator=(const Server &);

};

class DataCenter {
public:
    int id;
    int servers;
    AVLtree<Server, int> *servers_by_traffic;
    DataCenter *root;

    explicit DataCenter(int id);
};

class DataCenterManager {
public:
    UpTree<DataCenter> *farms;
    DynamicHashTable<Server> *hash_Servers;
    AVLtree<Server, int> *all_servers_by_traffic;
    int servers;
    int num_farms;

    explicit DataCenterManager(int n);

    StatusType MergeDataCenters(int dataCenter1, int dataCenter2);

    StatusType AddServer(int dataCenterID, int serverID);

    StatusType RemoveServer(int serverID);

    StatusType SetTraffic(int serverID, int traffic);

    StatusType SumHighestTrafficServers(int dataCenterID, int k, int *traffic);

    ~DataCenterManager();
};


#endif //DATACENTERMANAGER_DATACENTERMANAGER_H
