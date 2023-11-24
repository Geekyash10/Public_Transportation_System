#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <stack>
#include <unordered_set>
using namespace std;


int Totalnode = 7;
class EdgeInfo
{
public:
    int distance;
    int traffic;
    int redLight;

    EdgeInfo(int _distance, int _traffic, int _redLight)
        : distance(_distance), traffic(_traffic), redLight(_redLight) {}
};

class CityNode;

class CityEdge
{
public:
    CityNode *source;
    CityNode *destination;
    EdgeInfo info;

    CityEdge(CityNode *_source, CityNode *_destination, const EdgeInfo &_info) : source(_source), destination(_destination), info(_info) {}
};

class CityNode
{
public:
    int id;
    string type;
    vector<CityEdge *> edges;
    CityNode(int _id, const string &_type) : id(_id), type(_type) {}
};

class CityGraph
{

private:
    vector<CityNode *> city;

    // Helper function to find a node by ID
    CityNode *findNode(int nodeId)
    {
        for (CityNode *node : city)
        {
            if (node->id == nodeId)
            {
                return node;
            }
        }
        return nullptr;
    }

    void findAllPathsHelper(CityNode *currentNode, CityNode *endNode, vector<int> &currentPath, vector<vector<int>> &allPaths, unordered_set<int> &visitedNodes)
    {
        currentPath.push_back(currentNode->id);
        visitedNodes.insert(currentNode->id);

        if (currentNode == endNode)
        {
            // Found a path, add it to the list
            allPaths.push_back(currentPath);
        }
        else
        {
            // Continue DFS for unvisited neighbors
            for (CityEdge *edge : currentNode->edges)
            {
                if (visitedNodes.find(edge->destination->id) == visitedNodes.end())
                {
                    findAllPathsHelper(edge->destination, endNode, currentPath, allPaths, visitedNodes);
                }
            }
        }

        currentPath.pop_back();
        visitedNodes.erase(currentNode->id);
    }

    vector<int> findMostConvenientPath(const vector<vector<int>> &allPaths)
    {
        vector<int> mostConvenientPath;
        double minConvenienceScore = numeric_limits<double>::infinity();

        for (const auto &path : allPaths)
        {
            double convenienceScore = calculateConvenienceScore(path);
            cout << "convenient score  "<< convenienceScore << endl;
            if (convenienceScore < minConvenienceScore)
            {
                minConvenienceScore = convenienceScore;
                mostConvenientPath = path;
            }
        }
        cout<<endl;
        return mostConvenientPath;
    }

    // Calculate the convenience score for a path
    double calculateConvenienceScore(const vector<int> &path)
    {
        double totalScore = 0.0;
        const double epsilon = 1e-2;
        for (int  i = 0; i < path.size() - 1; ++i)
        {
            CityNode *source = findNode(path[i]);
            CityNode *destination = findNode(path[i + 1]);
            if (source && destination)
            {
                for (CityEdge *edge : source->edges)
                {
                    if (edge->destination == destination)
                    {
                        // Adjust weights to minimize distance, red light, and traffic
                        totalScore += 0.5 / (edge->info.distance + epsilon) + 0.3 / (edge->info.redLight + epsilon) + 0.2 / (edge->info.traffic + epsilon);
                        break;
                    }
                }
            }
        }

        return totalScore;
    }

    // Display a path
    void displayPath(const vector<int> &path)
    {
        for (size_t i = 0; i < path.size(); ++i)
        {
            cout << path[i];
            if (i < path.size() - 1)
            {
                cout << " -> ";
            }
        }

        cout << endl;
    }

public:
    vector<vector<int>> allPaths;
    // Add a new node to the city
    void addNode(int nodeId, const string &nodeType)
    {
        CityNode *newNode = new CityNode(nodeId, nodeType);
        city.push_back(newNode);
    }

    // Add a weighted directed edge from node1 to node2
    void addEdge(int node1, int node2, int distance, int traffic, int redLight)
    {
        CityNode *source = findNode(node1);
        CityNode *destination = findNode(node2);

        if (source && destination)
        {
            EdgeInfo edgeInfo(distance, traffic, redLight);
            CityEdge *newEdge = new CityEdge(source, destination, edgeInfo);
            source->edges.push_back(newEdge);
        }
    }

