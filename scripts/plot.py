import pandas as pd
import matplotlib.pyplot as plt

dados = pd.read_csv('speedup_results.csv')

programas = dados['PROGRAM'].unique()

for programa in programas:
    dados_programa = dados[dados['PROGRAM'] == programa]

    plt.figure(figsize=(8, 6))

    datasets = dados_programa['DATASET'].unique()
    for dataset in datasets:
        dados_dataset = dados_programa[dados_programa['DATASET'] == dataset]
        threads = dados_dataset['THREADS']
        speedup = dados_dataset['SPEEDUP']
        plt.plot(threads, speedup, marker='o', label=f'DATASET: {dataset}')

    plt.xlabel('Threads')
    plt.ylabel('Speedup')
    plt.title(f'Gráfico de Speedup para {programa}')
    plt.legend()
    plt.grid(True)

    # plt.savefig(f'speedup_{programa.lower()}.png')

    plt.show()
