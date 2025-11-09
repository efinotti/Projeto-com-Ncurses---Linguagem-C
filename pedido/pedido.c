#include "pedido.h"

Pedido listaPedidos[maxPedidos];
int totalPedidos = 0;

void obterDataAtual(char *destino, int tamanho){
    time_t agora = time(NULL);
    struct tm *tm_info = localtime(&agora);
    
    snprintf(destino, tamanho, "%02d/%02d/%04d",
        tm_info->tm_mday, tm_info->tm_mon + 1, tm_info->tm_year + 1900);
}

int codigoClienteJaExiste(FILE* fpC, int code){
    rewind(fpC);
    
    int identificador;
    char nome[maxNome];
    int telefone;
    char email[maxEmail];
    char rua[maxEndereco], setor[maxEndereco], cidade[maxEndereco],estado[maxEndereco];
    char cpf[tamCPF], razaoSocial[maxRazao], cnpj[tamCNPJ];

    while (fscanf(fpC,"%d,%29[^,],%d,%49[^,],%39[^,],%39[^,],%39[^,],%39[^,],%11[^,],%39[^,],%14[^\n]\n",
                  &identificador, nome, &telefone, email, rua, setor, cidade,estado,
                  cpf, razaoSocial, cnpj) == 11) {
        if (identificador == code) {
            return 1;
        }
    }
    return 0;
}

int codigoProdutoJaExiste(FILE* fpP, int code){
    rewind(fpP);

    int identificador;
    char descricao[maxDescricao];
    double preco;
    int estoque;

    while (fscanf(fpP, "%d,%[^,],%lf,%d\n", &identificador, descricao, &preco, &estoque) == 4) {
        if (identificador == code) {
            return 1;
        }
    }
    return 0;
}


int codigoPedidoJaExiste(FILE* fpPe, int code){
    rewind(fpPe);

    int codigoPedido;
    int codigoCliente;
    int codigoProduto;
    int quantidade;
    char data[maxData];
    char descricao[maxDescricao];
    float valorTotal;
    char status[maxStatus];

    while(fscanf(fpPe, "%d,%d,%d,%d,%[^,],%[^,],%f,%[^\n]\n",
                &codigoPedido, &codigoCliente, &codigoProduto, &quantidade, data,
                descricao, &valorTotal, status) == 8){
    if(codigoPedido == code){
        return 1;
        }
    }
    return 0;
}

void cadastrarPedido(FILE *fpPe, FILE *fpC, FILE *fpP){
    clear();

    Pedido pedido;

    printw("\n\n=====CADASTRO DE PEDIDO=====\n\n");

    printw("Digite o código do pedido (não pode ser 0):\n");
    fflush(stdout);
    scanw("%d", &pedido.codigoPedido);
    getch();

    if(codigoPedidoJaExiste(fpPe, pedido.codigoPedido)){
        printw("\nErro: Já existe este código de pedido!\n");
        getch();
        return;
    }

    printw("Digite o código do cliente:\n");
    fflush(stdout);
    scanw("%d", &pedido.codigoCliente);
    getch();
    
    if(!codigoClienteJaExiste(fpC, pedido.codigoCliente)){
        printw("\nErro: Cliente não encontrado!\n");
        getch();
        return;
    }

    printw("Digite o código do Produto:\n");
    fflush(stdout);
    scanw("%d", &pedido.codigoProduto);
    getch();
    
    if(!codigoProdutoJaExiste(fpP, pedido.codigoProduto)){
        printw("\nErro: Produto não encontrado!\n");
        getch();
        return;
    }
    
    printw("Digite a quantidade de itens:\n");
    fflush(stdout);
    scanw("%d", &pedido.quantidade);
    getch();

    obterDataAtual(pedido.data, maxData);
    
    printw("Data do pedido gerada automáticamente: %s\n", pedido.data);
    getch();
    
    printw("Faça a descrição do produto:\n");
    fflush(stdout);
    scanw("%s", pedido.descricao);
    getch();
    
    printw("Digite o valor total do produto:\n");
    fflush(stdout);
    scanw("%f", &pedido.valorTotal);
    getch();
    
   strcpy(pedido.status, "Pendente");
    
    fprintf(fpPe, "%d,%d,%d,%d,%s,%s,%.2f,%s\n", pedido.codigoPedido,pedido.codigoCliente,
            pedido.codigoProduto,pedido.quantidade,pedido.data,pedido.descricao,pedido.valorTotal,pedido.status);
        
    fflush(fpPe);

    printw("\nPedido cadastrado com sucesso!\n");
    refresh();
    getch(); 
}

