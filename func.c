/*
maio de 2020
Quinto semestre de Engenharia de Computação
Estruturas de Dados II
Trabalho de Tabelas Hash
*/

#include <stdlib.h>
#include <time.h>

#include "header.h" 

#ifdef __unix__ // sistema é linux
	void limpar() {
		system("clear");
	}
	
	void abrirexterno(int id) {
		char nomearq[25];
		snprintf(nomearq, 25, "libreoffice --view %i.csv", id);
		system(nomearq);
	}
#endif

#ifdef _WIN64 // sistema é windows
	void limpar() {
		system("cls");
	}
	
	void abrirexterno(int id) {
		char nomearq[6];
		snprintf(nomearq, 6, "%i.csv", id);
		system(nomearq);
	}
#endif

char lerchar() {
	char c;
	scanf(" %c", &c);
	return(c);
}

void aguardar() {
	printf("\nPressione enter para continuar.\n");
	
	while(getchar() != '\n'); // limpa o buffer stdin até um \n
	ungetc('\n', stdin); // devolve o \n ao stdin (causa problemas se nao for feito)
	
	getchar(); // recebe um caractere, na pratica, aguarda o usuario apertar enter
	
	while(getchar() != '\n'); // limpa o buffer stdin até um \n
	ungetc('\n', stdin); // devolve o \n ao stdin (causa problemas se nao for feito)
}

// funcoes principais
struct hash* criar(int quantidade, int tamanho) {
	struct hash *novo = inictabela(tamanho); // inicializa a tabela
	
	int i, aux;
	for(i = 0; i < quantidade; i++) { // repete quantidade vezes
		do {
			aux = aleatorio(-65536, 65536); // gera numero aleatorio no intervalo informado
		} while(buscar(novo, aux) != -1); // faz de novo até encontrar um que ainda não exista
		
		adicionar(novo, aux); // adiciona o numero encontrado
	}
	
	return(novo);
}

void mostrar(struct lt *lista) {
	if(lista->qtd == 1) {
		printf("Há 1 tabela carregada:\n\n");
	}
	else {
		printf("Há %i tabelas carregadas:\n\n", lista->qtd);
	}

	int i, j = 0;
	
	for(i = 0; i < lista->tam; i++) {
		if(lista->tabela[i] == NULL) { // caso a tabela i não exista...
			continue; // ...vai para a próxima
		}
		
		j++; // aumenta o numero de tabelas encontradas - deve tender a lista->qtd
		
		printf("\t#%i: %s, ", i, lista->tabela[i]->nome);
		
		if(lista->tabela[i]->qtd == 0) { // escreve a concordância gramatical corretamente
			printf("com %i posições, vazia", lista->tabela[i]->tam);
		}
		else {
			printf("com %i posições e %i elementos", lista->tabela[i]->tam, lista->tabela[i]->qtd);
		}
		
		if(j == lista->qtd) { // escreve a pontuação corretamente 
			printf(".\n"); // ponto se for o ultimo
		}
		else {
			printf(";\n"); // ponto e virgula se nao for
		}
	}
	printf("\n");
}

struct hash* unir(struct lt *ltab, int a, int b) {
	int aux = 0;
	while(aux < 1) {
		printf("Indique a quantidade de posições da nova tabela hash: ");
		scanf("%i", &aux);
	}
	
	struct hash *novo = inictabela(aux); // inicializa uma nova tabela
	
	int i, rep = 0; // rep: numeros repetidos
	struct item *chave; // ponteiro para item de lista encadeada
	
	for(i = 0; i < ltab->tabela[a]->tam; i++) { // percorre todos os itens da PRIMEIRA tabela
		chave = ltab->tabela[a]->chave[i]; // faz o ponteiro apontar para a posição i
		
		while(chave != NULL) { // se chave nao aponta para NULL, a lista encadeada ainda nao terminou
			if(buscar(novo, chave->valor) == -1) { // verifica se o item em chave ja nao existe (-1 = não)
				adicionar(novo, chave->valor);
			}
			else { // se já existem, está repetido e não será adicionado
				rep++;
			}
			chave = chave->prox; // ponteiro recebe endereço do próximo item da lista encadeada
		}
	}
	
	for(i = 0; i < ltab->tabela[b]->tam; i++) { // percorre todos os itens da SEGUNDA tabela
		chave = ltab->tabela[b]->chave[i];
		
		while(chave != NULL) {
			if(buscar(novo, chave->valor) == -1) {
				adicionar(novo, chave->valor);
			}
			else {
				rep++;
			}
			chave = chave->prox;
		}
	}
	
