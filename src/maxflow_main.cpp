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
#include <unordered_set>
#include <stack>
#include <functional>
#include <sys/resource.h> // Para uso de memória (Linux)

using namespace std;


// Structure to represent an edge
struct Edge {
    int to;
    int capacity;
    int flow;
    Edge* reverse;

    Edge(int t, int c) : to(t), capacity(c), flow(0), reverse(nullptr) {}
};

// Structure to represent a graph
class Graph {
public:
    int num_vertices;
    vector<vector<Edge*>> adj;
    map<pair<int, int>, Edge*> edge_map;

    Graph(int n) : num_vertices(n), adj(n) {}

    void add_edge(int u, int v, int capacity) {
        Edge* edge = new Edge(v, capacity);
        Edge* reverse_edge = new Edge(u, 0);  // Aresta residual
        edge->reverse = reverse_edge;
        reverse_edge->reverse = edge;
    
        adj[u].push_back(edge);
        adj[v].push_back(reverse_edge);
    
        // Mapeia ambas as direções
        edge_map[{u, v}] = edge;
        edge_map[{v, u}] = reverse_edge;
    }

    Edge* find_edge(int u, int v) {
        auto it = edge_map.find({u, v});
        if (it != edge_map.end()) {
            return it->second;
        }
        return nullptr;
    }

    ~Graph() {
        // Usamos um set para evitar dupla liberação
        std::unordered_set<Edge*> deleted_edges;
        
        for (int u = 0; u < num_vertices; ++u) {
            for (Edge* edge : adj[u]) {
                if (deleted_edges.find(edge) == deleted_edges.end()) {
                    deleted_edges.insert(edge);
                    deleted_edges.insert(edge->reverse);
                    delete edge;
                }
            }
            adj[u].clear();
        }
        edge_map.clear();
    }
};

// Function to read the graph from the file
Graph* read_graph(const string& filename, int& source, int& sink) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error opening file: " << filename << endl;
        return nullptr;
    }

    string line;
    int num_vertices = 0;
    int num_edges = 0;

    // Primeira passagem: encontrar número de vértices, source e sink
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

    if (source == sink || source < 0 || sink < 0 || source >= num_vertices || sink >= num_vertices) {
        cerr << "Invalid source/sink nodes: s=" << source << " t=" << sink << " n=" << num_vertices << endl;
        return nullptr;
    }

    // Cria o grafo
    Graph* graph = new Graph(num_vertices);

    // Segunda passagem: adicionar as arestas
    file.clear();              // limpa estado EOF
    file.seekg(0);             // volta ao início

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

// Versão otimizada do BFS para busca de caminho mais curto
bool optimized_bfs(Graph* graph, int source, int sink, vector<int>& path, long long& operations) {
    int n = graph->num_vertices;
    vector<int> parent(n, -1);
    vector<bool> visited(n, false);
    queue<int> q;

    visited[source] = true;
    q.push(source);
    operations++;

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        operations++;
        
        // Parar assim que encontrar o sink
        if (u == sink)
            break;
            
        for (Edge* edge : graph->adj[u]) {
            operations++;
            int v = edge->to;
            int residual = edge->capacity - edge->flow;
            
            if (residual > 0 && !visited[v]) {
                visited[v] = true;
                parent[v] = u;
                q.push(v);
                operations++;
                
                if (v == sink)
                    break;
            }
        }
    }

    if (parent[sink] == -1) {
        return false;
    }

    // Reconstruir o caminho
    int current = sink;
    while (current != -1) {
        path.push_back(current);
        current = parent[current];
        operations++;
    }
    reverse(path.begin(), path.end());
    return true;
}

// Versão otimizada do DFS randomizado
bool optimized_dfs(Graph* graph, int source, int sink, vector<int>& path, long long& operations) {
    int n = graph->num_vertices;
    static vector<bool> visited(n); // Usar variável estática para evitar realocações
    
    // Limpar vetor de visitados apenas nas posições necessárias
    for (int i = 0; i < n; i++) {
        visited[i] = false;
    }
    
    vector<int> parent(n, -1);
    
    // Usar DFS iterativo em vez de recursivo para evitar overhead de chamadas de função
    stack<int> s;
    s.push(source);
    visited[source] = true;
    operations++;
    
    while (!s.empty()) {
        int u = s.top();
        s.pop();
        operations++;
        
        if (u == sink) {
            break;
        }
        
        // Pré-processar e ordenar arestas
        vector<pair<int, int>> next_vertices; // {capacidade residual, vértice}
        next_vertices.reserve(graph->adj[u].size());
        
        for (Edge* edge : graph->adj[u]) {
            operations++;
            int v = edge->to;
            int residual = edge->capacity - edge->flow;
            
            if (residual > 0 && !visited[v]) {
                next_vertices.emplace_back(residual, v);
                parent[v] = u;
            }
        }
        
        // Ordenar por capacidade residual (maior primeiro)
        if (next_vertices.size() > 1) {
            sort(next_vertices.begin(), next_vertices.end(), 
                 [](const auto& a, const auto& b) { return a.first > b.first; });
        }
        
        // Empilhar vértices em ordem inversa para explorar primeiro os de maior capacidade
        for (auto it = next_vertices.rbegin(); it != next_vertices.rend(); ++it) {
            int v = it->second;
            visited[v] = true;
            s.push(v);
        }
    }
    
    // Verificar se encontrou caminho até o sink
    if (parent[sink] == -1) {
        return false;
    }
    
    // Reconstruir o caminho
    path.clear();
    int current = sink;
    while (current != -1) {
        path.push_back(current);
        current = parent[current];
        operations++;
    }
    reverse(path.begin(), path.end());
    
    return true;
}

