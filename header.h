/*
maio de 2020
Quinto semestre de Engenharia de Computação
Estruturas de Dados II
Trabalho de Tabelas Hash
*/

// elemento de lista encadeada, armazena uma chave
struct item {
	int valor;
	struct item *prox;
};

// tabela hash, armazena um array de lista de chaves
struct hash {
	char nome[256];
	int qtd, tam;
	struct item **chave;
};

// lista de tabelas hash, armazena um array de tabelas hash
struct lt {
	int qtd, tam;
	struct hash **tabela;
};

// funcoes principais
struct hash* criar(int quantidade, int tamanho); // cria uma tabela hash com itens aleatorios
void mostrar(struct lt *lista); // mostra as tabelas carregadas
struct hash* unir(struct lt *ltab, int a, int b); // une duas tabelas hash
int checar(struct lt *lista, int id); // checa se uma tabela existe
void adicionar(struct hash *tabela, int valor); // adiciona em elemento à tabela
void excluir(struct hash *tabela, int valor, int pos); // exclue um elemento da tabela

// funcoes auxiliares
struct hash* inictabela(int tamanho); // inicializa uma tabela hash
struct lt* iniclt(int tamanho); // inicializa a lista de tabelas

int hashear(int valor, int tamanho); // funcao de hashing
int aleatorio(int menor, int maior); // retorna um valor aleatorio entre menor e maior
int buscar(struct hash* tabela, int valor); // verifica se um item existe em tabela
void deletartab(struct lt *ltab, int id); // deleta uma tabela
void liberar(struct hash *tabela); // libera uma tabela completamente
int conseguirlugar(struct lt *ltab); // consegue lugat em ltab para uma nova tabela

// funcoes de impressao
void imprimirtabela(struct hash *tabela); // imprime tabela
void imprimirlista(struct item *itemlista); // imprime a lista encadeada itemlista

// funcoes de arquivo
int salvarlt(struct lt *lista); // salva a lista de tabelas
void salvartabela(struct hash *tabela, int id); // salva tabela 

struct lt* lerlista(); // lê uma lista de tabelas
struct hash* lertabela(int id); // lê uma tabela

struct item* colocar(int valor); // coloca valor em um item de lista, retornando-o
void abrirexterno(int id); // abre a tabela id com um programa externo como o excel

// menus
void menuabrir(struct lt *ltab, int id); // menu de detalhamento de uma tabela id
void menucriar(struct lt *ltab); // menu para criacao de novas tabelas

// outros
void limpar(); // limpa a tela
char lerchar(); // recebe um caractere e o retorna
void aguardar(); // substitui system('pause')
