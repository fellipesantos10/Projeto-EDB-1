#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "projeto.h"

void liberarLista(No **cabeca) {
    No *atual = *cabeca;
    while (atual) {
        No *prox = atual->proximo;
        free(atual);
        atual = prox;
    }
    *cabeca = NULL; 
}

int main(void) {
    No *cabeca = NULL; 
    int opcao, numProdutos = 0; // Inicializa numProdutos
    char nomeArquivo[] = "teste.csv";

    int capacidade = 100;
    Produto *produtos = malloc(capacidade * sizeof(Produto));
    if (!produtos) {
        printf("Erro ao alocar memoria para o vetor de produtos. \n");
        return 1;
    }

    // Inicializa a lista a partir do arquivo, se houver dados
    iniciarLista(nomeArquivo, &cabeca, produtos);
    criarArquivo(nomeArquivo);

    do {
        menu();                 
        opcao = interacao(&cabeca, produtos, capacidade, nomeArquivo);
        numProdutos = pegaTamanho(cabeca, produtos);
    } while (opcao != 10);

    // Libera a memoria alocada
    liberarLista(&cabeca); 
    free(produtos);

    printf("\nPrograma encerrado.\n");
    return 0;
}
