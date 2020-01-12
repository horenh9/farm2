#include "DataCenterManager.h"
#include <math.h>

#define STARTING_HASH 11

int pow(int num, int power);

////****************Server**************////

Server::Server(int id, int traffic, DataCenter *home) : id(id), traffic(traffic), home(home) {}

bool Server::operator==(const Server &server) const {
    return traffic == server.traffic && id == server.id;
}

bool Server::operator>(const Server &server) const {
    if (traffic > server.traffic)
        return true;
    else if (traffic == server.traffic)
        return id > server.id;
    else
        return false;
}

bool Server::operator<(const Server &server) const {
    return !(*this > server || *this == server);
}


Server &Server::operator=(const Server &other) {
    id = other.id;
    home = other.home;
    traffic = other.traffic;
    return *this;
}

////****************DataCenterManager**************////

DataCenter::DataCenter(int id) : id(id), servers(0), root(this) {
    servers_by_traffic = new AVLtree<Server, int>();
}

DataCenter::~DataCenter() {
    delete servers_by_traffic;
}

DataCenterManager::DataCenterManager(int n) : num_farms(n), servers(0) {
    hash_Servers = new DynamicHashTable<Server>(STARTING_HASH);
    all_servers_by_traffic = new AVLtree<Server, int>();
    farms = new UpTree<DataCenter>(n);
    for (int i = 1; i <= n; ++i) {
        DataCenter *farm = new DataCenter(i);
        UpVertex<DataCenter> *upVertex = new UpVertex<DataCenter>(i, farm);
        farms->parents[i] = upVertex;
    }
}

////             MergeDataCenters          ////
//declarations
void mergeAVL(DataCenter *dc1, DataCenter *dc2);

void SetAVLToArray(vertex<Server, int> *currHead, Server **array, int *position);

void mergeArray(Server **combined, Server **array1, Server **array2, int size1, int size2);

AVLtree<Server, int> *incompleteTree(int size);

vertex<Server, int> *createAVL(int height);

void removeLeaves(vertex<Server, int> *currHead, int *position, int k);

void setArrayInAVL(vertex<Server, int> *tree, Server **servers, int *position);

//implementaion
StatusType DataCenterManager::MergeDataCenters(int dataCenter1, int dataCenter2) {
    if (dataCenter1 < 1 || dataCenter1 > num_farms || dataCenter2 < 1 || dataCenter2 > num_farms)
        return INVALID_INPUT;
    int root1 = farms->Find(dataCenter1);
    int root2 = farms->Find(dataCenter2);
    if (farms->parents[root1]->size >= farms->parents[root2]->size)
        mergeAVL(farms->parents[root1]->data, farms->parents[root2]->data);
    else
        mergeAVL(farms->parents[root2]->data, farms->parents[root1]->data);
    farms->Union(root1, root2);
    return SUCCESS;
}

//merging 2 avl's into 1
void mergeAVL(DataCenter *dc1, DataCenter *dc2) {//putting dc2 avl in dc1 avl
    Server **serversArray1 = new Server *[dc1->servers];
    Server **serversArray2 = new Server *[dc2->servers];
    int patrolled = 0;
    SetAVLToArray(dc1->servers_by_traffic->head, serversArray1, &patrolled);
    patrolled = 0;
    SetAVLToArray(dc2->servers_by_traffic->head, serversArray2, &patrolled);
    Server **serversArrayTotal = new Server *[dc1->servers + dc2->servers];
    mergeArray(serversArrayTotal, serversArray1, serversArray2, dc1->servers, dc2->servers);
    AVLtree<Server, int> *combined = incompleteTree(dc1->servers + dc2->servers);
    patrolled = 0;
    setArrayInAVL(combined->head, serversArrayTotal, &patrolled);
    delete dc1->servers_by_traffic;
    delete dc2->servers_by_traffic;
    dc2->servers_by_traffic = nullptr;
    dc1->servers_by_traffic = combined;
    dc1->servers += dc2->servers;
    delete[]serversArray1;
    delete[]serversArray2;
    delete[]serversArrayTotal;
}

//making an array from an avl
void SetAVLToArray(vertex<Server, int> *currHead, Server **array, int *position) {
    if (currHead == nullptr)
        return;
    SetAVLToArray(currHead->left_son, array, position);
    array[*position] = &(currHead->key);
    (*position)++;
    SetAVLToArray(currHead->right_son, array, position);
}

