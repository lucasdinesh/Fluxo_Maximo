import matplotlib.pyplot as plt
import pandas as pd

# Dados fornecidos pelo usuário
data = [
    # grafo, algoritmo, fluxo_maximo, tempo_ms, iteracoes, operacoes, comprimento_medio, tempo_medio_iteracao_ms, memoria_max_kb, num_vertices
    ["matching", "bfs", 2803, 1613, 2803, 85028247, 4.39957, 0.575455, 9856, 6002],
    ["matching", "dfs", 2803, 1367, 2803, 11774933, 79.0681, 0.487692, 9856, 6002],
    ["matching", "fattest_path", 2803, 1808, 2803, 19187467, 6.2162, 0.645023, 9856, 6002],
    ["random_level", "bfs", 5828, 4554, 4171, 206322755, 86.0873, 1.09182, 9856, 6402],
    ["random_level", "dfs", 5828, 5504, 3673, 54267960, 1036.86, 1.4985, 9856, 6402],
    ["random_level", "fattest_path", 5828, 623, 283, 5252590, 181.392, 2.20141, 9856, 6402],
    ["mesh", "bfs", 5571, 1892, 3435, 97720708, 66.3132, 0.550801, 9856, 3602],
    ["mesh", "dfs", 5571, 2898, 3315, 33040480, 680.747, 0.874208, 9856, 3602],
    ["mesh", "fattest_path", 5571, 478, 301, 4488961, 128.352, 1.58804, 9856, 3602],
    ["basic_line", "bfs", 7272, 3440, 3193, 155090040, 44.9092, 1.07736, 9856, 6402],
    ["basic_line", "dfs", 7272, 4976, 4113, 45369095, 812.219, 1.20982, 9856, 6402],
    ["basic_line", "fattest_path", 7272, 1017, 286, 9089605, 105.752, 3.55594, 9856, 6402],
    ["goldbad", "bfs", 3000, 5785, 3000, 153060003, 3005.0, 1.92833, 9856, 9003],
    ["goldbad", "dfs", 3000, 8143, 3000, 72075027, 3005.0, 2.71433, 9856, 9003],
    ["goldbad", "fattest_path", 3000, 15504, 3000, 99066003, 3005.0, 5.168, 9856, 9003],
    ["expline", "bfs", 23400, 84, 94, 4631478, 35.234, 0.893617, 9856, 6402],
    ["expline", "dfs", 23400, 659, 1965, 7188402, 146.209, 0.335369, 9856, 6402],
    ["expline", "fattest_path", 23400, 12, 78, 124354, 43.4872, 0.153846, 9856, 6402],
]

df = pd.DataFrame(data, columns=[
    "grafo", "algoritmo", "fluxo_maximo", "tempo_ms", "iteracoes", "operacoes",
    "comprimento_medio", "tempo_medio_iteracao_ms", "memoria_max_kb", "num_vertices"
])

# Função para gerar gráfico de barras comparando algoritmos
def plot_comparison(metric, title, ylabel):
    plt.figure(figsize=(12, 6))
    for grafo in df['grafo'].unique():
        subset = df[df['grafo'] == grafo]
        plt.bar(
            [f"{grafo}\n{alg}" for alg in subset['algoritmo']],
            subset[metric],
            label=grafo
        )
    plt.title(title)
    plt.ylabel(ylabel)
    plt.xticks(rotation=45, ha='right')
    plt.tight_layout()
    plt.grid(axis='y')
    plt.show()

plot_comparison("tempo_ms", "Tempo de Execução por Algoritmo e Instância", "Tempo (ms)")
plot_comparison("iteracoes", "Número de Iterações por Algoritmo e Instância", "Iterações")
plot_comparison("operacoes", "Número de Operações por Algoritmo e Instância", "Operações")
plot_comparison("comprimento_medio", "Comprimento Médio dos Caminhos", "Arestas por Caminho")
