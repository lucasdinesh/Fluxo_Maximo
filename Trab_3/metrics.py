import subprocess
import time
import csv
import os

# Parâmetros de teste
n_values = [10, 50, 100, 200]         # número de equipes
alpha_values = [0.1, 0.3, 0.5, 0.7, 0.9]  # fração de jogos realizados
beta_values = [0.0, 0.25, 0.5, 0.75, 1.0] # viés da equipe 1

# Caminhos para os executáveis
generator = "./generate_tournament"
solver = "./vencer"

# Arquivo de saída
output_file = "experimentos_torneio.csv"

with open(output_file, mode="w", newline="") as file:
    writer = csv.writer(file)
    writer.writerow(["n", "alpha", "beta", "tempo_execucao_s", "resposta"])

    for n in n_values:
        for alpha in alpha_values:
            for beta in beta_values:
                cmd_gen = [generator, str(n), "10", str(alpha), str(beta)]

                # Gera o torneio e executa o programa
                gen_process = subprocess.Popen(cmd_gen, stdout=subprocess.PIPE)

                # Mede o tempo de execução do programa 'vencer'
                start = time.perf_counter()
                result = subprocess.run(solver, stdin=gen_process.stdout, capture_output=True, text=True)
                end = time.perf_counter()

                gen_process.stdout.close()  # Importante: fecha o pipe de leitura
                gen_process.wait()

                tempo = end - start
                resposta = result.stdout.strip()

                # Salva os dados
                writer.writerow([n, alpha, beta, tempo, resposta])
                print(f"n={n}, α={alpha}, β={beta} -> {resposta} em {tempo:.4f} s")
