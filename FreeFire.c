// ============================================================================
//     CÓDIGO DA ILHA – EDIÇÃO FREE FIRE | Nível Mestre
// ============================================================================
// Gerenciamento avançado de mochila com ordenação (Insertion Sort)
// e busca binária por nome.
// ============================================================================

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// --- Constantes ---
#define MAX_ITENS   10
#define MAX_NOME    50
#define MAX_TIPO    30

// --- Enum de Critério de Ordenação ---
typedef enum {
    ORDENAR_NOME      = 1,
    ORDENAR_TIPO      = 2,
    ORDENAR_PRIORIDADE = 3
} CriterioOrdenacao;

// --- Struct Item ---
typedef struct {
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int  quantidade;
    int  prioridade; // 1 (baixa) a 5 (alta)
} Item;

// --- Variáveis Globais ---
Item mochila[MAX_ITENS];
int  numItens        = 0;
int  comparacoes     = 0;
bool ordenadaPorNome = false;

// --- Protótipos ---
void limparTela(void);
void limparBuffer(void);
void exibirMenu(void);
void inserirItem(void);
void removerItem(void);
void listarItens(void);
void menuDeOrdenacao(void);
void insertionSort(CriterioOrdenacao criterio);
void buscaBinariaPorNome(void);

// ============================================================================
// FUNÇÃO PRINCIPAL
// ============================================================================
int main(void) {
    int opcao;

    printf("==============================================\n");
    printf("  🔫 BEM-VINDO AO CÓDIGO DA ILHA – FREE FIRE\n");
    printf("        Nível: MESTRE | Gestão de Mochila\n");
    printf("==============================================\n");

    do {
        exibirMenu();
        printf("Sua escolha: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1: inserirItem();        break;
            case 2: removerItem();        break;
            case 3: listarItens();        break;
            case 4: menuDeOrdenacao();    break;
            case 5: buscaBinariaPorNome(); break;
            case 0:
                printf("\n🏁 Encerrando missão. Boa sorte na ilha!\n\n");
                break;
            default:
                printf("\n[ERRO] Opção inválida. Tente novamente.\n");
        }

        if (opcao != 0) {
            printf("\nPressione ENTER para continuar...");
            getchar();
            limparTela();
        }

    } while (opcao != 0);

    return 0;
}

// ============================================================================
// IMPLEMENTAÇÃO DAS FUNÇÕES
// ============================================================================

// Simula limpeza de tela com linhas em branco (portável).
void limparTela(void) {
    for (int i = 0; i < 40; i++) printf("\n");
}

// Limpa o buffer de entrada.
void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Exibe o menu principal com status de ordenação.
void exibirMenu(void) {
    printf("\n╔══════════════════════════════════════════╗\n");
    printf("║        🎒 MOCHILA DE SOBREVIVÊNCIA        ║\n");
    printf("╠══════════════════════════════════════════╣\n");
    printf("║  Itens: %2d/%-2d  │  Ordenada por nome: %s  ║\n",
           numItens, MAX_ITENS,
           ordenadaPorNome ? "SIM" : "NÃO");
    printf("╠══════════════════════════════════════════╣\n");
    printf("║  1. Adicionar item                       ║\n");
    printf("║  2. Remover item                         ║\n");
    printf("║  3. Listar itens                         ║\n");
    printf("║  4. Ordenar itens                        ║\n");
    printf("║  5. Busca binária por nome               ║\n");
    printf("║  0. Sair                                 ║\n");
    printf("╚══════════════════════════════════════════╝\n");
}

// Adiciona novo item à mochila.
void inserirItem(void) {
    if (numItens >= MAX_ITENS) {
        printf("\n[AVISO] Mochila cheia! Remova algum item primeiro.\n");
        return;
    }

    Item novo;

    printf("\n--- ADICIONAR ITEM ---\n");
    printf("Nome   : ");
    fgets(novo.nome, MAX_NOME, stdin);
    novo.nome[strcspn(novo.nome, "\n")] = '\0';

    printf("Tipo   : ");
    fgets(novo.tipo, MAX_TIPO, stdin);
    novo.tipo[strcspn(novo.tipo, "\n")] = '\0';

    printf("Qtd    : ");
    scanf("%d", &novo.quantidade);

    do {
        printf("Prioridade (1-5): ");
        scanf("%d", &novo.prioridade);
        limparBuffer();
        if (novo.prioridade < 1 || novo.prioridade > 5)
            printf("[ERRO] Prioridade deve ser entre 1 e 5.\n");
    } while (novo.prioridade < 1 || novo.prioridade > 5);

    mochila[numItens++] = novo;
    ordenadaPorNome = false;

    printf("\n✅ Item \"%s\" adicionado com sucesso!\n", novo.nome);
}

// Remove item da mochila pelo nome.
void removerItem(void) {
    if (numItens == 0) {
        printf("\n[AVISO] A mochila está vazia!\n");
        return;
    }

    char busca[MAX_NOME];
    printf("\n--- REMOVER ITEM ---\n");
    printf("Nome do item a remover: ");
    fgets(busca, MAX_NOME, stdin);
    busca[strcspn(busca, "\n")] = '\0';

    int pos = -1;
    for (int i = 0; i < numItens; i++) {
        if (strcmp(mochila[i].nome, busca) == 0) {
            pos = i;
            break;
        }
    }

    if (pos == -1) {
        printf("\n[ERRO] Item \"%s\" não encontrado na mochila.\n", busca);
        return;
    }

    // Reorganiza o vetor preenchendo a lacuna
    for (int i = pos; i < numItens - 1; i++) {
        mochila[i] = mochila[i + 1];
    }
    numItens--;
    ordenadaPorNome = false;

    printf("\n🗑️  Item \"%s\" removido com sucesso!\n", busca);
}

