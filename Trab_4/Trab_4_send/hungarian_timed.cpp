#include <bits/stdc++.h>
using namespace std;

using namespace std::chrono;

const int INF = 1e9;

int main() {
    int n;
    cin >> n;

    vector<vector<int>> cost(n, vector<int>(n));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            cin >> cost[i][j];

    int expected_result;
    cin >> expected_result; // Não usamos, mas mantemos a leitura.

    vector<int> u(n + 1), vpot(n + 1), p(n + 1), way(n + 1);

    auto start_total = high_resolution_clock::now();

    vector<long long> busca_durations;

    for (int i = 1; i <= n; ++i) {
        p[0] = i;
        vector<int> minv(n + 1, INF);
        vector<bool> used(n + 1, false);
        int j0 = 0;

        auto start_busca = high_resolution_clock::now();

        do {
            used[j0] = true;
            int i0 = p[j0], delta = INF, j1 = 0;

            for (int j = 1; j <= n; ++j) {
                if (!used[j]) {
                    int cur = cost[i0 - 1][j - 1] - u[i0] - vpot[j];
                    if (cur < minv[j]) {
                        minv[j] = cur;
                        way[j] = j0;
                    }
                    if (minv[j] < delta) {
                        delta = minv[j];
                        j1 = j;
                    }
                }
            }

            for (int j = 0; j <= n; ++j) {
                if (used[j]) {
                    u[p[j]] += delta;
                    vpot[j] -= delta;
                } else {
                    minv[j] -= delta;
                }
            }
            j0 = j1;
        } while (p[j0] != 0);

        auto stop_busca = high_resolution_clock::now();
        auto duration_busca = duration_cast<microseconds>(stop_busca - start_busca);
        busca_durations.push_back(duration_busca.count());

        do {
            int j1 = way[j0];
            p[j0] = p[j1];
            j0 = j1;
        } while (j0);
    }

    auto stop_total = high_resolution_clock::now();
    auto duration_total = duration_cast<microseconds>(stop_total - start_total);

    int result = -vpot[0];

    cout << result << endl;

    cerr << fixed << setprecision(6);
    cerr << "Tempo total: " << duration_total.count() / 1000.0 << " ms\n";

    cerr << "Tempo médio por iteração: " 
         << (duration_total.count() / 1000.0) / n << " ms\n";

    long long total_busca = accumulate(busca_durations.begin(), busca_durations.end(), 0LL);
    cerr << "Tempo médio busca M-aumentante: " 
         << (total_busca / 1000.0) / n << " ms\n";

    return 0;
}
