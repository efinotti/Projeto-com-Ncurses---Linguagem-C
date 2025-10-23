#include "cliente.h"

int validaCPF(char cpf[tamCPF]) {
    int soma = 0;
    int j, i;
    int primeiroDigitoV, segundoDigitoV;
    for (i = 0; i < 11; i++) {
        if (cpf[i] < '0' || cpf[i] > '9') return 0;
    }

    soma = 0;
    j = 10;
    for (i = 0; i < 9; i++) {
        soma += (cpf[i] - '0') * j--;
    }
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

int codigoClienteJaExiste(FILE* fp, int code) {
    rewind(fp);

    int identificador;
    char nome[maxNome];
    int telefone;
    char email[maxEmail];
    char rua[maxEndereco], setor[maxEndereco], cidade[maxEndereco],
        estado[maxEndereco];
    char cpf[tamCPF], razaoSocial[maxRazao], cnpj[tamCNPJ];

    while (fscanf(fp,
                  "%d,%29[^,],%d,%49[^,],%39[^,],%39[^,],%39[^,],%39[^,],%11[^,"
                  "],%39[^,],%14[^\n]\n",
                  &identificador, nome, &telefone, email, rua, setor, cidade,
                  estado, cpf, razaoSocial, cnpj) == 11) {
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
    printw("2) Cadastro Nacional da Pessoa Jurídica\n");
    printw("3) Voltar\n\n");
    printw("Digite a opção escolhida: ");
    scanw("%d", &tCadastro);
    getch();
    refresh();

    switch (tCadastro) {
        case 1: {
            int validade = 0; 
            do {
                int code;
                clear();
                printw("===== Cadastro de Cliente (PF) =====\n\n");

                printw("Digite o código identificador: ");
                scanw("%d", &code);
                getch();
                refresh();

                int verdadeiro = codigoClienteJaExiste(fp, code);

                if (verdadeiro) {
                    clear();
                    printw("ERRO: Código de cliente já cadastrado!\n");
                    refresh();
                    getch();
                } else {
                    int validadeCPF;
                    char cpf[tamCPF];

                    clear();
                    printw("===== CADASTRO DE CPF =====\n\n");
                    printw("Digite o CPF (somente 11 números): ");
                    echo();
                    getnstr(cpf, 11);
                    noecho();
                    validadeCPF = validaCPF(cpf);

                    if (validadeCPF) {
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
                        if (strlen(cliente.nome) == 0) {
                            strcpy(cliente.nome, "NA");
                        }

                        printw(
                            "Digite o número de telefone (somente números): ");
                        scanw("%d", &cliente.telefone);
                        getch();

                        printw("Digite o seu email: ");
                        getstr(cliente.email);
                        if (strlen(cliente.email) == 0) {
                            strcpy(cliente.email, "NA");
                        }

                        printw("Digite o seu endereço na ordem:\n\n");
                        printw("Rua: ");
                        getstr(cliente.endereco.rua);
                        if (strlen(cliente.endereco.rua) == 0) {
                            strcpy(cliente.endereco.rua, "NA");
                        }

                        printw("Setor: ");
                        getstr(cliente.endereco.setor);
                        if (strlen(cliente.endereco.setor) == 0) {
                            strcpy(cliente.endereco.setor, "NA");
                        }

                        printw("Cidade: ");
                        getstr(cliente.endereco.cidade);
                        if (strlen(cliente.endereco.cidade) == 0) {
                            strcpy(cliente.endereco.cidade, "NA");
                        }

                        printw("Estado: ");
                        getstr(cliente.endereco.estado);
                        if (strlen(cliente.endereco.estado) == 0) {
                            strcpy(cliente.endereco.estado, "NA");
                        }
                        noecho();

                        fprintf(fp, "%d,%s,%d,%s,%s,%s,%s,%s,%s,%s,%s\n",
                                cliente.identificador, cliente.nome,
                                cliente.telefone, cliente.email,
                                cliente.endereco.rua, cliente.endereco.setor,
                                cliente.endereco.cidade,
                                cliente.endereco.estado, cliente.cpf,
                                cliente.razaoSocial, cliente.cnpj);
                        fflush(fp);

                        printw("\nCliente cadastrado com sucesso!\n");
                        validade = 1;
                    } else {
                        printw("\nCPF inválido! Digite novamente!\n");
                        getch();
                    }
                    refresh();
                    getch();
                }
            } while (validade != 1);
            break;
        }
        case 2: {
            Cliente cliente;
            int validado_loop = 0; 

            do {
                clear();
                printw("===== Cadastro de Cliente (PJ) =====\n\n");

                printw("Digite o código identificador do cliente: ");
                scanw("%d", &cliente.identificador);
                getch();
                refresh();
                
                int code_existe = codigoClienteJaExiste(fp, cliente.identificador);
                
                if (code_existe) {
                    clear();
                    printw("ERRO: Código de cliente já cadastrado!");
                    getch();
                    refresh();
                } else {
                    int cnpjValidado = 0;
                    char cnpj[tamCNPJ];

                    clear();
                    printw(
                        "Código não cadastrado! Prosseguimos com o "
                        "cadastro do "
                        "CNPJ.\n\n");
                    printw("Digite o CNPJ (somente 14 números): ");
                    echo();
                    getnstr(cnpj, 14);
                    noecho();

                    cnpjValidado = validaCNPJ(cnpj);

                    if (cnpjValidado) {
                        printw("\nCNPJ válido!\n");
                        getch();
                        refresh();

                        strcpy(cliente.cnpj, cnpj);
                        strcpy(cliente.cpf, "NA");

                        clear();
                        printw("===== CADASTRO (Código: %d) =====\n\n",
                               cliente.identificador);
                        printw("Digite a razão social: ");
                        echo();
                        getstr(cliente.razaoSocial);
                        if (strlen(cliente.razaoSocial) == 0) {
                            strcpy(cliente.razaoSocial, "NA");
                        }

                        printw("Digite o nome para o contato: ");
                        getstr(cliente.nome);
                        if (strlen(cliente.nome) == 0) {
                            strcpy(cliente.nome, "NA");
                        }

                        printw(
                            "Digite o telefone para contato (somente "
                            "números): ");
                        scanw("%d", &cliente.telefone);
                        getch();

                        printw("Digite o seu email: ");
                        getstr(cliente.email);
                        if (strlen(cliente.email) == 0) {
                            strcpy(cliente.email, "NA");
                        }

                        printw("Digite o seu endereço na ordem:\n");
                        printw("Rua: ");
                        getstr(cliente.endereco.rua);
                        if (strlen(cliente.endereco.rua) == 0) {
                            strcpy(cliente.endereco.rua, "NA");
                        }

                        printw("Setor: ");
                        getstr(cliente.endereco.setor);
                        if (strlen(cliente.endereco.setor) == 0) {
                            strcpy(cliente.endereco.setor, "NA");
                        }

                        printw("Cidade: ");
                        getstr(cliente.endereco.cidade);
                        if (strlen(cliente.endereco.cidade) == 0) {
                            strcpy(cliente.endereco.cidade, "NA");
                        }

                        printw("Estado: ");
                        getstr(cliente.endereco.estado);
                        if (strlen(cliente.endereco.estado) == 0) {
                            strcpy(cliente.endereco.estado, "NA");
                        }
                        noecho();

                        fprintf(fp, "%d,%s,%d,%s,%s,%s,%s,%s,%s,%s,%s\n",
                                cliente.identificador, cliente.nome,
                                cliente.telefone, cliente.email,
                                cliente.endereco.rua, cliente.endereco.setor,
                                cliente.endereco.cidade,
                                cliente.endereco.estado, cliente.cpf,
                                cliente.razaoSocial, cliente.cnpj);
                        fflush(fp);

                        printw("\nCliente cadastrado com sucesso!\n");
                        validado_loop = 1; 
                    } else {
                        printw("\nCNPJ inválido! Refaça novamente!\n");
                        getch();
                    }

                    refresh();
                    getch();
                }
            } while (validado_loop != 1);
            break;
        }
        case 3:
            break;
        default:
            printw("Opção inválida!\n");
            refresh();
            getch();
            break;
    }
}

void consultarClientes(FILE* fp) {
    clear();
    int identificador;
    char nome[maxNome];
    int telefone;
    char email[maxEmail];
    char rua[maxEndereco], setor[maxEndereco], cidade[maxEndereco],
        estado[maxEndereco];
    char cpf[tamCPF], razaoSocial[maxRazao], cnpj[tamCNPJ];
    int code;
    int contador = 0;

    rewind(fp);

    printw("===== CONSULTAR CLIENTES =====\n\n");

    printw("Digite o código verificador: ");
    scanw("%d", &code);
    getch(); 
    refresh();

    while (fscanf(fp,
                  "%d,%29[^,],%d,%49[^,],%39[^,],%39[^,],%39[^,],%39[^,],%11[^,"
                  "],%39[^,],%14[^\n]\n",
                  &identificador, nome, &telefone, email, rua, setor, cidade,
                  estado, cpf, razaoSocial, cnpj) == 11) {
        if (identificador == code) {
            contador++;
            printw("%d,%s,%d,%s,%s,%s,%s,%s,%s,%s,%s\n", identificador, nome,
                   telefone, email, rua, setor, cidade, estado, cpf,
                   razaoSocial, cnpj);
        }
    }

    if (contador == 0) {
        printw("Não foi encontrado nenhum item com este identificador!");
    }

    printw("\n\nPressione qualquer tecla para continuar...");
    refresh();
    getch();
}

FILE* deletarClientes(FILE* fp) {
    clear();
    int identificador;
    char nome[maxNome];
    int telefone;
    char email[maxEmail];
    char rua[maxEndereco], setor[maxEndereco], cidade[maxEndereco],
        estado[maxEndereco];
    char cpf[tamCPF], razaoSocial[maxRazao], cnpj[tamCNPJ];

    char* nomeArquivo = "clientes.csv";
    char* temporario = "clientesTemp.csv";
    int code;
    int encontrado = 0;

    FILE* fpTemp = fopen(temporario, "w");

    if (fpTemp == NULL) {
        printw("ERRO: Não foi possível criar o arquivo temporário.\n");
        refresh();
        getch();
        return fp;
    }

    printw("====== REMOÇÃO DE CLIENTES ======\n\n");
    printw("Digite o código identificador: ");
    scanw("%d", &code);
    getch();

    rewind(fp);

    while (fscanf(fp,
                  "%d,%29[^,],%d,%49[^,],%39[^,],%39[^,],%39[^,],%39[^,],%11[^,"
                  "],%39[^,],%14[^\n]\n",
                  &identificador, nome, &telefone, email, rua, setor, cidade,
                  estado, cpf, razaoSocial, cnpj) == 11) {
        if (identificador != code) {
            fprintf(fpTemp, "%d,%s,%d,%s,%s,%s,%s,%s,%s,%s,%s\n", identificador,
                    nome, telefone, email, rua, setor, cidade, estado, cpf,
                    razaoSocial, cnpj);
        } else {
            encontrado = 1;
        }
    }

    fclose(fp);
    fclose(fpTemp);

    if (encontrado) {
        if (remove(nomeArquivo) != 0) {
            printw("ERRO: Não foi possível deletar o arquivo original.\n");
        } else if (rename(temporario, nomeArquivo) != 0) {
            printw("ERRO: Não foi possível renomear o arquivo temporário.\n");
        } else {
            printw("Cliente removido com sucesso!\n");
        }
    } else {
        printw("ERRO: Cliente com código %d não encontrado.\n", code);
        remove(temporario);
    }

    refresh();
    getch();

    fp = fopen(nomeArquivo, "a+");
    if (fp == NULL) {
        endwin();
        printf("Erro fatal ao reabrir o arquivo de clientes!\n");
        exit(EXIT_FAILURE);
    }
    return fp;
}

void listarClientes(FILE* fp) {
    clear();
    int identificador;
    char nome[maxNome];
    int telefone;
    char email[maxEmail];
    char rua[maxEndereco], setor[maxEndereco], cidade[maxEndereco],
        estado[maxEndereco];
    char cpf[tamCPF], razaoSocial[maxRazao], cnpj[tamCNPJ];
    
    int contador = 0;

    rewind(fp);

    printw("===== LISTA DE TODOS OS CLIENTES =====\n\n");

    while (fscanf(fp,
                  "%d,%29[^,],%d,%49[^,],%39[^,],%39[^,],%39[^,],%39[^,],%11[^,"
                  "],%39[^,],%14[^\n]\n",
                  &identificador, nome, &telefone, email, rua, setor, cidade,
                  estado, cpf, razaoSocial, cnpj) == 11) {
        printw("%d,%s,%d,%s,%s,%s,%s,%s,%s,%s,%s\n", identificador, nome,
               telefone, email, rua, setor, cidade, estado, cpf, razaoSocial,
               cnpj);
    }

    if (contador == 0) {
        printw("Nenhum cliente cadastrado.\n");
    }
    printw("\n\nPressione qualquer tecla para continuar...");
    refresh();
    getch();
}