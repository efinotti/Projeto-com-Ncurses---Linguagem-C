#ifndef PRODUTOS_H
#define PRODUTOS_H


#define maxDescricao 100
#define MAX_LINE 200
#define ARQUIVO_PRODUTOS "produtos.csv"
#define TEMP_FILE "temp_produtos.csv" 
#define DELIMITER ","

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int identificador;
    char descricao[maxDescricao];
    double preco;
    int estoque;
} Produtos;

#endif