    void findAllPaths(int start, int end)
    {
        CityNode *startNode = findNode(start);
        CityNode *endNode = findNode(end);

        if (!startNode || !endNode)
        {
            cout << "Invalid start or end node." << endl;
            return;
        }

        vector<int> currentPath;
        unordered_set<int> visitedNodes;

        cout << "All paths from " << startNode->type << startNode->id << " to " << endNode->type << endNode->id << ":" << endl;
        findAllPathsHelper(startNode, endNode, currentPath, allPaths, visitedNodes);

        // Display all paths
        for (const auto &path : allPaths)
        {
            displayPath(path);
        }

        cout << endl
             << "MOST CONVENIENT PATH  IS";

        cout << endl
             << endl;

        if (!allPaths.empty())
        {
            vector<int> mostConvenientPath = findMostConvenientPath(allPaths);
            cout << "Most convenient path: ";
            displayPath(mostConvenientPath);
        }
        else
        {
            cout << "No paths found." << endl;
        }

    
    }

    // Display the graph
    void displayGraph()
    {
        for (int i = 1; i <= Totalnode ; ++i)
        {
            cout << "+------";
        }
        cout << "+" << endl;

        for (int i = 1; i <= Totalnode; ++i)
        {
            CityNode *node = findNode(i);
            if (node)
            {
                cout << "| " << node->type[0] << i << "  ";
            }
            else
            {
                cout << "| ";
            }
        }
        cout << "|" << endl;

        for (int i = 1; i <= Totalnode; ++i)
        {
            cout << "+------";
        }
        cout << "+" << endl;

        cout << endl
             << endl;

        for (int i = 1; i <= Totalnode; ++i)
        {
            CityNode *node = findNode(i);
            if (node)
            {
                vector<CityEdge *> edges = node->edges;
                for (auto edge : edges)
                {
                    cout << node->type[0] << i << " --> ";

                    cout << edge->destination->type[0] << edge->destination->id << " (D: " << edge->info.distance
                         << ", T: " << edge->info.traffic << ", RL: " << edge->info.redLight << ") ";

                    cout << endl;
                }
                cout << endl;
            }
            else
            {
                cout << "                 " << endl;
            }
        }

        for (int i = 1; i <= Totalnode; ++i)
        {
            cout << "+-----------------";
        }
        cout << "+" << endl;
    }
};

int generateRandomNumber(int lower, int upper)
{

    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<int> distribution(lower, upper);
    return distribution(gen);
}

