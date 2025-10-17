#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#define maxNome 30
#define maxEmail 50
#define maxDescricao 100
#define maxEndereco 40
#define maxData 10

typedef struct {
    char rua[maxEndereco];
    char setor[maxEndereco];
    char cidade[maxEndereco];
    char estado[maxEndereco];
} Endereco;

typedef struct {
    int identificador;
    char nome[maxNome];
    int telefone;
    char email[maxEmail];

} Cliente;

typedef struct {
    int identificador;
    char descricao[maxDescricao];
    double preco;
    int estoque;
} Produtos;

typedef struct {
    int identificadorProduto;
    int identificadorPedido;
    int quantidade;
    double precoSubtotal;
} ItemPedidos;

typedef struct {
    int identificadorAutoral;
    int identificadorCliente;
    int itens;
    char data[10];
    double precoTotal;
} Pedidos;