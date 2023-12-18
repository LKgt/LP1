#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cctype>
#include <algorithm>
#include <random>

// Struct para armazenar as informações de data
struct Data {
    int dia;
    int mes;
    int ano;
};

// Struct para representar o status do pedido
struct Status {
    int codigo;  // 0 para 'EmAndamento', 1 para 'Concluido', 2 para 'Cancelado'
    std::string descricao;
};

// Classe para representar um Produto
class Produto {
public:
    long long idProduto;
    std::string nomeProduto;
    float precoProduto;
    int estoqueProduto;

    // Construtor
    Produto(long long id, const std::string& nome, float preco, int estoque)
        : idProduto(id), nomeProduto(nome), precoProduto(preco), estoqueProduto(estoque) {}

    //Para imprimir detalhes do produto
    void ImprimirDetalhes() const {
        std::cout << "ID do Produto: " << idProduto
                  << ", Nome: " << nomeProduto
                  << ", Preço: " << precoProduto
                  << ", Estoque: " << estoqueProduto << std::endl;
    }
};

// Classe para representar um Pedido
class Pedido {
public:
    long long idPedido;
    Data dataPedido;
    long long idProduto;
    int quantidade;
    std::string cliente;
    Status status;

    // Construtor
    Pedido(long long id, const Data& data, long long idProd, int qtd, const std::string& cli, const Status& sts)
        : idPedido(id), dataPedido(data), idProduto(idProd), quantidade(qtd), cliente(cli), status(sts) {}

    // Para imprimir os detalhes do pedido
    void ImprimirDetalhes() const {
        std::cout << "ID do Pedido: " << idPedido << ", Data: " << dataPedido.dia << "/" << dataPedido.mes << "/" << dataPedido.ano << ", ID do Produto: " << idProduto << ", Quantidade: " << quantidade << ", Cliente: " << cliente << ", Status: " << status.descricao << std::endl;
    }
};


// Função para obter a data
Data obterData() {
    Data data;
    std::cout << "Digite a data do pedido (dia mes ano separados por espaços): ";
    std::cin >> data.dia >> data.mes >> data.ano;
    return data;
}

// Função para verificar se um ID de produto existe na lista de produtos
bool idProdutoExiste(const std::vector<Produto>& listaProdutos, long long idProduto) {
    for (const auto& produto : listaProdutos) {
        if (produto.idProduto == idProduto) {
            return true;  // ID do produto existe
        }
    }
    return false;  // ID do produto não existe
}

// Função para verificar se um ID de pedido existe na lista de pedidos
bool idPedidoExiste(const std::vector<Pedido>& listaPedidos, long long idPedido) {
    for (const auto& pedido : listaPedidos) {
        if (pedido.idPedido == idPedido) {
            return true;
        }
    }
    return false;
}

// Função para gerar um ID de produto automaticamente e chama uma função para verificar se o ID já existe
long long gerarIdProduto(const std::vector<Produto>& listaProdutos) {
    long long novoIdProduto;
    // Utilizando rand() para gerar um número aleatório
    do {
        novoIdProduto = rand() % 90000 + 10000;
    } while (idProdutoExiste(listaProdutos, novoIdProduto));

    return novoIdProduto;
}

// Função para gerar um ID de pedido aleatório
long long gerarIdPedido(const std::vector<Pedido>& listaPedidos) {
    long long novoIdPedido;
    // Utilizando rand() para gerar um número aleatório
    do {
        novoIdPedido = rand() % 90000 + 10000; // Gera um ID entre 10000 e 99999
    } while (idPedidoExiste(listaPedidos, novoIdPedido));

    return novoIdPedido;
}

// Função para obter a descrição do status do pedido
std::string ObterStatusPedido(int codigo) {
    switch (codigo) {
        case 0:
            return "EmAndamento";
        case 1:
            return "Concluido";
        case 2:
            return "Cancelado";
        default:
            return "Desconhecido";
    }
}

