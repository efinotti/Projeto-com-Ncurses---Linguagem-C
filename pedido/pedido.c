#include "pedido.h"


int codigoPedidoJaExiste(FILE* fp, int code){
    rewind(fp);

    int codigoPedido;
    int codigoCliente;
    int codigoProduto;
    int quantidade;
    char data[maxData];
    char descricao[maxDescricao];
    float valorTotal;
    char status[maxStatus];

    while(fscanf(fp, "%d,%d,%d,%d,%[^, ],%[^, ],%f,%[^\n]\n",
                &codigoPedido, &codigoCliente, &codigoProduto, &quantidade, data,
                descricao, &valorTotal, status) == 8){
    if(codigoPedido == code){
        return 1;
        }
    }
    return 0;
}

void cadastrarPedido(FILE* fp){
    clear();

    Pedido pedido;

    printw("=====CADASTRO DE PEDIDO=====\n\n");

    printw("Digite o código do pedido:\n");
    scanw("%d", &pedido.code);
    getch();

    if(codigoPedidoJaExistente(fp, pedido.code)){
        printw("\nErro: Já existe este código!\n");
        getch();
        return;
    }

    printw("Digite o código do cliente:\n");
    scanw("%d", &pedido.codigoCliente);
    getch();

    printw("Digite o código do Produto:\n");
    scanw("%d", &pedido.codigoProduto);
    getch();

    printw("Digite a quantidade de itens:\n");
    scanw("%d", &pedido.quantidade);
    getch();

    printw("Digite a data:\n"); /*estou pensando em ver se da pra pegar a data automaticamente do PC da pessoa, pq parece paia pedir*/
    scanw("%s", pedido.data);   
    getch();

    printw("Digite o valor total do produto:\n");
    scanw("%f", &pedido.valorTotal);
    getch();

    fprintf(fp, "%d,%d,%d,%d,%[^, ],%.2f\n", pedido.code,pedido.codigoCliente,
            pedido.codigoProduto,pedido.quantidade,pedido.data,pedido.valorTotal);
        
    fflush(fp);

    printw("\nPedido cadastrado com sucesso!\n");
    refresh();
    getch(); 
}