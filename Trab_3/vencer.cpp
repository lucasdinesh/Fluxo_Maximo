#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <tuple>
#include <limits>
#include <cassert>

using namespace std;

const int INF = numeric_limits<int>::max();

struct Edge {
    int to, rev;
    int capacity;
};

class MaxFlow {
    vector<vector<Edge>> graph;
public:
    MaxFlow(int n) : graph(n) {}

    void addEdge(int from, int to, int capacity) {
        graph[from].push_back({to, (int)graph[to].size(), capacity});
        graph[to].push_back({from, (int)graph[from].size() - 1, 0});
    }

    int bfs(int s, int t, vector<int>& parent, vector<int>& parentEdge) {
        int n = graph.size();
        vector<bool> visited(n, false);
        queue<int> q;
        q.push(s);
        visited[s] = true;
        parent[s] = -1;

        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int i = 0; i < graph[u].size(); ++i) {
                Edge& e = graph[u][i];
                if (!visited[e.to] && e.capacity > 0) {
                    visited[e.to] = true;
                    parent[e.to] = u;
                    parentEdge[e.to] = i;
                    if (e.to == t) return true;
                    q.push(e.to);
                }
            }
        }
        return false;
    }

    int edmondsKarp(int s, int t) {
        int flow = 0;
        int n = graph.size();
        vector<int> parent(n), parentEdge(n);
        while (bfs(s, t, parent, parentEdge)) {
            int path_flow = INF;
            for (int v = t; v != s; v = parent[v]) {
                int u = parent[v];
                path_flow = min(path_flow, graph[u][parentEdge[v]].capacity);
            }
            for (int v = t; v != s; v = parent[v]) {
                int u = parent[v];
                Edge &e = graph[u][parentEdge[v]];
                e.capacity -= path_flow;
                graph[v][e.rev].capacity += path_flow;
            }
            flow += path_flow;
        }
        return flow;
    }
};

int main() {
    int n;
    cin >> n;
    vector<int> w(n); // vitórias
    for (int i = 0; i < n; ++i) cin >> w[i];

    vector<vector<int>> g(n, vector<int>(n, 0));
    // Preencher a matriz de resultados dos jogos
    for (int i = 0; i < n; ++i)
        for (int j = i + 1; j < n; ++j)
            cin >> g[i][j];

    // Preencher a parte inferior da matriz com os valores simétricos
    for (int i = 0; i < n; ++i)
        for (int j = i + 1; j < n; ++j)
            g[j][i] = g[i][j];

    // Cálculo das vitórias totais possíveis
    int maxWins = 0;
    for (int j = 1; j < n; ++j)
        maxWins += g[0][j];

    // Verificar se a equipe 1 já é impossibilitada de vencer
    for (int i = 1; i < n; ++i) {
        int maxPossibleWins = w[i] + maxWins;
        if (w[i] > w[0] + maxWins) {  // Se alguma outra equipe pode vencer a equipe 1
            cout << "não" << endl;
            return 0;
        }
    }

    int s = 0, t = 1;
    int id = 2;
    map<pair<int, int>, int> gameNode;
    vector<int> teamNode(n, -1);
    for (int i = 1; i < n; ++i)
        teamNode[i] = id++;

    for (int i = 1; i < n; ++i)
        for (int j = i + 1; j < n; ++j)
            if (g[i][j] > 0)
                gameNode[{i, j}] = id++;

    MaxFlow mf(id);
    int totalGames = 0;

    // Conectando as arestas do fluxo
    // s → jogo
    for (auto& [pair, node] : gameNode) {
        int i = pair.first, j = pair.second;
        int cap = g[i][j];
        mf.addEdge(s, node, cap);
        totalGames += cap;
        mf.addEdge(node, teamNode[i], INF);
        mf.addEdge(node, teamNode[j], INF);
    }

    // time → t
    for (int i = 1; i < n; ++i) {
        int mi = maxWins - w[i];
        mf.addEdge(teamNode[i], t, mi);
    }

    int flow = mf.edmondsKarp(s, t);

    cout << (flow == totalGames ? "sim" : "não") << endl;
    return 0;
}