	ltab->tabela[conseguirlugar(ltab)] = novo; // arranja um lugar para a nova tabela e a insere lá
	ltab->qtd += 1; // aumenta contador de tabelas
	
	if(rep > 0) { // se há números repetidos
		if(rep == 1) { // concordância gramatical
			printf("\n1 item está duplicado e foi adicionado apenas uma vez à nova tabela.");
		}
		else {
			printf("\n%i itens estão duplicados e foram adicionados apenas uma vez à nova tabela.", rep);
		}
	}
	else {
		printf("\nNenhum item está duplicado.");
	}
	
	return(novo);
}

int checar(struct lt *lista, int id) {
	if(id > lista->tam || id < 0 || lista->tabela[id] == NULL) { // id > tamanho da relação OU id negativo OU tabela aponta para NULL
		return(0);
	}
	else { // id < tamanho da relação E id positivo E tabela não aponta para NULL
		return(1);
	}
}

void adicionar(struct hash *tabela, int valor) {
	int posicao = hashear(valor, tabela->tam); // decide o destino do valor, através da função de hashing
	struct item *novoitem = (struct item*) malloc(sizeof(struct item)); // aloca a memória para o item da lista encadeada
	
	novoitem->valor = valor; // item da l.e. recebe o valor 
	novoitem->prox = tabela->chave[posicao]; // item da l.e. passa a ser o primeiro, apontando para o antigo primeiro
		
	tabela->chave[posicao] = novoitem; // coloca o item no lugar escolhido
	tabela->qtd += 1; // aumenta a quantidade de itens da tabela
}

void excluir(struct hash *tabela, int valor, int pos) {
	struct item **ptr = &((*tabela).chave[pos]); // ponteiro duplo recebe o endereco da chave a ser excluida
	struct item *aux;
	
	while((**ptr).valor != valor) { // enquanto o valor do item l.e. não for o alvo...
		ptr = &((**ptr).prox); // ...continua percorrendo a lista
	}
	
	aux = *ptr; // guarda o endereco do alvo
	*ptr = (**ptr).prox; // coloca o endereco do proximo item da l.e. no lugar do endereco antigo
	free(aux); // libera o item que agora está "solto"
	tabela->qtd -= 1; // diminui qtd em 1
}

// funcoes auxiliares
struct hash* inictabela(int tamanho) {
	struct hash *novo = (struct hash*) malloc(sizeof(struct hash));
	
	novo->qtd = 0;
	novo->tam = tamanho;
	novo->chave = (struct item**) malloc(tamanho*sizeof(struct item*)); // aloca espaco para 'tamanho' ponteiros para l.e.
	
	int i;
	for(i = 0; i < tamanho; i++) {
		novo->chave[i] = NULL;
	}
		
	return(novo);
}

struct lt* iniclt(int tamanho) {
	struct lt *novolt = (struct lt*) malloc(sizeof(struct lt));
	
	novolt->qtd = 0;
	novolt->tam = tamanho;
	novolt->tabela = (struct hash**) malloc(tamanho*sizeof(struct hash*)); // aloca espaco para 'tamanho' ponteiros para tabela
	
	int i;
	for(i = 0; i < tamanho; i++) {
		novolt->tabela[i] = NULL;
	}
	
	return(novolt);
}

int hashear(int valor, int tamanho) {
	if(valor >= 0) {
		return(valor % tamanho); // divide o valor pelo tamanho da tabela
	}
	else { // se o valor for negativo, deve-se considerar seu módulo
		return((valor*(-1)) % tamanho);
	}
}

void deletartab(struct lt *ltab, int id) {	
	liberar(ltab->tabela[id]); // libera a tabela 
	ltab->tabela[id] = NULL; // faz com que seu ponteiro aponte para NULL
	
	ltab->qtd -= 1;
}

void liberar(struct hash *tabela) {
	int i;
	struct item *aux, *bux;
	
	for(i = 0; i < tabela->tam; i++) { // percorre todas as posições da tabela
		aux = tabela->chave[i];
		
		while(aux != NULL) { // percorre toda a lista até o final
			bux = aux->prox; // armazena o endereco do proximo item
			free(aux); // libera item da lista 
			aux = bux; // recebe o endereco do proximo item
		}
	}
	
	free(tabela); // libera a tabela
}

