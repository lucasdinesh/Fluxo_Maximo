import pandas as pd
import matplotlib.pyplot as plt

# Lê o CSV
df = pd.read_csv('resultados_experimentos.csv')

# Gráfico 1: Tempo total vs n
plt.figure(figsize=(8,5))
plt.plot(df['n'], df['tempo_total_ms'], marker='o')
plt.xlabel('n')
plt.ylabel('Tempo total (ms)')
plt.title('Tempo total de execução vs n')
plt.grid(True)
plt.savefig('tempo_total_vs_n.png')
plt.show()

# Gráfico 2: Tempo médio busca vs n
plt.figure(figsize=(8,5))
plt.plot(df['n'], df['tempo_medio_busca_ms'], marker='o', color='green')
plt.xlabel('n')
plt.ylabel('Tempo médio busca M-aumentante (ms)')
plt.title('Tempo médio da busca M-aumentante vs n')
plt.grid(True)
plt.savefig('tempo_busca_vs_n.png')
plt.show()
