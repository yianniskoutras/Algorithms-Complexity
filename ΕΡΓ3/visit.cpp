#include <iostream>
#include <climits>
#include <vector>
#include <sstream>
#include <limits>

using namespace std;

struct Edge{
    int weight;
    bool isTimeDepedent;
    int startTime, endTime;
    Edge(int weight, bool isTimeDependent, int startTime, int endTime) : weight(weight), isTimeDepedent(isTimeDependent),
                                                                         startTime(startTime), endTime(endTime) {}
};


class Graph {
private:
    int V;
    vector<vector<Edge>> adj_matrix;
    int K;
    int* cities;

public:
    Graph(int numVertices, int numCities) {
        V = numVertices;
        K = numCities;
        adj_matrix = vector<vector<Edge>>(V, vector<Edge>(V, {0, false, 0, 0}));
        cities = new int[K];
    }
    
    ~Graph() {
        
        delete[] cities;
    }

    void addEdge(int src, int dest, int weight){
        adj_matrix[src-1][dest-1].weight = weight;
        adj_matrix[dest-1][src-1].weight = weight;
    }

    void addCities(int i, int u){
        cities[i] = u;
    }

    int miniDist(int distance[], bool Tset[])  {
        int minimum=INT_MAX,ind;

        for(int k=0;k<V;k++)
        {
            if(Tset[k]==false && distance[k]<=minimum)
            {
                minimum=distance[k];
                ind=k;
            }
        }
        return ind;
    }

    void updateGraph(){
        int timer = 0;
        for(int i=0; i<K-1; i++){
            int u = cities[i]-1;
            int v = cities[i+1]-1;
            adj_matrix[u][v].isTimeDepedent = true;
            adj_matrix[v][u].isTimeDepedent = true;
            adj_matrix[u][v].startTime = timer;
            adj_matrix[v][u].startTime = timer;
            adj_matrix[u][v].endTime = timer + adj_matrix[u][v].weight - 1;
            adj_matrix[v][u].endTime = adj_matrix[u][v].endTime;
            timer += adj_matrix[u][v].weight;
        }
    }

    int withinInterval(int m, int k, int to){
        if (!adj_matrix[m][k].isTimeDepedent) return -1;
        if(to >= adj_matrix[m][k].startTime && to <= adj_matrix[m][k].endTime) return adj_matrix[m][k].endTime - to + 1;
        else return -1;
    }


    int timed_dijkstra(int src, int dest, int to) // adjacency matrix
    {
        int distance[V]; // // array to calculate the minimum distance for each node
        int time_so_far[V];
        bool Tset[V];// boolean array to mark visited and unvisited for each node


        for(int k = 0; k<V; k++)
        {
            distance[k] = INT_MAX;
            Tset[k] = false;
            time_so_far[k] = 0;
        }

        distance[src-1] = 0;   // Source vertex distance is set 0
        time_so_far[src-1] = to;

        //start of main process

        for(int i = 0; i<V; i++)
        {
            int m=miniDist(distance,Tset);
            Tset[m]=true;
            for(int k = 0; k<V; k++)
            {
                int time_delay = withinInterval(m, k, time_so_far[m]);
                if(!Tset[k] && time_delay>0 && distance[m]!=INT_MAX && distance[m] + adj_matrix[m][k].weight + time_delay < distance[k]) {
                    distance[k] = adj_matrix[m][k].weight + time_delay + distance[m];
                    to += adj_matrix[m][k].weight + time_delay;
                    time_so_far[k] = time_so_far[m] + adj_matrix[m][k].weight + time_delay;
                }

                else if(!Tset[k] && adj_matrix[m][k].weight && distance[m]!=INT_MAX && distance[m] + adj_matrix[m][k].weight < distance[k]) {
                    distance[k] = distance[m] + adj_matrix[m][k].weight;
                    to += adj_matrix[m][k].weight;
                    time_so_far[k] = time_so_far[m] + adj_matrix[m][k].weight;
                }
            }
        }


        //end of main process

        /*cout<<"Vertex\t\tDistance from source vertex"<<endl;
        for(int k = 0; k<V; k++) {
            cout<< k+1 <<"\t\t\t"<<distance[k]<<endl;
        }*/
        return distance[dest-1];
    }

};

int main()
{
    int N,M;
    cin >> N >> M;

    //cout << N << M << endl;
    int A,B,T,K;
    cin >> A >> B >> T >> K;

    //cout << A << B << T << K << endl;

    Graph g(N, K);  //Create the graph

    cin.ignore();
    
    // Add the cities to be visited by the president
    
string str;
  if(getline(cin, str)) {
    istringstream sstr(str);
    int val;
    int i=0;
    while(sstr >> val && i < K){
       g.addCities(i, val);
       ++i;
    }
  }

    


    int src, dest, weight;
    for(int i=0; i<M; i++){
        cin >> src >> dest >> weight;
        g.addEdge(src, dest, weight);
    }

    
    g.updateGraph();

    int res = g.timed_dijkstra(A, B, T);
    cout << res << endl;
    return 0;
}