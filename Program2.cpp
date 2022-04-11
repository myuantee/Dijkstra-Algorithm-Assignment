#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <math.h>
#include <queue>

using namespace std;
typedef pair<int,int> iPair;

const int P_COUNT = 10; // Number of planets
const int E_COUNT = 15; // Number of edges

class Planet
{
    public:
        string p_name;
        int x,y,z;  // coordinates
        int weight,profit,distance,predecessor;
};

class Edge
{
    public:
        int planet1;
        int planet2;
        int distance;
};


string* strToArr(string line, int size){ // convert string into array
    string* items = new string[size];
    string item;
    char delim = ' ';
    int counter = 0;

    for(int i = 0; i < line.length(); i++){

        if(line[i] == delim){
            items[counter++] = item;
            item = "";
            continue;
        }
        item += line[i];
    }
    items[counter] = item; // add the last item into the list

    return items;
}

void readFile(Planet* planets, string filename){ // to read planet file
    ifstream myfile;
    myfile.open(filename);

    if (myfile.is_open()) {
        string line;
        string* items;
        int counter = 0;

        while (getline(myfile, line)){
            items = strToArr(line, 6);
            planets[counter].p_name = items[0];
            planets[counter].x = stoi(items[1]);
            planets[counter].y = stoi(items[2]);
            planets[counter].z = stoi(items[3]);
            planets[counter].weight = stoi(items[4]);
            planets[counter].profit = stoi(items[5]);
            counter++;
        }
    }
    myfile.close();
}

void readFile(Edge* edges, string filename){// to read edges file
    ifstream myfile;
    myfile.open(filename);
    int size = 0;

    if(myfile.is_open()){
        string line;
        string* items;
        int counter = 0;

        while(getline(myfile, line)){
            items = strToArr(line, 2);

            edges[counter].planet1 = stoi(items[0]);
            edges[counter].planet2 = stoi(items[1]);
            counter++;
        }
    }
    myfile.close();
}

void initmap(char map[9][7]){ // initialise an empty map
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 7; j++)
            map[i][j] = ' ';
    }
}

void map_planets(char map[9][7]){ // locating Planet in the right index
    map[0][3] = 'A';
    map[2][1] = 'C';
    map[2][5] = 'B';
    map[4][2] = 'E';
    map[4][4] = 'F';
    map[6][0] = 'D';
    map[6][2] = 'H';
    map[6][4] = 'J';
    map[6][6] = 'G';
    map[8][3] = 'I';
}

 void connect(char map[9][7], int planet1, int planet2){ // connecting 2 vertices
        if(planet1 > planet2)
            swap(planet1,planet2);

        if(planet1 == 0){     // A
            if(planet2 == 1)  // A - B
                map[1][4] = '\\';
            else              // A- C
                map[1][2] = '/';
        }
        else if(planet1 == 1){ // B
            if(planet2 == 5)   // B - F
                map[3][4] = '/';
            else{              // B - G
                map[2][6] = '+';
                map[3][6] = '|';
                map[4][6] = '|';
                map[5][6] = '|';
            }
        }
        else if(planet1 == 2){ // C
            if(planet2 == 4)   // C - E
                map[3][2] = '\\';
            else{              // C - D
                map[2][0] = '+';
                map[3][0] = '|';
                map[4][0] = '|';
                map[5][0] = '|';
            }
        }
        else if(planet1 == 3){ // D
            if(planet2 == 7)   // D - H
                map[6][1] = '-';
            else{              // D - I
                map[7][0] = '|';
                map[8][0] = '+';
                map[8][1] = '-';
                map[8][2] = '-';
            }
        }
        else if(planet1 == 6){ // G
            if(planet2 == 9)   // G - J
                map[6][5] = '-';
            else{              // G - I
                map[7][6] = '|';
                map[8][6] = '+';
                map[8][5] = '-';
                map[8][4] = '-';
            }
        }
        else if(planet1 == 4){ // E
            if(planet2 == 7)   // E - H
                map[5][2] = '|';
            else               // E - F
                map[4][3] = '-';
        }
        else if(planet1 == 5){ // F
            if(planet2 == 9)   // F - I
                map[5][4] = '|';
        }
        else if(planet1 == 7){ // H
            if(planet2 == 8)   // H - I
                map[7][2] = '\\';
        }
        else if(planet1 == 8){ // J
            if(planet2 == 9)   // J - I
                map[7][4] = '/';
        }
    }

