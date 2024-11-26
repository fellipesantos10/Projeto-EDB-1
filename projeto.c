#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "projeto.h"

// Insere no início da lista.
void inserir_inicio(No **cabeca, Produto novo_produto) {
    No *novo_no = (No *)malloc(sizeof(No));
    if (!novo_no) {
        printf("Erro ao alocar memória.\n");
        return;
    }

    novo_no->produto = novo_produto;
    novo_no->proximo = *cabeca;
    novo_no->anterior = NULL;

    if (*cabeca) {
        (*cabeca)->anterior = novo_no;
    }

    *cabeca = novo_no;
}

// Insere no fim da lista.
void inserir_fim(No **cabeca, Produto novo_produto) {
    No *novo_no = (No *)malloc(sizeof(No));
    if (!novo_no) {
        printf("Erro ao alocar memória.\n");
        return;
    }

    novo_no->produto = novo_produto;
    novo_no->proximo = NULL;

    if (*cabeca == NULL) {
        novo_no->anterior = NULL;
        *cabeca = novo_no;
    } else {
        No *aux = *cabeca;
        while (aux->proximo != NULL) {
            aux = aux->proximo;
        }
        aux->proximo = novo_no;
        novo_no->anterior = aux;
    }
}

// Insere em uma posicao especifica na lista.
void inserir_posicao(No **cabeca, Produto novo_produto, int posicao) {
    if (posicao < 0) {
        printf("=> Posição inválida!\n");
        return;
    }

    if (posicao == 0) {
        inserir_inicio(cabeca, novo_produto);
        return;
    }

    No *aux = *cabeca;
    int cont = 0;

    while (aux != NULL && cont < posicao - 1) {
        aux = aux->proximo;
        cont++;
    }

    if (aux == NULL) {
        printf("=> Posição inválida!\n");
        return;
    }

    No *novo_no = (No *)malloc(sizeof(No));
    if (!novo_no) {
        printf("Erro ao alocar memória.\n");
        return;
    }

    novo_no->produto = novo_produto;
    novo_no->proximo = aux->proximo;
    novo_no->anterior = aux;

    if (aux->proximo != NULL) {
        aux->proximo->anterior = novo_no;
    }
    aux->proximo = novo_no;
}

// Remove o inicio da lista.
void remover_inicio(No **cabeca) {
    if (*cabeca == NULL) {
        printf("=> Lista vazia!\n");
        return;
    }

    No *temp = *cabeca;
    *cabeca = (*cabeca)->proximo;

    if (*cabeca != NULL) {
        (*cabeca)->anterior = NULL;
    }

    free(temp);
}

// Remove o fim da lista.
void remover_fim(No **cabeca) {
    if (*cabeca == NULL) {
        printf("=> Lista vazia!\n");
        return;
    }

    No *aux = *cabeca;
    while (aux->proximo != NULL) {
        aux = aux->proximo;
    }

    if (aux->anterior == NULL) {
        *cabeca = NULL;
    } else {
        aux->anterior->proximo = NULL;
    }

    free(aux);
}

// Remove de uma posicao especifica.
void remover_posicao(No **cabeca, int posicao) {
    if (*cabeca == NULL) {
        printf("=> Lista vazia!\n");
        return;
    }

    if (posicao < 0) {
        printf("=> Posição inválida!\n");
        return;
    }

    No *aux = *cabeca;
    int cont = 0;

    while (aux != NULL && cont < posicao) {
        aux = aux->proximo;
        cont++;
    }

    if (aux == NULL) {
        printf("=> Posição inválida!\n");
        return;
    }

    if (aux == *cabeca) {
        remover_inicio(cabeca);
    } else if (aux->proximo == NULL) {
        remover_fim(cabeca);
    } else {
        aux->anterior->proximo = aux->proximo;
        aux->proximo->anterior = aux->anterior;
        free(aux);
    }
}

// Busca um valor na lista.
No *buscar_valor(No *cabeca, int valor) {
    No *aux = cabeca;

    while (aux != NULL && aux->produto.id != valor) {
        aux = aux->proximo;
    }

    return aux;
}

// Libera a memória da lista.
void limparMemoria(No **cabeca) {
    while (*cabeca != NULL) {
        remover_inicio(cabeca);
    }
}


