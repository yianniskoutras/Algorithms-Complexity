#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

double c = 0.0;


struct Result {
    int gain, weight;
    Result(int gain, int weight) : gain(gain), weight(weight) {}
};


struct Edge {
    int src, dest, gain, weight;

    Edge(int src, int dest, int gain, int weight) : src(src), dest(dest), gain(gain), weight(weight) {}
};

int findParent(vector<int>& parent, int vertex) {
    if (parent[vertex] == -1)
        return vertex;
    return parent[vertex] = findParent(parent, parent[vertex]);
}

void unionSets(vector<int>& parent, int x, int y) {
    int rootX = findParent(parent, x);
    int rootY = findParent(parent, y);

    if (rootX != rootY)
        parent[rootX] = rootY;
}

bool compareEdgesGain(const Edge& a, const Edge& b) {
    return a.gain > b.gain; // Sort gain in descending order
}


bool compareEdgesWeight(const Edge& a, const Edge& b) {
    return a.weight < b.weight; // Sort weight in ascending order
}

// Function to compare edges based on gain-to-weight ratio
bool compareEdgesDummy(const Edge& e1, const Edge& e2, double c) {
    return (e1.gain - c * e1.weight) > (e2.gain - c * e2.weight);
}

bool compareEdgesDiff(const Edge& e1, const Edge& e2) {
    return compareEdgesDummy(e1, e2, c);
}

int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}


int pmax(vector<Edge>& graph, int numVertices) {
    sort(graph.begin(), graph.end(), compareEdgesGain);
    int total_gain = 0;
    vector<int> parent(numVertices, -1);
    vector<Edge> maxSpanningTree;

    for (const Edge& edge : graph) {
        int srcParent = findParent(parent, edge.src);
        int destParent = findParent(parent, edge.dest);

        if (srcParent != destParent) {
            maxSpanningTree.push_back(edge);
            total_gain += edge.gain;
            unionSets(parent, srcParent, destParent);
        }
    }

    return total_gain;
}

int wmin(vector<Edge>& graph, int numVertices) {
    sort(graph.begin(), graph.end(), compareEdgesWeight);
    int total_weight = 0;
    vector<int> parent(numVertices, -1);
    vector<Edge> maxSpanningTree;

    for (const Edge& edge : graph) {
        int srcParent = findParent(parent, edge.src);
        int destParent = findParent(parent, edge.dest);

        if (srcParent != destParent) {
            maxSpanningTree.push_back(edge);
            total_weight += edge.weight;
            unionSets(parent, srcParent, destParent);
        }
    }

    return total_weight;
}


Result answer (vector<Edge>& graph, int numVertices) {
    Result data(0,0);
    double left = 0.0;
    double right = static_cast<double>(pmax(graph, numVertices)) / wmin(graph, numVertices);



    c = (right - left) / 2;



    while(right-left > pow(10, -8)) {
        sort(graph.begin(), graph.end(), compareEdgesDiff);
        vector<int> parent(numVertices, -1);
        vector<Edge> maxSpanningTree;

        data.gain = 0;
        data.weight = 0;

    
        for (const Edge& edge : graph) {

            int srcParent = findParent(parent, edge.src);
            int destParent = findParent(parent, edge.dest);

            if (srcParent != destParent) {
                maxSpanningTree.push_back(edge);
                data.gain += edge.gain;
                data.weight += edge.weight;
                unionSets(parent, srcParent, destParent);
            }

        }

        double check = (data.gain - c*data.weight);
        //I can do better
        if(check > 0) {

            double temp = c;
            c = (right + c) / 2;
            left = temp;
        }

            //I went too far
        else {
            double temp = c;
            c = (c + left) / 2;
            right = temp;
        }
        
        
    }


    return data;

}







int main() {
    int V, E;
    cin >> V >> E;
    
    vector<Edge> graph;
    int u, v, gain, weight;
    
    for(int i=0; i<E; i++) {
        cin >> u >> v >> gain >> weight;
        graph.push_back(Edge(u, v, gain, weight));
    }
    

    Result result = answer(graph, E);
    int div = gcd(result.gain, result.weight);
    cout << result.gain/div << " " << result.weight/div << endl;
    return 0;
}

