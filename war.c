#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

typedef struct {
    char cor[10];
    char* missao; 
    int num_territorios_conquistados; 
} Jogador;

int NUM_TERRITORIOS = 0;
int NUM_JOGADORES = 3; 

char* MISSOES[] = {
    "Vencer o jogo conquistando 5 territorios.",
    "Vencer o jogo eliminando todas as tropas da cor Azul.",
    "Vencer o jogo eliminando todas as tropas da cor Vermelha.",
    "Vencer o jogo eliminando todas as tropas da cor Verde.",
    "Vencer o jogo conquistando 4 territorios seguidos.",
    "Vencer o jogo conquistando 6 territorios."
};
const int TOTAL_MISSOES = 6;

char* CORES_JOGADORES[] = {"Vermelho", "Azul", "Verde"};

Territorio* alocarMapa();
Jogador* alocarJogadores();
void atribuirMissao(Jogador* jogador);
void cadastrarTerritorios(Territorio* mapa, Jogador* jogadores);
void exibirTerritorios(Territorio* mapa);
int escolherTerritorio(Territorio* mapa, const char* tipo, const char* corProibida);
void atacar(Territorio* mapa, Jogador* jogadores);
int rolarDado();
void exibirMissao(char* missao); 
int verificarMissao(Jogador* jogador, Territorio* mapa); 
void liberarMemoria(Territorio* mapa, Jogador* jogadores);

int rolarDado() {
    return (rand() % 6) + 1;
}

void atribuirMissao(Jogador* jogador) {
    int indice_sorteado = rand() % TOTAL_MISSOES;
    char* missao_sorteada = MISSOES[indice_sorteado];
    
    jogador->missao = (char*)malloc(strlen(missao_sorteada) + 1);
    
    if (jogador->missao == NULL) {
        printf("Erro na alocacao de memoria para a missao.\n");
        exit(1);
    }
    
    strcpy(jogador->missao, missao_sorteada);
}

void exibirMissao(char* missao) {
    printf("Sua Missao Estrategica: %s\n", missao);
}

Territorio* alocarMapa() {
    int n;
    Territorio* mapa;

    printf("--- Alocacao de Memoria ---\n");
    printf("Quantos territorios serao cadastrados? ");
    scanf("%d", &n);

    NUM_TERRITORIOS = n;

    mapa = (Territorio*)calloc(NUM_TERRITORIOS, sizeof(Territorio));

    if (mapa == NULL) {
        printf("\nERRO: Falha na alocacao de memoria do mapa!\n");
        exit(1); 
    }

    printf("Memoria alocada com sucesso para %d territorios.\n\n", NUM_TERRITORIOS);
    return mapa;
}

Jogador* alocarJogadores() {
    Jogador* jogadores = (Jogador*)calloc(NUM_JOGADORES, sizeof(Jogador));
    if (jogadores == NULL) {
        printf("\nERRO: Falha na alocacao de memoria dos jogadores!\n");
        exit(1);
    }
    
    for (int i = 0; i < NUM_JOGADORES; i++) {
        strcpy(jogadores[i].cor, CORES_JOGADORES[i]);
        jogadores[i].num_territorios_conquistados = 0;
        atribuirMissao(&jogadores[i]);
    }
    printf("Cadastrados %d jogadores iniciais (Cores: Vermelho, Azul, Verde).\n", NUM_JOGADORES);
    return jogadores;
}

