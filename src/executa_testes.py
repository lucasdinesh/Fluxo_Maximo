import os
import subprocess
import re
import csv

PASTA_INSTANCIAS = "instancias"
PROGRAMA = "./maxflow"
ARQUIVO_CSV = "resultados3.csv"

def extrair_dados(output, grafo_nome):
    linhas = []
    num_vertices = None

    # Pega número de vértices, se disponível
    match_vertices = re.search(r"Número de vértices:\s*(\d+)", output)
    if match_vertices:
        num_vertices = int(match_vertices.group(1))

    # Divide a saída por algoritmo
    blocos = output.split("Executando algoritmo:")
    for bloco in blocos[1:]:
        alg_match = re.match(r"\s*(\w+)", bloco)
        if not alg_match:
            continue
        algoritmo = alg_match.group(1)

        fluxo = re.search(r"Fluxo máximo:\s*(\d+)", bloco)
        tempo = re.search(r"Tempo de execução:\s*(\d+)", bloco)
        iters = re.search(r"Número de iterações:\s*(\d+)", bloco)
        ops = re.search(r"Número de operações:\s*(\d+)", bloco)
        comprimento_medio = re.search(r"Comprimento médio dos caminhos aumentantes:\s*([\d.]+)", bloco)
        tempo_medio_iter = re.search(r"Tempo médio por iteração:\s*([\d.]+)", bloco)
        memoria_max = re.search(r"Uso máximo de memória:\s*(\d+)", bloco)

        linha = {
            "grafo": grafo_nome,
            "algoritmo": algoritmo,
            "fluxo_maximo": int(fluxo.group(1)) if fluxo else None,
            "tempo_ms": int(tempo.group(1)) if tempo else None,
            "iteracoes": int(iters.group(1)) if iters else None,
            "operacoes": int(ops.group(1)) if ops else None,
            "comprimento_medio": float(comprimento_medio.group(1)) if comprimento_medio else None,
            "tempo_medio_iteracao_ms": float(tempo_medio_iter.group(1)) if tempo_medio_iter else None,
            "memoria_max_kb": int(memoria_max.group(1)) if memoria_max else None,
            "num_vertices": num_vertices
        }
        linhas.append(linha)

    return linhas

def main():
    resultados = []

    for arquivo in os.listdir(PASTA_INSTANCIAS):
        if not arquivo.endswith(".gr"):
            continue

        caminho = os.path.join(PASTA_INSTANCIAS, arquivo)
        grafo_nome = os.path.splitext(arquivo)[0]

        print(f"Executando {PROGRAMA} com {caminho}...")

        try:
            output = subprocess.check_output([PROGRAMA, caminho], text=True)
            dados = extrair_dados(output, grafo_nome)
            resultados.extend(dados)
        except subprocess.CalledProcessError as e:
            print(f"Erro ao executar {grafo_nome}: {e}")

    if resultados:
        with open(ARQUIVO_CSV, "w", newline="") as csvfile:
            writer = csv.DictWriter(csvfile, fieldnames=resultados[0].keys())
            writer.writeheader()
            writer.writerows(resultados)
        print(f"\n✅ Resultados salvos em '{ARQUIVO_CSV}'")
    else:
        print("Nenhum resultado processado.")

if __name__ == "__main__":
    main()
