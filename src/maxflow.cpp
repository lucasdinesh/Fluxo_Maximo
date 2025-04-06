#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>
#include <random>
#include <string>
#include <sstream>
#include <map>
#include <chrono>

using namespace std;

// Structure to represent an edge
struct Edge {
    int to;
    int capacity;
    int flow;
    Edge* reverse;
};

// Structure to represent a graph
class Graph {
public:
    int num_vertices;
    vector<vector<Edge*>> adj;
    map<pair<int, int>, Edge*> edge_map;

    Graph(int n) : num_vertices(n), adj(n) {}

    void add_edge(int u, int v, int capacity) {
        Edge* edge = new Edge{v, capacity, 0, nullptr};
        Edge* reverse_edge = new Edge{u, 0, 0, edge};
        edge->reverse = reverse_edge;
        adj[u].push_back(edge);
        adj[v].push_back(reverse_edge);
        edge_map[{u, v}] = edge;
    }

    Edge* find_edge(int u, int v) {
        auto it = edge_map.find({u, v});
        if (it != edge_map.end()) {
            return it->second;
        }
        return nullptr;
    }
};

// Function to read the graph from the file
Graph* read_graph(const string& filename, int& source, int& sink) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return nullptr;
    }

    string line;
    int num_vertices = 0;
    int num_edges = 0;
    while (getline(file, line)) {
        stringstream ss(line);
        char type;
        ss >> type;
        if (type == 'p') {
            string problem_type;
            ss >> problem_type >> num_vertices >> num_edges;
        } else if (type == 'n') {
            int node_id;
            char node_type;
            ss >> node_id >> node_type;
            if (node_type == 's') {
                source = node_id - 1;
            } else if (node_type == 't') {
                sink = node_id - 1;
            }
        }
    }

    Graph* graph = new Graph(num_vertices);
    file.clear();
    file.seekg(0);

    while (getline(file, line)) {
        stringstream ss(line);
        char type;
        ss >> type;
        if (type == 'a') {
            int u, v, capacity;
            ss >> u >> v >> capacity;
            graph->add_edge(u - 1, v - 1, capacity);
        }
    }

    file.close();
    return graph;
}

// Function to find the fattest path
bool fattest_path(Graph* graph, int source, int sink, vector<int>& path, long long& operations) {
    int n = graph->num_vertices;
    vector<int> parent(n, -1);
    vector<int> capacity(n, 0);
    vector<bool> visited(n, false);
    queue<int> q;

    capacity[source] = INT_MAX;
    visited[source] = true;
    q.push(source);
    operations++; // Visit source

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        operations++; // Dequeue operation
        for (Edge* edge : graph->adj[u]) {
            operations++; // Edge touched
            if (edge->capacity - edge->flow > 0 && !visited[edge->to]) {
                visited[edge->to] = true;
                parent[edge->to] = u;
                capacity[edge->to] = min(capacity[u], edge->capacity - edge->flow);
                q.push(edge->to);
                operations++; // Enqueue operation
            }
        }
    }

    if (!visited[sink]) {
        return false;
    }

    int current = sink;
    while (current != -1) {
        path.push_back(current);
        current = parent[current];
        operations++; // Path construction
    }
    reverse(path.begin(), path.end());
    return true;
}

// Function to find the shortest path (BFS)
bool shortest_path_bfs(Graph* graph, int source, int sink, vector<int>& path, long long& operations) {
    int n = graph->num_vertices;
    vector<int> parent(n, -1);
    vector<bool> visited(n, false);
    queue<int> q;

    visited[source] = true;
    q.push(source);
    operations++; // Visit source

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        operations++; // Dequeue operation
        for (Edge* edge : graph->adj[u]) {
            operations++; // Edge touched
            if (edge->capacity - edge->flow > 0 && !visited[edge->to]) {
                visited[edge->to] = true;
                parent[edge->to] = u;
                q.push(edge->to);
                operations++; // Enqueue operation
            }
        }
    }

    if (!visited[sink]) {
        return false;
    }

    int current = sink;
    while (current != -1) {
        path.push_back(current);
        current = parent[current];
        operations++; // Path construction
    }
    reverse(path.begin(), path.end());
    return true;
}

