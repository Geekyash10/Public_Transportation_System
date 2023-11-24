#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <memory>
#include <unordered_set>

class CityNode; // Forward declaration

class EdgeInfo {
public:
    int distance;
    int traffic;
    int redLight;

    EdgeInfo(int _distance, int _traffic, int _redLight)
        : distance(_distance), traffic(_traffic), redLight(_redLight) {}
};

class CityEdge {
public:
    std::shared_ptr<CityNode> source;
    std::shared_ptr<CityNode> destination;
    EdgeInfo info;

    CityEdge(const std::shared_ptr<CityNode>& _source, const std::shared_ptr<CityNode>& _destination, const EdgeInfo& _info)
        : source(_source), destination(_destination), info(_info) {}
};

class CityNode : public std::enable_shared_from_this<CityNode> {
public:
    int id;
    std::string type;
    std::vector<std::shared_ptr<CityEdge>> edges;

    CityNode(int _id, const std::string& _type) : id(_id), type(_type) {}

    void addEdge(const std::shared_ptr<CityNode>& destination, const EdgeInfo& info) {
        auto newEdge = std::make_shared<CityEdge>(shared_from_this(), destination, info);
        edges.push_back(newEdge);
    }
};

class CityGraph {
private:
    std::vector<std::shared_ptr<CityNode>> city;

    // Helper function to find a node by ID
    std::shared_ptr<CityNode> findNode(int nodeId) {
        for (const auto& node : city) {
            if (node->id == nodeId) {
                return node;
            }
        }
        return nullptr;
    }

    void findAllPathsHelper(
        const std::shared_ptr<CityNode>& currentNode, const std::shared_ptr<CityNode>& endNode,
        std::vector<int>& currentPath, std::vector<std::vector<int>>& allPaths, std::unordered_set<int>& visitedNodes
    ) {
        currentPath.push_back(currentNode->id);
        visitedNodes.insert(currentNode->id);

        if (currentNode == endNode) {
            // Found a path, add it to the list
            allPaths.push_back(currentPath);
        }
        else {
            // Continue DFS for unvisited neighbors
            for (const auto& edge : currentNode->edges) {
                if (visitedNodes.find(edge->destination->id) == visitedNodes.end()) {
                    findAllPathsHelper(edge->destination, endNode, currentPath, allPaths, visitedNodes);
                }
            }
        }

        currentPath.pop_back();
        visitedNodes.erase(currentNode->id);
    }

    std::vector<int> findMostConvenientPath(const std::vector<std::vector<int>>& allPaths) {
        std::vector<int> mostConvenientPath;
        double minConvenienceScore = std::numeric_limits<double>::infinity();

        for (const auto& path : allPaths) {
            double convenienceScore = calculateConvenienceScore(path);
            std::cout << "Convenience Score: " << convenienceScore << std::endl;
            if (convenienceScore < minConvenienceScore) {
                minConvenienceScore = convenienceScore;
                mostConvenientPath = path;
            }
        }

        return mostConvenientPath;
    }

    double calculateConvenienceScore(const std::vector<int>& path) {
        double totalScore = 0.0;
        const double epsilon = 1e-2;

        for (size_t i = 0; i < path.size() - 1; ++i) {
            const auto& source = findNode(path[i]);
            const auto& destination = findNode(path[i + 1]);

            if (source && destination) {
                for (const auto& edge : source->edges) {
                    if (edge->destination == destination) {
                        // Adjust weights to minimize distance, red light, and traffic
                        totalScore += 0.5 / (edge->info.distance + epsilon) +
                                      0.3 / (edge->info.redLight + epsilon) +
                                      0.2 / (edge->info.traffic + epsilon);
                        break;
                    }
                }
            }
        }

        return totalScore;
    }

    void displayPath(const std::vector<int>& path) {
        for (size_t i = 0; i < path.size(); ++i) {
            std::cout << path[i];
            if (i < path.size() - 1) {
                std::cout << " -> ";
            }
        }

        std::cout << std::endl;
    }

public:
    std::vector<std::vector<int>> allPaths;

    void addNode(int nodeId, const std::string& nodeType) {
        auto newNode = std::make_shared<CityNode>(nodeId, nodeType);
        city.push_back(newNode);
    }

    void addEdge(int node1, int node2, int distance, int traffic, int redLight) {
        auto source = findNode(node1);
        auto destination = findNode(node2);

        if (source && destination) {
            EdgeInfo edgeInfo(distance, traffic, redLight);
            source->addEdge(destination, edgeInfo);
        }
    }

    void findAllPaths(int start, int end) {
        auto startNode = findNode(start);
        auto endNode = findNode(end);

        if (!startNode || !endNode) {
            std::cout << "Invalid start or end node." << std::endl;
            return;
        }

        std::vector<int> currentPath;
        std::unordered_set<int> visitedNodes;

        std::cout << "All paths from " << startNode->type << startNode->id << " to " << endNode->type << endNode->id << ":" << std::endl;
        findAllPathsHelper(startNode, endNode, currentPath, allPaths, visitedNodes);

        // Display all paths
        for (const auto& path : allPaths) {
            displayPath(path);
        }

        std::cout << std::endl << "MOST CONVENIENT PATH IS" << std::endl << std::endl;

        if (!allPaths.empty()) {
            std::vector<int> mostConvenientPath = findMostConvenientPath(allPaths);
            std::cout << "Most convenient path: ";
            displayPath(mostConvenientPath);
        }
        else {
            std::cout << "No paths found." << std::endl;
        }

        allPaths.clear();
    }

