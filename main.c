/*
maio de 2020
Quinto semestre de Engenharia de Computação
Estruturas de Dados II
Trabalho de Tabelas Hash
*/

#include <stdio.h>
#include <stdlib.h>

#include "func.c" 

int main() {
	ungetc('\n', stdin);
	srand(time(NULL));
	struct lt *ltab = lerlista();
	
	int repetir = 1, aux, bux, deletoutodos = 0;
	char op;
	
	while(repetir == 1) {
		limpar();
		printf("\n\t\tImplementação de tabelas hash\n\n");
		

		if(ltab->qtd == 0) {
			printf("Nenhuma tabela carregada.\n\n");
			
			printf("n. nova tabela\n");
			
			if(deletoutodos == 1) {
				printf("s. atualizar arquivos (apagar arquivos das tabelas deletadas)\n");
			}
			
			printf("\n > ");
			op = lerchar();
			
			switch(op) {
				case 'n':
					menucriar(ltab);
					break;
					
				case 's':
					if(deletoutodos == 0) {
						printf("\nNão há tabelas suficientes para esta ação.");
						break;
					}
					
					if(salvarlt(ltab) == 0) {
						printf("\nAtualizado com sucesso\n");
					}
					else {
						printf("\nOcorreu um erro\n");
					}
					
					deletoutodos = 0;
					aguardar();
					break;
			}
		}
		else {
			mostrar(ltab);
			
			printf("t. abrir tabela\n");
			if(ltab->qtd < ltab->tam) {
				printf("n. nova tabela\n");
			}
			if(ltab->qtd >= 2) {
				printf("u. unir tabelas\n");
			}
			printf("s. atualizar arquivos\n");
			
			printf("\n > ");
			op = lerchar();
			
			switch(op) {
				case 'n':
					if(ltab->qtd == ltab->tam) {
						printf("\nNão há vagas para novas tabelas.");
						break;
					}
					menucriar(ltab);
					break;
					
				case 't':
					printf("Selecionar tabela: ");
					scanf("%i", &aux);
					menuabrir(ltab, aux);
					system("start %i.csv");
					break;
					
				case 's':
					if(salvarlt(ltab) == 0) {
						printf("\nSalvo com sucesso\n");
					}
					else {
						printf("\nOcorreu um erro\n");
					}
					
					aguardar();
					break;
				
				case 'u':
					if(ltab->qtd < 2) {
						printf("\nNão há tabelas suficientes para esta ação.");
						break;
					}
					
					aux = -1;
					bux = -1;
					printf("\nSelecione as tabelas desejadas:\n\n");
					
					printf("Primeira tabela: ");
					scanf("%i", &aux);
					while(checar(ltab, aux) != 1) {
						printf("A tabela não existe. Primeira tabela: ");
						scanf("%i", &aux);
					}
					
					printf("Segunda tabela: ");
					scanf("%i", &bux);
					while(checar(ltab, bux) != 1) {
						printf("A tabela não existe. Segunda tabela: ");
						scanf("%i", &bux);
					}
					
					if(unir(ltab, aux, bux) != NULL) {
						printf("\nUnião realizada com sucesso.");
					}
					else {
						printf("\nHouve um erro na tarefa: unir(ltab, aux, bux) retornou NULL.");
					}
					aguardar();
					break;
					
				default:
					printf("%i: opção inválida.", op);
					aguardar();
					break;
			}
			
			if(ltab->qtd == 0) {
				deletoutodos = 1;
			}
		}
	}
}
