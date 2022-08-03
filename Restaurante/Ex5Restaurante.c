#include <stdio.h>
#include <string.h>

#define MAX 51

typedef struct{
	int dia;
	int mes;
	int ano;
} DATA;

typedef struct{
	int hora;
	int min;
} HORARIO;

typedef struct{
	char nome[MAX];
	int quantidade;
	float preco;
	float total;
} ITEM;

typedef struct{
	char nome[MAX];
	int quantidade;
	float preco;
} CONSUMO;

typedef struct{
	char nome[MAX];
	float vendas;
} GARCOM;

typedef struct{
	char garcom[MAX];
	char mesaNome[MAX];
	HORARIO entrada;
	HORARIO saida;
	int pessoas;
	CONSUMO item[MAX];
	int totalItem;
} MESA;

void scanItem(FILE *f, MESA *vet){
	int i;
	for(i = 0; fscanf(f, "%s%*c", vet->item[i].nome); i++){ // descartar aqui o \n pois a proxima coisa a ser lida sera uma string %[^\n]!!
		if(!strcmp(vet->item[i].nome, "<fim>")) break;
		fscanf(f, "%d", &vet->item[i].quantidade);
		fscanf(f, "%f", &vet->item[i].preco);
	}
	vet->totalItem = i;
}

void lerDados(char *fnm, MESA vet[], DATA *x, int *n){
	int i;
	FILE *f;
	f = fopen(fnm, "r");
	fscanf(f, "%d%*c%d%*c%d%*c", &x->dia, &x->mes, &x->ano);
	for(i = 0; fscanf(f, "%[^\n]%*c", vet[i].garcom) != EOF; i++){
		fscanf(f, "%[^\n]%*c", vet[i].mesaNome);
		fscanf(f, "%d%*c%d%*c", &vet[i].entrada.hora, &vet[i].entrada.min);
		fscanf(f, "%d%*c%d%*c", &vet[i].saida.hora, &vet[i].saida.min);
		fscanf(f, "%d", &vet[i].pessoas);
		scanItem(f, &vet[i]);
		//fseek(f, 2, SEEK_CUR); // necessario deslocar o ponteiro do arquivo em 2 para descartar o \n!!!!!!!
	}
	*n = i;
	fclose(f);
}

int verificaItem(char str[], ITEM vet1[], int index, int *repetido){
	int i;
	for(i = 0; i < index; i++)
		if(!strcmp(str, vet1[i].nome)){
			*repetido = i;
			return 1;
		}
	return 0;
}

void getITEM(MESA vet[], ITEM vet1[], int n, int *n1){
	int i, j, index = 0, repetido;
	for(i = 0; i < n; i++)
		for(j = 0; j < vet[i].totalItem; j++){
			if(verificaItem(vet[i].item[j].nome, vet1, index, &repetido)){ // atualizando item!!
				vet1[repetido].quantidade += vet[i].item[j].quantidade;
				vet1[repetido].total = vet1[repetido].quantidade * vet1[repetido].preco;
			}
			else{ // setando novo item!!
				strcpy(vet1[index].nome, vet[i].item[j].nome);
				vet1[index].preco = vet[i].item[j].preco;
				vet1[index].quantidade = vet[i].item[j].quantidade;
				vet1[index].total = vet1[index].quantidade * vet1[index].preco;
				index++;
			}
		}
	*n1 = index;
}

void relatorioItem(char *fnm, ITEM vet1[], int n1){
	int i;
	FILE *f;
	f = fopen(fnm, "w");
	for(i = 0; i < n1; i++){
		fprintf(f, "%d - ", i+1);
		fprintf(f, "Nome = %s\n", vet1[i].nome);
		fprintf(f, "    Quantidade = %d\n", vet1[i].quantidade);
		fprintf(f, "    Preco = %.2f\n", vet1[i].preco);
		fprintf(f, "    Total = %.2f\n", vet1[i].total);
		fprintf(f, "\n");
	}
	fclose(f);
}

int comparador(const void *a, const void *b){
	return ((*(ITEM *)b).quantidade - (*(ITEM *)a).quantidade);
}

void ordenaItem(ITEM vet1[], int n1){
	qsort(vet1, n1, sizeof(ITEM), comparador);
}

