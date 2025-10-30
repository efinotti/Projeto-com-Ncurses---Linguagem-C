#include "pedido.h"

void obterDataAtual(char *destino, int tamanho){
    time_t agora = time(NULL);
    struct tm *tm_info = localtime(&agora);
    
    snprintf(destino, tamanho, "%02d/%02d/%04d",
            tm_info->tm_mday, tm_info->tm_mon + 1, tm_info->tm_year + 1900);
}

int codigoClienteJaExiste(FILE* fpC, int code){
    rewind(fp);
    
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
    rewind(fp);

    int codigoProduto;
    char nome[maxNome];
    char descricao[maxDescricao];
    float preco;
    int quantidade;

    while(fscanf(fpP,"%d,%[^,],%[^,],%f,%d\n",&codigoProduto, nome, descricao, &preco, &quantidade) == 5){
        if(codigoProduto == code){
            return 1;
        }
    }
    return 0;
}

int codigoPedidoJaExiste(FILE* fpPe, int code){
    rewind(fp);

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

void cadastrarPedido(FILE* fpPe){
    clear();

    Pedido pedido;

    printw("\n\n=====CADASTRO DE PEDIDO=====\n\n");

    printw("Digite o código do pedido:\n");
    scanw("%d", &pedido.codigoPedido);
    getch();

    if(codigoPedidoJaExiste(fpPe, pedido.codigoPedido)){
        printw("\nErro: Já existe este código de pedido!\n");
        getch();
        return;
    }

    printw("Digite o código do cliente:\n");
    scanw("%d", &pedido.codigoCliente);
    getch();
    
    if(!codigoClienteJaExiste(fpC, pedido.codigoCliente)){
        printw("\nErro: Cliente não encontrado!\n");
        getch();
        return;
    }

    printw("Digite o código do Produto:\n");
    scanw("%d", &pedido.codigoProduto);
    getch();
    
    if(!codigoProdutoExiste(fpP, pedido.codigoProduto)){
        printw("\nErro: Produto não encontrado!\n");
        getch();
        return;
    }
    
    printw("Digite a quantidade de itens:\n");
    scanw("%d", &pedido.quantidade);
    getch();

    obterDataAtual(pedido.data, maxData);
    
    printw("Data do pedido gerada automáticamente: %s\n", pedido.data);
    getch();
    
    printw("Faça a descrição do produto:\n");
    scanw("%s", pedido.descricao);
    getch();
    
    printw("Digite o valor total do produto:\n");
    scanw("%f", &pedido.valorTotal);
    getch();
    
   strcpy(pedido.status, "Pendente");
    
    fprintf(fpPe, "%d,%d,%d,%d,%s,%s,%s,%.2f\n", pedido.codigoPedido,pedido.codigoCliente,
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
    
    int codigoCliente, opc;
    Pedido pedido;
    
    printw("Informe o código do Cliente:\n");
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
                
        if(pedido.codigoCliente == codigoCliente){
            printw("\nCódigo do Pedido: %d | Descrição: %s | Quantidade: %d\n",
                    pedido.codigoPedido,pedido.descricao,pedido.quantidade);
        }
    }
    printw("\nQual pedido você deseja consultar?:\n");
    scanw("%d", &opc);
    
    rewind(fpPe);
    
    while(fscanf(fpPe, "%d,%d,%d,%d,%10[^,],%99[^,],%f,%19[^,]",
                &pedido.codigoPedido,&pedido.codigoCliente,&pedido.codigoProduto,&pedido.quantidade,
                pedido.data,pedido.descricao,&pedido.valorTotal,pedido.status) == 8){
    
    if(pedido.codigoCliente == opc && pedido.codigoCliente == codigoCliente){
        printw("\n=====DETALHES DO PEDIDO=====\n");
        printw("Cliente: %d\nProduto: %d\nQuantidade: %d\nData: %s\nStatus: %s\nValor: %.2f\nDescrição: %s\n",&pedido.codigoPedido,&pedido.codigoCliente,&pedido.codigoProduto,&pedido.quantidade,
                pedido.data,pedido.descricao,&pedido.valorTotal,pedido.status);
    }
}

void atualizarPedido(FILE *fpPe){
    
}

FILE* deletarPedido(FILE *fpPe){
    
}