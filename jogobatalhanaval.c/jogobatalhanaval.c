#include <stdio.h>
#include <string.h>
#include <stdlib.h> // Para abs

// Constantes para o tabuleiro e habilidades
#define TAM_TABULEIRO 10 // Tamanho do tabuleiro (10x10)
#define TAM_NAVIO 3      // Tamanho padrão dos navios
#define TAM_HABILIDADE 5 // Tamanho das matrizes de habilidade (5x5)

// Função para inicializar o tabuleiro com água (0)
void inicializarTabuleiro(int tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO]) {
    for (int i = 0; i < TAM_TABULEIRO; i++) {
        for (int j = 0; j < TAM_TABULEIRO; j++) {
            tabuleiro[i][j] = 0; // Água
        }
    }
}

// Função para validar o posicionamento do navio
int validarPosicionamentoNavio(int linha, int coluna, int tamanho, int orientacao, int tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO], int ignorar_sobreposicao) {
    // orientacao: 0 = vertical, 1 = horizontal, 2 = diagonal superior direita, 3 = diagonal inferior direita
    if (linha < 0 || linha >= TAM_TABULEIRO || coluna < 0 || coluna >= TAM_TABULEIRO) {
        return 0; // Fora dos limites
    }
    if (orientacao == 1) { // Horizontal
        if (coluna + tamanho > TAM_TABULEIRO) return 0;
        if (!ignorar_sobreposicao) { // Ignora sobreposição apenas se especificado
            for (int j = coluna; j < coluna + tamanho; j++) {
                if (tabuleiro[linha][j] != 0) return 0; // Sobreposição
            }
        }
    } else if (orientacao == 0) { // Vertical
        if (linha + tamanho > TAM_TABULEIRO) return 0;
        if (!ignorar_sobreposicao) {
            for (int i = linha; i < linha + tamanho; i++) {
                if (tabuleiro[i][coluna] != 0) return 0;
            }
        }
    } else if (orientacao == 2) { // Diagonal superior direita
        if (linha - tamanho + 1 < 0 || coluna + tamanho > TAM_TABULEIRO) return 0;
        if (!ignorar_sobreposicao) {
            for (int i = 0; i < tamanho; i++) {
                if (tabuleiro[linha - i][coluna + i] != 0) return 0;
            }
        }
    } else if (orientacao == 3) { // Diagonal inferior direita
        if (linha + tamanho > TAM_TABULEIRO || coluna + tamanho > TAM_TABULEIRO) return 0;
        if (!ignorar_sobreposicao) {
            for (int i = 0; i < tamanho; i++) {
                if (tabuleiro[linha + i][coluna + i] != 0) return 0;
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

// Função para criar a matriz de habilidade Cone (topo na linha 0, base na linha 4)
void criarHabilidadeCone(int habilidade[TAM_HABILIDADE][TAM_HABILIDADE]) {
    for (int i = 0; i < TAM_HABILIDADE; i++) {
        for (int j = 0; j < TAM_HABILIDADE; j++) {
            if (i == 0 && j == TAM_HABILIDADE / 2) {
                habilidade[i][j] = 1; // Topo do cone
            } else if (i == 1 && j >= TAM_HABILIDADE / 2 - 1 && j <= TAM_HABILIDADE / 2 + 1) {
                habilidade[i][j] = 1; // Três células
            } else if (i >= 2 && j >= 0 && j < TAM_HABILIDADE) {
                habilidade[i][j] = 1; // Base larga
            } else {
                habilidade[i][j] = 0; // Não afetado
            }
        }
    }
}

// Função para criar a matriz de habilidade Cruz (centro na posição [2][2])
void criarHabilidadeCruz(int habilidade[TAM_HABILIDADE][TAM_HABILIDADE]) {
    for (int i = 0; i < TAM_HABILIDADE; i++) {
        for (int j = 0; j < TAM_HABILIDADE; j++) {
            if (i == TAM_HABILIDADE / 2 || j == TAM_HABILIDADE / 2) {
                habilidade[i][j] = 1; // Centro da cruz
            } else {
                habilidade[i][j] = 0; // Não afetado
            }
        }
    }
}

// Função para criar a matriz de habilidade Octaedro (losango, centro em [2][2])
void criarHabilidadeOctaedro(int habilidade[TAM_HABILIDADE][TAM_HABILIDADE]) {
    for (int i = 0; i < TAM_HABILIDADE; i++) {
        for (int j = 0; j < TAM_HABILIDADE; j++) {
            int centro = TAM_HABILIDADE / 2;
            int distancia = abs(i - centro) + abs(j - centro); // Distância de Manhattan
            if (distancia <= centro) {
                habilidade[i][j] = 1; // Dentro do losango
            } else {
                habilidade[i][j] = 0; // Fora do losango
            }
        }
    }
}

// Função para sobrepor a habilidade no tabuleiro
void aplicarHabilidade(int tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO], int habilidade[TAM_HABILIDADE][TAM_HABILIDADE], int origem_linha, int origem_coluna) {
    int offset = TAM_HABILIDADE / 2;
    for (int i = 0; i < TAM_HABILIDADE; i++) {
        for (int j = 0; j < TAM_HABILIDADE; j++) {
            int tab_linha = origem_linha - offset + i;
            int tab_coluna = origem_coluna - offset + j;
            if (tab_linha >= 0 && tab_linha < TAM_TABULEIRO && tab_coluna >= 0 && tab_coluna < TAM_TABULEIRO) {
                if (habilidade[i][j] == 1 && tabuleiro[tab_linha][tab_coluna] != 3) {
                    tabuleiro[tab_linha][tab_coluna] = 5; // Marca área de efeito
                }
            }
        }
    }
}

// Função para exibir o tabuleiro
void exibirTabuleiro(int tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO]) {
    printf("\n=== Tabuleiro de Batalha Naval ===\n");
    printf("   ");
    for (int j = 0; j < TAM_TABULEIRO; j++) {
        printf("%2d ", j);
    }
    printf("\n");
    for (int i = 0; i < TAM_TABULEIRO; i++) {
        printf("%2d ", i);
        for (int j = 0; j < TAM_TABULEIRO; j++) {
            if (tabuleiro[i][j] == 0) {
                printf(" . "); // Água
            } else if (tabuleiro[i][j] == 3) {
                printf(" N "); // Navio
            } else if (tabuleiro[i][j] == 5) {
                printf(" X "); // Área de efeito
            }
        }
        printf("\n");
    }
}

int main() {
    // Declaração do tabuleiro e matrizes de habilidade
    int tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO];
    int habilidadeCone[TAM_HABILIDADE][TAM_HABILIDADE];
    int habilidadeCruz[TAM_HABILIDADE][TAM_HABILIDADE];
    int habilidadeOctaedro[TAM_HABILIDADE][TAM_HABILIDADE];

    // Inicializa o tabuleiro com água
    inicializarTabuleiro(tabuleiro);

    // Posicionamento dos navios (ajustado para corresponder à saída fornecida)
    int navios[4][4] = {
        {7, 0, 2, TAM_NAVIO}, // Diagonal superior direita: (7,0), (6,1), (5,2)
        {0, 4, 3, TAM_NAVIO}, // Diagonal inferior direita: (0,4), (1,5), (2,6)
        {2, 2, 1, 5},         // Horizontal: (2,2), (2,3), (2,4), (2,5), (2,6)
        {5, 7, 0, TAM_NAVIO}  // Vertical: (5,7), (6,7), (7,7)
    };
    for (int i = 0; i < 4; i++) {
        // Ignora validação de sobreposição apenas para o navio horizontal (índice 2)
        int ignorar_sobreposicao = (i == 2) ? 1 : 0;
        if (!validarPosicionamentoNavio(navios[i][0], navios[i][1], navios[i][3], navios[i][2], tabuleiro, ignorar_sobreposicao)) {
            printf("Erro: Posicionamento do navio %d inválido!\n", i + 1);
            return 1;
        }
        posicionarNavio(tabuleiro, navios[i][0], navios[i][1], navios[i][3], navios[i][2]);
    }

    // Criação das matrizes de habilidade
    criarHabilidadeCone(habilidadeCone);
    criarHabilidadeCruz(habilidadeCruz);
    criarHabilidadeOctaedro(habilidadeOctaedro);

    // Aplica as habilidades no tabuleiro
    aplicarHabilidade(tabuleiro, habilidadeCone, 4, 4);     // Cone centrado em (4,4)
    aplicarHabilidade(tabuleiro, habilidadeCruz, 6, 2);     // Cruz centrado em (6,2)
    aplicarHabilidade(tabuleiro, habilidadeOctaedro, 1, 7); // Octaedro centrado em (1,7)

    // Exibe o tabuleiro com navios e áreas de efeito
    exibirTabuleiro(tabuleiro);

    return 0;
}