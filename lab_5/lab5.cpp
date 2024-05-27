#include "Graph.h"
using namespace std;

void menu(Graph& graph);
void print_path(vector<pair<float, float>>& path);
void generate_graph(const string& name, int vertex_num);
void fill_graph(const string& name, Graph& graph);

int main(){
    Graph my_graph;
    generate_graph("my_graph.txt", 20);
    fill_graph("my_graph.txt", my_graph);
    menu(my_graph);
    my_graph.SaveInFile("my_graph.txt");
}

void menu(Graph &graph)
{
    cout << endl;
    cout << "menu:" << endl;
    cout << "1. Add new vertex\n2. Add new edge" << endl;
    cout << "3. Delete vertex\n4. Delete edge\n5. BFS" << endl;
    cout << "6. Bellman-Ford\n7. Find shortest path to all vertices\n8. Print graph" << endl;
    int com;
    cout << "Enter command: ";
    cin >> com;
    if (com == 1){
        float x, y;
        cout << "Enter x: ";
        cin >> x;
        cout << "Enter y: ";
        cin >> y;
        int flag = graph.AddVertex(x, y);
        if (flag == -1) cout << "Vertex with x = " << x << " y = " << y<< " already exists" << endl;
        menu(graph);
    }
    else if (com == 2){
        float x1, y1, x2, y2;
        cout << "Enter first vertex" << endl;
        cout << "Enter x1: ";
        cin >> x1;
        cout << "Enter y1: ";
        cin >> y1;
        cout << "Enter second vertex" << endl;
        cout << "Enter x2: ";
        cin >> x2;
        cout << "Enter y2: ";
        cin >> y2;
        int flag = graph.AddEdge(x1, y1, x2, y2);
        if (flag == -1) cout << "No such vertex" << endl;
        menu(graph);
    }
    else if (com == 3){
        float x, y;
        cout << "Enter x: ";
        cin >> x;
        cout << "Enter y: ";
        cin >> y;
        int flag = graph.DeleteVertex(x, y);
        if (flag == -1) cout << "No such vertex x = " << x << " y = " << y << endl;
        menu(graph);
    }
    else if (com == 4){
        float x1, y1, x2, y2;
        cout << "Enter first vertex" << endl;
        cout << "Enter x1: ";
        cin >> x1;
        cout << "Enter y1: ";
        cin >> y1;
        cout << "Enter second vertex" << endl;
        cout << "Enter x2: ";
        cin >> x2;
        cout << "Enter y2: ";
        cin >> y2;
        int flag = graph.DeleteEdge(x1, y1, x2, y2);
        if (flag == -1) cout << "No such vertex" << endl;
        menu(graph);
    }
    else if (com == 5){
        float x1, y1, x2, y2;
        cout << "Enter first vertex" << endl;
        cout << "Enter x1: ";
        cin >> x1;
        cout << "Enter y1: ";
        cin >> y1;
        cout << "Enter second vertex" << endl;
        cout << "Enter x2: ";
        cin >> x2;
        cout << "Enter y2: ";
        cin >> y2;

        pair<vector<pair<float, float>>, bool> result = graph.BFS(x1, y1, x2, y2);

        if (result.second == false) cout << "Path does not exist" << endl;
        else print_path(result.first);

        menu(graph);

    }
    else if (com == 6){
        float x1, y1, x2, y2;
        cout << "Enter first vertex" << endl;
        cout << "Enter x1: ";
        cin >> x1;
        cout << "Enter y1: ";
        cin >> y1;
        cout << "Enter second vertex" << endl;
        cout << "Enter x2: ";
        cin >> x2;
        cout << "Enter y2: ";
        cin >> y2;

        pair<vector<pair<float, float>>, int> result = graph.BellmanFord(x1, y1, x2, y2);

        if (result.second == INT_MAX) cout << "Path does not exist" << endl;
        else{
            cout << "path weight = " << result.second << endl;
            print_path(result.first);
        }

        menu(graph);
    }
    else if (com == 7){
        float x, y;
        cout << "Enter x: ";
        cin >> x;
        cout << "Enter y: ";
        cin >> y;

        Graph result = graph.ShortestPath(x, y);
        if (result.IsEmpty()) cout << "No such vertex" << endl;
        else{
            cout << "Enter name of file: ";
            string name;
            cin >> name;
            result.print(name);
        }

        menu(graph);
    }
    else if (com == 8){
        cout << "Enter name of file: ";
        string name;
        cin >> name;
        graph.print(name);

        menu(graph);
    }
}

void print_path(vector<pair<float, float>> &path)
{
    cout << "("<< path[0].first << "," << path[0].second << ")";
    for (int i = 1; i < path.size(); ++i){
        cout << " -> ";
        cout << "("<< path[i].first << "," << path[i].second << ")";
    }
}

void generate_graph(const string &name, int vertex_num)
{
    ofstream file;
    file.open(name);
    for (int i = 0; i < vertex_num; ++i){
        file << "vertex " << i << " " << i << endl;
    }

    int start = 0;
    int end = vertex_num;

    for (int i = 0; i < vertex_num; ++i){
        for (int j = 0; j <  rand() % (3 + 1) ; ++j){
            int  v = rand() % (end - start + 1) + start;
            file << "edge " << i << " " << i << " " << v << " " << v << endl;
        }
    }

    file.close();
}

void fill_graph(const string &name, Graph& graph)
{
  ifstream file; 
  file.open(name);
  float x1, y1, x2, y2;
  string com;
  
  while(file >> com){
    if(com == "vertex"){
      file >> x1;
      file >> y1;
      graph.AddVertex(x1, y1);
    }
    if(com == "edge"){
      file >> x1;
      file >> y1;
      file >> x2;
      file >> y2;
      graph.AddEdge(x1, y1, x2, y2);
    }
  }
  file.close();
}