// Função para ler os dados dos produtos do arquivo
void LerProdutosArquivo(std::vector<Produto>& listaProdutos) {
    std::ifstream arquivo("Produtos.txt");

    if (arquivo.is_open()) {
        while (!arquivo.eof()) {
            long long idProduto;
            std::string nomeProduto;
            float precoProduto;
            int estoqueProduto;

            arquivo >> idProduto >> nomeProduto >> precoProduto >> estoqueProduto;

            if (!arquivo.fail()) {
                Produto produto(idProduto, nomeProduto, precoProduto, estoqueProduto);
                listaProdutos.push_back(produto);
            }
        }
        std::cout << "Dados dos produtos lidos com sucesso do arquivo: Produtos.txt" << std::endl;
    } else {
        std::cerr << "Erro ao abrir o arquivo de produtos para leitura." << std::endl;
    }
}

// Função para ler os dados de pedidos do arquivo
void LerPedidosArquivo(std::vector<Pedido>& listaPedidos) {
    std::ifstream arquivo("Pedidos.txt");

    if (arquivo.is_open()) {
        while (!arquivo.eof()) {
            long long idPedido;
            Data dataPedido;
            long long idProduto;
            int quantidade;
            std::string cliente;
            Status status;


            arquivo >> idPedido >> dataPedido.dia >> dataPedido.mes >> dataPedido.ano
                    >> idProduto >> quantidade >> cliente >> status.codigo;

            if (!arquivo.fail()) {
              
              int codigoStatus;
              arquivo >> codigoStatus;

              Status status;
              status.codigo = codigoStatus;
              status.descricao = ObterStatusPedido(codigoStatus);

              Pedido pedido(idPedido, dataPedido, idProduto, quantidade, cliente, status);
              listaPedidos.push_back(pedido);
            }
        }
        std::cout << "Dados dos pedidos lidos com sucesso do arquivo: Pedidos.txt" << std::endl;
    } else {
        std::cerr << "Erro ao abrir o arquivo de pedidos para leitura." << std::endl;
    }
}

// Função para adicionar um novo produto à lista
void AdicionarProduto(std::vector<Produto>& listaProdutos) {
    
    std::string nome;
    float preco;
    int estoque;
    long long id = gerarIdProduto(listaProdutos);

    std::cout << "Digite o Nome do Produto: ";
    std::cin.ignore();
    std::getline(std::cin, nome);

    std::cout << "Digite o Preço do Produto: ";
    std::cin >> preco;

    std::cout << "Digite a quantidade do Produto: ";
    std::cin >> estoque;

    Produto novoProduto(id, nome, preco, estoque);
    listaProdutos.push_back(novoProduto);

    std::cout << "Produto adicionado com sucesso!" << std::endl;
}

// Função para adicionar ao estoque existente na lista
void AdicionarEstoque(std::vector<Produto>& listaProdutos) {
    long long idProduto;
    int quantidade;

    std::cout << "Digite o ID do Produto para adicionar noi estoque: ";
    std::cin >> idProduto;

    if (!idProdutoExiste(listaProdutos, idProduto)) {
        std::cout << "ID do Produto não encontrado. Operação cancelada." << std::endl;
        return;
    }

    std::cout << "Digite a quantidade a ser adicionada no estoque: ";
    std::cin >> quantidade;

    // Adiciona ao estoque existente do produto
    for (auto& produto : listaProdutos) {
        if (produto.idProduto == idProduto) {
            produto.estoqueProduto += quantidade;
            std::cout << "Estoque do Produto atualizado com sucesso!" << std::endl;
            return;
        }
    }
}

// Função para salvar os dados dos produtos em um arquivo de texto
void SalvarProdutosEmArquivo(const std::vector<Produto>& listaProdutos) {
    std::ofstream arquivo("Produtos.txt", std::ios::app);

    if (arquivo.is_open()) {
        for (const auto& produto : listaProdutos) {
            arquivo << produto.idProduto << ' ' << produto.nomeProduto << ' ' << produto.precoProduto << ' ' << produto.estoqueProduto << '\n';
        }
        std::cout << "Dados dos produtos salvos com sucesso no arquivo: Produtos.txt" << std::endl;
    } else {
        std::cerr << "Erro ao abrir o arquivo para escrita." << std::endl;
    }
}



