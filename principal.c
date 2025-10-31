#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cliente/cliente.h"
#include "Produtos.h"

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

int main() {
    FILE* fpClientes = fopen("clientes.csv", "a+");
    FILE* fpProdutos = fopen("produtos.csv", "a+");
    FILE* fpPedidos = fopen("pedidos.csv", "a+");

    if (fpClientes == NULL || fpProdutos == NULL || fpPedidos == NULL) {
        printf("Erro fatal: Não foi possível abrir os arquivos .csv necessários.\n");
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
                            deletarProdutos(fpProdutos);
                            fpProdutos = fopen(ARQUIVO_PRODUTOS, "a+"); 
                            if (fpProdutos == NULL) {
                                printf("ERRO CRÍTICO: Não foi possível reabrir o arquivo após a deleção.\n"); 
                                exit(1); 
                            }
                            break;
                        case 4:
                            listarProdutos(fpProdutos);
                            break;
                        case 5:
                            break;
                        default:
                            printw(
                                "Código inválido! Tente novamente um código válido.");
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
