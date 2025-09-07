#include <stdio.h>
#include <string.h>

// Constantes para o tabuleiro e navios
#define TAM_TABULEIRO 10 // Tamanho do tabuleiro (10x10)
#define TAM_NAVIO 3      // Tamanho fixo dos navios (3 posições)

// Função para inicializar o tabuleiro com água (0)
void inicializarTabuleiro(int tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO]) {
    for (int i = 0; i < TAM_TABULEIRO; i++) {
        for (int j = 0; j < TAM_TABULEIRO; j++) {
            tabuleiro[i][j] = 0; // Água
        }
    }
}

// Função para validar o posicionamento do navio
int validarPosicionamento(int linha, int coluna, int tamanho, int horizontal, int tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO]) {
    // Verifica se as coordenadas iniciais estão dentro do tabuleiro
    if (linha < 0 || linha >= TAM_TABULEIRO || coluna < 0 || coluna >= TAM_TABULEIRO) {
        return 0; // Fora dos limites
    }

    // Verifica se o navio cabe no tabuleiro
    if (horizontal) {
        if (coluna + tamanho > TAM_TABULEIRO) {
            return 0; // Navio horizontal ultrapassa o limite
        }
        // Verifica sobreposição
        for (int j = coluna; j < coluna + tamanho; j++) {
            if (tabuleiro[linha][j] != 0) {
                return 0; // Sobreposição detectada
            }
        }
    } else {
        if (linha + tamanho > TAM_TABULEIRO) {
            return 0; // Navio vertical ultrapassa o limite
        }
        // Verifica sobreposição
        for (int i = linha; i < linha + tamanho; i++) {
            if (tabuleiro[i][coluna] != 0) {
                return 0; // Sobreposição detectada
            }
        }
    }
    return 1; // Posicionamento válido
}

// Função para posicionar um navio no tabuleiro
void posicionarNavio(int tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO], int linha, int coluna, int tamanho, int horizontal) {
    if (horizontal) {
        // Posiciona navio horizontalmente
        for (int j = coluna; j < coluna + tamanho; j++) {
            tabuleiro[linha][j] = 3; // Marca navio
        }
    } else {
        // Posiciona navio verticalmente
        for (int i = linha; i < linha + tamanho; i++) {
            tabuleiro[i][coluna] = 3; // Marca navio
        }
    }
}

// Função para exibir o tabuleiro
void exibirTabuleiro(int tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO]) {
    printf("\n=== Tabuleiro de Batalha Naval ===\n");
    // Imprime cabeçalho com índices de colunas
    printf("   ");
    for (int j = 0; j < TAM_TABULEIRO; j++) {
        printf("%2d ", j);
    }
    printf("\n");
    
    // Imprime linhas do tabuleiro
    for (int i = 0; i < TAM_TABULEIRO; i++) {
        printf("%2d ", i); // Índice da linha
        for (int j = 0; j < TAM_TABULEIRO; j++) {
            if (tabuleiro[i][j] == 0) {
                printf(" . "); // Água
            } else if (tabuleiro[i][j] == 3) {
                printf(" N "); // Navio
            }
        }
        printf("\n");
    }
}

int main() {
    // Declaração do tabuleiro
    int tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO];

    // Inicializa o tabuleiro com água
    inicializarTabuleiro(tabuleiro);

    // Coordenadas iniciais dos navios (definidas diretamente no código)
    int navio1_linha = 2, navio1_coluna = 3; // Navio horizontal
    int navio2_linha = 5, navio2_coluna = 7; // Navio vertical

    // Valida posicionamento do navio horizontal
    if (!validarPosicionamento(navio1_linha, navio1_coluna, TAM_NAVIO, 1, tabuleiro)) {
        printf("Erro: Posicionamento do navio horizontal inválido!\n");
        return 1;
    }

    // Posiciona o navio horizontal
    posicionarNavio(tabuleiro, navio1_linha, navio1_coluna, TAM_NAVIO, 1);

    // Valida posicionamento do navio vertical
    if (!validarPosicionamento(navio2_linha, navio2_coluna, TAM_NAVIO, 0, tabuleiro)) {
        printf("Erro: Posicionamento do navio vertical inválido!\n");
        return 1;
    }

    // Posiciona o navio vertical
    posicionarNavio(tabuleiro, navio2_linha, navio2_coluna, TAM_NAVIO, 0);

    // Exibe o tabuleiro com os navios posicionados
    exibirTabuleiro(tabuleiro);

    return 0;
}