void oriMap(char map[9][7], Edge* edges, int size){
    initmap(map);
    map_planets(map);
    cout << "Original Map :"<<endl;
    for(int i = 0; i < size; i++){
        connect(map, edges[i].planet1, edges[i].planet2);
    }
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 7; j++)
            cout << map[i][j];
        cout << endl;
    }
}

 int calculateDistance(Planet a, Planet b){
                int x = pow((a.x - b.x), 2);
                int y = pow((a.y - b.y), 2);
                int z = pow((a.z - b.z), 2);
                int distance = sqrt(x + y + z);
            return distance;
            }


class Graph
{
    int V;
    public:
    Graph(int V){
        this -> V = V;
    }
    void addEdge(vector<pair<int, int>> adjList[], int u, int vertex, int dist) {
        adjList[u].push_back(make_pair(vertex, dist));
        adjList[vertex].push_back(make_pair(u, dist));
    }


    void insertAdj(vector<pair<int, int>> adjList[], Planet* planets, Edge* edges){
        int index1, index2; // representing planet1 and planet2 in each Edge
        for(int i = 0; i < E_COUNT; i++){
            index1 = edges[i].planet1;
            index2 = edges[i].planet2;
            edges[i].distance = calculateDistance(planets[index1], planets[index2]);
            addEdge(adjList,index1,index2,edges[i].distance);
        }
    }

    void dijkstra(vector<pair<int,int>> adjList[],Planet* planets, char map[9][7],int src,int V){
        // uses priority queue as min heap
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        vector<int> setDist(V, INT_MAX); // set distance from source node are INT_MAX

        pq.push(make_pair(0, src)); // place the source node in the priority queue
        setDist[src] = 0;
        planets[0].predecessor = -1;

        while(!pq.empty()) {
            // get the first shortest distance from the priority queue
            //
            int u = pq.top().second;
            pq.pop();

            for (auto x : adjList[u]) { // for edges connected vertex u
                //x = {vertex, dist}
                int vertex = x.first; // gets the vertex of current adjacent of vertex u
                int dist = x.second; // gets the distance of current adjacent of vertex u
                if (setDist[vertex] > setDist[u] + dist) { // if have shorter path,
                    setDist[vertex] = setDist[u] + dist; // replaces to the shortest
                    pq.push(make_pair(setDist[vertex], vertex));
                    planets[vertex].distance = setDist[vertex];
                    planets[vertex].predecessor = u;
                }
            }
        }

        cout << "\nPlanets list:" << endl;
        for (int i = 0; i < V; i++){
            cout << i
            << ": "
            << planets[i].p_name
            << " ("
            << planets[i].x
            << ", "
            << planets[i].y
            << ", "
            << planets[i].z
            << ")"
            << " Distance = "
            << planets[i].distance
            << " Predecessor = "
            << planets[i].predecessor
            << endl;
            connect(map, i, planets[i].predecessor);
        }
    }

 void displayMap(char map[9][7]){
        map_planets(map);
        cout << "\nShortest Path :"<<endl;
        for(int i = 0; i < 9; i++){
            for(int j = 0; j < 7; j++)
                cout << map[i][j];
            cout << endl;
        }
    }
};

int main()
{
    Planet planets[P_COUNT];
    Edge edges[E_COUNT];
    readFile(planets, "A2planets.txt");
    readFile(edges, "edges.txt");


    char map[9][7];
    oriMap(map,edges,E_COUNT);
    Graph g(P_COUNT);
    vector<pair<int,int>> adjList[P_COUNT];
    initmap(map);
    g.insertAdj(adjList,planets,edges);
    g.dijkstra(adjList,planets,map,0,P_COUNT);
    g.displayMap(map);
    return 0;
}
