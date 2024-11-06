#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//                                         PARTE DA DANDARA                                     //
typedef struct
{
    int id;
    char nome[50];
    float valor;
    char descricao[100];
    char tipo[20];
    int qtdProduto; // quantidade do produto no estoque
    int qntVendida; // quantidade de produto vendido
} Produto;

void viewprodutos()
{
    FILE *produtos = fopen("produtos.txt", "r");

    if (produtos == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    printf("Esses são todos os produtos já cadastrados:\n\n");
    char linha[200];
    while (fgets(linha, sizeof(linha), produtos) != NULL)
    {
        printf("%s", linha); // le o arquivo linha por linha e exibe cada uma
    }
    printf("\n");

    fclose(produtos);
}

int loadprodutos(Produto produtos[], FILE *arquivo)
{
    int i = 0;
    char linha[200];

    rewind(arquivo); // volta pro inicio do arquivo

    while (fscanf(arquivo, " ID: %d\n", &produtos[i].id) == 1)
    {
        fscanf(arquivo, " Nome: %[^\n]\n", produtos[i].nome);
        fscanf(arquivo, " Valor: %f\n", &produtos[i].valor);
        fscanf(arquivo, " Descrição: %[^\n]\n", produtos[i].descricao);
        fscanf(arquivo, " Tipo: %[^\n]\n", produtos[i].tipo);
        fscanf(arquivo, " Quantidade de produto no estoque: %d\n\n", &produtos[i].qtdProduto); // Lê a quantidade e pula a linha em branco entre registros

        i++; // Incrementa para o próximo produto
    }
    return i; // total de produtos lidos
}

int buscarid(Produto produtos[], int total, int id)
{
    for (int i = 0; i < total; i++)
    {
        if (produtos[i].id == id)
        { // percorre o array ate achar o id
            return i;
        }
    }
    return -1;
}

int editinfo()
{
    viewprodutos();
    Produto produtos[50];
    int id;
    int totalprodutos = 0;

    FILE *arq = fopen("produtos.txt", "r+");
    if (arq == NULL)
    {
        printf("Falha ao abrir o arquivo.\n");
        return -1;
    }

    totalprodutos = loadprodutos(produtos, arq);

    printf("Digite o ID do produto que gostaria de editar:\n");
    scanf("%d", &id);

    int i = buscarid(produtos, totalprodutos, id);
    if (i == -1)
    {
        printf("Produto não encontrado.\n");
        fclose(arq);
        return -1;
    }

    int escolha;
    printf("O que deseja editar?\n1. Nome\n2. Valor\n3. Descrição\n4. Tipo\n5. Quantidade de produto no estoque\n6. Tudo\n7. Excluir produto\n8. Cancelar operação\n");
    scanf("%d", &escolha);

    switch (escolha)
    {
    case 1:
        printf("Novo nome: ");
        getchar();
        fgets(produtos[i].nome, 50, stdin);
        produtos[i].nome[strcspn(produtos[i].nome, "\n")] = '\0';
        break;
    case 2:
        printf("Novo valor: ");
        scanf("%f", &produtos[i].valor);
        break;
    case 3:
        printf("Nova descrição: ");
        getchar();
        fgets(produtos[i].descricao, 100, stdin);
        produtos[i].descricao[strcspn(produtos[i].descricao, "\n")] = '\0';
        break;
    case 4:
        printf("Novo tipo (ex: Bebida, Porção): ");
        getchar();
        fgets(produtos[i].tipo, 20, stdin);
        produtos[i].tipo[strcspn(produtos[i].tipo, "\n")] = '\0';
        break;
    case 5:
        printf("Nova quantidade de produto no estoque: ");
        scanf("%d", &produtos[i].qtdProduto);
        break;
    case 6:
        printf("Novo nome: ");
        getchar();
        fgets(produtos[i].nome, 50, stdin);
        produtos[i].nome[strcspn(produtos[i].nome, "\n")] = '\0';
        printf("Novo valor: ");
        scanf("%f", &produtos[i].valor);
        getchar();
        printf("Nova descrição: ");
        fgets(produtos[i].descricao, 100, stdin);
        produtos[i].descricao[strcspn(produtos[i].descricao, "\n")] = '\0';
        printf("Novo tipo (ex: Bebida, Porção): ");
        fgets(produtos[i].tipo, 20, stdin);
        produtos[i].tipo[strcspn(produtos[i].tipo, "\n")] = '\0';
        break;
    case 7:
        produtos[i].id = -1; // marca como excluído
        break;
    case 8:
        fclose(arq);
        return 0;
    default:
        printf("Opção inválida.\n");
    }

    freopen("produtos.txt", "w", arq); // muda o modo de um arquivo ja aberto
    for (int j = 0; j < totalprodutos; j++)
    {
        if (produtos[j].id != -1)
        {
            fprintf(arq, "ID: %d\nNome: %s\nValor: %.2f\nDescrição: %s\nTipo: %s\nQuantidade de produto no estoque: %d\n\n",
                    produtos[j].id, produtos[j].nome, produtos[j].valor, produtos[j].descricao, produtos[j].tipo, produtos[j].qtdProduto);
        }
    }

    fclose(arq);
    printf("Informações editadas com sucesso.\n");
    return 0;
}

void addproduto()
{
    FILE *arq = fopen("produtos.txt", "a+");
    int newid = 0;
    Produto produtos[50];
    Produto novo;

    if (arq == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    newid = loadprodutos(produtos, arq) + 1; // pega o total de produtos + 1 p ser o id do novo produto (evita id repetido)
    novo.id = newid;

    getchar();
    printf("Qual o nome do produto que gostaria de adicionar?\n");
    fgets(novo.nome, 50, stdin);
    novo.nome[strcspn(novo.nome, "\n")] = '\0'; // tirando o \n

    printf("Qual o valor desse produto?\n");
    scanf("%f", &novo.valor);
    getchar();

    printf("Insira uma breve descrição sobre o produto.\n");
    fgets(novo.descricao, 100, stdin);
    novo.descricao[strcspn(novo.descricao, "\n")] = '\0';

    printf("Qual o tipo do produto (ex: Bebida, Porção)?\n");
    fgets(novo.tipo, 20, stdin);
    novo.tipo[strcspn(novo.tipo, "\n")] = '\0';

    printf("Qual a quantidade desse produto no estoque?\n");
    scanf("%d", &novo.qtdProduto);
    getchar();

    fprintf(arq, "ID: %d\nNome: %s\nValor: %.2f\nDescrição: %s\nTipo: %s\nQuantidade de produto no estoque: %d\n\n", novo.id, novo.nome, novo.valor, novo.descricao, novo.tipo, novo.qtdProduto);

    fclose(arq);
    printf("Produto cadastrado com sucesso!\n");
}

//                                         PARTE DA BIA                                     //
void listarProdutos()
{
    FILE *produtos = fopen("produtos.txt", "r");

    if (produtos == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    printf("Esses são todos os produtos do estoque:\n\n");
    char linha[200];
    while (fgets(linha, sizeof(linha), produtos) != NULL)
    {
        printf("%s", linha); // le o arquivo linha por linha e exibe cada uma
    }
    printf("\n");

    fclose(produtos);
}

void consultarProdutos()
{
    FILE *arqProdutos = fopen("produtos.txt", "r");
    if (arqProdutos == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    Produto produtos[50];
    int totalProdutos = loadprodutos(produtos, arqProdutos);
    fclose(arqProdutos);

    int escolha = 0;

    while (escolha != 5)
    {
        printf("\nComo deseja consultar o produto:\n");
        printf("1. ID\n");
        printf("2. Sair;\n");
        scanf("%d", &escolha);

        switch (escolha)
        {
        case 1:
        {
            int idBusca, encontrado = 0; // Coloquei as declarações dentro de um bloco
            printf("Digite o ID do produto: ");
            scanf("%d", &idBusca);
            for (int i = 0; i < totalProdutos; i++)
            {
                if (produtos[i].id == idBusca)
                {
                    printf("\nProduto encontrado com o ID: %d:\nNome: %s - Valor: %.2f - Descrição: %s - Tipo: %s - Quantidade no estoque: %d\n",
                           produtos[i].id, produtos[i].nome, produtos[i].valor, produtos[i].descricao, produtos[i].tipo, produtos[i].qtdProduto);
                    encontrado = 1;
                    break;
                }
            }
            if (!encontrado)
            {
                printf("Produto não encontrado.\n");
            }
            break;
        }
        case 2:
            return;
        default:
            printf("Escolha inválida.\n");
        }
    }
}

void estoque()
{
    int escolha = 0;

    while (escolha != 5)
    {
        printf("\nMenu de Gerenciamento de Estoque:\n");
        printf("1. Listar os produtos do estoque\n");
        printf("2. Consultar produtos do estoque\n");
        printf("3. Sair;\n");
        scanf("%d", &escolha);

        switch (escolha)
        {
        case 1:
            listarProdutos();
            break;
        case 2:
            consultarProdutos();
            break;
        case 3:
            return;
        default:
            printf("Escolha inválida.\n");
        }
    }
}

//                                         PARTE DA DANDARA                                     //
void cadastro()
{
    int escolha = 0;

    while (escolha != 4)
    {
        printf("O que você deseja fazer?\n1. Adicionar novo produto;\n2. Editar informações já existentes;\n3. Visualizar produtos cadastrados;\n4. Gerenciar estoque;\n5. Continuar.\n");
        scanf("%d", &escolha);

        switch (escolha)
        {
        case 1:
            addproduto();
            break;
        case 2:
            editinfo();
            break;
        case 3:
            viewprodutos();
            break;
        case 4:
            estoque();
            break;
        case 5:
            return;
        default:
            printf("Escolha inválida.\n");
        }
    }
}

//                                         PARTE DA MIRELLA                                     //

typedef struct
{
    int codigo;
    int quantidade;
    char produto[50];
    float valor;
    int id;
    
} Pedido;

void consultarPedidos()
{
    FILE *arquivo = fopen("pedidos.txt", "r");

    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    printf("Aqui está a lista de pedidos feitos:\n\n");
    char linha[200];
    while (fgets(linha, sizeof(linha), arquivo) != NULL)
    {
        printf("%s", linha);
    }
    printf("\n");

    fclose(arquivo);
}

int contarPedidos(Pedido pedidos[], FILE *arquivo)
{
    int i = 0;
    char linha[100];

    rewind(arquivo);

    while (fgets(linha, sizeof(linha), arquivo) != NULL)
    {

        if (strncmp(linha, "Código Comanda:", 15) == 0)
        { // verifica se os 15 primeiros caracteres da linha são "Código Comanda:".
            i++;
        }
    }
    return i;
}

float buscarPreco(char *nomeProduto)
{
    FILE *arqProdutos = fopen("produtos.txt", "r");
    if (arqProdutos == NULL)
    {
        printf("Erro ao abrir o arquivo de produtos.\n");
        return -1;
    }

    Produto produtos[50];
    int totalProdutos = loadprodutos(produtos, arqProdutos);
    fclose(arqProdutos);

    for (int i = 0; i < totalProdutos; i++)
    {
        if (strcmp(produtos[i].nome, nomeProduto) == 0)
        {
            return produtos[i].valor;
        }
    }
    printf("Produto não encontrado.\n");
    return -1;
}

int buscarId(char *nomeProduto)
{
    FILE *arqProdutos = fopen("produtos.txt", "r");
    if (arqProdutos == NULL)
    {
        printf("Erro ao abrir o arquivo de produtos.\n");
        return -1;
    }

    Produto produtos[50];
    int totalProdutos = loadprodutos(produtos, arqProdutos);
    fclose(arqProdutos);

    for (int i = 0; i < totalProdutos; i++)
    {
        if (strcmp(produtos[i].nome, nomeProduto) == 0)
        {
            return produtos[i].id;
        }
    }
    printf("Produto não encontrado.\n");
    return -1;
}

void deletarTudo()
{
    FILE *arquivo = fopen("pedidos.txt", "w"); // Abre com o w pra resetar o arquivo
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
    fclose(arquivo);
    printf("Todos os pedidos foram deletados.\n");
}

void deletarComanda()
{

    int codigoComanda;
    printf("Digite o código da comanda que deseja excluir: ");
    scanf("%d", &codigoComanda);

    FILE *arquivo = fopen("pedidos.txt", "r");
    FILE *transicao = fopen("transicao.txt", "w");

    if (arquivo == NULL || transicao == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        if (arquivo)
            fclose(arquivo);
        if (transicao)
            fclose(transicao);
        return;
    }

    char linha[200];
    int copiar = 1;

    while (fgets(linha, sizeof(linha), arquivo) != NULL)
    {
        if (strncmp(linha, "Código Comanda:", 15) == 0)
        {
            int codigoAtual;
            sscanf(linha, "Código Comanda: %d", &codigoAtual);
            copiar = (codigoAtual != codigoComanda); // ignorar a comanda se for o código para excluir
        }

        if (copiar)
        { // copia as comandas no arquivo temporário
            fputs(linha, transicao);
        }
    }
    fclose(arquivo);
    fclose(transicao);

    // Substitui o arquivo original pelo temporário
    remove("pedidos.txt");
    rename("transicao.txt", "pedidos.txt");

    printf("Comanda %d excluída com sucesso.\n", codigoComanda);
}

void cadastrarPedido()
{

    FILE *arqProdutos = fopen("produtos.txt", "r");
    if (arqProdutos == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    Produto produtos[50];
    int totalProdutos = loadprodutos(produtos, arqProdutos);
    fclose(arqProdutos);

    printf("\n");
    printf("Os produtos disponiveis são:\n");
    for (int i = 0; i < totalProdutos; i++)
    {
        printf("%d - %s - %.2f\n", i, produtos[i].nome, produtos[i].valor);
    }

    FILE *arquivo = fopen("pedidos.txt", "a+");
    Pedido novoPedido;
    int quantItems, totalPedidos = 0;
    Pedido pedidos[100];
    float totalComanda = 0.0;

    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    totalPedidos = contarPedidos(pedidos, arquivo);
    int codigoComanda = totalPedidos + 1;

    printf("\n");
    printf("Quantos produtos diferentes deseja adicionar à comanda?\n");
    scanf("%d", &quantItems);

    fprintf(arquivo, "Código Comanda: %d\n", codigoComanda);

    for (int i = 0; i < quantItems; i++)
    {
        Pedido novoPedido;
        int escolhaProduto;

        printf("Produto %d:\n", i + 1);
        printf("Indice do produto: ");
        scanf("%d", &escolhaProduto);

        strcpy(novoPedido.produto, produtos[escolhaProduto].nome);

        novoPedido.valor = buscarPreco(novoPedido.produto);
        if (novoPedido.valor == -1)
        {
            printf("Erro. Produto não encontrado.\n");
            continue;
        }

        novoPedido.id = buscarId(novoPedido.produto);
        if (novoPedido.id == -1)
        {
            printf("Erro. Produto não encontrado.\n");
            continue;
        }

        printf("Quantidade: ");
        scanf("%d", &novoPedido.quantidade);

        if (novoPedido.quantidade > produtos[escolhaProduto].qtdProduto)
        {
            printf("Quantidade solicitada não disponível. Apenas %d unidades restantes.\n", produtos[escolhaProduto].qtdProduto);
            continue;
        }

        float totalProduto = novoPedido.valor * novoPedido.quantidade;
        totalComanda += totalProduto;

        produtos[escolhaProduto].qntVendida += novoPedido.quantidade; // adicionei pra fazer a quantidade vendida

        fprintf(arquivo, "Produto: %s\nID: %d\nQuantidade: %d\nValor Unitário: %.2f\nTotal: %.2f\n",
                novoPedido.produto, novoPedido.id, novoPedido.quantidade, novoPedido.valor, totalProduto);
        printf("Produto %d cadastrado na comanda com sucesso!\n", i + 1);

        produtos[escolhaProduto].qtdProduto -= novoPedido.quantidade;
    }

    fprintf(arquivo, "\nValor Total da Comanda: %.2f\n\n", totalComanda);
    fclose(arquivo);

    // Atualiza o arquivo de produtos
    arqProdutos = fopen("produtos.txt", "w");
    if (arqProdutos == NULL)
    {
        printf("Erro ao abrir o arquivo para atualização.\n");
        return;
    }

    for (int i = 0; i < totalProdutos; i++)
    {
        fprintf(arqProdutos, "ID: %d\nNome: %s\nValor: %.2f\nDescrição: %s\nTipo: %s\nQuantidade de produto no estoque: %d\n\n", produtos[i].id, produtos[i].nome, produtos[i].valor, produtos[i].descricao, produtos[i].tipo, produtos[i].qtdProduto);
    }

    fclose(arqProdutos);

    printf("Todos os produtos foram cadastrados na comanda com sucesso!\n");
}

void pedido()
{
    int escolha = 0;

    while (escolha != 5)
    {
        printf("\n");
        printf("\nO que você deseja fazer?\n");
        printf("1. Adicionar novo pedido;\n");
        printf("2. Visualizar pedidos cadastrados;\n");
        printf("3. Excluir um pedido específico;\n");
        printf("4. Excluir todos os pedidos;\n");
        printf("5. Sair.\n");
        scanf("%d", &escolha);

        switch (escolha)
        {
        case 1:
            cadastrarPedido();
            break;
        case 2:
            consultarPedidos();
            break;
        case 3:
            deletarComanda();
            break;
        case 4:
            deletarTudo();
        case 5:
            return;
        default:
            printf("Escolha inválida.\n");
        }
    }
}

//                   Parte da Andrezza                         //

void GerarRelatorioEstoque(Produto produtos[], int totalProdutos) {
    FILE *arquivo = fopen("relatorio_estoque.txt", "w");

    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo de relatório de estoque.\n");
        return;
    }

    fprintf(arquivo, "Relatorio de Estoque:\n\n");
    fprintf(arquivo, "ID\tTipo\t\tQuantidade em Estoque\t\tValor Unit.\t\tValor em Estoque\tNome\n");
    fprintf(arquivo, "----------------------------------------------------------------------------------------\n");

    float valorTotalEstoque = 0.0;
    int totalItensEstoque = 0;

    for (int i = 0; i < totalProdutos; i++) {
        float valorEstoqueProduto = produtos[i].qtdProduto * produtos[i].valor;
        fprintf(arquivo, "%d\t%-10s\t\t\t%d\t\t\t\t\tR$%.2f\t\t\tR$%.3f\t\t\t%-15s\n", produtos[i].id, produtos[i].tipo, produtos[i].qtdProduto, produtos[i].valor, valorEstoqueProduto, produtos[i].nome);

        valorTotalEstoque += valorEstoqueProduto;
        totalItensEstoque += produtos[i].qtdProduto;
    }

    fprintf(arquivo, "----------------------------------------------------------------------------------------\n");
    fprintf(arquivo, "Total de itens em estoque: %d\n", totalItensEstoque);
    fprintf(arquivo, "Valor total em estoque: R$%.2f\n", valorTotalEstoque);

    fclose(arquivo);
    printf("Relatorio de estoque gerado com sucesso.\n");
}

void GerarRelatorioVendas(Produto produtos[], int totalProdutos) {
    FILE *arquivo = fopen("relatorio_vendas.txt", "w");
    FILE *pedidos = fopen("pedidos.txt", "r");

    if (arquivo == NULL || pedidos == NULL) {
        printf("Erro ao criar o arquivo de relatorio de vendas ou ler pedidos.\n");
        return;
    }

    // Array para armazenar quantidade vendida de cada produto
    int vendas[50] = {0}; // Inicializa todas as posições com 0
    
    // Ler o arquivo de pedidos e contar vendas
    char linha[200];
    int id_atual = -1;
    int quantidade_atual = 0;
    
    while (fgets(linha, sizeof(linha), pedidos) != NULL) {
        if (strncmp(linha, "ID: ", 4) == 0) {
            sscanf(linha, "ID: %d", &id_atual);
        }
        else if (strncmp(linha, "Quantidade: ", 11) == 0) {
            sscanf(linha, "Quantidade: %d", &quantidade_atual);
            // Procura o índice do produto com esse ID
            for (int i = 0; i < totalProdutos; i++) {
                if (produtos[i].id == id_atual) {
                    vendas[i] += quantidade_atual;
                    break;
                }
            }
        }
    }

    fprintf(arquivo, "Relatório de Vendas:\n\n");
    fprintf(arquivo, "ID\tTipo\t\tQuantidade Vendida\tValor Unit.\t\tValor Total\t\tNome\n");
    fprintf(arquivo, "----------------------------------------------------------------------------------------\n");

    float valorTotalVendas = 0.0;
    int totalItensVendidos = 0;

    for (int i = 0; i < totalProdutos; i++) {
        if (vendas[i] > 0) {
            float valorTotal = vendas[i] * produtos[i].valor;
            fprintf(arquivo, "%d\t%-10s\t%d\t\t\t\t\tR$%.2f\t\t\tR$%.3f\t\t\t%-20s\n", produtos[i].id, produtos[i].tipo, vendas[i], produtos[i].valor, valorTotal, produtos[i].nome);
            
            valorTotalVendas += valorTotal;
            totalItensVendidos += vendas[i];
        }
    }

    fprintf(arquivo, "----------------------------------------------------------------------------------------\n");
    fprintf(arquivo, "Total de itens vendidos: %d\n", totalItensVendidos);
    fprintf(arquivo, "Valor total das vendas: R$%.2f\n", valorTotalVendas);

    fclose(arquivo);
    fclose(pedidos);
    printf("Relatorio de vendas gerado com sucesso.\n");
}

void relatorio()
{
    int escolha = 0;

    while (escolha != 3)
    {
        printf("\n");
        printf("\nO que voce deseja fazer?\n");
        printf("1. Gerar relatorio de estoque.\n");
        printf("2. Gerar relatorio de vendas.\n");
        printf("3. Sair.\n");
        scanf("%d", &escolha);

        Produto produtos[50];
        int totalProdutos = loadprodutos(produtos, fopen("produtos.txt", "r"));

        switch (escolha)
        {
        case 1:
            GerarRelatorioEstoque(produtos, totalProdutos);
            break;
        case 2:
            GerarRelatorioVendas(produtos, totalProdutos);
            break;
        case 3:
            return;
        default:
            printf("Escolha invalida.\n");
        }
    }
}

int main()
{
    int escolha = 0;

    while (escolha != 4)
    {
        printf("\n");
        printf("\nO que voce deseja fazer?\n");
        printf("1. Menu de cadastro e estoque.\n");
        printf("2. Menu de pedidos.\n");
        printf("3. Menu dos relatorios.\n");
        printf("4. Sair.\n");
        scanf("%d", &escolha);

        switch (escolha)
        {
        case 1:
            cadastro();
            break;
        case 2:
            pedido();
            break;
        case 3:
            relatorio();
            break;
        case 4:
            return 0;
        default:
            printf("Escolha invalida.\n");
        }
    }

    return 0;
}