void createCity(CityGraph &city)
{

    int Dlower = 3;
    int Dupper = 10;
    int RLlower = 0;
    int RLupper = 4;
    int Tlower = 2;
    int Tupper = 8;


    city.addEdge(1, 2, generateRandomNumber(Dlower, Dupper), generateRandomNumber(Tlower, Tupper), generateRandomNumber(RLlower, RLupper));
    city.addEdge(2, 1, generateRandomNumber(Dlower, Dupper), generateRandomNumber(Tlower, Tupper), generateRandomNumber(RLlower, RLupper));
    city.addEdge(1, 5, generateRandomNumber(Dlower, Dupper), generateRandomNumber(Tlower, Tupper), generateRandomNumber(RLlower, RLupper));
    city.addEdge(2, 5, generateRandomNumber(Dlower, Dupper), generateRandomNumber(Tlower, Tupper), generateRandomNumber(RLlower, RLupper));
    city.addEdge(2, 4, generateRandomNumber(Dlower, Dupper), generateRandomNumber(Tlower, Tupper), generateRandomNumber(RLlower, RLupper));
    city.addEdge(3, 4, generateRandomNumber(Dlower, Dupper), generateRandomNumber(Tlower, Tupper), generateRandomNumber(RLlower, RLupper));
    city.addEdge(3, 6, generateRandomNumber(Dlower, Dupper), generateRandomNumber(Tlower, Tupper), generateRandomNumber(RLlower, RLupper));
    city.addEdge(4, 3, generateRandomNumber(Dlower, Dupper), generateRandomNumber(Tlower, Tupper), generateRandomNumber(RLlower, RLupper));
    city.addEdge(4 ,6, generateRandomNumber(Dlower, Dupper), generateRandomNumber(Tlower, Tupper), generateRandomNumber(RLlower, RLupper));
    city.addEdge(5, 2, generateRandomNumber(Dlower, Dupper), generateRandomNumber(Tlower, Tupper), generateRandomNumber(RLlower, RLupper));
    city.addEdge(5, 3, generateRandomNumber(Dlower, Dupper), generateRandomNumber(Tlower, Tupper), generateRandomNumber(RLlower, RLupper));
    city.addEdge(6, 5, generateRandomNumber(Dlower, Dupper), generateRandomNumber(Tlower, Tupper), generateRandomNumber(RLlower, RLupper));
    city.addEdge(6, 4, generateRandomNumber(Dlower, Dupper), generateRandomNumber(Tlower, Tupper), generateRandomNumber(RLlower, RLupper));
    city.addEdge(6, 7, generateRandomNumber(Dlower, Dupper), generateRandomNumber(Tlower, Tupper), generateRandomNumber(RLlower, RLupper));
    city.addEdge(5,7, generateRandomNumber(Dlower, Dupper), generateRandomNumber(Tlower, Tupper), generateRandomNumber(RLlower, RLupper));
    city.addEdge(3,7, generateRandomNumber(Dlower, Dupper), generateRandomNumber(Tlower, Tupper), generateRandomNumber(RLlower, RLupper));
    city.addEdge(7,4, generateRandomNumber(Dlower, Dupper), generateRandomNumber(Tlower, Tupper), generateRandomNumber(RLlower, RLupper));
    city.addEdge(7,6, generateRandomNumber(Dlower, Dupper), generateRandomNumber(Tlower, Tupper), generateRandomNumber(RLlower, RLupper));
    city.addEdge(2,7, generateRandomNumber(Dlower, Dupper), generateRandomNumber(Tlower, Tupper), generateRandomNumber(RLlower, RLupper));


}



int main()
{
    CityGraph city;

    // Add 15 city nodes with different types
    for (int i = 1; i <= Totalnode; ++i)
    {
        string nodeType;
        if (i % 3 == 0)
        {
            nodeType = "Bus Stop";
        }
        else if (i % 3 == 1)
        {
            nodeType = "Taxi Stand";
        }
        else
        {
            nodeType = "Auto Stand";
        }
        city.addNode(i, nodeType);
    }

    createCity(city);

    // Display the graph
    city.displayGraph();
    cout << endl
         << endl;

    int choice;
    cout << "Enter Choice" << endl;
    cout << "1. Show the path between 2 nodes" << endl;
    cout << "2. Exit" << endl;
    cin >> choice;

    while (choice != 2)
    {
        if (choice == 1)
        {
            int start, end;
            cout << "Enter the starting position (1-7): ";
            cin >> start;

            // Validate start position
            if (start < 1 || start > 15)
            {
                cout << "Invalid starting position. Please enter a number between 1 and 6." << endl;
                continue;
            }

            cout << "Enter the destination position (1-7): ";
            cin >> end;

            // Validate end position
            if (end < 1 || end > 15)
            {
                cout << "Invalid destination position. Please enter a number between 1 and 6." << endl;
                continue;
            }

            // Validate that start and end positions are different
            if (start == end)
            {
                cout << "Start and end positions should be different." << endl;
                continue;
            }

            // Find and display all paths between the start and end positions
            city.findAllPaths(start, end);

            if (city.allPaths.empty())
            {
                cout << "No paths found between nodes " << start << " and " << end << "." << endl;
            }

            city.allPaths.clear();
        }
        else
        {
            cout << "Invalid choice. Please enter 1 or 2." << endl;
        }

        cout << endl
             << "Enter Choice" << endl;
        cout << "1. Show the path between 2 nodes" << endl;
        cout << "2. Exit" << endl;
        cin >> choice;
    }

    return 0;
}