int conseguirlugar(struct lt *ltab) {
	int i;
	for(i = 0; i < ltab->tam; i++) { // percorre lista de tabelas...
		if(ltab->tabela[i] == NULL) { // ...até achar posição vazia
			return(i);
		}
	}
	
	return(-1); // caso não haja posição vazia; não deve chegar aqui nunca
}

// funcoes de impressao
void imprimirtabela(struct hash *tabela) {
	int i;
	for(i = 0; i < tabela->tam; i++) {
		printf("\t%i: ", i);
		imprimirlista(tabela->chave[i]);
		printf("\n");
	}
	
	printf("\n");
}

void imprimirlista(struct item *itemlista) {
	struct item *ptr;
	
	for(ptr = itemlista; ptr != NULL; ptr = ptr->prox) { // percorre lista inteira 
		printf("%i", ptr->valor);
		
		if(ptr->prox != NULL) { // verifica se há próximo item
			printf(", ");
		}
	}
}

int aleatorio(int menor, int maior) {
	return(rand() % (maior - menor + 1) + menor);
}

int buscar(struct hash* tabela, int valor) {
	int pos = hashear(valor, tabela->tam); // descobre a posição onde o item, se existir, deve estar
	struct item *ptr;
	
	for(ptr = tabela->chave[pos]; ptr != NULL; ptr = ptr->prox) { // percorre toda a l.e.
		if(ptr->valor == valor) { // se encontrar o número...
			return(pos); // ...retorna a posição do valor, descoberta anteriormente
		}
	}

	return(-1); // caso o número não seja encontrado
};

// funcoes de arquivo
struct lt* lerlista() {
	struct lt *ltab = iniclt(10);
	
	int i;
	for(i = 0; i < 10; i++) { // busca arquivos com nome de 0.csv a 9.csv
		ltab->tabela[i] = lertabela(i);
		
		if(ltab->tabela[i] != NULL) { // se lertabela(i) não retornou NULL, quer dizer que há arquivo i.csv
			ltab->qtd += 1;
		}
	}
	
	return(ltab);
}

struct hash* lertabela(int id) {
	char nomearq[6];
	snprintf(nomearq, 6, "%i.csv", id); // preenche a string nomearq com o nome do arquivo a ser aberto
	
	FILE *arq = fopen(nomearq, "r");
	
	if(arq == NULL) { // se o aquivo não existir, arq armazena NULL
		return(NULL);
	}
	
	int tam = 0; // quantidade de posições a ser inferida durante a leitura
	char chaux, nil[256]; // nil só será usada para descarte
	
	fscanf(arq, "%s\n", nil); // descarta a primeira linha do arquivo, que contém o nome da tabela
	
	while((chaux = fgetc(arq)) != EOF) { // enquanto o aquivo não terminar
		ungetc(chaux, arq); // recoloca chaux, que foi lido para verificar o término do arquivo
		while(getc(arq) != '\n'); // percorre uma linha inteira (que representa uma lista encadeada), ignorando seu conteúdo
		tam++;
	} // ao fim do while, tam é a quantidade de posições da tabela
	
	rewind(arq); // volta o arquivo ao início 
	
	struct hash *tabhash = inictabela(tam); // inicializa a tabela, com tam posições
	struct item **ptr;
	
	fscanf(arq, "%s\n", tabhash->nome); // lê a primeira linha do arquivo, que contém o nome da tabela
	
	int i, aux;
	
	for(i = 0; i < tam; i++) { // percorre toda a tabela
		ptr = &(tabhash->chave[i]); // ptr aponta para a posição i
		chaux = fgetc(arq); // lê o primeiro caracetere da linha referente à posição i
		
		if(chaux == '\n') { // se o caracter lido for \n, a posição está vazia
			if(feof(arq) != 0) { // se o arquivo tiver terminado, sai do for
				break;
			}
			else { // senao, continua o for
				continue;
			}
		}
		else { // se o caracter lido nao for \n
			ungetc(chaux, arq); // recoloca o caracter lido
				
			do {
				fscanf(arq, "%i", &aux); // lê um inteiro
				*ptr = colocar(aux); // coloca o no na l.e.
				ptr = &(*ptr)->prox; // ptr aponta para o endereco que armazena o endereço do próximo item
				tabhash->qtd += 1; // aumenta a quantidade de itens da tabela
			} while(fgetc(arq) != '\n'); // faz isso tudo até que se encontre um \n - essa linha é responsável por consumir as vírgulas
		}
	}
	