void redimensionarVetor(Produto **produtos, int *capacidade) {
    int nova_capacidade = (*capacidade) * 2;
    Produto *novo_vetor = (Produto *)malloc(nova_capacidade * sizeof(Produto));
    if (!novo_vetor) {
        printf("Erro ao alocar memória para redimensionamento do vetor.\n");
        return;
    }

    memcpy(novo_vetor, *produtos, (*capacidade) * sizeof(Produto));
    free(*produtos);

    *produtos = novo_vetor;
    *capacidade = nova_capacidade;

    printf("Vetor redimensionado. Nova capacidade: %d.\n", nova_capacidade);
}

// Imprime os valores da lista.
void imprimirLista(No *cabeca) {
    if (cabeca == NULL) {
        printf("=> Lista vazia!\n");
        return;
    }

    printf("***********************************\n");
    printf("ID\tNome\tQuant\tPreco\n");
    printf("***********************************\n");

    No *aux = cabeca;
    while (aux != NULL) {
        Produto p = aux->produto;
        printf("%-6d %-10s %-10d %.2f\n", p.id, p.nome, p.quant, p.preco);
        aux = aux->proximo;
    }
    printf("***********************************\n");
}

void criarArquivo(const char *nomeArquivo){
    FILE *arquivo = fopen(nomeArquivo, "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir ou criar o arquivo.\n");
        return;
    }

    fseek(arquivo, 0, SEEK_END);
    if (ftell(arquivo) == 0) {
        fprintf(arquivo, "ID, Nome, Quant, Preco\n");
        printf("Arquivo criado. \n");
    }

    fclose(arquivo);
}

void salvarArquivo(No *cabeca, Produto produtos[], const char *nomeArquivo){
    int tamanho = pegaTamanho(cabeca, produtos);
    reescreverArquivo(produtos, tamanho, nomeArquivo);
    printf("Lista salva com sucesso.\n");
    return;
}

void iniciarLista(const char *nomeArquivo, No **cabeca, Produto produtos[]){
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Nenhum registro encontrado no arquivo.\n");
        return;
    }

    char linha[256];
    fgets(linha, sizeof(linha), arquivo); 

    while (fgets(linha, sizeof(linha), arquivo)) {
        Produto p;
        
        if (sscanf(linha, "%d,%49[^,],%d,%f", &p.id, p.nome, &p.quant, &p.preco) == 4) {
            // Inserir o produto na lista encadeada no final
            inserir_fim(cabeca, p);
        } else {
            printf("Erro ao interpretar a linha: %s", linha);
        }
    }

    fclose(arquivo);
}

void ordenarArquivo(Produto produtos[], int tamanho, Produto *novoProduto, int posicao, int operacao, const char *nomeArquivo) {
    if (operacao == 1) { // Inserir
        if (posicao < 0 || posicao > tamanho) {
            printf("Posição invalida para insercao.\n");
            return;
        }

        
        for (int i = tamanho; i > posicao; i--) {
            produtos[i] = produtos[i - 1];
        }

        // Insere o novo produto
        produtos[posicao] = *novoProduto;
        tamanho++;

    } else if (operacao == 2) { 
        if (posicao < 0 || posicao >= tamanho) {
            printf("Posicao invalida para remocao.\n");
            return;
        }

        
        for (int i = posicao; i < tamanho - 1; i++) {
            produtos[i] = produtos[i + 1];
        }

        tamanho--;
    } else {
        printf("Operacao invalida.\n");
    }
}

void reescreverArquivo(Produto produtos[], int tamanho, const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "w");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo para gravacao.\n");
        return;
    }

    
    fprintf(arquivo, "ID,Nome,Quant,Preco\n");

    
    for (int i = 0; i < tamanho; i++) {
        fprintf(arquivo, "%d,%s,%d,%.2f\n", 
                produtos[i].id, produtos[i].nome, produtos[i].quant, produtos[i].preco);
    }

    fclose(arquivo);
    printf("Alteracoes realizadas no arquivo principal com sucesso. \n");
}

int pegaTamanho(No *cabeca, Produto produtos[])
{
    No *aux = cabeca;
    int i = 0;

    while (aux != NULL)
    {
        produtos[i] = aux->produto;
        aux = aux->proximo;
        i++;
    }

    return i; // Retorna o tamanho do array
}

