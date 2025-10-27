#ifndef PEDIDO_H
#define PEDIDO_H


#define maxData 11
#define maxStatus 20
#define maxDescricao 100

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cliente.h"

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

int codigoPedidoJaExiste(FILE * fp, int code);
void cadastrarPedido(FILE *fp);
void listarPedidos(FILE *fp);
void consultarPedido(FILE *fp, int code);
FILE* deletarPedido(FILE* fp);
void atualizarStatusPedido(FILE *fp, int code, const char *novoStatus);

#endif