// Função para adicionar um novo pedido a lista
void AdicionarPedido(std::vector<Pedido>& listaPedidos, const std::vector<Produto>& listaProdutos) {
    
    Data data;
    long long idProduto;
    int quantidade;
    std::string cliente;
    Status status;
    std::string telefone;

    long long id = gerarIdPedido(listaPedidos);

    std::cout << "Digite a Data do Pedido (dia mes ano): ";
    std::cin >> data.dia >> data.mes >> data.ano;

    std::cout << "Digite o ID do Produto: ";
    std::cin >> idProduto;

    std::cout << "Digite a Quantidade do Pedido: ";
    std::cin >> quantidade;

    std::cout << "Digite o Cliente: ";
    std::cin.ignore();
    std::getline(std::cin, cliente);

    status.codigo = 0;
    status.descricao = "EmAndamento";

    Pedido novoPedido(id, data, idProduto, quantidade, cliente, status);
    listaPedidos.push_back(novoPedido);

    std::cout << "Pedido adicionado com sucesso!" << std::endl;
}


// Função para salvar os dados dos pedidos em um arquivo de texto
void SalvarPedidosEmArquivo(const std::vector<Pedido>& listaPedidos) {
    std::ofstream arquivo("Pedidos.txt", std::ios::app);

    if (arquivo.is_open()) {
        for (const auto& pedido : listaPedidos) {
            arquivo << pedido.idPedido << ' ' << pedido.dataPedido.dia << ' ' << pedido.dataPedido.mes << ' ' << pedido.dataPedido.ano
                    << ' ' << pedido.idProduto << ' ' << pedido.quantidade << ' ' << pedido.cliente << ' ' << pedido.status.codigo << '\n';
        }
        std::cout << "Dados dos pedidos adicionados com sucesso no arquivo: Pedidos.txt" << std::endl;
    } else {
        std::cerr << "Erro ao abrir o arquivo para escrita." << std::endl;
    }
}

// Função para remover um produto
void RemoverProduto(std::vector<Produto>& listaProdutos) {
    long long idProduto;

    std::cout << "Digite o ID do Produto a ser removido: ";
    std::cin >> idProduto;

    auto it = std::remove_if(listaProdutos.begin(), listaProdutos.end(), [idProduto](const Produto& produto) {
        return produto.idProduto == idProduto;
    });

    if (it != listaProdutos.end()) {
        listaProdutos.erase(it, listaProdutos.end());
        std::cout << "Produto removido com sucesso!" << std::endl;
    } else {
        std::cout << "ID do Produto não encontrado. Nenhum produto removido." << std::endl;
    }
}

// Função para imprimir os pedidos na lista
void ImprimirPedidos(const std::vector<Pedido>& listaPedidos) {
    std::cout << "Pedidos:" << std::endl;
    for (const auto& pedido : listaPedidos) {
        std::cout << "ID do Pedido: " << pedido.idPedido << ", Data: " << pedido.dataPedido.dia << "/" << pedido.dataPedido.mes << "/" << pedido.dataPedido.ano
                  << ", ID do Produto: " << pedido.idProduto << ", Quantidade: " << pedido.quantidade << ", Cliente: " << pedido.cliente << ", Status: " << pedido.status.descricao << std::endl;
    }
}

// Função para imprimir os produtos na lista
void ImprimirProdutos(const std::vector<Produto>& listaProdutos) {
    std::cout << "Produtos:" << std::endl;
    for (const auto& produto : listaProdutos) {
        std::cout << "ID do Produto: " << produto.idProduto << ", Nome: " << produto.nomeProduto
                  << ", Preço: " << produto.precoProduto << ", Estoque: " << produto.estoqueProduto << std::endl;
    }
}

// Função para cancelar um pedido
void CancelarPedido(std::vector<Pedido>& listaPedidos) {
    long long idPedido;

    std::cout << "Digite o ID do Pedido a ser cancelado: ";
    std::cin >> idPedido;

    auto it = std::find_if(listaPedidos.begin(), listaPedidos.end(), [idPedido](const Pedido& pedido) {
        return pedido.idPedido == idPedido;
    });

    if (it != listaPedidos.end()) {
        // Pedido encontrado
        if (it->status.codigo == 0) { // Verifica se o pedido está 'em andamento'
            it->status.codigo = 2;     // Altera o status para 'Cancelado'
            it->status.descricao = "Cancelado";
            std::cout << "Pedido cancelado com sucesso!" << std::endl;
        } else {
            std::cout << "O pedido já está concluído ou cancelado. Operação cancelada." << std::endl;
        }
    } else {
        std::cout << "ID do Pedido não encontrado. Operação cancelada." << std::endl;
    }
}

