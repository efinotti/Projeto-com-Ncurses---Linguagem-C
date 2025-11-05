#ifndef PEDIDO_H
#define PEDIDO_H


#define maxData 11
#define maxStatus 20
#define maxDescricao 100
#define maxPedidos 100

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "cliente.h"
#include "Produtos.h"

typedef struct {
    int codigoPedido;
    int codigoCliente;
    int codigoProduto;
    int quantidade;
    char data[maxData];
    char descricao[maxDescricao];
    float valorTotal;
    char status[maxStatus];
}Pedido;

Pedido listaPedidos[maxPedidos];

extern int totalPedidos;
int codigoPedidoJaExiste(FILE * fpPe, int code);
void cadastrarPedido(FILE *fpPe);
void listarPedidos(FILE *fpPe);
void consultarPedido(FILE *fpPe, int code);
FILE* deletarPedido(FILE* fpPe);
void atualizarStatusPedido(FILE *fpPe, int code, const char *novoStatus);
void obterDataAtual(char *destino, int tamanho);

#endif