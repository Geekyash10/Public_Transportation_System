#include <iostream>
#include <vector>
#include <string>

class EdgeInfo {
public:
    int distance;
    int traffic;
    int redLight;

    EdgeInfo(int _distance, int _traffic, int _redLight)
        : distance(_distance), traffic(_traffic), redLight(_redLight) {}
};

class CityNode;

class CityEdge {
public:
    CityNode* destination;
    EdgeInfo info;
    CityEdge* next;

    CityEdge(CityNode* _destination, const EdgeInfo& _info) : destination(_destination), info(_info), next(nullptr) {}
};

class CityNode {
public:
    int id;
    std::string type;
    CityEdge* next;

    CityNode(int _id, const std::string& _type) : id(_id), type(_type), next(nullptr) {}
};

class CityGraph {
public:
    // Add a new node to the graph
    void addNode(int nodeId, const std::string& nodeType) {
        CityNode* newNode = new CityNode(nodeId, nodeType);
        nodes.push_back(newNode);
    }

    // Add a weighted directed edge from node1 to node2
    void addEdge(int node1, int node2, int distance, int traffic, int redLight) {
        CityNode* source = findNode(node1);
        CityNode* destination = findNode(node2);

        if (source && destination) {
            EdgeInfo edgeInfo(distance, traffic, redLight);
            CityEdge* newEdge = new CityEdge(destination, edgeInfo);
            newEdge->next = source->next;
            source->next = newEdge;
        }
    }

    // Display the graph
void displayGraph() {
    for (int i = 1; i <= 15; ++i) {
        std::cout << "+-----------------";
    }
    std::cout << "+" << std::endl;

    for (int i = 1; i <= 15; ++i) {
        CityNode* node = findNode(i);
        if (node) {
            std::cout << "| " << node->type[0] << i << "             ";
        } else {
            std::cout << "|                 ";
        }
    }
    std::cout << "|" << std::endl;

    for (int i = 1; i <= 15; ++i) {
        CityNode* node = findNode(i);
        if (node) {
            CityEdge* edge = node->next;
            std::cout << "| ";
            while (edge) {
                std::cout << edge->destination->type[0] << edge->destination->id << " (D: " << edge->info.distance
                          << ", T: " << edge->info.traffic << ", RL: " << edge->info.redLight << ") ";
                edge = edge->next;
            }
            std::cout << std::endl;
        } else {
            std::cout << "                  " << std::endl;
        }
    }

    for (int i = 1; i <= 15; ++i) {
        std::cout << "+-----------------";
    }
    std::cout << "+" << std::endl;
}

private:
    std::vector<CityNode*> nodes;

    // Helper function to find a node by ID
    CityNode* findNode(int nodeId) {
        for (CityNode* node : nodes) {
            if (node->id == nodeId) {
                return node;
            }
        }
        return nullptr;
    }
};

int main() {
    CityGraph city;

    // Add 15 nodes with different types
    for (int i = 1; i <= 15; ++i) {
        std::string nodeType;
        if (i % 3 == 0) {
            nodeType = "Bus Stop";
        } else if (i % 3 == 1) {
            nodeType = "Taxi Stand";
        } else {
            nodeType = "Auto Stand";
        }
        city.addNode(i, nodeType);
    }

    // Add weighted directed edges between nodes
    city.addEdge(1, 2, 5, 2, 1);
    city.addEdge(1, 4, 8, 3, 2);
    city.addEdge(2, 3, 4, 1, 0);
    city.addEdge(2, 5, 7, 2, 1);
    city.addEdge(3, 6, 6, 3, 2);
    city.addEdge(4, 5, 3, 1, 0);
    city.addEdge(4, 7, 9, 2, 1);
    city.addEdge(5, 6, 2, 1, 0);
    city.addEdge(5, 8, 5, 2, 1);
    city.addEdge(6, 9, 4, 1, 0);
    city.addEdge(7, 8, 3, 2, 1);
    city.addEdge(7, 10, 6, 3, 2);
    city.addEdge(8, 9, 7, 2, 1);
    city.addEdge(8, 11, 8, 3, 2);
    city.addEdge(9, 12, 5, 1, 0);
    city.addEdge(10, 11, 4, 2, 1);
    city.addEdge(11, 12, 3, 1, 0);
    city.addEdge(12, 13, 2, 2, 1);
    city.addEdge(13, 14, 1, 1, 0);
    city.addEdge(14, 15, 4, 3, 2);

    // Display the graph
    city.displayGraph();

    return 0;
}
