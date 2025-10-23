#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cliente/cliente.h"

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
    char data[maxData];
    double precoTotal;
} Pedidos;

int sair(FILE* fpC, FILE* fpP, FILE* fpPe) {
    fclose(fpC);
    fclose(fpP);
    fclose(fpPe);
    endwin();
    return 0;
}

int menuInicial() {
    initscr();
    start_color();
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);
    clear();
    bkgd(COLOR_PAIR(2));
    printw("===== MENU INICIAL =====\n\n");
    printw("1) Manter Clientes\n");
    printw("2) Manter Produtos\n");
    printw("3) Manter Pedidos\n");
    printw("4) Sair\n\n");
    printw("Selecione uma opção: ");
    refresh();
    int valor;
    scanw("%d", &valor);
    getch();
    return valor;
}

int manterClientes() {
    int x;
    clear();
    printw("===== MANTER CLIENTES =====\n\n");
    printw("1) Cadastro de Clientes\n");
    printw("2) Consultar Clientes\n");
    printw("3) Remover Clientes\n");
    printw("4) Listar Clientes\n");
    printw("5) Voltar\n\n");
    printw("Digite a opção escolhida: ");
    scanw("%d", &x);
    getch();
    refresh();
    return x;
}
int manterProdutos() {
    int x;
    clear();
    printw("===== MANTER PRODUTOS =====\n\n");
    printw("1) Cadastro de Produtos\n");
    printw("2) Consultar Produtos\n");
    printw("3) Remover Produtos\n");
    printw("4) Listar Produtos\n");
    printw("5) Voltar\n\n");
    printw("Digite a opção escolhida: ");
    scanw("%d", &x);
    getch();
    refresh();
    return x;
}

int codigoProdutoJaExiste(FILE* fp, int code) {
    int codigo, estoque;
    char descricao[maxDescricao];
    double preco;

    int valido = 0;

    rewind(fp);

    while (fscanf(fp, "%d,%99[^,],%lf,%d", &codigo, descricao, &preco,
                  &estoque) == 4) {
        if (codigo == code) {
            return 1;
        }
    }

    return 0;
}

void cadastrarProdutos(FILE* fp) {
    Produtos produto;

    int codigo_existe;

    do {
        clear();
        printw("===== CADASTRO DE PRODUTO =====\n\n");

        printw("Digite um código identificador para o produto: ");
        scanw("%d", &produto.identificador);
        getch();

        codigo_existe = codigoProdutoJaExiste(fp, produto.identificador);

        if (codigo_existe == 1) {
            clear();
            printw("ERRO: O código %d já está cadastrado!\n",
                   produto.identificador);
            printw("Por favor, digite um código diferente.\n");
            refresh();
            getch();
        }

    } while (codigo_existe == 1);

    clear();

    printw("===== CADASTRO DE PRODUTO (Cód: %d) =====\n\n",
           produto.identificador);

    printw("Digite a descrição do produto: ");
    echo();
    getnstr(produto.descricao, maxDescricao - 1);
    noecho();

    printw("Digite o preço do produto: ");
    scanw("%lf", &produto.preco);
    getch();

    printw("Digite quantos itens tem no estoque: ");
    scanw("%d", &produto.estoque);
    getch();

    fprintf(fp, "%d,%s,%.2lf,%d\n", produto.identificador, produto.descricao,
            produto.preco, produto.estoque);
    fflush(fp);

    printw("\nProduto cadastrado com sucesso!");
    refresh();
    getch();
}

void consultarProdutos(FILE* fp) {
    Produtos produto;
    int codigo;
    int encontrado = 0;

    clear();
    printw("===== CONSULTAR PRODUTOS =====\n\n");

    printw("Digite o código para ser consultado: ");
    scanw("%d", &codigo);

    refresh();

    clear();

    printw("===== CONSULTAR PRODUTOS =====\n\n");

    rewind(fp);

    while (fscanf(fp, "%d,%99[^,],%lf,%d", &produto.identificador,
                  produto.descricao, &produto.preco, &produto.estoque) == 4) {
        if (codigo == produto.identificador) {
            printw("%d,%s,%lf,%d", produto.identificador, produto.descricao,
                   produto.preco, produto.estoque);
            encontrado++;
        }
    }

    if (encontrado == 0) {
        printw(
            "Não existe qualquer produto com este identificador!\nTente com um "
            "valor válido!");
    }

    refresh();
    getch();
}

FILE* deletarProdutos(FILE* fp) {
    clear();
    printw("Função 'Deletar Produtos' não implementada.\n");
    refresh();
    getch();
    return fp;
}

void listarProdutos(FILE* fp) {
    clear();
    printw("Função 'Listar Produtos' não implementada.\n");
    refresh();
    getch();
}

int main() {
    FILE* fpClientes = fopen("clientes.csv", "a+");
    FILE* fpProdutos = fopen("produtos.csv", "a+");
    FILE* fpPedidos = fopen("pedidos.csv", "a+");

    if (fpClientes == NULL || fpProdutos == NULL || fpPedidos == NULL) {
        printf(
            "Erro fatal: Não foi possível abrir os arquivos .csv "
            "necessários.\n");
        return EXIT_FAILURE;
    }

    int inicioValor;

    do {
        inicioValor = menuInicial();
        clear();
        switch (inicioValor) {
            case 1: {
                int clienteValor;
                do {
                    clienteValor = manterClientes();
                    switch (clienteValor) {
                        case 1:
                            cadastrarClientes(fpClientes);
                            break;
                        case 2:
                            consultarClientes(fpClientes);
                            break;
                        case 3:
                            fpClientes = deletarClientes(fpClientes);
                            break;
                        case 4:
                            listarClientes(fpClientes);
                            break;
                        case 5:
                            break;
                        default:
                            printw("Opção Inválida!");
                            refresh();
                            getch();
                            break;
                    }
                } while (clienteValor != 5);
                break;
            }
            case 2:
                int produtoValor;
                do {
                    produtoValor = manterProdutos();
                    switch (produtoValor) {
                        case 1:
                            cadastrarProdutos(fpProdutos);
                            break;
                        case 2:
                            consultarProdutos(fpProdutos);
                            break;
                        case 3:
                            fpProdutos = deletarProdutos(fpProdutos);
                            break;
                        case 4:
                            listarProdutos(fpProdutos);
                            break;
                        case 5:
                            break;
                        default:
                            printw(
                                "Código inválido! Tente novamente um código "
                                "válido.");
                            refresh();
                            getch();
                            break;
                    }

                } while (produtoValor != 5);
                break;
            case 3:
                printw("Você escolheu: Manter Pedidos (Não implementado)\n");
                refresh();
                getch();
                break;

            case 4:
                clear();
                printw("Saindo do programa...\n");
                refresh();
                break;
            default:
                printw("Opção inválida.\n");
                refresh();
                getch();
                break;
        }
    } while (inicioValor != 4);

    printw("\nPressione qualquer tecla para sair...");
    refresh();
    getch();
    sair(fpClientes, fpProdutos, fpPedidos);
    return EXIT_SUCCESS;
}