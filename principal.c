#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

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
    printw("3) Manter Pedidos\n\n");
    printw("Selecione uma opção: ");
    refresh();
    int valor;
    scanw("%d", &valor);
    return valor;
}

int manterClientes() {
    int x;
    clear();
    printw("===== MANTER CLIENTES =====\n\n");
    printw("1) Cadastro de Clientes\n");
    printw("2) Consultar Clientes\n");
    printw("3) Remover Clientes\n");
    printw("4) Listar Clientes\n\n");
    printw("Digite a opção escolhida: ");
    scanw("%d", &x);
    refresh();
    return x;
}

int validaCPF(char cpf[12]) {
    int soma = 0;
    int j, i;
    int primeiroDigitoV, segundoDigitoV;
    for (i = 0; i < 11; i++) {
        if (cpf[i] < '0' || cpf[i] > '9') return 0;
    }
    soma = 0;
    j = 10;
    for (i = 0; i < 9; i++) soma += (cpf[i] - '0') * j--;
    int modSum1 = soma % 11;
    primeiroDigitoV = (modSum1 < 2) ? 0 : 11 - modSum1;

    soma = 0;
    j = 11;
    for (i = 0; i < 10; i++) soma += (cpf[i] - '0') * j--;
    int modSum2 = soma % 11;
    segundoDigitoV = (modSum2 < 2) ? 0 : 11 - modSum2;

    return (primeiroDigitoV == (cpf[9] - '0') && segundoDigitoV == (cpf[10] - '0'));
}

int validaCNPJ(char cnpj[15]) {
    int i, soma, resto;
    int digito1, digito2;
    int pesos1[12] = {5, 4, 3, 2, 9, 8, 7, 6, 5, 4, 3, 2};
    int pesos2[13] = {6, 5, 4, 3, 2, 9, 8, 7, 6, 5, 4, 3, 2};

    for (i = 0; i < 14; i++) {
        if (cnpj[i] < '0' || cnpj[i] > '9') return 0;
    }

    soma = 0;
    for (i = 0; i < 12; i++) soma += (cnpj[i] - '0') * pesos1[i];
    resto = soma % 11;
    digito1 = (resto < 2) ? 0 : 11 - resto;

    soma = 0;
    for (i = 0; i < 13; i++) soma += (cnpj[i] - '0') * pesos2[i];
    resto = soma % 11;
    digito2 = (resto < 2) ? 0 : 11 - resto;

    return (digito1 == (cnpj[12] - '0') && digito2 == (cnpj[13] - '0'));
}

void cadastrarClientes() {
    clear();
    int tCadastro;
    printw("===== TIPO DE CADASTRO =====\n\n");
    printw("1) Cadastro de Pessoa Física\n");
    printw("2) Cadastro Nacional da Pessoa Jurídica\n\n");
    printw("Digite a opção escolhida: ");
    scanw("%d", &tCadastro);
    refresh();

    switch (tCadastro) {
        case 1: {
            clear();
            char cpf[12];
            printw("Digite o CPF (somente números): ");
            echo();
            scanw("%11s", cpf);
            noecho();
            int ehValido = validaCPF(cpf);
            if (ehValido) printw("\nCPF válido!\n");
            else printw("\nCPF inválido!\n");
            refresh();
            getch();
            break;
        }
        case 2: {
            clear();
            char cnpj[15];
            printw("Digite o CNPJ (somente números): ");
            echo();
            scanw("%14s", cnpj);
            noecho();
            int ehValido = validaCNPJ(cnpj);
            if (ehValido) printw("\nCNPJ válido!\n");
            else printw("\nCNPJ inválido!\n");
            refresh();
            getch();
            break;
        }
        default:
            printw("Opção inválida!\n");
            refresh();
            getch();
            break;
    }
}

int main() {
    FILE* fpClientes = fopen("clientes.csv", "a+");
    FILE* fpProdutos = fopen("produtos.csv", "a+");
    FILE* fpPedidos = fopen("pedidos.csv", "a+");

    int inicioValor = menuInicial();
    clear();

    switch (inicioValor) {
        case 1: {
            int manterValor = manterClientes();
            switch (manterValor) {
                case 1: cadastrarClientes(); break;
                case 2: printw("Consultar Clientes"); break;
                case 3: printw("Remover Clientes"); break;
                case 4: printw("Listar Clientes!"); break;
                default: printw("Opção Inválida!"); break;
            }
            break;
        }
        case 2: printw("Você escolheu: Manter Produtos\n"); break;
        case 3: printw("Você escolheu: Manter Pedidos\n"); break;
        default: printw("Opção inválida.\n"); break;
    }

    printw("\nPressione qualquer tecla para sair...");
    refresh();
    getch();
    sair(fpClientes, fpProdutos, fpPedidos);
    return EXIT_SUCCESS;
}