void listarPedidos(FILE *fpPe){
    clear();
    rewind(fpPe);
    
    Pedido pedido;
    
    printw("\n\n=====LISTA DE PEDIDOS=====\n\n");
    
    while(fscanf(fpPe, "%d,%d,%d,%d,%10[^,],%99[^,],%f,%19[^,]",
                &pedido.codigoPedido,&pedido.codigoCliente,&pedido.codigoProduto,&pedido.quantidade,
                pedido.data,pedido.descricao,&pedido.valorTotal,pedido.status) == 8){
                
            printw("Código do Pedido: %d | Cliente: %d | Produto: %d | Descrição: %s | Quantidade: %d | Valor: %.2f | Data: %s | Status: %s\n",
                    pedido.codigoPedido,pedido.codigoCliente,pedido.codigoProduto,pedido.descricao,pedido.quantidade,pedido.valorTotal,
                    pedido.data,pedido.status);
    }
            
    refresh();
    getch();
}

void consultarPedido(FILE *fpPe, FILE *fpC){
    rewind(fpPe);
    
    int codigoCliente, opc, encontro = 0;
    Pedido pedido;
    
    printw("\nInforme o código do Cliente:\n");
    fflush(stdout);
    scanw("%d", &codigoCliente);
    getch();
    
    if(!codigoClienteJaExiste(fpC,codigoCliente)){
        printw("\nErro: Cliente não encontrado!\n");
        getch();
        return;
    }
    
    clear();
    printw("\n=====PEDIDOS DO CLIENTE %d=====\n", codigoCliente);
    
    while(fscanf(fpPe, "%d,%d,%d,%d,%10[^,],%99[^,],%f,%19[^\n]",
                &pedido.codigoPedido,&pedido.codigoCliente,&pedido.codigoProduto,&pedido.quantidade,
                pedido.data,pedido.descricao,&pedido.valorTotal,pedido.status) == 8){
                
        if(pedido.codigoCliente == codigoCliente && pedido.codigoPedido != 0){
            printw("\nCódigo do Pedido: %d | Descrição: %s | Quantidade: %d\n",
                    pedido.codigoPedido,pedido.descricao,pedido.quantidade);
            encontro = 1;
        }
    }
    
    if(encontro == 0){
        printw("\nErro: Nenhum Pedido Encontrado!\n");
        getch();
        return fpPe;
    }
    
    printw("\nQual pedido você deseja consultar?:\n");
    fflush(stdout);
    scanw("%d", &opc);
    
    rewind(fpPe);
    
    while(fscanf(fpPe, "%d,%d,%d,%d,%10[^,],%99[^,],%f,%19[^,]",
                &pedido.codigoPedido,&pedido.codigoCliente,&pedido.codigoProduto,&pedido.quantidade,
                pedido.data,pedido.descricao,&pedido.valorTotal,pedido.status) == 8){
    
    if(pedido.codigoPedido == opc && pedido.codigoCliente == codigoCliente && pedido.codigoPedido != 0){
        printw("\n=====DETALHES DO PEDIDO=====\n");
        printw("Cliente: %d\nProduto: %d\nQuantidade: %d\nData: %s\nStatus: %s\nValor: %.2f\nDescrição: %s\n",pedido.codigoPedido,pedido.codigoCliente,pedido.codigoProduto,pedido.quantidade,
                pedido.data,pedido.descricao,pedido.valorTotal,pedido.status);
        }
    }
}