// Function to find a path using randomized DFS
bool randomized_dfs(Graph* graph, int u, int sink, vector<bool>& visited, vector<int>& path, long long& operations) {
    visited[u] = true;
    path.push_back(u);
    operations++; // Visit node

    if (u == sink) {
        return true;
    }

    vector<int> neighbors;
    for (Edge* edge : graph->adj[u]) {
        operations++; // Edge touched
        if (edge->capacity - edge->flow > 0 && !visited[edge->to]) {
            neighbors.push_back(edge->to);
        }
    }

    if (neighbors.empty()) {
        path.pop_back();
        return false;
    }

    random_device rd;
    mt19937 g(rd());
    shuffle(neighbors.begin(), neighbors.end(), g);

    for (int v : neighbors) {
        if (randomized_dfs(graph, v, sink, visited, path, operations)) {
            return true;
        }
    }

    path.pop_back();
    return false;
}

bool randomized_dfs_wrapper(Graph* graph, int source, int sink, vector<int>& path, long long& operations) {
    int n = graph->num_vertices;
    vector<bool> visited(n, false);
    return randomized_dfs(graph, source, sink, visited, path, operations);
}

// Function to find the bottleneck capacity
int find_bottleneck(Graph* graph, const vector<int>& path, long long& operations) {
    int bottleneck = INT_MAX;
    for (size_t i = 0; i < path.size() - 1; ++i) {
        Edge* edge = graph->find_edge(path[i], path[i + 1]);
        operations++; // Edge touched
        if (edge) {
            bottleneck = min(bottleneck, edge->capacity - edge->flow);
        }
    }
    return bottleneck;
}

// Function to update the flow along the path
void update_flow(Graph* graph, const vector<int>& path, int flow, long long& operations) {
    for (size_t i = 0; i < path.size() - 1; ++i) {
        Edge* edge = graph->find_edge(path[i], path[i + 1]);
        operations++; // Edge touched
        if (edge) {
            edge->flow += flow;
            edge->reverse->flow -= flow;
        }
    }
}

// Ford-Fulkerson algorithm
int ford_fulkerson(Graph* graph, int source, int sink, bool (*find_path)(Graph*, int, int, vector<int>&, long long&), long long& iterations, long long& operations) {
    int max_flow = 0;
    vector<int> path;
    while (find_path(graph, source, sink, path, operations)) {
        iterations++;
        int bottleneck = find_bottleneck(graph, path, operations);
        update_flow(graph, path, bottleneck, operations);
        max_flow += bottleneck;
        path.clear();
    }
    return max_flow;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <input_file>" << endl;
        return 1;
    }

    string filename = argv[1];
    int source, sink;
    Graph* graph = read_graph(filename, source, sink);

    if (!graph) {
        return 1;
    }

    // Define the algorithms to run
    map<string, bool (*)(Graph*, int, int, vector<int>&, long long&)> algorithms = {
        {"fattest", fattest_path},
        {"bfs", shortest_path_bfs},
        {"dfs", randomized_dfs_wrapper}
    };

    for (const auto& pair : algorithms) {
        string algorithm_type = pair.first;
        bool (*find_path)(Graph*, int, int, vector<int>&, long long&) = pair.second;

        // Reset the flow for each algorithm
        for (int u = 0; u < graph->num_vertices; ++u) {
            for (Edge* edge : graph->adj[u]) {
                edge->flow = 0;
            }
        }

        long long iterations = 0;
        long long operations = 0;

        // Time the execution
        auto start_time = chrono::high_resolution_clock::now();
        int max_flow = ford_fulkerson(graph, source, sink, find_path, iterations, operations);
        auto end_time = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);

        cout << "Algorithm: " << algorithm_type << endl;
        cout << "Maximum flow: " << max_flow << endl;
        cout << "Execution time: " << duration.count() << " microseconds" << endl;
        cout << "Number of iterations: " << iterations << endl;
        cout << "Number of operations: " << operations << endl;
        cout << endl;
    }

    return 0;
}