	fclose(arq);
	return(tabhash);
}

struct item* colocar(int valor) {
	struct item *novo = (struct item*) malloc(sizeof(struct item)); // aloca o espaço para um item de l.e.
	
	novo->valor = valor;
	novo->prox = NULL;
	
	return(novo);
}

int salvarlt(struct lt *lista) {
	int i;	
	for(i = 0; i < lista->tam; i++) { // percorre todas as tabelas...
		salvartabela(lista->tabela[i], i); // ...salvando-as
	}
	
	return(0);
}

void salvartabela(struct hash *tabela, int id) {
	char nome[6];
	snprintf(nome, 6, "%i.csv", id); // preenche a string nome com o nome do arquivo a ser aberto

	if(tabela == NULL || tabela->qtd == 0) { // se a tabela não existir, o arquivo referente à ela deve ser deletado
		remove(nome); // exclui o arquivo, caso exista
		return; // sai da função
	}

	FILE *arq = fopen(nome, "w"); 
	
	fprintf(arq, "%s\n", tabela->nome); // salva o nome da tabela na primeira linha do arquivo
	
	struct item *chave; // ponteiro para item de l.e.
	int i;
	for(i = 0; i < tabela->tam; i++) { // percorre todas as posições da tabela
		chave = tabela->chave[i];
		
		while(chave != NULL) { // percorre todos os itens da l.e. 
			fprintf(arq, "%i", chave->valor); // escreve o valor no arquivo
			if(chave->prox != NULL) { // verifica se há próximo item
				fprintf(arq, ","); // se houver, escreve uma vírgula
			}
			chave = chave->prox;
		}
		
		if(i < tabela->tam ) { // se i não for a última posição
			fprintf(arq, "\n");
		}
	}
	
	fclose(arq);
}

// menus

void menuabrir(struct lt *ltab, int id) {
	if(checar(ltab, id) == 0) {
		printf("A tabela #%i não existe.", id);
		aguardar();
		return;
	}
	
	limpar();
	
	printf("\n\t\tTabela #%i: %s (%i posições e %i itens)\n\n", id, ltab->tabela[id]->nome, ltab->tabela[id]->tam, ltab->tabela[id]->qtd);
	
	if(ltab->tabela[id]->tam <= 30 && ltab->tabela[id]->qtd < 100) { // imprime a tabela caso ela nao tenha muitas unidades para não poluir a tela
		imprimirtabela(ltab->tabela[id]); 
	}
	
	char op;
	int aux, pos;
	
	printf("b. buscar número\n");
	printf("i. inserir numero\n");
	printf("r. remover numero\n");
	printf("d. deletar tabela\n");
	if(ltab->tabela[id]->tam > 30 || ltab->tabela[id]->qtd > 100) { // mostra a opcao de imprimir caso já nao tenha sido impressa 
		printf("m. imprimir no terminal\n");
	}
	printf("a. abrir em programa externo\n");
	printf("z. retornar\n");
	
	printf("\n > ");
	op = lerchar();
	
	switch(op) {
		case 'b':
			printf("\nBuscar número: ");
			scanf("%i", &aux);
			
			pos = buscar(ltab->tabela[id], aux);
			
			if(pos == -1) {
				printf("\nO valor não existe.");
			}
			else {
				printf("\nO valor %i está presente na posição %i:\n\n", aux, pos);
				
				printf("%i: ", pos);
				
				if((ltab->tabela[id]->qtd)/(ltab->tabela[id]->tam) <= 10) { // se a quantidade de média de itens por posição for menor que 10...
					imprimirlista(ltab->tabela[id]->chave[pos]); // ...imprime a lista: isso é útil para tabelas muito grande para nao poluir a tela
				}
				else { // se for maior...
					printf("[...], %i, [...]", aux); // ...resume a lista
				}
				
				printf("\n");
			}
			
			aguardar();
			
			break;
			
		case 'i':
			printf("\nInserir número: ");
			scanf("%i", &aux);
			
			adicionar(ltab->tabela[id], aux);
			
			break;
			
		case 'r':
			printf("\nRemover número: ");
			scanf("%i", &aux);
			
			pos = buscar(ltab->tabela[id], aux);
			
			if(pos == - 1) {
				printf("\nO valor não existe.");
			}
			else {
				excluir(ltab->tabela[id], aux, pos);
				//printf("%i excluído", aux);
			}
			
			break;
		
		case 'd':
			printf("\nTem certeza que deseja deletar a tabela? \n");
			printf("\ns/n  > ");
			
			op = lerchar();
			
			if(op == 's') {
				deletartab(ltab, id);
				printf("\nDeletada com sucesso.\n");
				aguardar();
				return;
			}
			else { // qualquer coisa diferente de s cancela a operação
				printf("\nExclusão cancelada.");
				aguardar();
			}
			
			break;
			
		case 'a':
			printf("\nNenhuma modificação feita manualmente será salva.\n"); // modificações não devem ser feitas diretamente
			
			salvartabela(ltab->tabela[id], id); // garante que o arquivo referente à tabela existe
			abrirexterno(id);
			salvartabela(ltab->tabela[id], id); // sobrescreve qualquer modificação feita manualmente
			
			aguardar();
			break;
			
		case 'm':
			if(ltab->tabela[id]->tam > 30 || ltab->tabela[id]->qtd > 100) { // só realiza a operação caso a opção tenha aparecido 
				limpar();
				printf("\n\t\tTabela #%i: %s (%i posições e %i itens)\n\n", id, ltab->tabela[id]->nome, ltab->tabela[id]->tam, ltab->tabela[id]->qtd);
				imprimirtabela(ltab->tabela[id]);
				aguardar();
			} // caso contrário o comando é ignorado
			
			break;
			
		case 'z':
			return;
			break;
		
		default:
			printf("%c: opção inválida.", op);
			break;
	}
	
	menuabrir(ltab, id);
}

