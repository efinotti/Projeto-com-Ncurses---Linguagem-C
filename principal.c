#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define maxNome 30
#define maxRazao 40
#define maxEmail 50
#define maxDescricao 100
#define maxEndereco 40
#define maxData 10

#define tamCPF 12
#define tamCNPJ 15

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
    getch(); // Limpa o buffer
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
    getch(); // Limpa o buffer
    refresh();
    return x;
}

int validaCPF(char cpf[tamCPF]) {
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

    return (primeiroDigitoV == (cpf[9] - '0') &&
            segundoDigitoV == (cpf[10] - '0'));
}

int validaCNPJ(char cnpj[tamCNPJ]) {
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

int codigoJaExiste(FILE* fp, int code) {
    rewind(fp);

    int identificador;
    char nome[maxNome];
    int telefone;
    char email[maxEmail];
    char rua[maxEndereco], setor[maxEndereco], cidade[maxEndereco],
        estado[maxEndereco];
    char cpf[tamCPF], razaoSocial[maxRazao], cnpj[tamCNPJ];

    while (fscanf(fp, "%d,%29[^,],%d,%49[^,],%39[^,],%39[^,],%39[^,],%39[^,],%11[^,],%39[^,],%14[^\n]\n",
                  &identificador, nome, &telefone, email, 
                  rua, setor, cidade, estado,
                  cpf, razaoSocial, cnpj) == 11) {
        if (identificador == code) {
            return 1;
        }
    }
    return 0;
}

void cadastrarClientes(FILE* fp) {
    clear();
    int tCadastro;
    printw("===== TIPO DE CADASTRO =====\n\n");
    printw("1) Cadastro de Pessoa Física\n");
    printw("2) Cadastro Nacional da Pessoa Jurídica\n\n");
    printw("Digite a opção escolhida: ");
    scanw("%d", &tCadastro);
    getch(); // Limpa o buffer
    refresh();

    switch (tCadastro) {
        case 1: {
            int code;

            clear();
            printw("===== Cadastro de Cliente (PF) =====\n\n");

            printw("Digite o código identificador: ");
            scanw("%d", &code);
            getch(); // Limpa o buffer
            refresh();

            if (codigoJaExiste(fp, code)) {
                clear();
                printw("ERRO: Código de cliente já cadastrado!\n");
                refresh();
                getch();
            } else {
                clear();
                char cpf[tamCPF];
                printw("Digite o CPF (somente 11 números): ");
                echo();
                getnstr(cpf, 11); // Usando getnstr
                noecho();

                if (validaCPF(cpf)) {
                    printw("\nCPF válido!\n");
                    refresh();

                    Cliente cliente;
                    cliente.identificador = code;
                    strcpy(cliente.cpf, cpf);
                    strcpy(cliente.razaoSocial, "NA");
                    strcpy(cliente.cnpj, "NA");


                    clear();
                    printw("===== CADASTRO (Código: %d) =====\n\n",
                           cliente.identificador);
                    printw("Digite o seu nome: ");
                    echo();
                    getstr(cliente.nome);
                    printw("Digite o número de telefone (somente números): ");
                    scanw("%d", &cliente.telefone);
                    getch(); // Limpa o buffer
                    printw("Digite o seu email: ");
                    getstr(cliente.email);
                    printw("Digite o seu endereço na ordem:\n\n");
                    printw("Rua: ");
                    getstr(cliente.endereco.rua);
                    printw("Setor: ");
                    getstr(cliente.endereco.setor);
                    printw("Cidade: ");
                    getstr(cliente.endereco.cidade);
                    printw("Estado: ");
                    getstr(cliente.endereco.estado);
                    noecho();

                    fprintf(fp, "%d,%s,%d,%s,%s,%s,%s,%s,%s,%s,%s\n",
                            cliente.identificador, cliente.nome,
                            cliente.telefone, cliente.email,
                            cliente.endereco.rua, cliente.endereco.setor,
                            cliente.endereco.cidade, cliente.endereco.estado,
                            cliente.cpf, cliente.razaoSocial, cliente.cnpj);
                    fflush(fp);

                    printw("\nCliente cadastrado com sucesso!\n");
                } else {
                    printw("\nCPF inválido!\n");
                }
                refresh();
                getch();
            }
            break;
        }
        case 2: {
            int codigo;
            Cliente cliente;

            clear();
            printw("===== Cadastro de Cliente (PJ) =====\n\n");

            printw("Digite o código identificador do cliente: ");
            scanw("%d", &cliente.identificador);
            getch(); // Limpa o buffer
            refresh();

            if (codigoJaExiste(fp, cliente.identificador)) {
                clear();
                printw("ERRO: Código de cliente já cadastrado!");
                getch(); // Usando getch
                refresh();
            } else {
                clear();
                char cnpj[tamCNPJ];
                printw(
                    "Código não cadastrado! Prosseguimos com o cadastro do "
                    "CNPJ.\n\n");
                printw("Digite o CNPJ (somente 14 números): ");
                echo();
                getnstr(cnpj, 14); // Usando getnstr
                noecho();
                
                if (validaCNPJ(cnpj)) {
                    printw("\nCNPJ válido!\n");
                    getch(); // Usando getch
                    refresh();

                    strcpy(cliente.cnpj, cnpj);
                    strcpy(cliente.cpf, "NA");

                    clear();
                    printw("===== CADASTRO (Código: %d) =====\n\n",
                           cliente.identificador);
                    printw("Digite a razão social: ");
                    echo();
                    getstr(cliente.razaoSocial);
                    printw("Digite o nome para o contato: ");
                    getstr(cliente.nome);
                    printw("Digite o telefone para contato (somente números): ");
                    scanw("%d", &cliente.telefone);
                    getch(); // Limpa o buffer
                    printw("Digite o seu email: ");
                    getstr(cliente.email);
                    printw("Digite o seu endereço na ordem:\n");
                    printw("Rua: ");
                    getstr(cliente.endereco.rua);
                    printw("Setor: ");
                    getstr(cliente.endereco.setor);
                    printw("Cidade: ");
                    getstr(cliente.endereco.cidade);
                    printw("Estado: ");
                    getstr(cliente.endereco.estado);
                    noecho();

                    fprintf(fp, "%d,%s,%d,%s,%s,%s,%s,%s,%s,%s,%s\n", 
                            cliente.identificador,
                            cliente.nome, cliente.telefone, cliente.email,
                            cliente.endereco.rua, cliente.endereco.setor,
                            cliente.endereco.cidade, cliente.endereco.estado,
                            cliente.cpf, cliente.razaoSocial, cliente.cnpj);
                    fflush(fp);

                    printw("\nCliente cadastrado com sucesso!\n");

                } else {
                    printw("\nCNPJ inválido!\n");
                    getch(); // Usando getch
                }
                refresh();
                getch();
            }

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

    // Se não conseguir abrir qualquer dos arquivos, ele irá finalizar
    if (fpClientes == NULL || fpProdutos == NULL || fpPedidos == NULL) {
        endwin();
        printf("Erro fatal: Não foi possível abrir os arquivos .csv necessários.\n");
        return EXIT_FAILURE;
    }

    int inicioValor = menuInicial();
    clear();


    // Menu Inicial
    switch (inicioValor) {
        case 1: {
            int manterValor = manterClientes();
            switch (manterValor) {
                case 1:
                    cadastrarClientes(fpClientes);
                    break;
                case 2:
                    printw("Consultar Clientes (Não implementado)");
                    break;
                case 3:
                    printw("Remover Clientes (Não implementado)");
                    break;
                case 4:
                    printw("Listar Clientes (Não implementado)");
                    break;
                default:
                    printw("Opção Inválida!");
                    break;
            }
            break;
        }
        case 2:
            printw("Você escolheu: Manter Produtos (Não implementado)\n");
            break;
        case 3:
            printw("Você escolheu: Manter Pedidos (Não implementado)\n");
            break;
        default:
            printw("Opção inválida.\n");
            break;
    }

    printw("\nPressione qualquer tecla para sair...");
    refresh();
    getch();
    sair(fpClientes, fpProdutos, fpPedidos);
    return EXIT_SUCCESS;
}