#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
using namespace std;

class Graph {
    int vertices;
    vector<pair<pair<int, int>, int> > adjList;

public:
    Graph(int vertices);
    void addEdge(int source, int destination, int weight);
    void removeEdge(vector<pair<pair<int, int>, int> >::iterator it);
    bool edgeIsBridge(int source, int destination);
    void sortEdgesbyWeight();
    int T_star(int K);
    void DFSUtil(int v, vector<bool>& visited);
    int NumberOfConnectedComponents();
    void printgraph();
};

bool compareEdges(const pair<pair<int, int>, int>& a, const pair<pair<int, int>, int>& b) {
    return a.second < b.second;
}

Graph::Graph(int vertices) {
    this->vertices = vertices;
    //adjList = vector<pair<pair<int, int>, int>>(vertices, make_pair(make_pair(0, 0), 0));
}

void Graph::addEdge(int source, int destination, int weight) {
    // Forward edge
    adjList.push_back(make_pair(make_pair(source, destination), weight));
    
}

void Graph::removeEdge(vector<pair<pair<int, int>, int> >::iterator it) {
    adjList.erase(it);
}

void Graph::printgraph(){
    for(pair<pair<int, int>, int>& edge: adjList) cout << "Edges " 
    << edge.first.first << " - " << edge.first.second << " with weight "
    << edge.second << endl;
}

int Graph::NumberOfConnectedComponents() {
    vector<bool> visited(vertices, false);
    int count = 0;
    for (int v = 0; v < vertices; v++) {
        if (!visited[v]) {
            DFSUtil(v, visited);
            count++;
        }
    }
    return count;
}


void Graph::DFSUtil(int v, vector<bool>& visited) {
    // Mark the current node as visited
    visited[v] = true;
    //cout << "Visiting vertex " << v << endl;

    // Recur for all the vertices adjacent to this vertex
    for (const auto& edge : adjList) {
        int u = edge.first.first;
        int w = edge.first.second;


//cout << "Considering edge: " << u << " -- " << w << endl;

        if (u == v && !visited[w])
            DFSUtil(w, visited);
        else if (w == v && !visited[u])
            DFSUtil(u, visited);
    }
}


bool Graph::edgeIsBridge(int source, int destination) {
    bool flag;

    // Find the iterator pointing to the edge to be removed
    auto it = find_if(adjList.begin(), adjList.end(), [&](const pair<pair<int, int>, int>& edge) {
        return (edge.first.first == source && edge.first.second == destination) ||
               (edge.first.first == destination && edge.first.second == source);
    });

        auto originalPosition = it;
        auto ogcontent = *it;
        // Count the connected components
        int countConnectedSubgraphs = NumberOfConnectedComponents();
        
        int weight = it->second;
        // Remove the given edge using the iterator
        removeEdge(it);

        // Again count the connected components
        int newCount = NumberOfConnectedComponents();
        

        if (countConnectedSubgraphs == newCount) {
            flag = false;
        } else {
            flag = true;
        }

    
        // Add the given edge back to the graph
        //addEdge(source, destination, weight);
        adjList.insert(originalPosition, ogcontent);
        //sortEdgesbyWeight();
    
    
    
    return flag;
}



void Graph::sortEdgesbyWeight() {
    sort(adjList.begin(), adjList.end(), compareEdges);

}


int Graph::T_star(int K){
    if(adjList.size() <= K) return -1;
    int res;
    sortEdgesbyWeight();
    auto it = adjList.begin();
    
    while(K>0) { 
        //cout << "number of iteration" << endl;
        
        if(edgeIsBridge(it->first.first, it->first.second)) {
            K--;
            
            if(next(it) == adjList.end())  return -1;
            
            else if(K==0) break;
                 else ++it;
            
        }
        
        else{
           if(next(it) == adjList.end()) return -1;
            else{
               
               removeEdge(it);
               //cout << "weight: " << it->second << endl;
            }
        }
        
    }
        it = next(it);
        
        while(next(it)!= adjList.end()) {
            if(edgeIsBridge(it->first.first, it->first.second)) return it->second;
            else removeEdge(it);
            
    }
        return -1;
}


int main() {
    int N, M, K;
    cin >> N >> M >> K;

    Graph g(N);
    int a, b, t;

    for (int i = 0; i < M; ++i) {
        cin >> a >> b >> t;
        g.addEdge(a-1, b-1, t);
    }
    
    
    int res = g.T_star(K);
    
    if(res>0) cout << res << endl;
    else cout << "infinity" << endl;
    
    return 0;
}
