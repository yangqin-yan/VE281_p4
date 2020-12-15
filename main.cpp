#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <sstream>

using namespace std;



class Node{
private:
    int distance = INT_MAX;
    bool visited = false;
    int predecessor = INT_MAX;

public:
    int node_id = 0;
    Node() = default;
    ~Node() = default;
    int get_distance();
    bool get_visited();
    int get_predecessor();
    void set_distance(int d);
    void set_visited(bool v);
    void set_predecessor(int n);
};

int Node::get_distance() {
    return distance;
}

bool Node::get_visited() {
    return visited;
}

int Node::get_predecessor() {
    return predecessor;
}

void Node::set_distance(int d) {
    distance = d;
}

void Node::set_predecessor(int n) {
    predecessor = n;
}

void Node::set_visited(bool v) {
    visited = v;
}

class Graph{
public:
    int num_nodes;
    vector<vector<int>> dis_matrix;
    Node *node;
    Graph(int num = 0);
    ~Graph();
    void insert_edge(int start_node, int end_node, int distance);
    void print_matrix();
    void print_nodes();
};


Graph::Graph(int num): num_nodes(num), node(new Node[num_nodes])
{
    vector<int> temp(num_nodes, INT_MAX);
    dis_matrix.resize(num_nodes, temp);
    for(int i = 0; i < num_nodes; i++){
        node[i].node_id = i;
    }
}

Graph::~Graph() {
    delete[] node;
}


void Graph::insert_edge(int start_node, int end_node, int distance) {
    if(dis_matrix[start_node][end_node] > distance) {
        dis_matrix[start_node][end_node] = distance;
        // node[end_node].set_distance(distance);
        // node[end_node].set_predecessor(start_node);
    }
}

void Graph::print_matrix() {
    for(int i = 0; i < dis_matrix.size(); i++){
        for(int j = 0; j < dis_matrix.size(); j++){
            cout << dis_matrix[i][j] << " ";
        }
        cout << endl;
    }
}

void Graph::print_nodes() {
    for(int i = 0; i < num_nodes; i++){
        cout << "Node " << i << " " << "distance: " << node[i].get_distance() << " "
        << "visited: " << node[i].get_visited() << " "
        << "predecessor: " << node[i].get_predecessor() << endl;
    }
}

class cmp{
public:
    bool operator()(Node & n1, Node & n2){
        return n1.get_distance() > n2.get_distance();
    }
};

int find_shortest_path(Graph &graph, int source, int destination){
    priority_queue<Node, vector<Node>, cmp> queue;
    graph.node[source].set_distance(0);
    // graph.print_nodes();
    for(int i = 0; i < graph.num_nodes; i++){
        queue.push(graph.node[i]);
    }
    while(!queue.empty()){
        const Node &temp = queue.top();
        for(int j = 0; j < graph.num_nodes; j++){
            if(graph.dis_matrix[temp.node_id][j] < INT_MAX){
                if(graph.node[j].get_distance() > graph.dis_matrix[temp.node_id][j] + graph.node[temp.node_id].get_distance()){
                    graph.node[j].set_distance(graph.dis_matrix[temp.node_id][j] + graph.node[temp.node_id].get_distance());
                    graph.node[j].set_predecessor(temp.node_id);
                    graph.node[j].set_visited(true);
                }
            }
        }
        queue.pop();
    }
    return graph.node[destination].get_distance();
}


// return true if the graph is a DAG, return false otherwise.
bool topological_sort(Graph &graph){
    int *incoming_table = new int[graph.num_nodes];

    // compute the in-degrees of all nodes
    for(int i = 0; i < graph.num_nodes; i++){
        incoming_table[i] = 0;
        for(int j = 0; j < graph.num_nodes; j++){
            if(graph.dis_matrix[j][i] < INT_MAX){
                incoming_table[i]++;
            }
        }
    }

    queue<int> q;
    for(int i = 0; i < graph.num_nodes; i++){
        if(incoming_table[i] == 0){
            q.push(i);
        }
    }

    while(!q.empty()){
        int v = q.front();
        for(int i = 0; i < graph.num_nodes; i++){
            if(graph.dis_matrix[v][i] < INT_MAX){
                incoming_table[i]--;
                if(incoming_table[i] == 0){
                    q.push(i);
                }
            }
        }
        q.pop();
    }

    for(int i = 0; i < graph.num_nodes; i++){
        if(incoming_table[i] != 0){
            delete[] incoming_table;
            return false;
        }
    }

    delete[] incoming_table;
    return true;
}


// Return the total weight of MST if MST exists in the graph, otherwise return -1
int MST(Graph &graph){

    vector<int> set1, set2;
    graph.node[0].set_predecessor(0);
    graph.node[0].set_distance(0);
    for(int i = 0; i < graph.num_nodes; i++){
        set2.push_back(i);
    }

    while(!set2.empty()){
        int minimum = INT_MAX;
        int node_id = INT_MAX;
        for(int i = 0; i < set2.size(); i++){
            if(minimum > graph.node[set2[i]].get_distance()){
                minimum = graph.node[set2[i]].get_distance();
                node_id = i;
            }
        }
        if(node_id == INT_MAX){
            return -1;
        }
        set1.push_back(set2[node_id]);
        set2.erase(set2.begin() + node_id);
        for(int i = 0; i < set2.size(); i++){
            if(graph.dis_matrix[set2[i]][set1[set1.size() - 1]] < INT_MAX){
                if(graph.node[set2[i]].get_distance() > graph.dis_matrix[set2[i]][set1[set1.size() - 1]]){
                    graph.node[set2[i]].set_distance(graph.dis_matrix[set2[i]][set1[set1.size() - 1]]);
                    graph.node[set2[i]].set_predecessor(set1[set1.size() - 1]);
                }
            }
        }
    }

    int total = 0;
    for(int i = 0; i < graph.num_nodes; i++){
        total += graph.node[i].get_distance();
    }
    return total;
}


int main() {
    int num_nodes, source_node, destination_node, num_edges;
    cin >> num_nodes;
    cin >> source_node;
    cin >> destination_node;
//    cin >> num_edges;
    Graph g_direct(num_nodes), g_undirect(num_nodes);
//    for(int i = 0; i < num_edges; i++){
//        int start_node, end_node, weight;
//        cin >> start_node >> end_node >> weight;
//        g_direct.insert_edge(start_node, end_node, weight);
//        g_undirect.insert_edge(start_node, end_node, weight);
//        g_undirect.insert_edge(end_node, start_node, weight);
//    }

    string str;
    while(getline(cin, str)){
        int start_node, end_node, weight;
        stringstream is(str);
        is >> start_node >> end_node >> weight;
        g_direct.insert_edge(start_node, end_node, weight);
        g_undirect.insert_edge(start_node, end_node, weight);
        g_undirect.insert_edge(end_node, start_node, weight);
    }

    int shortest_length = find_shortest_path(g_direct, source_node, destination_node);

    cout << "Shortest path length is " << shortest_length << endl;
    if(topological_sort(g_direct)){
        cout << "The graph is a DAG" << endl;
    }
    else{
        cout << "The graph is not a DAG" << endl;
    }

    int total_weight = MST(g_undirect);
    if(total_weight == -1){
        cout << "No MST exists!" << endl;
    }
    else{
        cout << "The total weight of MST is " << total_weight << endl;
    }
    /// test
    g_direct.print_nodes();
    cout << "\n";
    g_direct.print_matrix();
    return 0;
}