#ifndef PROJETO_H
#define PROJETO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura Produto
typedef struct produto
{
  int id;
  char nome[50];
  int quant;
  float preco;
} Produto;

// Estrutura No
typedef struct no
{
  Produto produto;
  struct no *proximo;
  struct no *anterior;
} No;

void inserir_inicio(No **cabeca, Produto novo_produto);
void inserir_fim(No **cabeca, Produto novo_produto);
void inserir_posicao(No **cabeca, Produto novo_produto, int posicao);
void remover_inicio(No **cabeca);
void remover_fim(No **cabeca);
void remover_posicao(No **cabeca, int posicao);
No *buscar_valor(No *cabeca, int valor);

void limparMemoria(No **cabeca);
void redimensionarVetor(Produto **produtos, int *capacidade);
void imprimirLista(No *cabeca);

void criarArquivo(const char *nomeArquivo);
void salvarArquivo(No *cabeca, Produto produtos[], const char *nomeArquivo);

void iniciarLista(const char *nomeArquivo, No **cabeca, Produto produtos[]);
void ordenarArquivo(Produto produtos[], int tamanho, Produto *novoProduto, int posicao, int operacao, const char *nomeArquivo);
void reescreverArquivo(Produto produtos[], int tamanho, const char *nomeArquivo);
int pegaTamanho(No *cabeca, Produto produtos[]);

void menu();
int interacao(No **cabeca, Produto produtos[], int capacidade,  const char *nomeArquivo);

#endif