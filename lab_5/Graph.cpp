#include "Graph.h"

int Graph::AddVertex(float x, float y)
{
    pair<float, float> add_vertex(x,y);
    if (outgoing.count(add_vertex) == 1) return -1;

    outgoing.emplace(add_vertex, set<pair<float, float>>());
    incoming.emplace(add_vertex, set<pair<float, float>>());

    return 0;
}

int Graph::AddEdge(float x1, float y1, float x2, float y2)
{
    pair<float, float> from(x1, y1);
    pair<float, float> to(x2, y2);

    if (outgoing.count(from) == 0 || outgoing.count(to) == 0) return -1;

    outgoing[from].insert(to);
    incoming[to].insert(from);
    return 0;
}

int Graph::DeleteVertex(float x, float y)
{
    pair<float, float> del_vertex(x,y);
    if (outgoing.count(del_vertex) == 0) return -1;

    for(const auto& to : outgoing[del_vertex]){
      incoming[to].erase(del_vertex);
    }
    for(const auto& from : incoming[del_vertex]){
      outgoing[from].erase(del_vertex);
    }

    outgoing.erase(del_vertex);
    incoming.erase(del_vertex);
    return 0;
}

int Graph::DeleteEdge(float x1, float y1, float x2, float y2)
{
    pair<float, float> from(x1, y1);
    pair<float, float> to(x2, y2);

    if (outgoing.count(from) == 0 || outgoing.count(to) == 0) return -1;

    outgoing[from].erase(to);
    incoming[to].erase(from);
    return 0;
}

void Graph::print(const string& name)
{
    ofstream file;
    string filename = name + ".dot";
    file.open(filename);
    file << "digraph my_graph{" << endl;

    for(const auto& [from, target] : outgoing){
        file <<  "\"" << from.first << ", " << from.second << "\""<< endl;
    }

    for(const auto& [from, target] : outgoing){
        for(auto to : target){
            file <<  "\"" << from.first << ", " << from.second << "\""<< "->" <<  "\""<< to.first << ", " << to.second <<  "\""<< ";"  << endl;
      }
    }
    file << "}";
    file.close();
}

pair<vector<pair<float, float>>, bool> Graph::BFS(float x1, float y1, float x2, float y2)
{
    pair<float, float> from(x1, y1);
    pair<float, float> to(x2, y2);

    bool found = false;
    queue<pair<float, float>> q;
    vector<pair<float, float>> path;

    if (outgoing.count(from) == 0 || outgoing.count(to) == 0) return make_pair(path, found);

    set<pair<float, float>> discovered;
    map<pair<float, float>, pair<float, float>> prev_vertex;

    q.push(from);
    discovered.insert(from);

    while(!q.empty()){
        if (q.front() == to) break;

        for(auto _to : outgoing[q.front()]){
          if(discovered.count(_to) == 0){
            q.push(_to);
            discovered.insert(_to);
            prev_vertex[_to] = q.front();
          }
        }

        q.pop();
    }

    
    if (discovered.count(to) != 0){
        found = true;
        vector<pair<float, float>> rev_path;
        pair<float, float> current = to;
        while(current != from){
            rev_path.push_back(current);
            current = prev_vertex[current];
        }
        path.push_back(from);
        while(!rev_path.empty()){
            path.push_back(rev_path.back());
            rev_path.pop_back();
        }
    }

    return make_pair(path, found);
}

pair<vector<pair<float, float>>, int> Graph::BellmanFord(float x1, float y1, float x2, float y2)
{
    pair<float, float> from(x1, y1);
    pair<float, float> to(x2, y2);
    int path_weight = INT_MAX;
    vector<pair<float, float>> path;

    if (outgoing.count(from) == 0 || outgoing.count(to) == 0) return make_pair(path, path_weight);

    map<pair<float, float>, pair<float, float>> prev_vertex;
    map<pair<float, float>, int> distance;

    for (const auto& [_from, _to] : outgoing){
        distance.emplace(_from, INT_MAX);
    }

    distance[from] = 0;

    for(int i = 0; i < outgoing.size() - 1; ++i){
        for (const auto& [_from, target] : outgoing){
            for(auto _to : target){
                if (distance[_from] != INT_MAX && (distance[_from] + 1) < distance[_to]){
                    distance[_to] = distance[_from] + 1;
                    prev_vertex[_to] = _from;
                }
            }
        }
    }

    if (distance[to] != INT_MAX){
        path_weight = distance[to];
        vector<pair<float, float>> rev_path;
        pair<float, float> current = to;
        while(current != from){
            rev_path.push_back(current);
            current = prev_vertex[current];
        }
        path.push_back(from);
        while(!rev_path.empty()){
            path.push_back(rev_path.back());
            rev_path.pop_back();
        }
    }

    return make_pair(path, path_weight);
}

Graph Graph::ShortestPath(float x, float y)
{
    Graph new_graph;
    pair<float, float> from(x,y);
    if (outgoing.count(from) == 0) return new_graph;

    queue<pair<float, float>> q;
    set<pair<float, float>> discovered;
    map<pair<float, float>, pair<float, float>> prev_vertex;

    q.push(from);
    discovered.insert(from);

    while(!q.empty()){
        for(auto _to : outgoing[q.front()]){
          if(discovered.count(_to) == 0){
            q.push(_to);
            discovered.insert(_to);
            prev_vertex[_to] = q.front();
          }
        }
        q.pop();
    }

    new_graph.AddVertex(x, y);
    for (const auto& [_to, _from] : prev_vertex){
        new_graph.AddVertex(_to.first, _to.second);
    }
    
    for (const auto& [_to, _from] : prev_vertex){
        new_graph.AddEdge(_from.first, _from.second, _to.first, _to.second);
    }

    return new_graph;
    
}

bool Graph::IsEmpty()
{
    return outgoing.empty();
}

void Graph::SaveInFile(const string &name)
{
    ofstream file;
    file.open(name);

    for (const auto& [from, to] : outgoing){
        file << "vertex " << from.first << " " << from.second << endl;
    }

    for (const auto& [from, target] : outgoing){
        for (auto to: target){
            file << "edge " << from.first  << " " << from.second << " " << to.first << " " << to.second << endl;
        }
    }

    file.close();
}