void cadastrarTerritorios(Territorio* mapa, Jogador* jogadores) {
    printf("--- Cadastro dos Territorios ---\n");
    
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        Territorio* t = mapa + i;
        int id_jogador = i % NUM_JOGADORES; 
        
        printf("\nTerritorio %d:\n", i + 1);

        printf("  Nome: ");
        scanf("%29s", t->nome); 

        strcpy(t->cor, jogadores[id_jogador].cor); 
        t->tropas = 1 + (rand() % 5); 

        jogadores[id_jogador].num_territorios_conquistados++;
    }
    printf("\nCadastro e distribuicao de cores iniciais concluidos.\n");
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
        if (scanf("%d", &id) != 1 || id < 0 || id < NUM_TERRITORIOS) {
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
void atacar(Territorio* mapa, Jogador* jogadores) {
    int id_a, id_d;
    Territorio *atacante, *defensor;
    int dado_a, dado_d;
    
    printf("\n--- SIMULACAO DE ATAQUE ---\n");

    id_a = escolherTerritorio(mapa, "atacante", ""); 
    atacante = mapa + id_a; 
    
    id_d = escolherTerritorio(mapa, "defensor", atacante->cor); 
    defensor = mapa + id_d; 

    Jogador* atacante_jogador = NULL;
    for (int i = 0; i < NUM_JOGADORES; i++) {
        if (strcmp(jogadores[i].cor, atacante->cor) == 0) {
            atacante_jogador = &jogadores[i];
            break;
        }
    }

    if (atacante->tropas < 2) {
        printf("Ataque cancelado: O atacante precisa de no minimo 2 tropas.\n");
        return;
    }
    
    printf("\n-- BATALHA: %s (%s) vs %s (%s) --\n", 
           atacante->nome, atacante->cor, defensor->nome, defensor->cor);

    dado_a = rolarDado();
    dado_d = rolarDado();

    printf("Rolagem de Dados: Atacante tirou %d | Defensor tirou %d\n", dado_a, dado_d);

    if (dado_a > dado_d) {
        printf("VITORIA DO ATACANTE! (%s domina %s)\n", atacante->cor, defensor->nome);
        
        if (strcmp(defensor->cor, atacante->cor) != 0) {
            atacante_jogador->num_territorios_conquistados++;
        }
        
        strcpy(defensor->cor, atacante->cor); 
        
        int tropas_transferidas = atacante->tropas / 2;

        defensor->tropas = tropas_transferidas;
        atacante->tropas -= tropas_transferidas;

        printf("Transferencia de %d tropas. Tropas restantes do atacante: %d\n", 
               tropas_transferidas, atacante->tropas);
        
    } else {
        printf("DEFESA BEM SUCEDIDA! (%s manteve %s)\n", defensor->cor, defensor->nome);
        
        atacante->tropas -= 1; 

        printf("O atacante (%s) perdeu 1 tropa. Tropas restantes: %d\n", 
               atacante->nome, atacante->tropas);
    }
}
int verificarMissao(Jogador* jogador, Territorio* mapa) {
    char* missao = jogador->missao;
    
    if (strstr(missao, "conquistando")) {
        int alvo;
        sscanf(missao, "Vencer o jogo conquistando %d territorios", &alvo);
        
        if (jogador->num_territorios_conquistados >= alvo) {
            return 1;
        }
    }
    
    else if (strstr(missao, "eliminando")) {
        char cor_alvo[10];
        sscanf(missao, "Vencer o jogo eliminando todas as tropas da cor %s", cor_alvo);
        
        int cor_encontrada = 0;
        for (int i = 0; i < NUM_TERRITORIOS; i++) {
            if (strcmp((mapa + i)->cor, cor_alvo) == 0) {
                cor_encontrada = 1;
                break;
            }
        }
        if (!cor_encontrada) {
            return 1;
        }
    }
    
    return 0;
}

void liberarMemoria(Territorio* mapa, Jogador* jogadores) {
    for (int i = 0; i < NUM_JOGADORES; i++) {
        if (jogadores[i].missao != NULL) {
            free(jogadores[i].missao);
            jogadores[i].missao = NULL;
        }
    }
    
    if (jogadores != NULL) {
        free(jogadores);
        jogadores = NULL;
    }
    
    if (mapa != NULL) {
        free(mapa);
        mapa = NULL;
    }
    
    printf("\n\nMemoria alocada (territorios e missoes) foi liberada com sucesso.\n");
}

int main() {
    srand(time(NULL));

    Territorio* mapa_territorios = NULL;
    Jogador* jogadores = NULL;
    char opcao;
    int vencedor_encontrado = 0;
    
    mapa_territorios = alocarMapa();
    jogadores = alocarJogadores();

    printf("\n--- MISSOES INICIAIS ---\n");
    for (int i = 0; i < NUM_JOGADORES; i++) {
        printf("Jogador %s: ", jogadores[i].cor);
        exibirMissao(jogadores[i].missao); 
    }
    printf("------------------------\n");

    cadastrarTerritorios(mapa_territorios, jogadores);
    exibirTerritorios(mapa_territorios);

    do {
        printf("\n\n--- TURNO DE JOGO ---\n");
        
        atacar(mapa_territorios, jogadores);
        exibirTerritorios(mapa_territorios);

        for (int i = 0; i < NUM_JOGADORES; i++) {
            if (verificarMissao(&jogadores[i], mapa_territorios)) { // Passagem por referência
                printf("\n\n======================================================\n");
                printf("!!! MISSAO CUMPRIDA !!!\n");
                printf("O JOGADOR DA COR %s VENCEU O JOGO!\n", jogadores[i].cor);
                printf("Missao: %s\n", jogadores[i].missao);
                printf("======================================================\n\n");
                vencedor_encontrado = 1;
                break;
            }
        }

        if (vencedor_encontrado) break;

        printf("Continuar (C) ou Sair (S)? ");
        while (getchar() != '\n'); 
        scanf(" %c", &opcao);

    } while (opcao != 'S' && opcao != 's');
    
    liberarMemoria(mapa_territorios, jogadores);

    return 0;
}

 /*
 * O principal objetivo do código foi usar a 'struct Territorio' para agrupar as informações (nome, cor e tropas) em um único "pacote".
 * Criei um vetor (lista) com 5 posições dessa struct para cadastrar 5 territórios.
 * Os laços 'for' repetem a entrada de dados (scanf) e a saída de dados (printf).
 * Para acessar os dados, usamos a sintaxe: lista[posicao].campo.
 * No terceiro commit, foi implementada a funcionalidade de Ataque e a estrutura avançada de gerenciamento de memória do sistema War
 * No qaurto commit, foi adicionada a camada estratégica do jogo War, implementando o sistema de missões.
 */
