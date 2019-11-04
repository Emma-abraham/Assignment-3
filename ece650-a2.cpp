//
// Created by Emmanuel Abraham on 2019-10-22.
//

// Compile with c++ ece650-a2cpp -std=c++11 -o ece650-a2
#include <iostream>
#include <cstdio>
#include <sstream>
#include <cstring>
#include <vector>
#include <queue>
#include <cstdlib>

using namespace std;

// This class represents an undirected graph using a vector adjacency list representation

class outputTree {
public:
    int V;                                      // Number of vertices
    vector<vector<int> >* Records;
    outputTree(int V);                          // The Constructor
    void addEdge(int a, int b);                 // function to add an edge to graph
    void bfsAlgorithm(int startNode, int endNode, vector<int>* path);    // prints BFS traversal from a given source (startNode)
};

outputTree::outputTree(int V)  {
    outputTree::V = V;
    outputTree::Records = new vector<vector <int> >(V);
}

void outputTree::addEdge(int a, int b)          // because the graph is undirected
{
    (*Records)[a].push_back(b);
    (*Records)[b].push_back(a);
}
// using the BFS algorithm for the traversal calculation
void outputTree::bfsAlgorithm(int startNode, int endNode, vector<int> *backPath) {
    struct NodeStore {
        int node_id;
        int distance;
        vector<int> path;
    };

    vector<NodeStore*> line(V);
    for (int a = 0; a < V; a++) {
        line[a] = new NodeStore;
        line[a]->node_id = a;
        line[a]->distance = -1;
    }
    queue<NodeStore*> levels;
    line[startNode]->node_id = startNode;
    line[startNode]->path.push_back(startNode);
    line[startNode]->distance = 0;
    levels.push(line[startNode]);
    while(!levels.empty()){
        NodeStore* move = levels.front();
        levels.pop();
        for (int a : (*Records)[move->node_id]){
            if ((*line[a]).distance != -1){
                continue;
            }
            line[a]->path = move->path;
            line[a]->path.push_back(a);
            line[a]->distance = move->distance + 1;
            if (a == endNode) {
                (*backPath) = line[a]->path;
                return;
            }
            levels.push(line[a]);
        }
    }
    backPath = nullptr;
    return;
}

int main() {

    char inputString[300];
    char inputCommand;
    char* m;
    char* parse = (char*)" E{<,>}";
    int numNode = -1;
    int counter;
    int edgeLine[300];
    int a, startNode, endNode;
    outputTree tree(100);

    // Edge input status - have edges been inputted?
    // -1 : No V, No E; 0 : V inputted, waiting for E; 1 - There is E input
    int state = -1;

    // while (!=cin.eof()) declaring End of File this way was attributed as bad coding
    while (true) {

        fgets(inputString, 500, stdin);

        inputCommand = inputString[0];

        // End of File error
        if (feof(stdin)) {
            cout << endl;
            exit(0);
        }

        else if (inputCommand == 'V') {
            if (state == 0) {
                continue;
            }
            sscanf(inputString, "V %d", &numNode);

            cout << inputString << endl;

            if (numNode == 0) {
                cout << "Error: this graph is empty.\n";
                continue;
            }
            if (numNode < 0) {
                cout << "Error: node value is not valid.\n";
                continue;
            }

            tree.V = numNode;
            tree.Records->clear();
            tree.Records = new vector<vector<int> >(numNode);

            state = 0;
        }

        else if (inputCommand == 'E') {

            cout << inputString << endl;

            counter = 0;
            if (state == 1) {
                cout << "Error: Edges have been inputted for these vertices already.\n";
                continue;
            }
            if (numNode == -1) {
                cout << "Error: there has been no V input.\n";
                continue;
            }

            m = strtok(inputString, parse);

            // iterating through the E string to parse all the digits ignoring all other characters
            while (m != nullptr) {
                sscanf(m, "%d", &edgeLine[counter++]);
                m = strtok(nullptr, parse);
            }

            for (a = 0; a < counter - 1; a = a + 2){

                if ((tree.V <= edgeLine[a]) | (tree.V <= edgeLine[a + 1])){

                    cout << "Error: edge input for a non-existing vertex.\n";
                    tree.Records->clear();
                    tree.Records = new vector<vector<int> >(numNode);
                    state = 0;
                    break;
                }
                tree.addEdge(edgeLine[a], edgeLine[a + 1]);
            }
            state = 1;

        }

        else if (inputCommand == 's'){
            if (state == 0) {
                continue;
            }

            sscanf(inputString, "s %d %d", &startNode, &endNode);
            vector<int> *path;
            path = new vector<int>;
            if ((tree.V <= startNode) | (tree.V <= endNode)) {
                cout << "Error: inputted vertex point does not exist\n";
                continue;
            }
            if (startNode == endNode) {
                cout << startNode << endl;
                continue;
            }
            tree.bfsAlgorithm(startNode, endNode, path);
            if (path->empty()) {
                cout << "Error: no path exists between specified vertices\n" << endl;
            }

            // Printing the shortest part as an output in format X-X-X
            for (a = 0; (unsigned) a < path->size(); a++) {
                if ((unsigned) a < path->size() - 1) {
                    cout << path->at(a) << "-";
                } else {
                    cout << path->at(a) << '\n';
                    break;
                }
            }

        }
    }
}