// Lista todos os itens em formato de tabela.
void listarItens(void) {
    printf("\n--- ITENS NA MOCHILA (%d/%d) ---\n", numItens, MAX_ITENS);

    if (numItens == 0) {
        printf("  [Mochila vazia]\n");
        return;
    }

    printf("\n%-3s %-20s %-15s %-6s %s\n",
           "#", "Nome", "Tipo", "Qtd", "Prioridade");
    printf("------------------------------------------------------\n");
    for (int i = 0; i < numItens; i++) {
        printf("%-3d %-20s %-15s %-6d %d/5\n",
               i + 1,
               mochila[i].nome,
               mochila[i].tipo,
               mochila[i].quantidade,
               mochila[i].prioridade);
    }
    printf("------------------------------------------------------\n");
}

// Menu para escolha do critério de ordenação.
void menuDeOrdenacao(void) {
    if (numItens == 0) {
        printf("\n[AVISO] A mochila está vazia!\n");
        return;
    }

    int opcao;
    printf("\n--- ORDENAR MOCHILA ---\n");
    printf("  1. Por nome (A-Z)\n");
    printf("  2. Por tipo (A-Z)\n");
    printf("  3. Por prioridade (maior → menor)\n");
    printf("Critério: ");
    scanf("%d", &opcao);
    limparBuffer();

    CriterioOrdenacao criterio;
    switch (opcao) {
        case 1: criterio = ORDENAR_NOME;       break;
        case 2: criterio = ORDENAR_TIPO;       break;
        case 3: criterio = ORDENAR_PRIORIDADE; break;
        default:
            printf("[ERRO] Critério inválido.\n");
            return;
    }

    comparacoes = 0;
    insertionSort(criterio);

    const char *nomes[] = {"nome", "tipo", "prioridade"};
    printf("\n✅ Mochila ordenada por %s!\n", nomes[opcao - 1]);
    printf("📊 Comparações realizadas: %d\n", comparacoes);

    listarItens();
}

// Insertion Sort com critério dinâmico de ordenação.
void insertionSort(CriterioOrdenacao criterio) {
    for (int i = 1; i < numItens; i++) {
        Item chave = mochila[i];
        int j = i - 1;

        // Determina se chave deve ir antes de mochila[j]
        while (j >= 0) {
            int cmp = 0;
            comparacoes++;

            switch (criterio) {
                case ORDENAR_NOME:
                    cmp = strcmp(mochila[j].nome, chave.nome);
                    break;
                case ORDENAR_TIPO:
                    cmp = strcmp(mochila[j].tipo, chave.tipo);
                    break;
                case ORDENAR_PRIORIDADE:
                    // Maior prioridade primeiro
                    cmp = chave.prioridade - mochila[j].prioridade;
                    break;
            }

            if (cmp > 0) {
                mochila[j + 1] = mochila[j];
                j--;
            } else {
                break;
            }
        }
        mochila[j + 1] = chave;
    }

    // Atualiza flag de ordenação por nome
    ordenadaPorNome = (criterio == ORDENAR_NOME);
}

// Busca binária por nome (exige ordenação prévia por nome).
void buscaBinariaPorNome(void) {
    if (numItens == 0) {
        printf("\n[AVISO] A mochila está vazia!\n");
        return;
    }

    if (!ordenadaPorNome) {
        printf("\n[AVISO] A mochila precisa estar ordenada por nome antes\n");
        printf("         de realizar a busca binária!\n");
        printf("         Use a opção 4 → Ordenar por nome primeiro.\n");
        return;
    }

    char busca[MAX_NOME];
    printf("\n--- BUSCA BINÁRIA POR NOME ---\n");
    printf("Nome do item: ");
    fgets(busca, MAX_NOME, stdin);
    busca[strcspn(busca, "\n")] = '\0';

    int esq = 0, dir = numItens - 1, meio;
    int resultado = -1;
    int passos = 0;

    while (esq <= dir) {
        meio = (esq + dir) / 2;
        passos++;
        int cmp = strcmp(mochila[meio].nome, busca);

        if (cmp == 0) {
            resultado = meio;
            break;
        } else if (cmp < 0) {
            esq = meio + 1;
        } else {
            dir = meio - 1;
        }
    }

    printf("\n🔍 Passos realizados na busca: %d\n", passos);

    if (resultado != -1) {
        printf("\n✅ Item encontrado!\n");
        printf("   Nome      : %s\n", mochila[resultado].nome);
        printf("   Tipo      : %s\n", mochila[resultado].tipo);
        printf("   Quantidade: %d\n", mochila[resultado].quantidade);
        printf("   Prioridade: %d/5\n", mochila[resultado].prioridade);
    } else {
        printf("\n❌ Item \"%s\" não encontrado na mochila.\n", busca);
    }
}