//merging 2 sorted arrays
void mergeArray(Server **combined, Server **array1, Server **array2, int size1, int size2) {
    int position1 = 0, position2 = 0;
    for (int i = 0; i < size1 + size2; ++i) {
        if (position2 < size2 && position1 < size1) {
            if (*(array1[position1]) > *(array2[position2])) {
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

//creating an empty incomplete tree with "size" nodes
AVLtree<Server, int> *incompleteTree(int size) {
    int height = (int) floor(log2(size));
    AVLtree<Server, int> *servers = new AVLtree<Server, int>();
    servers->head = createAVL(height);
    int position = 0;
    removeLeaves(servers->head, &position, (pow(2, height + 1) - 1) - size);
    return servers;
}

int pow(int num, int power) {
    int res = num;
    for (int i = 1; i < power; ++i)
        res *= num;
    return res;
}

//creating an empty complete tree
vertex<Server, int> *createAVL(int height) {
    vertex<Server, int> *curr = new vertex<Server, int>(Server(0, 0, nullptr), nullptr);
    if (height > 0) {
        curr->left_son = createAVL(height - 1);
        curr->left_son->parent = curr;
        curr->right_son = createAVL(height - 1);
        curr->right_son->parent = curr;
        return curr;
    } else
        return curr;
}

//removing k leafs from a complete tree and transforming him to an incomplete tree
void removeLeaves(vertex<Server, int> *currHead, int *position, int k) {
    if (currHead == nullptr)
        return;
    if (*position < k) {
        removeLeaves(currHead->right_son, position, k);
        if (currHead->right_son == nullptr && currHead->left_son == nullptr) {
            if (currHead->parent != nullptr) {
                if (currHead->parent->right_son == currHead)
                    currHead->parent->right_son = nullptr;
                else
                    currHead->parent->left_son = nullptr;
            }
            delete currHead;
            (*position)++;
        } else
            removeLeaves(currHead->left_son, position, k);
    }
}

//filling the avl with data from sorted array. doing post order patrol
void setArrayInAVL(vertex<Server, int> *tree, Server **servers, int *position) {
    if (tree == nullptr)
        return;
    setArrayInAVL(tree->left_son, servers, position);
    tree->key = *servers[*position];
    tree->data = &tree->key.traffic;
    (*position)++;
    int sub_left = 0, sub_right = 0;
    setArrayInAVL(tree->right_son, servers, position);
    if (tree->left_son != nullptr) {
        tree->rank_left_son = tree->left_son->rank_left_son + tree->left_son->rank_right_son + *tree->left_son->data;
        sub_left = tree->left_son->sub_size;
        tree->height_left = height(tree->left_son);
    } else
        tree->rank_left_son = 0;
    if (tree->right_son != nullptr) {
        tree->rank_right_son = tree->right_son->rank_left_son + tree->right_son->rank_right_son + *tree->right_son->data;
        sub_right = tree->right_son->sub_size;
        tree->height_right = height(tree->right_son);
    } else
        tree->rank_right_son = 0;
    tree->sub_size = 1 + sub_left + sub_right;
}

////           Others             ////

vertex<Server, int> *selectK(vertex<Server, int> *vertex, int i);

int sumK(vertex<Server, int> *index);

StatusType DataCenterManager::AddServer(int dataCenterID, int serverID) {
    if (dataCenterID < 1 || dataCenterID > num_farms || serverID <= 0)
        return INVALID_INPUT;
    if (hash_Servers->find(serverID) != nullptr)
        return FAILURE;
    int root = farms->Find(dataCenterID);
    Server *server = new Server(serverID, 0, farms->parents[root]->parent);
    hash_Servers->add(serverID, server);
    return SUCCESS;
}

StatusType DataCenterManager::RemoveServer(int serverID) {
    if (serverID <= 0)
        return INVALID_INPUT;
    Node<Server> *temp = hash_Servers->find(serverID);
    if (temp == nullptr)
        return FAILURE;
    Server *server = temp->data;
    int dataCenterId = farms->Find(server->home->id);
    DataCenter *farm = farms->parents[dataCenterId]->data;
    if (server->traffic > 0) {
        vertex<Server, int> *temp1 = get_vertex_by_ID(farm->servers_by_traffic->head, *server);
        farm->servers_by_traffic->remove_vertex(*server);
        delete temp1;
        vertex<Server, int> *temp2 = get_vertex_by_ID(all_servers_by_traffic->head, *server);
        all_servers_by_traffic->remove_vertex(*server);
        delete temp2;
    }
    hash_Servers->remove(serverID);
    hash_Servers->table[serverID] = nullptr;
    delete server;
    return SUCCESS;
}

StatusType DataCenterManager::SetTraffic(int serverID, int traffic) {
    if (serverID <= 0 || traffic < 0)
        return INVALID_INPUT;
    Node<Server> *temp = hash_Servers->find(serverID);
    if (temp == nullptr)
        return FAILURE;
    Server *server = temp->data;
    int dataCenterId = farms->Find(server->home->id);
    DataCenter *farm = farms->parents[dataCenterId]->parent;
    vertex<Server, int> *temp1 = nullptr;
    vertex<Server, int> *temp2 = nullptr;
    if (server->traffic > 0) {
        temp1 = get_vertex_by_ID(farm->servers_by_traffic->head, *server);
        farm->servers_by_traffic->remove_vertex(*server);
        farm->servers--;
        temp2 = get_vertex_by_ID(all_servers_by_traffic->head, *server);
        all_servers_by_traffic->remove_vertex(*server);
        servers--;
    } else if (traffic != 0) {
        temp1 = new vertex<Server, int>(*server, &server->traffic);
        temp2 = new vertex<Server, int>(*server, &server->traffic);
    }
    server->traffic = traffic;
    if (traffic != 0) {
        temp1->key.traffic = traffic;
        temp1->data = &temp1->key.traffic;
        temp2->key.traffic = traffic;
        temp2->data = &temp2->key.traffic;
        farm->servers_by_traffic->add_vertex(temp1);
        farm->servers++;
        all_servers_by_traffic->add_vertex(temp2);
        servers++;
    } else {
        if (temp1 != nullptr) {
            delete temp1;
            delete temp2;
        }
    }

    return SUCCESS;

}

StatusType DataCenterManager::SumHighestTrafficServers(int dataCenterID, int k, int *traffic) {
    if (dataCenterID < 0 || dataCenterID > num_farms || k < 0 || traffic == nullptr)
        return INVALID_INPUT;
    if (k == 0) {
        *traffic = 0;
        return SUCCESS;
    }
    vertex<Server, int> *index = nullptr;
    if (dataCenterID == 0) {
        if (k > all_servers_by_traffic->head->sub_size)
            k = all_servers_by_traffic->head->sub_size;
        index = selectK(all_servers_by_traffic->head, all_servers_by_traffic->head->sub_size - k + 1);
        *traffic = *index->data + index->rank_right_son + sumK(index);
    } else {
        DataCenter *dataCenter = farms->parents[farms->Find(dataCenterID)]->parent;
        if (k > dataCenter->servers_by_traffic->head->sub_size)
            k = dataCenter->servers_by_traffic->head->sub_size;
        index = selectK(dataCenter->servers_by_traffic->head, dataCenter->servers_by_traffic->head->sub_size - k + 1);
        *traffic = *index->data + index->rank_right_son + sumK(index);
    }
    return SUCCESS;

}

vertex<Server, int> *selectK(vertex<Server, int> *vertex, int k) {
    if (vertex == nullptr)
        return nullptr;
    int vl_size = 0;
    if (vertex->left_son != nullptr)
        vl_size = vertex->left_son->sub_size;
    if (vl_size == k - 1)
        return vertex;
    if (vl_size > k - 1)
        return selectK(vertex->left_son, k);
    else
        return selectK(vertex->right_son, k - vl_size - 1);
}

int sumK(vertex<Server, int> *index) {
    if (index->parent == nullptr)
        return 0;
    if ((index->key) < (index->parent->key))
        return *(index->parent->data) + index->parent->rank_right_son + sumK(index->parent);
    else
        return sumK(index->parent);
}

DataCenterManager::~DataCenterManager() {
    for (int i = 1; i <= num_farms; ++i)
        delete farms->parents[i]->data;
    delete farms;
    delete all_servers_by_traffic;
    delete hash_Servers;
}