int verificaGarcom(char str[], GARCOM vet2[], int index, int *repetido){
	int i;
	for(i = 0; i < index; i++)
		if(!strcmp(str, vet2[i].nome)){
			*repetido = i;
			return 1;
		}
	return 0;
}

void setVendas(float *vendas, MESA vet){
	int i;
	for(i = 0; i < vet.totalItem; i++)
		*vendas += vet.item[i].quantidade * vet.item[i].preco;
}

void getGarcom(MESA vet[], GARCOM vet2[], int n, int *n2){
	int i, j, index = 0, repetido;
	for(i = 0; i < n; i++){
		if(verificaGarcom(vet[i].garcom, vet2, index, &repetido)) // atualizando garcom!!
			setVendas(&vet2[repetido].vendas, vet[i]);
		else{ // setando novo garcom!!!
			strcpy(vet2[index].nome, vet[i].garcom);
			vet2[index].vendas = 0;
			setVendas(&vet2[index].vendas, vet[i]);
			index++;
		}
	}
	*n2 = index;
}

void relatorioGarcom(char *fnm, GARCOM vet2[], int n2){
	int i;
	FILE *f;
	f = fopen(fnm, "w");
	for(i = 0; i < n2; i++){
		fprintf(f, "%d - ", i+1);
		fprintf(f, "Garcom = %s\n", vet2[i].nome);
		fprintf(f, "    Vendas = %.2f\n", vet2[i].vendas);
		fprintf(f, "\n");
	}
	fclose(f);
}

int comparador2(const void *a, const void *b){
	return ((*(GARCOM *)b).vendas - (*(GARCOM *)a).vendas);
}

void ordenaGarcom(GARCOM vet2[], int n2){
	qsort(vet2, n2, sizeof(GARCOM), comparador2);
}

void printItem(MESA vet, FILE *f, float *total){
	int i;
	for(i = 0; i < vet.totalItem; i++){
		*total += vet.item[i].quantidade * vet.item[i].preco;
		fprintf(f, "%s ", vet.item[i].nome);
		fprintf(f, "%d ", vet.item[i].quantidade);
		fprintf(f, "R$%.2f\n", vet.item[i].preco);
	}
}

float totalVendas(GARCOM vet2[], int n2){
	int i;
	float total = 0;
	for(i = 0; i < n2; i++)
		total += vet2[i].vendas;
	return total;
}

void relatorioDetalhado(char *fnm, DATA x, MESA vet[], GARCOM vet2[], int n, int n2){
	int i; float total;
	FILE *f;
	f = fopen(fnm, "w");
	fprintf(f, "%02d/%02d/%02d\n\n", x.dia, x.mes, x.ano);
	for(i = 0; i < n; i++){
		total = 0;
		fprintf(f, "%s\n", vet[i].mesaNome);
		fprintf(f, "%02d:%02d ", vet[i].entrada.hora, vet[i].entrada.min);
		fprintf(f, "%02d:%02d\n", vet[i].saida.hora, vet[i].saida.min);
		fprintf(f, "Garcom = %s\n", vet[i].garcom);
		printItem(vet[i], f, &total);
		fprintf(f, "Mesa = R$%.2f\n", total);
		fprintf(f, "\n");
	}
	fprintf(f, "\nTotal vendas = R$%.2f\n", totalVendas(vet2, n2));
}

int main(){
	MESA vet[MAX];
	ITEM vet1[MAX];
	GARCOM vet2[MAX];
	DATA x;
	int i, n, n1, n2;
	
	lerDados("consumo.txt", vet, &x, &n);
	getITEM(vet, vet1, n, &n1);
	relatorioItem("produtos.txt", vet1, n1);
	ordenaItem(vet1, n1);
	relatorioItem("produtosx.txt", vet1, n1);
	getGarcom(vet, vet2, n, &n2);
	relatorioGarcom("garcom.txt", vet2, n2);
	ordenaGarcom(vet2, n2);
	relatorioGarcom("garcomx.txt", vet2, n2);
	relatorioDetalhado("detalhes.txt", x, vet, vet2, n, n2);
	
	return 0;
}