    void displayGraph() {
        for (const auto& node : city) {
            std::cout << "+------";
        }
        std::cout << "+" << std::endl;

        for (const auto& node : city) {
            if (node) {
                std::cout << "| " << node->type[0] << node->id << "  ";
            }
            else {
                std::cout << "| ";
            }
        }
        std::cout << "|" << std::endl;

        for (const auto& node : city) {
            std::cout << "+------";
        }
        std::cout << "+" << std::endl << std::endl;

        for (const auto& node : city) {
            if (node) {
                const std::vector<std::shared_ptr<CityEdge>>& edges = node->edges;
                for (const auto& edge : edges) {
                    std::cout << node->type[0] << node->id << " --> ";
                    std::cout << edge->destination->type[0] << edge->destination->id
                              << " (D: " << edge->info.distance
                              << ", T: " << edge->info.traffic
                              << ", RL: " << edge->info.redLight << ") ";
                    std::cout << std::endl;
                }
                std::cout << std::endl;
            }
            else {
                std::cout << "                 " << std::endl;
            }
        }

        for (const auto& node : city) {
            std::cout << "+-----------------";
        }
        std::cout << "+" << std::endl;
    }
};

int generateRandomNumber(int lower, int upper) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(lower, upper);
    return distribution(gen);
}

void createCity(CityGraph& city) {
    int Dlower = 3;
    int Dupper = 10;
    int RLlower = 0;
    int RLupper = 4;
    int Tlower = 2;
    int Tupper = 8;

    city.addEdge(1, 2, generateRandomNumber(Dlower, Dupper), generateRandomNumber(Tlower, Tupper), generateRandomNumber(RLlower, RLupper));
    city.addEdge(1, 5, generateRandomNumber(Dlower, Dupper), generateRandomNumber(Tlower, Tupper), generateRandomNumber(RLlower, RLupper));
    city.addEdge(2, 5, generateRandomNumber(Dlower, Dupper), generateRandomNumber(Tlower, Tupper), generateRandomNumber(RLlower, RLupper));
    city.addEdge(2, 4, generateRandomNumber(Dlower, Dupper), generateRandomNumber(Tlower, Tupper), generateRandomNumber(RLlower, RLupper));
    city.addEdge(3, 4, generateRandomNumber(Dlower, Dupper), generateRandomNumber(Tlower, Tupper), generateRandomNumber(RLlower, RLupper));
    city.addEdge(3, 6, generateRandomNumber(Dlower, Dupper), generateRandomNumber(Tlower, Tupper), generateRandomNumber(RLlower, RLupper));
    city.addEdge(4, 3, generateRandomNumber(Dlower, Dupper), generateRandomNumber(Tlower, Tupper), generateRandomNumber(RLlower, RLupper));
    city.addEdge(4, 6, generateRandomNumber(Dlower, Dupper), generateRandomNumber(Tlower, Tupper), generateRandomNumber(RLlower, RLupper));
    city.addEdge(5, 2, generateRandomNumber(Dlower, Dupper), generateRandomNumber(Tlower, Tupper), generateRandomNumber(RLlower, RLupper));
    city.addEdge(5, 3, generateRandomNumber(Dlower, Dupper), generateRandomNumber(Tlower, Tupper), generateRandomNumber(RLlower, RLupper));
    city.addEdge(6, 5, generateRandomNumber(Dlower, Dupper), generateRandomNumber(Tlower, Tupper), generateRandomNumber(RLlower, RLupper));
    city.addEdge(6, 4, generateRandomNumber(Dlower, Dupper), generateRandomNumber(Tlower, Tupper), generateRandomNumber(RLlower, RLupper));
}

int main() {
    CityGraph city;

    // Add 15 city nodes with different types
    for (int i = 1; i <= 6; ++i) {
        std::string nodeType;
        if (i % 3 == 0) {
            nodeType = "Bus Stop";
        }
        else if (i % 3 == 1) {
            nodeType = "Taxi Stand";
        }
        else {
            nodeType = "Auto Stand";
        }
        city.addNode(i, nodeType);
    }

    createCity(city);

    // Display the graph
    city.displayGraph();
    std::cout << std::endl << std::endl;

    int choice;
    std::cout << "Enter Choice" << std::endl;
    std::cout << "1. Show the path between 2 nodes" << std::endl;
    std::cout << "2. Exit" << std::endl;
    std::cin >> choice;

    while (choice != 2) {
        if (choice == 1) {
            int start, end;
            std::cout << "Enter the starting position (1-6): ";
            std::cin >> start;

            // Validate start position
            if (start < 1 || start > 15) {
                std::cout << "Invalid starting position. Please enter a number between 1 and 6." << std::endl;
                continue;
            }

            std::cout << "Enter the destination position (1-6): ";
            std::cin >> end;

            // Validate end position
            if (end < 1 || end > 15) {
                std::cout << "Invalid destination position. Please enter a number between 1 and 6." << std::endl;
                continue;
            }

            // Validate that start and end positions are different
            if (start == end) {
                std::cout << "Start and end positions should be different." << std::endl;
                continue;
            }

            // Find and display all paths between the start and end positions
            city.findAllPaths(start, end);

            if (city.allPaths.empty()) {
                std::cout << "No paths found between nodes " << start << " and " << end << "." << std::endl;
            }

            city.allPaths.clear();
        }
        else {
            std::cout << "Invalid choice. Please enter 1 or 2." << std::endl;
        }

        std::cout << std::endl << "Enter Choice" << std::endl;
        std::cout << "1. Show the path between 2 nodes" << std::endl;
        std::cout << "2. Exit" << std::endl;
        std::cin >> choice;
    }

    return 0;
}
