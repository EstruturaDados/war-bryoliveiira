#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

int NUM_TERRITORIOS = 0;

Territorio* alocarMemoria();
void cadastrarTerritorios(Territorio* mapa);
void exibirTerritorios(Territorio* mapa);
int escolherTerritorio(Territorio* mapa, const char* tipo, const char* corProibida);
void atacar(Territorio* mapa);
void liberarMemoria(Territorio* mapa);
int rolarDado();

int rolarDado() {
    return (rand() % 6) + 1;
}

Territorio* alocarMemoria() {
    int n;
    Territorio* mapa;

    printf("--- Alocacao de Memoria ---\n");
    printf("Quantos territorios serao cadastrados? ");
    scanf("%d", &n);

    NUM_TERRITORIOS = n;

    mapa = (Territorio*)calloc(NUM_TERRITORIOS, sizeof(Territorio));

    if (mapa == NULL) {
        printf("\nERRO: Falha na alocacao de memoria!\n");
        exit(1); 
    }

    printf("Memoria alocada com sucesso para %d territorios.\n\n", NUM_TERRITORIOS);
    return mapa;
}

void cadastrarTerritorios(Territorio* mapa) {
    printf("--- Cadastro dos Territorios ---\n");

    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        Territorio* t = mapa + i;

        printf("\nTerritorio %d:\n", i + 1);

        printf("  Nome: ");
        scanf("%29s", t->nome); 

        printf("  Cor do exercito: ");
        scanf("%9s", t->cor);

        printf("  Tropas (min 1): ");
        scanf("%d", &(t->tropas));
        if (t->tropas < 1) t->tropas = 1;
    }
    printf("\nCadastro concluido.\n");
}

void exibirTerritorios(Territorio* mapa) {
    printf("\n======================================================\n");
    printf("--- SITUACAO ATUAL DOS TERRITORIOS ---\n");
    printf("======================================================\n");

    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        Territorio* t = mapa + i;
        printf("ID: [%d] | Nome: %-15s | Cor: %-9s | Tropas: %d\n",
               i, t->nome, t->cor, t->tropas);
    }
    printf("======================================================\n\n");
}

int escolherTerritorio(Territorio* mapa, const char* tipo, const char* corProibida) {
    int id;
    int valido = 0;
    
    while (!valido) {
        printf("Escolha o ID do Territorio %s (0 a %d): ", tipo, NUM_TERRITORIOS - 1);
        if (scanf("%d", &id) != 1 || id < 0 || id >= NUM_TERRITORIOS) {
            printf("Erro: ID invalido. Tente novamente.\n");
            while (getchar() != '\n');
            continue;
        }
        
        if (strcmp(corProibida, "") != 0 && strcmp((mapa + id)->cor, corProibida) == 0) {
            printf("Erro: Nao e possivel atacar um territorio da mesma cor (%s).\n", corProibida);
        } else {
            valido = 1;
        }
    }
    return id;
}

void atacar(Territorio* mapa) {
    int id_a, id_d;
    Territorio *atacante, *defensor;
    int dado_a, dado_d;
    
    printf("\n--- SIMULACAO DE ATAQUE ---\n");

    id_a = escolherTerritorio(mapa, "atacante", ""); 
    atacante = mapa + id_a; 
    
    id_d = escolherTerritorio(mapa, "defensor", atacante->cor); 
    defensor = mapa + id_d; 

    printf("\n-- BATALHA: %s (%s) vs %s (%s) --\n", 
           atacante->nome, atacante->cor, defensor->nome, defensor->cor);

    if (atacante->tropas < 2) {
        printf("Ataque cancelado: O atacante precisa de no minimo 2 tropas.\n");
        return;
    }
    
    dado_a = rolarDado();
    dado_d = rolarDado();

    printf("Rolagem de Dados: Atacante tirou %d | Defensor tirou %d\n", dado_a, dado_d);

    if (dado_a > dado_d) {
        printf("VITORIA DO ATACANTE! (%s domina %s)\n", atacante->cor, defensor->nome);
        
        strcpy(defensor->cor, atacante->cor); 
        
        int tropas_transferidas = atacante->tropas / 2;

        defensor->tropas = tropas_transferidas;
        atacante->tropas -= tropas_transferidas;

        printf("Transferencia de %d tropas. Novo total de tropas do atacante: %d\n", 
               tropas_transferidas, atacante->tropas);
        
    } else {
        printf("DEFESA BEM SUCEDIDA! (%s manteve %s)\n", defensor->cor, defensor->nome);
        
        atacante->tropas -= 1; 

        printf("O atacante (%s) perdeu 1 tropa. Tropas restantes: %d\n", 
               atacante->nome, atacante->tropas);
    }
    
    exibirTerritorios(mapa);
}

void liberarMemoria(Territorio* mapa) {
    if (mapa != NULL) {
        free(mapa);
        printf("\nMemoria alocada para os territorios foi liberada com sucesso.\n");
    }
}

int main() {
    srand(time(NULL));

    Territorio* mapa_territorios = NULL;
    char opcao;
    
    mapa_territorios = alocarMemoria();
    cadastrarTerritorios(mapa_territorios);
    exibirTerritorios(mapa_territorios);

    do {
        printf("\nO que deseja fazer?\n");
        printf(" [A] - Atacar\n");
        printf(" [S] - Sair (e liberar memoria)\n");
        printf(" Escolha: ");
        
        while (getchar() != '\n'); 
        scanf(" %c", &opcao);

        if (opcao == 'A' || opcao == 'a') {
            atacar(mapa_territorios);
        } else if (opcao != 'S' && opcao != 's') {
            printf("Opcao invalida.\n");
        }

    } while (opcao != 'S' && opcao != 's');
    
    liberarMemoria(mapa_territorios);

    return 0;
}

 /*
 * O principal objetivo do código foi usar a 'struct Territorio' para agrupar as informações (nome, cor e tropas) em um único "pacote".
 * Criei um vetor (lista) com 5 posições dessa struct para cadastrar 5 territórios.
 * Os laços 'for' repetem a entrada de dados (scanf) e a saída de dados (printf).
 * Para acessar os dados, usamos a sintaxe: lista[posicao].campo.
 * No terceiro commit, foi implementada a funcionalidade de Ataque e a estrutura avançada de gerenciamento de memória do sistema War
 */