void atualizarPedido(FILE *fpPe, FILE *fpC, FILE *fpP){
    rewind(fpPe);
    
    int codigoCliente, opc, mod, encontro = 0;
    Pedido pedido;
    
    printw("\nInforme o código do Cliente:\n");
    fflush(stdout);
    scanw("%d",&codigoCliente);
    getch();
    
    if(!codigoClienteJaExiste(fpC,codigoCliente)){
        printw("\nErro: Cliente não encontrado!\n");
        getch();
        return fpPe;
    }
    
    clear();
    printw("\n=====PEDIDOS DO CLIENTE %d=====\n", codigoCliente);
    
    for (int i = 0; i < totalPedidos; i++) {
        if (listaPedidos[i].codigoCliente == codigoCliente && listaPedidos[i].codigoPedido != 0) {
            printw("Código Pedido: %d | Produto: %d | Quantidade: %d | Descrição: %s | Status: %s\n",
                   listaPedidos[i].codigoPedido,
                   listaPedidos[i].codigoProduto,
                   listaPedidos[i].quantidade,
                   listaPedidos[i].descricao,
                   listaPedidos[i].status);
            encontro = 1;
        }
    }

    if (!encontro) {
        printw("\nNenhum pedido encontrado!\n");
        getch();
        return;
    }

    printw("\nDigite o código do pedido que deseja modificar:\n");
    fflush(stdout);
    scanw("%d", &opc);

    for (int i = 0; i < totalPedidos; i++) {
        if (listaPedidos[i].codigoPedido == opc && listaPedidos[i].codigoCliente == codigoCliente) {

            printw("\n===== MENU DE MODIFICAÇÕES =====\n");
            printw("1 - Código do Produto\n2 - Quantidade\n3 - Descrição\n4 - Status\n");
            printw("Qual campo deseja modificar?:\n");
            fflush(stdout);
            scanw("%d", &mod);

            switch(mod){
                case 1:
                    printw("Novo código do produto:\n");
                    fflush(stdout);
                    scanw("%d", &listaPedidos[i].codigoProduto);
                    printw("\nAlteração feita com sucesso!\n");
                    break;

                case 2:
                    printw("Nova quantidade:\n");
                    fflush(stdout);
                    scanw("%d", &listaPedidos[i].quantidade);
                    printw("\nAlteração feita com sucesso!\n");
                    break;

                case 3:
                    printw("Nova descrição:\n");
                    fflush(stdout);
                    scanw("%s", listaPedidos[i].descricao);
                    printw("\nAlteração feita com sucesso!\n");
                    break;

                case 4:
                    printw("Novo status:\n");
                    fflush(stdout);
                    scanw("%s", listaPedidos[i].status);
                    printw("\nAlteração feita com sucesso!\n");
                    break;

                default:
                    printw("Erro: Opção Inválida!\n");
                    break;
            }

            getch();
            return;
        }
    }

    printw("\nErro: Pedido não encontrado!\n");
    getch();
}

FILE* deletarPedido(FILE *fpPe, FILE *fpC){
    rewind(fpPe);
    
    int codigoCliente, opc, encontro = 0;
    Pedido pedido;
    
    printw("\nInforme o código do Cliente:\n");
    fflush(stdout);
    scanw("%d", &codigoCliente);
    getch();
    
    if(!codigoClienteJaExiste(fpC,codigoCliente)){
        printw("\nErro: Cliente não encontrado!\n");
        getch();
        return fpPe;
    }
    
    clear();
    printw("\n=====PEDIDOS DO CLIENTE %d=====\n", codigoCliente);
    
    while(fscanf(fpPe, "%d,%d,%d,%d,%10[^,],%99[^,],%f,%19[^\n]",
                &pedido.codigoPedido,&pedido.codigoCliente,&pedido.codigoProduto,&pedido.quantidade,
                pedido.data,pedido.descricao,&pedido.valorTotal,pedido.status) == 8){
                
        if(pedido.codigoCliente == codigoCliente && pedido.codigoPedido != 0){
            printw("\nCódigo do Pedido: %d | Descrição: %s | Quantidade: %d\n",
                    pedido.codigoPedido,pedido.descricao,pedido.quantidade);
            encontro = 1;
        }
    }
    
    if(encontro == 0){
        printw("\nErro: Nenhum Pedido Encontrado!\n");
        getch();
        return fpPe;
    }
    
    printw("\nInforme o código do pedido que você deseja desfazer:\n");
    fflush(stdout);
    scanw("%d", &opc);
    getch();
    
    rewind(fpPe);
    
    while(fscanf(fpPe, "%d,%d,%d,%d,%10[^,],%99[^,],%f,%19[^\n]",
                &pedido.codigoPedido,&pedido.codigoCliente,&pedido.codigoProduto,&pedido.quantidade,
                pedido.data,pedido.descricao,&pedido.valorTotal,pedido.status) == 8){
                    
        if(pedido.codigoPedido == opc){
            pedido.codigoPedido = 0;
            printw("\nO pedido foi desfeito com sucesso!\n");
            getch();
            break;
        }
    }
    
    fflush(fpPe);
    return fpPe;
}