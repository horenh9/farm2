#include "DataCenterManager.h"

#define STARTING_HUSH 11



////****************Server**************////

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

////****************DataCenterManager**************////

DataCenter::DataCenter(int id) : id(id), servers(0), servers_by_traffic(AVLtree<Server *, int>()), root(this) {}

DataCenterManager::DataCenterManager(int n) : size(n), servers(0), farms(UpTree<DataCenter>(n)) {
    DynamicHashTable<Server *> *hush_Servers = new DynamicHashTable<Server *>(STARTING_HUSH);
    AVLtree<Server *, int> all_servers_by_traffic = AVLtree<Server *, int>();
    for (int i = 0; i < n; ++i) {
        DataCenter *farm = new DataCenter(i);
        farms.parents[i]->parent = farm;//לא יודע למה יש פה נקודות
        farms.parents[i]->data = farm;
    }
}

////             MergeDataCenters          ////
//declarations
void mergeAVL(DataCenter *dc1, DataCenter *dc2);

void SetAVLToArray(vertex<Server *, int> *currHead, Server **array, int *position);

void mergeArray(Server **combined, Server **array1, Server **array2, int size1, int size2);

AVLtree<Server *, int> uncompleteTree(int size);

int logUp(int size);

vertex<Server *, int> *createAVL(int height);

void removeLeafs(vertex<Server *, int> *currHead, int *position, int k);

void setArrayInAVL(vertex<Server *, int> *tree, Server **servers, int *position);

//implementaion
StatusType DataCenterManager::MergeDataCenters(int dataCenter1, int dataCenter2) {
    if (dataCenter1 < 1 || dataCenter1 > size || dataCenter2 < 1 || dataCenter2 > size)
        return INVALID_INPUT;
    int root1 = farms.Find(dataCenter1);
    int root2 = farms.Find(dataCenter2);
    if (farms.parents[root1]->size > farms.parents[root2]->size)
        mergeAVL(farms.parents[root1]->data, farms.parents[root2]->data);
    else
        mergeAVL(farms.parents[root2]->data, farms.parents[root1]->data);
    farms.Union(root1, root2);//שוב נקודה לא יודע למה
    return SUCCESS;
}

//merging 2 avl's into 1
void mergeAVL(DataCenter *dc1, DataCenter *dc2) {//putting dc2 avl in dc1 avl
    Server **serversArray1 = new Server *[dc1->servers];
    Server **serversArray2 = new Server *[dc2->servers];
    int patrolled = 0;
    SetAVLToArray(dc1->servers_by_traffic.head, serversArray1, &patrolled);
    patrolled = 0;
    SetAVLToArray(dc2->servers_by_traffic.head, serversArray2, &patrolled);
    Server **serversArrayTotal = new Server *[dc1->servers + dc2->servers];
    mergeArray(serversArrayTotal, serversArray1, serversArray2, dc1->servers, dc2->servers);
    AVLtree<Server *, int> combined = uncompleteTree(dc1->servers + dc2->servers);
    patrolled = 0;
    setArrayInAVL(combined.head, serversArrayTotal, &patrolled);
    delete[]serversArray1;
    delete[]serversArray2;
    delete[]serversArrayTotal;
}

//making an array from an avl
void SetAVLToArray(vertex<Server *, int> *currHead, Server **array, int *position) {
    if (currHead == nullptr)
        return;
    SetAVLToArray(currHead->left_son, array, position);
    array[*position] = currHead->key;
    (*position)++;
    SetAVLToArray(currHead->right_son, array, position);
}

//merging 2 sorted arrays
void mergeArray(Server **combined, Server **array1, Server **array2, int size1, int size2) {
    int position1 = 0, position2 = 0;
    for (int i = 0; i < size1 + size2; ++i) {
        if (position2 < size2 && position1 < size1) {
            if (array1[position1] > array2[position2]) {
                combined[i] = array2[position2];
                position2++;
            } else {
                combined[i] = array1[position1];
                position1++;
            }
        } else {
            if (position1 < size1) {
                combined[i] = array1[position1];
                position1++;
            } else {
                combined[i] = array2[position2];
                position2++;
            }
        }
    }

}

//creating an empty uncomplete tree with "size" nodes
AVLtree<Server *, int> uncompleteTree(int size) {
    int height = logUp(size);
    AVLtree<Server *, int> *servers = new AVLtree<Server *, int>();
    servers->head = createAVL(height);
    int position = 0;
    removeLeafs(servers->head, &position, (2 ^ height) - size);
    return *servers;
}

//getting the log of a number, the upper value.
int logUp(int size) {
    int counter = 1;
    while (size > (2 ^ counter) - 1)
        counter++;
    return counter;
}

//creating an empty complete tree
vertex<Server *, int> *createAVL(int height) {
    if (height > 0) {
        vertex<Server *, int> *curr = new vertex<Server *, int>(nullptr, 0);
        curr->left_son = createAVL(height - 1);
        curr->right_son = createAVL(height - 1);
        return curr;
    }
    return nullptr;
}

//removing k leafs from a complete tree and transforming him to an uncomplete tree
void removeLeafs(vertex<Server *, int> *currHead, int *position, int k) {
    if (currHead == nullptr)
        return;
    if (*position < k) {
        removeLeafs(currHead->right_son, position, k);
        if (currHead->right_son == nullptr && currHead->left_son == nullptr) {
            delete currHead;
            (*position)++;
        } else
            removeLeafs(currHead->left_son, position, k);
    }
}

//filling the avl with data from sorted array
void setArrayInAVL(vertex<Server *, int> *tree, Server **servers, int *position) {
    if (tree == nullptr)
        return;
    setArrayInAVL(tree->left_son, servers, position);
    tree->key = servers[*position];
    (*position)++;
    setArrayInAVL(tree->right_son, servers, position);
}

////           Others             ////

StatusType DataCenterManager::AddServer(int dataCenterID, int serverID) {
    if (dataCenterID < 1 || dataCenterID > size)
        return INVALID_INPUT;
    if(hush_Servers->find(serverID)!= nullptr)
        return FAILURE;
    int root = farms.Find(dataCenterID);
    Server *server = new Server(serverID, 0, farms.parents[root]->parent);
    hush_Servers->// add or add to table?
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
        vertex<Server *, int> *vertex1 = new vertex<Server *, int>(server,
                                                                   nullptr);//אני יוצר 2 כי בכל remove vertex מוחקים(ממש, עם דיליט והכל) אחד
        farm->servers_by_traffic.add_vertex(vertex1);
        vertex<Server *, int> *vertex2 = new vertex<Server *, int>(server, nullptr);
        all_servers_by_traffic.add_vertex(vertex2);
    }
}

DataCenterManager::~DataCenterManager() {

}