// Versão otimizada do algoritmo para encontrar caminho "mais gordo"
bool optimized_fattest_path(Graph* graph, int source, int sink, vector<int>& path, long long& operations) {
    int n = graph->num_vertices;
    vector<int> parent(n, -1);
    vector<int> capacity(n, 0);
    vector<bool> visited(n, false);
    
    // Usar priority_queue para sempre escolher o caminho mais gordo
    priority_queue<pair<int, int>> pq; // {capacidade, vértice}
    
    capacity[source] = INT_MAX;
    pq.push({INT_MAX, source});
    operations++;
    
    while (!pq.empty() && !visited[sink]) {
        int u = pq.top().second;
        pq.pop();
        operations++;
        
        if (visited[u]) continue;
        visited[u] = true;
        
        for (Edge* edge : graph->adj[u]) {
            operations++;
            int v = edge->to;
            int residual = edge->capacity - edge->flow;
            
            if (residual > 0 && !visited[v]) {
                int new_capacity = min(capacity[u], residual);
                if (new_capacity > capacity[v]) {
                    capacity[v] = new_capacity;
                    parent[v] = u;
                    pq.push({new_capacity, v});
                }
            }
        }
    }
    
    if (!visited[sink]) {
        return false;
    }
    
    // Reconstruir o caminho
    int current = sink;
    while (current != -1) {
        path.push_back(current);
        current = parent[current];
        operations++;
    }
    reverse(path.begin(), path.end());
    return true;
}

// Função para encontrar o bottleneck
int find_bottleneck(Graph* graph, const vector<int>& path, long long& operations) {
    int bottleneck = INT_MAX;
    for (size_t i = 0; i < path.size() - 1; ++i) {
        Edge* edge = graph->find_edge(path[i], path[i + 1]);
        operations++;
        if (edge) {
            bottleneck = min(bottleneck, edge->capacity - edge->flow);
        }
    }
    return bottleneck;
}

// Função para atualizar o fluxo ao longo do caminho
void update_flow(Graph* graph, const vector<int>& path, int flow, long long& operations) {
    for (size_t i = 0; i < path.size() - 1; ++i) {
        int u = path[i];
        int v = path[i+1];
        
        Edge* edge = graph->find_edge(u, v);
        if (!edge) {
            cerr << "ERRO CRÍTICO: Aresta " << u << "->" << v << " não encontrada!" << endl;
            continue;
        }

        edge->flow += flow;
        if (edge->reverse) {
            edge->reverse->flow -= flow;
        }
        operations++;
    }
}

// Ford-Fulkerson algorithm simplificado
int ford_fulkerson(Graph* graph, int source, int sink,
    std::function<bool(Graph*, int, int, std::vector<int>&, long long&)> find_path,
    long long& iterations, long long& operations)   {
         int max_flow = 0;
    vector<int> path;
    
    while (true) {
        path.clear();
        
        if (!find_path(graph, source, sink, path, operations)) {
            break;
        }
        
        iterations++;
        
        int bottleneck = find_bottleneck(graph, path, operations);
        update_flow(graph, path, bottleneck, operations);
        max_flow += bottleneck;
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

    cout << "Informações do grafo: " << endl;
    cout << "Número de vértices: " << graph->num_vertices << endl;
    cout << "Source: " << source << ", Sink: " << sink << endl;
    cout << "---------------------------------------------" << endl;

    // Definir os algoritmos a executar
    map<string, bool (*)(Graph*, int, int, vector<int>&, long long&)> algorithms = {
        {"bfs", optimized_bfs},
        {"dfs", optimized_dfs},
        {"fattest_path", optimized_fattest_path}
    };

    // Executar e comparar algoritmos
    for (const auto& pair : algorithms) {
        string algorithm_type = pair.first;
        cout << "Executando algoritmo: " << algorithm_type << endl;

        // Resetar o fluxo
        for (int u = 0; u < graph->num_vertices; ++u) {
            for (Edge* edge : graph->adj[u]) {
                edge->flow = 0;
            }
        }

        long long iterations = 0;
        long long operations = 0;
        long long total_path_length = 0;

        // Envolve o algoritmo de busca para medir o tamanho médio dos caminhos
        auto wrapped_find_path = [&](Graph* g, int s, int t, vector<int>& path, long long& ops) -> bool {
            bool found = pair.second(g, s, t, path, ops);
            if (found) total_path_length += path.size();
            return found;
        };

        auto start_time = chrono::high_resolution_clock::now();
        int max_flow = ford_fulkerson(graph, source, sink, wrapped_find_path, iterations, operations);
        auto end_time = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);

        double avg_path_length = (iterations > 0) ? static_cast<double>(total_path_length) / iterations : 0.0;
        double avg_time_per_iter = (iterations > 0) ? static_cast<double>(duration.count()) / iterations : 0.0;

        // Medir uso máximo de memória (Linux)
        struct rusage usage;
        getrusage(RUSAGE_SELF, &usage);
        long peak_memory_kb = usage.ru_maxrss; // Em kilobytes

        cout << "Resultados para " << algorithm_type << ":" << endl;
        cout << "Fluxo máximo: " << max_flow << endl;
        cout << "Tempo de execução: " << duration.count() << " milissegundos" << endl;
        cout << "Número de iterações: " << iterations << endl;
        cout << "Número de operações: " << operations << endl;
        cout << "Comprimento médio dos caminhos aumentantes: " << avg_path_length << endl;
        cout << "Tempo médio por iteração: " << avg_time_per_iter << " ms" << endl;
        cout << "Uso máximo de memória: " << peak_memory_kb << " KB" << endl;
        cout << "---------------------------------------------" << endl;
    }

    delete graph;
    return 0;
}