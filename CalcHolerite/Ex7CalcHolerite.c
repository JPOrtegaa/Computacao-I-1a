#include <stdio.h>

#define MAX 51

typedef struct{
	int dia;
	int mes;
	int ano;
} DATA;

typedef struct{
	int porcentagem;
	float desconto;
} INSS;

typedef struct{
	int porcentagem;
	float desconto;
} IRRF;

typedef struct{
	int filhos;
	float ganho;
} FAMILIA;

typedef struct{
	char nome[MAX];
	char cargo[MAX];
	DATA nascimento;
	FAMILIA salario;
	float salarioBruto;
	INSS inss;
	IRRF irrf;
	float salarioLiquido;
} FUNCIONARIO;

void lerDados(char *fnm, FUNCIONARIO vet[], int *n){
	int i;
	FILE *f;
	f = fopen(fnm, "r");
	for(i = 0; fscanf(f, "%[^\n]%*c", vet[i].nome) != EOF; i++){
		fscanf(f, "%[^\n]%*c", vet[i].cargo);
		fscanf(f, "%d/%d/%d", &vet[i].nascimento.dia, &vet[i].nascimento.mes, &vet[i].nascimento.ano);
		fscanf(f, "%d", &vet[i].salario.filhos);
		fscanf(f, "%f%*c", &vet[i].salarioBruto);
	}
	*n = i;
	fclose(f);
}

void calcINSS(FUNCIONARIO *vet){
	if(vet->salarioBruto <= 1399.12){
		vet->inss.porcentagem = 8;
		vet->inss.desconto = vet->salarioBruto * (vet->inss.porcentagem / 100.0);
	}
	else
	if(vet->salarioBruto <= 2331.88){
		vet->inss.porcentagem = 9;
		vet->inss.desconto = vet->salarioBruto * (vet->inss.porcentagem / 100.0);
	}
	else{
		vet->inss.porcentagem = 11;
		vet->inss.desconto = vet->salarioBruto * (vet->inss.porcentagem / 100.0);
	}
}

void calcIRRF(FUNCIONARIO *vet){
	if(vet->salarioBruto <= 1903.98){
		vet->irrf.porcentagem = 0;
		vet->irrf.desconto = vet->salarioBruto * (vet->irrf.porcentagem / 100.0);
	}
	else
	if(vet->salarioBruto <= 2826.65){
		vet->irrf.porcentagem = 7.5;
		vet->irrf.desconto = vet->salarioBruto * (vet->irrf.porcentagem / 100.0);
	}
	else
	if(vet->salarioBruto <= 3751.05){
		vet->irrf.porcentagem = 15;
		vet->irrf.desconto = vet->salarioBruto * (vet->irrf.porcentagem / 100.0);
	}
	else
	if(vet->salarioBruto <= 4664.68){
		vet->irrf.porcentagem = 22.5;
		vet->irrf.desconto = vet->salarioBruto * (vet->irrf.porcentagem / 100.0);
	}
	else{
		vet->irrf.porcentagem = 27.5;
		vet->irrf.desconto = vet->salarioBruto * (vet->irrf.porcentagem / 100.0);
	}
}

void calcDescontos(FUNCIONARIO *vet){
		calcINSS(vet);
		calcIRRF(vet);
}

void calcFamilia(FUNCIONARIO *vet){
	if(vet->salarioBruto <= 806.81)
		vet->salario.ganho = vet->salario.filhos * 41.37;
	else
	if(vet->salarioBruto <= 1212.64)
		vet->salario.ganho = vet->salario.filhos * 29.16;
}

void calcVencimentos(FUNCIONARIO *vet){
	calcFamilia(vet);
}

void calcSalarioLiquido(FUNCIONARIO *vet){
	vet->salarioLiquido = vet->salarioBruto + vet->salario.ganho - vet->inss.desconto - vet->irrf.desconto;
}

void calculos(FUNCIONARIO vet[], int n){
	int i;
	for(i = 0; i < n; i++){
		calcDescontos(&vet[i]);
		calcVencimentos(&vet[i]);
		calcSalarioLiquido(&vet[i]);
	}
}

void printSalarioFamilia(FUNCIONARIO x, FILE *f){
	if(x.salarioBruto < 1212.64 && x.salario.filhos)
		fprintf(f, "Salario Familia			%d				R$ %.2f\n", x.salario.filhos, x.salario.ganho);
	else
		fprintf(f, "Salario Familia			-				-\n");
}

void printIRRF(IRRF x, FILE *f){
	if(x.porcentagem)
		fprintf(f, "IRRF				%d%%									R$ %.2f\n", x.porcentagem, x.desconto);
	else
		fprintf(f, "IRRF				-									-\n");
}

void relatorio(char *fnm, FUNCIONARIO vet[], int n){
	int i;
	FILE *f;
	f = fopen(fnm, "w");
	for(i = 0; i < n; i++){
		fprintf(f, "Nome do funcionario: %-43s", vet[i].nome);
		fprintf(f, "Cargo: %s\n", vet[i].cargo);
		fprintf(f, "Descricao			Referencia			Vencimentos				Descontos\n");
		fprintf(f, "Salario Bruto							R$ %.2f\n", vet[i].salarioBruto);
		printSalarioFamilia(vet[i], f);
		fprintf(f, "INSS				%d%%									R$ %.2f\n", vet[i].inss.porcentagem, vet[i].inss.desconto);
		printIRRF(vet[i].irrf, f);
		fprintf(f, "										Total de descontos	R$ %.2f\n", vet[i].inss.desconto + vet[i].irrf.desconto);
		fprintf(f, "										Salario Liquido		R$ %.2f\n", vet[i].salarioLiquido);
		fprintf(f, "--------------------------------------------------------------------------------------------------------------------------------");
		fprintf(f, "\n");
	}
	fclose(f);
}

int main(){
	FUNCIONARIO vet[MAX];
	int n;

	lerDados("empresa.txt", vet, &n);
	calculos(vet, n);
	relatorio("holerites.txt", vet, n);

	return 0;
}
