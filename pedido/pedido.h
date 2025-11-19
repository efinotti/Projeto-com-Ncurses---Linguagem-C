#ifndef PEDIDO_H
#define PEDIDO_H


#define maxData 11
#define maxStatus 20
#define maxDescricao 100
#define maxPedidos 100
#define maxLinhas 200

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../cliente/cliente.h"
#include "../produto/Produtos.h"

#define ARQUIVO_PEDIDOS "pedidos.csv"

typedef struct {
    int codigoPedido;
    int codigoCliente;
    int codigoProduto;
    int quantidade;
    char data[maxData];
    char descricao[maxDescricao];
    double valorTotal;
    char status[maxStatus];
}Pedido;

typedef struct {
    int codigoPedido;
    int codigoCliente;
    char descricao[maxDescricao];
    int quantidade;
} PedidoMenu;

extern int totalPedidos;

int codigoPedidoJaExiste(FILE * fpPe, int code);
void cadastrarPedido(FILE *fpPe, FILE *fpC, FILE *fpP);
void listarPedidos(FILE *fpPe);
int consultarPedido(FILE *fpPe, FILE *fpC);
FILE* deletarPedido(FILE *fpPe, FILE *fpC);
int atualizarPedido(FILE *fpPe,  FILE *fpC, FILE *fpP);
void obterDataAtual(char *destino, int tamanho);
void obterDadosProduto(FILE *fpP,int codigoProduto, char *descricaoDestino, double *precoDestino);

#endif