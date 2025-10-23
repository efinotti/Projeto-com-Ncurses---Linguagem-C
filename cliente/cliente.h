#ifndef CLIENTE_H
#define CLIENTE_H


#define maxNome 30
#define maxRazao 40
#define maxEmail 50
#define maxDescricao 100
#define maxEndereco 40
#define maxData 10
#define tamCPF 12
#define tamCNPJ 15

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    Endereco endereco;
    char cpf[tamCPF];
    char razaoSocial[maxRazao];
    char cnpj[tamCNPJ];
} Cliente;

int validaCPF(char cpf[tamCPF]);
int validaCNPJ(char cnpj[tamCNPJ]);
int codigoJaExiste(FILE* fp, int code);
void cadastrarClientes(FILE* fp);
void consultarClientes(FILE* fp);
FILE* deletarClientes(FILE* fp);
void listarClientes(FILE* fp);

#endif