// Função para concluir um pedido
void ConcluirPedido(std::vector<Pedido>& listaPedidos, std::vector<Produto>& listaProdutos) {
    long long idPedido;

    std::cout << "Digite o ID do Pedido a ser concluído: ";
    std::cin >> idPedido;

    auto i = std::find_if(listaPedidos.begin(), listaPedidos.end(), [idPedido](const Pedido& pedido) {
        return pedido.idPedido == idPedido;
    });

    if (i != listaPedidos.end()) {
        // Pedido encontrado
        if (i->status.codigo == 0) { // Verificar se o pedido está 'em andamento'
            long long idProduto = i->idProduto;
            int quantidade = i->quantidade;

            // Encontrar o produto na lista de produtos
            auto produtoI = std::find_if(listaProdutos.begin(), listaProdutos.end(), [idProduto](const Produto& produto) {
                return produto.idProduto == idProduto;
            });

            if (produtoI != listaProdutos.end()) {
                // Produto encontrado, verifica se há estoque suficiente
                if (quantidade <= produtoI->estoqueProduto) {
                    // Subtrai da quantidade do produto no estoque
                    produtoI->estoqueProduto -= quantidade;

                    // Alterar o status do pedido para 'Concluido'
                    i->status.codigo = 1;
                    i->status.descricao = "Concluido";

                    std::cout << "Pedido concluído com sucesso! Estoque do Produto atualizado." << std::endl;
                } else {
                    std::cout << "Estoque insuficiente para concluir o pedido. Operação cancelada." << std::endl;
                }
            } else {
                std::cout << "Produto associado ao pedido não encontrado. Operação cancelada." << std::endl;
            }
        } else {
            std::cout << "O pedido já está concluído ou cancelado. Operação cancelada." << std::endl;
        }
    } else {
        std::cout << "ID do Pedido não encontrado. Operação cancelada." << std::endl;
    }
}



int main() {
    std::vector<Produto> listaProdutos;
    std::vector<Pedido> listaPedidos;
    char opcao;

    do {
        // Exibindo o menu
        std::cout << "\nMenu:\n";
        std::cout << "1. Adicionar novo produto\n";
        std::cout << "2. Adicionar estoque de um produto existente\n";
        std::cout << "3. Adicionar novo pedido\n";
        std::cout << "4. Imprimir produtos\n";
        std::cout << "5. Imprimir pedidos\n";
        std::cout << "6. Remover produto\n";
        std::cout << "7. Cancelar pedido\n";
        std::cout << "8. Concluir pedido\n";
        std::cout << "0. Sair\n";
        std::cout << "Escolha uma opção: ";
        std::cin >> opcao;

        // Executando a opção escolhida
        switch (opcao) {
            case '1':
                AdicionarProduto(listaProdutos);
                break;
            case '2':
                AdicionarEstoque(listaProdutos);
                break;
            case '3':
                AdicionarPedido(listaPedidos, listaProdutos);
                break;
            case '4':
                ImprimirProdutos(listaProdutos);
                break;
            case '5':
                ImprimirPedidos(listaPedidos);
                break;
            case '6':
                RemoverProduto(listaProdutos);
                break;
            case '7':
            CancelarPedido(listaPedidos);
            break;
            case '8':
                ConcluirPedido(listaPedidos, listaProdutos); 
              // Chamada da função para concluir pedido
                break;

            case '0':
                std::cout << "Encerrando o programa. Até mais!\n";
                break;
            default:
                std::cout << "Opção inválida. Tente novamente.\n";
        }
    } while (opcao != '0');

    // Salvando os dados dos produtos e pedidos em arquivos de texto
    SalvarProdutosEmArquivo(listaProdutos);
    SalvarPedidosEmArquivo(listaPedidos);

    return 0;
}