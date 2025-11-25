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
    float telefone;
    char email[maxEmail];
    char rua[maxEndereco], setor[maxEndereco], cidade[maxEndereco],
        estado[maxEndereco];
    char cpf[tamCPF], razaoSocial[maxRazao], cnpj[tamCNPJ];

    while (fscanf(fp,
                  "%d,%29[^,],%f,%49[^,],%39[^,],%39[^,],%39[^,],%39[^,],%11[^,"
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
    int tecla;
    int escolha = -1;
    int destaque = 0;

    char* escolhas[] = {"1) Cadastro de Pessoa Física",
                        "2) Cadastro de Pessoa Jurídica", "3) Sair"};

    int numEscolhas = sizeof(escolhas) / sizeof(char*);

    while (escolha == -1) {
        clear();
        mvprintw(0, 0, "===== TIPO DE CADASTRO =====");

        for (int i = 0; i < numEscolhas; i++) {
            int x = 5;
            int y = 5 + i;

            if (i == destaque) {
                attron(A_REVERSE);
            }

            mvprintw(y, x, "%s", escolhas[i]);
            attroff(A_REVERSE);
        }

        refresh();

        tecla = getch();

        switch (tecla) {
            case KEY_UP:
                destaque--;
                if (destaque < 0) {
                    destaque = numEscolhas - 1;
                }
                break;
            case KEY_DOWN:
                destaque++;
                if (destaque >= numEscolhas) {
                    destaque = 0;
                }
                break;
            case 10:
                escolha = destaque;
                break;
        }
    }

    switch (escolha) {
        case 0:
            int validade;
            do {
                clear();
                echo();

                int codigo;

                mvprintw(0, 0, "===== Cadastro de Cliente (PF) =====\n\n");
                mvprintw(5, 5, "Digite o código identificador: ");
                scanw("%d", &codigo);

                refresh();

                if (codigo <= 0) {
                    mvprintw(10, 5, "=== CODIGO NAO E VALIDO ===");
                    refresh();
                    getchar();
                    break;
                }

                int verdadeiro = codigoClienteJaExiste(fp, codigo);

                if (verdadeiro) {
                    clear();
                    mvprintw(0, 0, "ERRO: Código de cliente já cadastrado!");
                    refresh();
                    getch();
                } else {
                    clear();

                    int validadeCPF;
                    char cpf[tamCPF];

                    mvprintw(0, 0, "===== CADASTRO DE CPF =====");
                    mvprintw(5, 5, "Digite o CPF (somente 11 números): ");
                    getnstr(cpf, 12);
                    noecho();
                    validadeCPF = validaCPF(cpf);

                    if (validadeCPF) {
                        mvprintw(0, 0, "\nCPF válido!\n");
                        refresh();

                        Cliente cliente;
                        cliente.identificador = codigo;
                        strcpy(cliente.cpf, cpf);
                        strcpy(cliente.razaoSocial, "NA");
                        strcpy(cliente.cnpj, "NA");
                        getch();
                        clear();

                        mvprintw(0, 0, "===== CADASTRO (Código: %d) =====\n\n",
                                 cliente.identificador);
                        mvprintw(5, 5, "Digite o seu nome: ");
                        echo();
                        getstr(cliente.nome);
                        if (strlen(cliente.nome) == 0) {
                            strcpy(cliente.nome, "NA");
                        }

                        mvprintw(
                            7, 5,
                            "Digite o número de telefone (somente números): ");
                        scanw("%f", &cliente.telefone);
                        getch();

                        mvprintw(9, 5, "Digite o seu email: ");
                        getstr(cliente.email);
                        if (strlen(cliente.email) == 0) {
                            strcpy(cliente.email, "NA");
                        }

                        mvprintw(11, 5, "Digite o seu endereço na ordem:\n\n");
                        mvprintw(12, 5, "Rua: ");
                        getstr(cliente.endereco.rua);
                        if (strlen(cliente.endereco.rua) == 0) {
                            strcpy(cliente.endereco.rua, "NA");
                        }

                        mvprintw(13, 5, "Setor: ");
                        getstr(cliente.endereco.setor);
                        if (strlen(cliente.endereco.setor) == 0) {
                            strcpy(cliente.endereco.setor, "NA");
                        }

                        mvprintw(14, 5, "Cidade: ");
                        getstr(cliente.endereco.cidade);
                        if (strlen(cliente.endereco.cidade) == 0) {
                            strcpy(cliente.endereco.cidade, "NA");
                        }

                        mvprintw(15, 5, "Estado: ");
                        getstr(cliente.endereco.estado);
                        if (strlen(cliente.endereco.estado) == 0) {
                            strcpy(cliente.endereco.estado, "NA");
                        }
                        noecho();

                        fprintf(fp, "%d,%s,%f,%s,%s,%s,%s,%s,%s,%s,%s\n",
                                cliente.identificador, cliente.nome,
                                cliente.telefone, cliente.email,
                                cliente.endereco.rua, cliente.endereco.setor,
                                cliente.endereco.cidade,
                                cliente.endereco.estado, cliente.cpf,
                                cliente.razaoSocial, cliente.cnpj);
                        fflush(fp);

                        mvprintw(17, 5, "\nCliente cadastrado com sucesso!\n");
                        validade = 1;
                    } else {
                        printw("\nCPF inválido! Digite novamente!\n");
                        getch();
                    }
                }

            } while (validade != 1);
            break;
        case 1:
            int validade01;

            do {
                clear();
                echo();

                int codigo;

                mvprintw(0, 0, "===== Cadastro de Cliente (PJ) =====\n\n");
                mvprintw(5, 5, "Digite o código identificador: ");
                scanw("%d", &codigo);

                refresh();

                int verdadeiro = codigoClienteJaExiste(fp, codigo);

                if (verdadeiro) {
                    clear();
                    mvprintw(0, 0, "ERRO: Código de cliente já cadastrado!");
                    refresh();
                    getch();
                } else {
                    clear();

                    int validadeCNPJ;
                    char cnpj[tamCNPJ];

                    mvprintw(0, 0, "===== CADASTRO DE CNPJ =====");
                    mvprintw(5, 5, "Digite o CNPJ: ");
                    getnstr(cnpj, 15);
                    noecho();
                    validadeCNPJ = validaCNPJ(cnpj);

                    if (validadeCNPJ) {
                        mvprintw(0, 0, "\nCNPJ válido!\n");
                        refresh();

                        Cliente cliente;
                        cliente.identificador = codigo;
                        strcpy(cliente.cnpj, cnpj);
                        strcpy(cliente.cpf, "NA");
                        strcpy(cliente.email, "NA");
                        getch();
                        clear();

                        mvprintw(0, 0, "===== CADASTRO (Código: %d) =====\n\n",
                                 cliente.identificador);
                        mvprintw(5, 5, "Digite o seu nome: ");
                        echo();
                        getstr(cliente.nome);
                        if (strlen(cliente.nome) == 0) {
                            strcpy(cliente.nome, "NA");
                        }

                        mvprintw(5, 5, "Digite sua razão social: ");
                        getstr(cliente.razaoSocial);
                        if (strlen(cliente.razaoSocial) == 0) {
                            strcpy(cliente.razaoSocial, "NA");
                        }

                        mvprintw(
                            7, 5,
                            "Digite o número de telefone (somente números): ");
                        scanw("%f", &cliente.telefone);
                        getch();

                        mvprintw(9, 5, "Digite o seu email: ");
                        getstr(cliente.email);
                        if (strlen(cliente.email) == 0) {
                            strcpy(cliente.email, "NA");
                        }

                        mvprintw(11, 5, "Digite o seu endereço na ordem:\n\n");
                        mvprintw(12, 5, "Rua: ");
                        getstr(cliente.endereco.rua);
                        if (strlen(cliente.endereco.rua) == 0) {
                            strcpy(cliente.endereco.rua, "NA");
                        }

                        mvprintw(13, 5, "Setor: ");
                        getstr(cliente.endereco.setor);
                        if (strlen(cliente.endereco.setor) == 0) {
                            strcpy(cliente.endereco.setor, "NA");
                        }

                        mvprintw(14, 5, "Cidade: ");
                        getstr(cliente.endereco.cidade);
                        if (strlen(cliente.endereco.cidade) == 0) {
                            strcpy(cliente.endereco.cidade, "NA");
                        }

                        mvprintw(15, 5, "Estado: ");
                        getstr(cliente.endereco.estado);
                        if (strlen(cliente.endereco.estado) == 0) {
                            strcpy(cliente.endereco.estado, "NA");
                        }
                        noecho();

                        fprintf(fp, "%d,%s,%f,%s,%s,%s,%s,%s,%s,%s,%s\n",
                                cliente.identificador, cliente.nome,
                                cliente.telefone, cliente.email,
                                cliente.endereco.rua, cliente.endereco.setor,
                                cliente.endereco.cidade,
                                cliente.endereco.estado, cliente.cpf,
                                cliente.razaoSocial, cliente.cnpj);
                        fflush(fp);

                        mvprintw(17, 5, "\nCliente cadastrado com sucesso!\n");
                        validade = 1;
                    } else {
                        printw("\nCNPJ inválido! Digite novamente!\n");
                        getch();
                    }
                }

            } while (validade01 != 1);
            break;
    }
}

void consultarClientes(FILE* fp) {
    clear();
    int identificador;
    char nome[maxNome];
    float telefone;
    char email[maxEmail];
    char rua[maxEndereco], setor[maxEndereco], cidade[maxEndereco],
        estado[maxEndereco];
    char cpf[tamCPF], razaoSocial[maxRazao], cnpj[tamCNPJ];
    int code;
    int contador = 0;

    echo();

    rewind(fp);

    printw("===== CONSULTAR CLIENTES =====\n\n");

    printw("Digite o código verificador: ");
    scanw("%d", &code);
    getch();
    refresh();

    while (fscanf(fp,
                  "%d,%29[^,],%f,%49[^,],%39[^,],%39[^,],%39[^,],%39[^,],%11[^,"
                  "],%39[^,],%14[^\n]\n",
                  &identificador, nome, &telefone, email, rua, setor, cidade,
                  estado, cpf, razaoSocial, cnpj) == 11) {
        if (identificador == code) {
            contador++;
            printw(
                "\nIdentificador: %d\nNome: %s\nTelefone: %f\nEmail: %s\nRua: "
                "%s\nSetor: %s\nCidade: %s\nEstado: %s\nCPF: %s\nRazão Social: "
                "%s\nCNPJ: %s\n",
                identificador, nome, telefone, email, rua, setor, cidade,
                estado, cpf, razaoSocial, cnpj);
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
    float telefone;
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
                  "%d,%29[^,],%f,%49[^,],%39[^,],%39[^,],%39[^,],%39[^,],%11[^,"
                  "],%39[^,],%14[^\n]\n",
                  &identificador, nome, &telefone, email, rua, setor, cidade,
                  estado, cpf, razaoSocial, cnpj) == 11) {
        if (identificador != code) {
            fprintf(fpTemp, "%d,%s,%f,%s,%s,%s,%s,%s,%s,%s,%s\n", identificador,
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

int listarClientes(FILE* fp) {
    clear();

    int identificador;
    char nome[maxNome];
    float telefone;
    char email[maxEmail];
    char rua[maxEndereco], setor[maxEndereco], cidade[maxEndereco],
        estado[maxEndereco];
    char cpf[tamCPF], razaoSocial[maxRazao], cnpj[tamCNPJ];

    int contador = 0;

    rewind(fp);

    while (fscanf(fp,
                  "%d,%29[^,],%f,%49[^,],%39[^,],%39[^,],%39[^,],%39[^,],%11[^,"
                  "],%39[^,],%14[^\n]\n",
                  &identificador, nome, &telefone, email, rua, setor, cidade,
                  estado, cpf, razaoSocial, cnpj) == 11) {
        contador++;
    }

    if (contador == 0) {
        mvprintw(5, 5, "Nenhum cliente cadastrado.");
        refresh();
        getch();
        return -1;
    }

    Cliente* clientes = (Cliente*)malloc(contador * sizeof(Cliente));
    if (clientes == NULL) {
        mvprintw(5, 5, "Erro fatal: Falha ao alocar memória!");
        refresh();
        getch();
        return -1;
    }

    rewind(fp);
    int i = 0;
    while (fscanf(fp,
                  "%d,%29[^,],%f,%49[^,],%39[^,],%39[^,],%39[^,],%39[^,],%11[^,"
                  "],%39[^,],%14[^\n]\n",
                  &clientes[i].identificador, clientes[i].nome,
                  &clientes[i].telefone, clientes[i].email,
                  clientes[i].endereco.rua, clientes[i].endereco.setor,
                  clientes[i].endereco.cidade, clientes[i].endereco.estado,
                  clientes[i].cpf, clientes[i].razaoSocial,
                  clientes[i].cnpj) == 11) {
        i++;
    }

    int tecla;
    int escolha = -1;
    int destaque = 0;

    while (escolha == -1) {
        clear();
        mvprintw(0, 0, "===== SELECIONE UM CLIENTE =====");
        mvprintw(2, 5,
                 "Use as setas para navegar. ENTER para selecionar. 'q' para "
                 "cancelar.");

        for (i = 0; i < contador; i++) {
            int x = 5;
            int y = 5 + i;

            if (destaque == i) {
                attron(A_REVERSE);
                mvprintw(5, 60, "Email: %s", clientes[i].email);
                mvprintw(7, 60, "Telefone: %.2f", clientes[i].telefone);
                mvprintw(9, 60, "CPF: %s", clientes[i].cpf);
                mvprintw(11, 60, "CNPJ: %s", clientes[i].cnpj);
                mvprintw(13, 60, "Razão Social: %s", clientes[i].razaoSocial);
                mvprintw(15, 60, "Cidade: %s", clientes[i].endereco.cidade);
                mvprintw(17, 60, "Estado: %s", clientes[i].endereco.estado);
                mvprintw(19, 60, "Setor: %s", clientes[i].endereco.setor);
                mvprintw(21, 60, "Rua: %s", clientes[i].endereco.rua);
            }

            mvprintw(y, x, "%d - %s", clientes[i].identificador,
                     clientes[i].nome);

            attroff(A_REVERSE);
        }

        refresh();

        tecla = getch();

        switch (tecla) {
            case KEY_UP:
                destaque--;
                if (destaque < 0) {
                    destaque = contador - 1;
                }
                break;
            case KEY_DOWN:
                destaque++;
                if (destaque >= contador) {
                    destaque = 0;
                }
                break;
            case 10:
                escolha = clientes[destaque].identificador;
                break;
            case 'q':
            case 'Q':
                escolha = -1;
                break;
        }
    }

    free(clientes);
    clear();
    return escolha;
}