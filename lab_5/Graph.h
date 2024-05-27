#include <vector>
#include <stdexcept>
#include <iostream>
#include <map>
#include <set>
#include <fstream>
#include <queue>
#include <limits.h>

using namespace std;

struct Graph{
private:
    map<pair<float,float>, set<pair<float, float>>> outgoing;
    map<pair<float,float>, set<pair<float, float>>> incoming;
public:
    int AddVertex(float x, float y);
    int AddEdge(float x1, float y1, float x2, float y2);
    int DeleteVertex(float x, float y);
    int DeleteEdge(float x1, float y1, float x2, float y2);
    void print(const string& name);
    pair<vector<pair<float, float>>, bool> BFS(float x1, float y1, float x2, float y2);
    pair<vector<pair<float, float>>, int> BellmanFord(float x1, float y1, float x2, float y2);
    Graph ShortestPath(float x, float y);
    bool IsEmpty();
    void SaveInFile(const string &name);

};