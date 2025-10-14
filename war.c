#include <stdio.h>
#include <string.h>

struct Territorio {
    char nome[30];  
    char cor[10];   
    int tropas;    
};

int main() {
    struct Territorio lista_territorios[5];
    int i; 

    printf("--- Cadastro de 5 Territorios ---\n\n");

    for (i = 0; i < 5; i++) {
        printf("--- TERRITORIO %d ---\n", i + 1);

        printf("Nome: ");
        scanf("%29s", lista_territorios[i].nome);

        printf("Cor do Exercito: ");
        scanf("%9s", lista_territorios[i].cor);

        printf("Quantidade de Tropas (numero): ");
        scanf("%d", &lista_territorios[i].tropas);
        
        printf("\n"); 
    }

    printf("==========================================\n");
    printf("--- DADOS CADASTRADOS ---\n");
    printf("==========================================\n");

    for (i = 0; i < 5; i++) {
        printf("Territorio %d:\n", i + 1);
        printf("  Nome: %s\n", lista_territorios[i].nome);
        printf("  Cor: %s\n", lista_territorios[i].cor);
        printf("  Tropas: %d\n", lista_territorios[i].tropas);
        printf("------------------------------------------\n");
    }

    return 0;
}

 /*
 * O principal objetivo do código foi usar a 'struct Territorio' para agrupar as informações (nome, cor e tropas) em um único "pacote".
 * Criei um vetor (lista) com 5 posições dessa struct para cadastrar 5 territórios.
 * Os laços 'for' repetem a entrada de dados (scanf) e a saída de dados (printf).
 * Para acessar os dados, usamos a sintaxe: lista[posicao].campo.
 */