void menu()
{
    printf("\n========== MENU ==========\n");
    printf("1. Inserir produto no inicio. \n");
    printf("2. Inserir produto no fim. \n");
    printf("3. Inserir produto em posicao especifica. \n");
    printf("4. Remover produto do inicio. \n");
    printf("5. Remover produto do fim. \n");
    printf("6. Remover produto em posicao especifica. \n");
    printf("7. Buscar produto por id. \n");
    printf("8. Imprimir produtos. \n");
    printf("9. Salvar arquivo. \n");
    printf("10. Sair. \n");
    printf("==========================\n\n");
}

int interacao(No **cabeca, Produto produtos[], int capacidade,  const char *nomeArquivo)
{
    int numProdutos = 0; // Inicializa a contagem de produtos
    int opcao;

    printf("Escolha uma opcao: ");
    scanf("%d", &opcao);
    printf("\n");

    switch (opcao)
    {
    case 1:
    {
        Produto novoProduto;
        printf("Digite o id, nome, quant e preco do produto: ");
        scanf("%d %49s %d %f", &novoProduto.id, novoProduto.nome, &novoProduto.quant, &novoProduto.preco);

        if (numProdutos == capacidade) { 
            redimensionarVetor(&produtos, &capacidade);
        }

        inserir_inicio(cabeca, novoProduto);
        numProdutos++; // Incrementa o numero de produtos
        printf("Produto %s inserido no inicio.\n", novoProduto.nome);
        break;
    }
    case 2:
    {
        Produto novoProduto;
        printf("Digite o id, nome, quant e preco do produto: ");
        scanf("%d %49s %d %f", &novoProduto.id, novoProduto.nome, &novoProduto.quant, &novoProduto.preco);

        if (numProdutos == capacidade) { 
            redimensionarVetor(&produtos, &capacidade);
        }

        inserir_fim(cabeca, novoProduto);
        numProdutos++; // Incrementa o numero de produtos
        printf("Produto %s inserido no final.\n", novoProduto.nome);
        break;
    }
    case 3:
    {
        Produto novoProduto;
        int posicao;
        printf("Digite o id, nome, quant e preco do produto: ");
        scanf("%d %49s %d %f", &novoProduto.id, novoProduto.nome, &novoProduto.quant, &novoProduto.preco);
        printf("Posicoes disponiveis: ");
        printf("\nDigite a posicao para inserir o produto: ");
        scanf("%d", &posicao);

        if (numProdutos == capacidade) { 
            redimensionarVetor(&produtos, &capacidade);
        }

        inserir_posicao(cabeca, novoProduto, posicao);
        numProdutos++; 
        printf("Produto %s inserido na posicao %d.\n", novoProduto.nome, posicao);
        break;
    }
    case 4:
        remover_inicio(cabeca);
        numProdutos--; // Decrementa o numero de produtos
        printf("Produto removido do inicio.\n");
        break;
    case 5:
        remover_fim(cabeca);
        numProdutos--; 
        break;
    case 6:
    {
        int posicao;
        printf("Posicoes disponiveis: ");
        printf("\nDigite a posicao para remover o produto: ");
        scanf("%d", &posicao);
        remover_posicao(cabeca, posicao);
        numProdutos--; 
        break;
    }
    case 7:
    {
        int id;
        printf("Digite o id do produto a ser buscado: ");
        scanf("%d", &id);
        No *encontrado = buscar_valor(*cabeca, id);
        if (encontrado)
        {
            printf("Produto encontrado: %s, id: %d, quant: %d, Preco: %.2f\n",
                   encontrado->produto.nome, encontrado->produto.id,
                   encontrado->produto.quant, encontrado->produto.preco);
        }
        else
        {
            printf("Produto com id %d nao encontrado.\n", id);
        }
        break;
    }
    case 8:
        imprimirLista(*cabeca); 
        break;
    case 9:
    {
        salvarArquivo(*cabeca, produtos, nomeArquivo);
        break;
    }
    case 10:
        printf("Encerrando programa...\n");
        break;
    default:
        printf("Opcao invalida. Tente novamente.\n");
        break;
    }
    return opcao;
}
