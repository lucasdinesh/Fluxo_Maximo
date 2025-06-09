import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Lê os dados
df = pd.read_csv("experimentos_torneio.csv")

# Corrige os tipos de dados (e remove linhas inválidas)
df["n"] = pd.to_numeric(df["n"], errors="coerce")
df["alpha"] = pd.to_numeric(df["alpha"], errors="coerce")
df["beta"] = pd.to_numeric(df["beta"], errors="coerce")
df["tempo_execucao_s"] = pd.to_numeric(df["tempo_execucao_s"], errors="coerce")
df["resposta"] = df["resposta"].astype(str)

print("Tipos de dados:")
print(df.dtypes)
print("\nPrimeiras linhas:")
print(df.head())

print("\nExemplo de entrada inválida (se houver):")
print(df[df["n"].apply(lambda x: isinstance(x, list) or isinstance(x, (pd.Series, pd.DataFrame)))])


# Remove linhas com dados faltantes (NaNs gerados pela coercion)
df.dropna(subset=["n", "alpha", "beta", "tempo_execucao_s"], inplace=True)

# --- GRÁFICO 1: Tempo de execução vs Número de Equipes ---
plt.figure(figsize=(10, 6))
sns.lineplot(data=df, x="n", y="tempo_execucao_s", hue="alpha", style="beta", markers=True, dashes=False)
plt.title("Tempo de execução vs Número de Equipes")
plt.xlabel("Número de equipes (n)")
plt.ylabel("Tempo de execução (s)")
plt.grid(True)
plt.tight_layout()
plt.savefig("grafico_tempo_vs_equipes.png")
plt.show()

# --- GRÁFICO 2: Probabilidade de vitória da equipe 1 vs Beta ---
df["venceu"] = df["resposta"].apply(lambda x: 1 if x.strip().lower() == "sim" else 0)
probabilidades = df.groupby(["n", "alpha", "beta"])["venceu"].mean().reset_index()

plt.figure(figsize=(10, 6))
sns.lineplot(data=probabilidades, x="beta", y="venceu", hue="alpha", style="n", markers=True, dashes=False)
plt.title("Probabilidade de vitória da equipe 1 vs Viés (β)")
plt.xlabel("Viés da equipe 1 (β)")
plt.ylabel("Probabilidade de vitória")
plt.ylim(0, 1)
plt.grid(True)
plt.tight_layout()
plt.savefig("grafico_probabilidade_vs_beta.png")
plt.show()

# --- GRÁFICO 3: Probabilidade de vitória vs Fração de jogos jogados (α) ---
probabilidades_alpha = df.groupby(["n", "beta", "alpha"])["venceu"].mean().reset_index()

plt.figure(figsize=(10, 6))
sns.lineplot(data=probabilidades_alpha, x="alpha", y="venceu", hue="beta", style="n", markers=True, dashes=False)
plt.title("Probabilidade de vitória da equipe 1 vs Fração de jogos jogados (α)")
plt.xlabel("Fração de jogos já jogados (α)")
plt.ylabel("Probabilidade de vitória")
plt.ylim(0, 1)
plt.grid(True)
plt.tight_layout()
plt.savefig("grafico_probabilidade_vs_alpha.png")
plt.show()
