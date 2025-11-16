#include "pedido.h"

Pedido listaPedidos[maxPedidos];
int totalPedidos = 0;

void obterDataAtual(char *destino, int tamanho){
    time_t agora = time(NULL);
    struct tm *tm_info = localtime(&agora);
    
    snprintf(destino, tamanho, "%02d/%02d/%04d",
        tm_info->tm_mday, tm_info->tm_mon + 1, tm_info->tm_year + 1900);
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

   while(fscanf(fpPe, "%d,%d,%d,%d,%[^,],%[^,],%f,%[^\n]%*c",
                  &codigoPedido, &codigoCliente, &codigoProduto, &quantidade, data,
                  descricao, &valorTotal, status) == 8){
    if(codigoPedido == code && codigoPedido != 0){ 
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

    if(pedido.codigoPedido == 0){ 
        printw("\nErro: O código do pedido não pode ser 0!\n");
        getch();
        return;
    }

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
    
    while(fscanf(fpPe, "%d,%d,%d,%d,%10[^,],%*c%99[^,],%*c%f,%19[^\n]%*c",
                 &pedido.codigoPedido,&pedido.codigoCliente,&pedido.codigoProduto,&pedido.quantidade,
                 pedido.data,pedido.descricao,&pedido.valorTotal,pedido.status) == 8){
                     
        if(pedido.codigoPedido != 0){
            printw("Código do Pedido: %d | Cliente: %d | Produto: %d | Descrição: %s | Quantidade: %d | Valor: %.2f | Data: %s | Status: %s\n",
                    pedido.codigoPedido,pedido.codigoCliente,pedido.codigoProduto,pedido.descricao,pedido.quantidade,pedido.valorTotal,
                    pedido.data,pedido.status);
        }
    }
            
    refresh();
    getch();
}

int consultarPedido(FILE *fpPe, FILE *fpC){
    rewind(fpPe);
    
    int codigoCliente, opc, encontro = 0;
    Pedido pedido;
    
    clear();
    printw("\nInforme o código do Cliente:\n");
    fflush(stdout);
    scanw("%d", &codigoCliente);
    
    if(!codigoClienteJaExiste(fpC,codigoCliente)){
        printw("\nErro: Cliente não encontrado!\n");
        getch();
        return 0;
    }
    
    clear();
    printw("\n=====PEDIDOS DO CLIENTE %d=====\n", codigoCliente);

    rewind(fpPe);
    
   while(fscanf(fpPe, "%d,%d,%d,%d,%10[^,],%*c%99[^,],%*c%f,%19[^\n]%*c",
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
        return 0;
    }
    
    printw("\nQual pedido você deseja consultar?:\n");
    fflush(stdout);
    scanw("%d", &opc);
    
    rewind(fpPe);
    
    while(fscanf(fpPe, "%d,%d,%d,%d,%10[^,],%*c%99[^,],%*c%f,%19[^\n]%*c",
                 &pedido.codigoPedido,&pedido.codigoCliente,&pedido.codigoProduto,&pedido.quantidade,
                 pedido.data,pedido.descricao,&pedido.valorTotal,pedido.status) == 8){
    
        if(pedido.codigoPedido == opc && pedido.codigoCliente == codigoCliente && pedido.codigoPedido != 0){
            printw("\n=====DETALHES DO PEDIDO=====\n");
            printw("Cliente: %d\nProduto: %d\nQuantidade: %d\nData: %s\nStatus: %s\nValor: %.2f\nDescrição: %s\n",pedido.codigoCliente,pedido.codigoProduto,pedido.quantidade,
                    pedido.data,pedido.status,pedido.valorTotal,pedido.descricao);
            break;
        }
    }

    getch();
    return 1;
}

int atualizarPedido(FILE *fpPe, FILE *fpC, FILE *fpP){
    rewind(fpPe);
    
    int codigoCliente, opc, mod, encontro = 0;
    Pedido pedido, pedidoModificado;
    
    printw("\nInforme o código do Cliente:\n");
    fflush(stdout);
    scanw("%d",&codigoCliente);
    getch();
    
    if(!codigoClienteJaExiste(fpC,codigoCliente)){
        printw("\nErro: Cliente não encontrado!\n");
        getch();
        return 0;
    }
    
    clear();
    printw("\n=====PEDIDOS DO CLIENTE %d=====\n", codigoCliente);
    
    rewind(fpPe);
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
        return 0;
    }

    printw("\nDigite o código do pedido que deseja modificar:\n");
    fflush(stdout);
    scanw("%d", &opc);
    getch();

    rewind(fpPe);
    int pedidoEncontrado = 0;

    while(fscanf(fpPe, "%d,%d,%d,%d,%10[^,],%99[^,],%f,%19[^\n]%*c",
                 &pedido.codigoPedido,&pedido.codigoCliente,&pedido.codigoProduto,&pedido.quantidade,
                 pedido.data,pedido.descricao,&pedido.valorTotal,pedido.status) == 8){
        
        if (pedido.codigoPedido == opc && pedido.codigoCliente == codigoCliente) {
            pedidoModificado = pedido;
            pedidoEncontrado = 1;
            break;
        }
    }
    
    if(!pedidoEncontrado){
         printw("\nErro: Pedido não encontrado ou não pertence ao cliente!\n");
         getch();
         return 0;
    }

    printw("\n===== MENU DE MODIFICAÇÕES =====\n");
    printw("1 - Código do Produto\n2 - Quantidade\n3 - Descrição\n4 - Status\n");
    printw("Qual campo deseja modificar?:\n");
    fflush(stdout);
    scanw("%d", &mod);
    getch();

    switch(mod){
        case 1:
            printw("Novo código do produto:\n");
            fflush(stdout);
            scanw("%d", &pedidoModificado.codigoProduto);
            break;
        case 2:
            printw("Nova quantidade:\n");
            fflush(stdout);
            scanw("%d", &pedidoModificado.quantidade);
            break;
        case 3:
            printw("Nova descrição:\n");
            fflush(stdout);
            scanw("%s", pedidoModificado.descricao);
            break;
        case 4:
            printw("Novo status:\n");
            fflush(stdout);
            scanw("%s", pedidoModificado.status);
            break;
        default:
            printw("Erro: Opção Inválida!\n");
            getch();
            return 0;
    }

    FILE *fpTemp = fopen("pedidos_temp.csv", "w");
    if (fpTemp == NULL) {
        printw("\nErro: Nao foi possivel criar arquivo temporario para atualizacao.\n");
        getch();
        return 0;
    }
    
    rewind(fpPe); 
    
    while(fscanf(fpPe, "%d,%d,%d,%d,%10[^,],%99[^,],%f,%19[^\n]%*c",
                 &pedido.codigoPedido,&pedido.codigoCliente,&pedido.codigoProduto,&pedido.quantidade,
                 pedido.data,pedido.descricao,&pedido.valorTotal,pedido.status) == 8){
        
        if (pedido.codigoPedido == opc) {
            fprintf(fpTemp, "%d,%d,%d,%d,%s,%s,%.2f,%s\n", 
                    pedidoModificado.codigoPedido, pedidoModificado.codigoCliente, 
                    pedidoModificado.codigoProduto, pedidoModificado.quantidade, 
                    pedidoModificado.data, pedidoModificado.descricao, 
                    pedidoModificado.valorTotal, pedidoModificado.status);
        } else {
             fprintf(fpTemp, "%d,%d,%d,%d,%s,%s,%.2f,%s\n", 
                    pedido.codigoPedido, pedido.codigoCliente, 
                    pedido.codigoProduto, pedido.quantidade, 
                    pedido.data, pedido.descricao, pedido.valorTotal, pedido.status);
        }
    }
    
    fclose(fpPe);
    fclose(fpTemp);

    remove(ARQUIVO_PEDIDOS);
    rename("pedidos_temp.csv",ARQUIVO_PEDIDOS);
    
    printw("\nAlteração feita com sucesso!\n");
    getch();
    return 1;
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
    
    rewind(fpPe);

    while(fscanf(fpPe, "%d,%d,%d,%d,%10[^,],%99[^,],%f,%19[^\n]%*c",
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

    FILE *fpTemp = fopen("pedidos_temp.csv", "w"); 
    if (fpTemp == NULL) {
        printw("\nErro: Nao foi possivel criar arquivo temporario.\n");
        getch();
        return fpPe;
    }
    
    rewind(fpPe);
    int pedidoDeletado = 0;
    
    while(fscanf(fpPe, "%d,%d,%d,%d,%10[^,],%99[^,],%f,%19[^\n]%*c",
                 &pedido.codigoPedido,&pedido.codigoCliente,&pedido.codigoProduto,&pedido.quantidade,
                 pedido.data,pedido.descricao,&pedido.valorTotal,pedido.status) == 8){

        if (pedido.codigoPedido != opc) {
            fprintf(fpTemp, "%d,%d,%d,%d,%s,%s,%.2f,%s\n", 
                    pedido.codigoPedido, pedido.codigoCliente, 
                    pedido.codigoProduto, pedido.quantidade, 
                    pedido.data, pedido.descricao, pedido.valorTotal, pedido.status);
        } else {
            pedidoDeletado = 1; 
        }
    }

    fclose(fpPe);
    fclose(fpTemp);

    remove(ARQUIVO_PEDIDOS);
    rename("pedidos_temp.csv",ARQUIVO_PEDIDOS);

    fpPe = fopen(ARQUIVO_PEDIDOS, "a+");
    if (fpPe == NULL) {
         printw("\nERRO CRÍTICO: Nao foi possivel reabrir o arquivo de pedidos.\n");
         exit(1); 
    }

    if (pedidoDeletado) {
        printw("\nO pedido %d foi desfeito com sucesso!\n", opc);
    } else {
         printw("\nErro: Pedido %d nao encontrado!\n", opc);
    }

    fflush(fpPe);
    getch();
    return fpPe;
}