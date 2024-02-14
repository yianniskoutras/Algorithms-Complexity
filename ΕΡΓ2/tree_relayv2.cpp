#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include<limits>
#include <algorithm>

using namespace std;

const int INF = numeric_limits<int>::max();

void DFS(int node, vector<bool> &visited, vector<vector<int> > &adjMatrix, vector<int> &path,
         map<int, vector<int> > &ancestors, vector<vector<int> > &extendedGraph, vector<int> &weightPath);

vector<int> dijkstra(int start, const vector<vector<int> > &graph, const vector<int> &currNodeAnc);

int main() {
    int nodes, edges;
    //cout << "Enter the number of nodes: ";
    cin >> nodes;
    edges = nodes - 1;

    vector<vector<int> > adjMatrix(nodes, vector<int>(nodes, 0));
    vector<bool> visited(nodes, false);
    vector<int> path, weightPath(nodes, 0);
    map<int, vector<int> > ancestors;
    vector<vector<int> > extendedGraph(nodes, vector<int>(nodes, 0));
    vector<vector<int> > augGraph(nodes, vector<int>(nodes, 0));
    map<int, pair<int, int> > TimeInfo;

    //cout << "Enter edges in the format 'node1 node2 weight':\n";
    for (int i = 0; i < edges; ++i) {
        int node1, node2, weight;
        cin >> node1 >> node2 >> weight;
        adjMatrix[node1 - 1][node2 - 1] = weight;
        adjMatrix[node2 - 1][node1 - 1] = weight;
    }

    for (int i = 1; i <= edges; ++i) {
        int t_setup, tperlen;
        cin >> t_setup >> tperlen;
        TimeInfo[i] = make_pair(t_setup, tperlen);
    }


    DFS(0, visited, adjMatrix, path, ancestors, extendedGraph, weightPath);

    // Print the ancestors of each node
    /*for (const auto &pair : ancestors) {
        cout << "Node " << pair.first +1 << " has ancestors: ";
        for (int ancestor : pair.second) {
            cout << ancestor + 1  << " ";
        }
        cout << endl;
    }*/

    bool isFirstNode = true;

// Loop through each node and its ancestors
    for (const auto &nodeAndAncestors: ancestors) {
        int accumulatedDistance = 0;
        auto lastAncestorIterator = nodeAndAncestors.second.rbegin();
        int currentNode = nodeAndAncestors.first;

        // Calculate the distance for each ancestor of the current node
        for (auto currentAncestorIterator = nodeAndAncestors.second.rbegin();
             currentAncestorIterator != nodeAndAncestors.second.rend(); ++currentAncestorIterator) {
            if (currentAncestorIterator == nodeAndAncestors.second.rbegin()) {
                accumulatedDistance += adjMatrix[*currentAncestorIterator][currentNode];
            } else {
                accumulatedDistance += adjMatrix[*lastAncestorIterator][*currentAncestorIterator];
            }

            // Update the augmented graph with the calculated distance
            augGraph[currentNode][*currentAncestorIterator] =
                    accumulatedDistance * TimeInfo[currentNode].second + TimeInfo[currentNode].first;
            augGraph[*currentAncestorIterator][currentNode] = augGraph[currentNode][*currentAncestorIterator];
            lastAncestorIterator = currentAncestorIterator;
        }

        // Skip the root node as it doesn't have ancestors
        if (isFirstNode) {
            isFirstNode = false;
            continue;
        }

        // Calculate the shortest distances from the current node to all other nodes
//        vector<int> shortestDistances = dijkstra(currentNode, augGraph, ancestors[currentNode]);
//        cout << shortestDistances[0] << " ";
    }

//    cout << endl;



    for (int i = 1; i < nodes; ++i) {
        vector<int> distances = dijkstra(i, augGraph, ancestors[i]);
        if (i < nodes - 1) {
            cout << distances[0] << " ";
        } else {
            cout << distances[0];
        }
    }
    cout << endl;


    return 0;
}

void DFS(int node, vector<bool> &visited, vector<vector<int> > &adjMatrix, vector<int> &path,
         map<int, vector<int> > &ancestors, vector<vector<int> > &extendedGraph, vector<int> &weightPath) {
    visited[node] = true;
    path.push_back(node);

    if (!path.empty()) {
        ancestors[node] = vector<int>(path.begin(), path.end() - 1);

        // Calculate and update the cumulative weight from current node to its ancestors
        int cumulativeWeight = 0;
        for (int i = path.size() - 2; i >= 0; --i) {
            cumulativeWeight += adjMatrix[path[i]][path[i + 1]];
            extendedGraph[node][path[i]] = cumulativeWeight;
            weightPath[path[i]] = cumulativeWeight;
        }
    }

    for (int i = 0; i < adjMatrix[node].size(); ++i) {
        if (adjMatrix[node][i] != 0 && !visited[i]) {
            DFS(i, visited, adjMatrix, path, ancestors, extendedGraph, weightPath);
        }
    }

    path.pop_back();
}


vector<int> dijkstra(int start, const vector<vector<int> > &graph, const vector<int> &currNodeAnc) {
    int n = graph.size();
    vector<int> dist(n, INF);
    priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > pq;

    dist[start] = 0;
    pq.push(make_pair(0, start));

    while (!pq.empty()) {
        int distance = pq.top().first;
        int node = pq.top().second;
        pq.pop();

        if (distance > dist[node]) continue;

        for (int i = 0; i < n; ++i) {
            if (find(currNodeAnc.begin(), currNodeAnc.end(), i) != currNodeAnc.end() && graph[node][i] != 0) {
                int newDist = distance + graph[node][i];
                if (newDist < dist[i]) {
                    dist[i] = newDist;
                    pq.push(make_pair(newDist, i));
                }
            }
        }
    }

    return dist;
}