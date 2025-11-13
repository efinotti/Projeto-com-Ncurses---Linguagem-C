# Projeto em C com Ncurses

## 🚀 Sobre o Projeto

Este projeto é um Sistema de Gerenciamento de Vendas (ou um "mini-ERP") desenvolvido inteiramente em Linguagem C. Ele foi criado como uma solução robusta e leve para simular as operações básicas de um pequeno negócio, rodando diretamente no terminal.

O sistema utiliza a biblioteca ncurses para construir uma Interface de Usuário baseada em Texto (TUI), oferecendo menus navegáveis e uma experiência de usuário interativa, superando as limitações de um console C padrão.

A persistência dos dados (salvamento) é gerenciada através de arquivos de texto formatados em CSV (.csv), garantindo que os dados não sejam perdidos ao fechar o programa e tornando o sistema portátil e fácil de inspecionar.

### Principais Funcionalidades

O sistema foi projetado de forma modular, dividido em três pilares principais:

1. Módulo de Clientes (Implementado) O módulo de clientes é a base do sistema e está totalmente funcional. Ele permite:

    - Cadastro Dual: Registro de clientes como Pessoa Física (PF) ou Pessoa Jurídica (PJ).

    - Validação de Documentos: Possui algoritmos integrados para validar matematicamente a autenticidade de números de CPF e CNPJ antes de salvar.

    - CRUD Completo: Funções para Cadastrar, Consultar (por código), Listar (todos) e Remover clientes do arquivo clientes.csv.

2. Módulo de Produtos (Planejado) A estrutura de dados para produtos já está definida (struct Produtos), prevendo o controle de estoque, descrição e preço. O próximo passo é implementar as funções CRUD para produtos.csv.

3. Módulo de Pedidos (Planejado) A modelagem de dados (struct Pedidos e struct ItemPedidos) está pronta, estabelecendo a lógica relacional onde um pedido pode conter múltiplos itens. O objetivo futuro é criar a lógica para vincular clientes a produtos, gerando um novo pedido em pedidos.csv.

## 📋 Pré-requisitos

Para compilar e executar este projeto, você precisará ter:

  - Um compilador C (como o gcc)

  - A biblioteca ncurses instalada (geralmente libncurses-dev ou ncurses-devel dependendo da sua distribuição Linux)

## 🔧 Compilação

Para compilar o código, utilize o seguinte comando no seu terminal:
Bash

```gcc principal.c cliente/cliente.c -o principal -lncurses```

## ▶️ Execução

Após a compilação bem-sucedida, um arquivo executável chamado principal será criado. Para executar o programa, utilize:
Bash

```./principal```

## 👥 Autores

  - Enzo Oliveira Finotti (Sistemas de Informação - Universidade Estadual de Goiás)
  - Daniel Sobrinho Mendes (Sistemas de Informação - Universidade Estadual de Goiás)
  - Gabriel Prado Menezes (Sistemas de Informação - Universidade Estadual de Goiás)
