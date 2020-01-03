#include "DataCenterManager.h"

Server::Server(const int id, int traffic, DataCenter *home) : id(id), traffic(traffic), home(home) {}

bool Server::operator>(const Server &server) const {
    if (this->traffic > server.traffic)
        return true;
    else if (this->traffic == server.traffic)
        return this->id > server.id;
    else
        return false;
}

bool Server::operator<(const Server &server) const {
    return !(this > &server);
}

DataCenter::DataCenter(int id) : id(id), servers(0), servers_by_traffic(AVLtree<Server *, int>()), root(this) {}

DataCenterManager::DataCenterManager(int n) : size(n), servers(0), farms(UpTree<DataCenter>(n)) {
//    Hush_Table *hush_Servers = nullptr;
    AVLtree<int, Server *> all_servers_by_traffic = AVLtree<int, Server *>();
    for (int i = 0; i < n; ++i) {
        DataCenter *farm = new DataCenter(i);
        farms.parents[i]->parent = farm;//לא יודע למה יש פה נקודות
        farms.parents[i]->data = farm;
    }
}

StatusType DataCenterManager::MergeDataCenters(int dataCenter1, int dataCenter2) {
    if (dataCenter1 < 1 || dataCenter1 > size || dataCenter2 < 1 || dataCenter2 > size)
        return INVALID_INPUT;
    int root1 = farms.Find(dataCenter1);
    int root2 = farms.Find(dataCenter2);
    //  mergeAVL(farms.parents[root1]->data, farms.parents[root2]->data)
    farms.Union(root1, root2);//שוב נקודה לא יודע למה
    return SUCCESS;
}

StatusType DataCenterManager::AddServer(int dataCenterID, int serverID) {
    if (dataCenterID < 1 || dataCenterID > size)
        return INVALID_INPUT;
    //בדיקה אם קיים בטבלת הערבול
    //add server to hush table
    int root = farms.Find(dataCenterID);
    Server *server = new Server(serverID, 0, farms.parents[root]->parent);
    return SUCCESS;
}

StatusType DataCenterManager::RemoveServer(int serverID) {
    if (serverID < 0)
        return INVALID_INPUT;
    Server *server = nullptr;    //מוצא אותו בטבלת הערבול ודרכו את החווה שלו, גם מסיר אותו מטבלת הערבול
    //הוספת בדיקה אם קיים שרת כזה
    int dataCenterId = farms.Find(server->home->id);
    DataCenter *farm = farms.parents[dataCenterId]->data;
    if (server->traffic > 0) {
        farm->servers_by_traffic.remove_vertex(server);
        all_servers_by_traffic.remove_vertex(server);
    }
    delete server;
    return SUCCESS;
}

StatusType DataCenterManager::SetTraffic(int serverID, int traffic) {
    if (serverID < 0 || traffic < 0)
        return INVALID_INPUT;
    Server *server = nullptr;    //מוצא אותו בטבלת הערבול ודרכו את החווה שלו, גם מסיר אותו מטבלת הערבול
    //הוספת בדיקה אם קיים שרת כזה
    int dataCenterId = farms.Find(server->home->id);
    DataCenter *farm = farms.parents[dataCenterId]->data;
    if (server->traffic > 0) {
        farm->servers_by_traffic.remove_vertex(server);
        all_servers_by_traffic.remove_vertex(server);
    }
    server->traffic = traffic;
    if (traffic != 0) {
        vertex<Server*, int> *vertex1 =new vertex<Server*, int>(server, nullptr);//אני יוצר 2 כי בכל remove vertex מוחקים(ממש, עם דיליט והכל) אחד
        farm->servers_by_traffic.add_vertex(vertex1);
        vertex<Server*, int> *vertex2 =new vertex<Server*, int>(server, nullptr);
        all_servers_by_traffic.add_vertex(vertex2);
    }
}

DataCenterManager::~DataCenterManager() {

}


