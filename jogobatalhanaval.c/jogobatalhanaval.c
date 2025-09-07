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
int validarPosicionamento(int linha, int coluna, int tamanho, int orientacao, int tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO]) {
    // orientacao: 0 = vertical, 1 = horizontal, 2 = diagonal superior direita, 3 = diagonal inferior direita
    // Verifica se as coordenadas iniciais estão dentro do tabuleiro
    if (linha < 0 || linha >= TAM_TABULEIRO || coluna < 0 || coluna >= TAM_TABULEIRO) {
        return 0; // Fora dos limites
    }

    // Verifica limites e sobreposição com base na orientação
    if (orientacao == 1) { // Horizontal
        if (coluna + tamanho > TAM_TABULEIRO) {
            return 0; // Ultrapassa limite
        }
        for (int j = coluna; j < coluna + tamanho; j++) {
            if (tabuleiro[linha][j] != 0) {
                return 0; // Sobreposição
            }
        }
    } else if (orientacao == 0) { // Vertical
        if (linha + tamanho > TAM_TABULEIRO) {
            return 0; // Ultrapassa limite
        }
        for (int i = linha; i < linha + tamanho; i++) {
            if (tabuleiro[i][coluna] != 0) {
                return 0; // Sobreposição
            }
        }
    } else if (orientacao == 2) { // Diagonal superior direita (linha--, coluna++)
        if (linha - tamanho + 1 < 0 || coluna + tamanho > TAM_TABULEIRO) {
            return 0; // Ultrapassa limite
        }
        for (int i = 0; i < tamanho; i++) {
            if (tabuleiro[linha - i][coluna + i] != 0) {
                return 0; // Sobreposição
            }
        }
    } else if (orientacao == 3) { // Diagonal inferior direita (linha++, coluna++)
        if (linha + tamanho > TAM_TABULEIRO || coluna + tamanho > TAM_TABULEIRO) {
            return 0; // Ultrapassa limite
        }
        for (int i = 0; i < tamanho; i++) {
            if (tabuleiro[linha + i][coluna + i] != 0) {
                return 0; // Sobreposição
            }
        }
    }
    return 1; // Posicionamento válido
}

// Função para posicionar um navio no tabuleiro
void posicionarNavio(int tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO], int linha, int coluna, int tamanho, int orientacao) {
    if (orientacao == 1) { // Horizontal
        for (int j = coluna; j < coluna + tamanho; j++) {
            tabuleiro[linha][j] = 3; // Marca navio
        }
    } else if (orientacao == 0) { // Vertical
        for (int i = linha; i < linha + tamanho; i++) {
            tabuleiro[i][coluna] = 3; // Marca navio
        }
    } else if (orientacao == 2) { // Diagonal superior direita
        for (int i = 0; i < tamanho; i++) {
            tabuleiro[linha - i][coluna + i] = 3; // Marca navio
        }
    } else if (orientacao == 3) { // Diagonal inferior direita
        for (int i = 0; i < tamanho; i++) {
            tabuleiro[linha + i][coluna + i] = 3; // Marca navio
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

    // Coordenadas e orientações dos navios (ajustadas para corresponder à saída fornecida)
    // Navio 1: Diagonal superior direita em (7,0)
    int navio1_linha = 7, navio1_coluna = 0, navio1_orientacao = 2;
    // Navio 2: Diagonal inferior direita em (0,0)
    int navio2_linha = 0, navio2_coluna = 0, navio2_orientacao = 3;
    // Navio 3: Horizontal em (2,3) com tamanho 3
    int navio3_linha = 2, navio3_coluna = 3, navio3_orientacao = 1;
    // Navio 4: Vertical em (5,7)
    int navio4_linha = 5, navio4_coluna = 7, navio4_orientacao = 0;

    // Array para armazenar informações dos navios
    int navios[4][3] = {
        {navio1_linha, navio1_coluna, navio1_orientacao}, // Diagonal superior direita
        {navio2_linha, navio2_coluna, navio2_orientacao}, // Diagonal inferior direita
        {navio3_linha, navio3_coluna, navio3_orientacao}, // Horizontal
        {navio4_linha, navio4_coluna, navio4_orientacao}  // Vertical
    };

    // Posiciona os navios e valida
    for (int i = 0; i < 4; i++) {
        int linha = navios[i][0];
        int coluna = navios[i][1];
        int orientacao = navios[i][2];

        // Valida posicionamento do navio
        if (!validarPosicionamento(linha, coluna, TAM_NAVIO, orientacao, tabuleiro)) {
            printf("Erro: Posicionamento do navio %d inválido!\n", i + 1);
            return 1;
        }

        // Posiciona o navio
        posicionarNavio(tabuleiro, linha, coluna, TAM_NAVIO, orientacao);
    }

    // Exibe o tabuleiro com os navios posicionados
    exibirTabuleiro(tabuleiro);

    return 0;
}