void menucriar(struct lt *ltab) {
	limpar();
	printf("\n\t\tCriar nova tabela\n\n");
	
	char op;
	int aux, bux;
	
	printf("a. preencher com valores aleatorios\n");	
	printf("v. criar tabela vazia\n");
	printf("z. retornar\n");	
	
	printf("\n > ");
	op = lerchar();
	
	struct hash *novatab; 
	
	switch(op) {
		case 'a':
			do { // aguarda um valor válido para o tamanho da tabela
				printf("\nTamanho da tabela: ");
				scanf("%i", &aux);
			} while(aux <= 1);
			
			printf("Quantidade de itens: ");
			scanf("%i", &bux);
			
			while(bux >= 131072) {
				printf("\nA quantidade inserida é maior que a quantidade de números no intervalo [-2^16, 2^16].\n");
				printf("Quantidade de itens: ");
				scanf("%i", &bux);
			}
			
			novatab = criar(bux, aux);
			if(novatab == NULL) {
				printf("\nHouve um erro na criação da tabela. criar(ltab, aux) == NULL.\n");
			}
			else {
				printf("Tabela criada com sucesso.\n");
			}
			
			break;
		
		case 'v':
			do { // aguarda um valor válido para o tamanho da tabela
				printf("\nTamanho da tabela: ");
				scanf("%i", &aux);
			} while(aux <= 1);
			
			novatab = inictabela(aux);
			if(novatab == NULL) {
				printf("\nHouve um erro na criação da tabela. inictabela(ltab, aux) == NULL.\n");
				aguardar();
			}
			else {
				printf("Tabela criada com sucesso.\n");
			}
		
			break;
		
		case 'z':
			return;
			
		default:
			printf("%c: opção inválida.", op);
			menucriar(ltab);
			return;
	}
	
	aux = conseguirlugar(ltab); // arranja um lugar para a nova tabela
	ltab->tabela[aux] = novatab; // coloca a nova tabela em seu lugar
	ltab->qtd += 1; // aumenta o contador de tabelas 
	
	printf("\nPressione enter para chamar a tabela de %c, ou insira um nome customizado.\nNome: ", aux + 'A');
	
	while(getchar() != '\n'); // limpa o buffer completamente - o \n será recolocado mais tarde
	
	op = getchar(); // recebe um caractere, permitindo \n
	
	if(op == '\n') { // se o primeiro caractere lido for \n, o usuário apenas apertou enter
		novatab->nome[0] = aux + 'A'; // neste caso, a tabela recebe como nome uma letra correspondente ao seu número
	}
	else { // se nao for \n, o usuario digitou alguma coisa antes de apertar enter
		ungetc(op, stdin); // recoloca o caractere tirado de volta no buffer de entrada
		scanf(" %s", novatab->nome); // // lê o buffer e o coloca como nome da tabela
	}
	
	ungetc('\n', stdin); // recoloca o \n no buffer, causa problemas se nao for feito
	aguardar();
	
	menuabrir(ltab, aux); // abre a tabela recém criada
}
