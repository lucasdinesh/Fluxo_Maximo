import subprocess
import csv
import os

# Lista de arquivos de teste
arquivos = [
    'ap10_1.dat',
    'ap20_2.dat',
    'ap50_6.dat',
    'ap100_2.dat',
    'ap200_10.dat',
    'ap500_2.dat',
    # adicione mais arquivos conforme necessário
]

# Caminho para o executável
executavel = './hungarian_timed'

# Nome do CSV de saída
csv_saida = 'resultados_experimentos.csv'

with open(csv_saida, 'w', newline='') as csvfile:
    writer = csv.writer(csvfile)
    writer.writerow(['arquivo', 'n', 'resultado', 'tempo_total_ms', 
                     'tempo_medio_iter_ms', 'tempo_medio_busca_ms'])

    for arq in arquivos:
        with open(arq, 'r') as f:
            n = int(f.readline())

        # Executa o programa
        process = subprocess.Popen(
            [executavel],
            stdin=open(arq),
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            universal_newlines=True
        )

        saida, erros = process.communicate()

        resultado = saida.strip()
        
        tempo_total = None
        tempo_medio_iter = None
        tempo_medio_busca = None

        for linha in erros.split('\n'):
            if 'Tempo total' in linha:
                tempo_total = float(linha.split(':')[1].strip().split()[0])
            elif 'Tempo médio por iteração' in linha:
                tempo_medio_iter = float(linha.split(':')[1].strip().split()[0])
            elif 'Tempo médio busca M-aumentante' in linha:
                tempo_medio_busca = float(linha.split(':')[1].strip().split()[0])

        writer.writerow([arq, n, resultado, tempo_total, tempo_medio_iter, tempo_medio_busca])

print(f'Resultados salvos em {csv_saida}')
