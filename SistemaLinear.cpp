#include "SistemaLinear.h"
#include "FatoracaoLU.h"
#include "FatoracaoLDP.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>

// Matriz padrão para calibração
const std::vector<std::vector<double>> SistemaLinear::matrizPadrao = {
    {3, -2, 1},
    {1, -3, 4},
    {9, 4, -5}
};

// Vetor padrão para calibração
const std::vector<double> SistemaLinear::vetorPadrao = {8, 6, 11};

SistemaLinear::SistemaLinear() {
    n = 0;
    matrizA.clear();
    vetorF.clear();
    resultadoLU.sucesso = false;
    resultadoLDP.sucesso = false;
}

//  Funções visuais

void SistemaLinear::imprimirMatrizColorida(const std::string& nome, const std::vector<std::vector<double>>& matriz) {
    if (matriz.empty()) return;
    std::cout << "\n> Matriz Resultante \033[1;34m" << nome << "\033[0m:" << std::endl;
    for (int i = 0; i < n; i++) {
        std::cout << "[ ";
        for (int j = 0; j < n; j++) {
            if (i == j) std::cout << "\033[1;33m"; // Amarelo na diagonal
            std::cout << std::setw(8) << std::fixed << std::setprecision(4) << matriz[i][j];
            if (i == j) std::cout << "\033[0m";
        }
        std::cout << " ]" << std::endl;
    }
}

void SistemaLinear::imprimirMatrizFinal(const std::vector<double>& d) {
    std::cout << "\n> Matriz Resultante Final (Sistema Solucionado [I]{d} = {d_final}):" << std::endl;
    for (int i = 0; i < n; i++) {
        std::cout << "[ ";
        for (int j = 0; j < n; j++) {
            double val = (i == j) ? 1.0 : 0.0;
            if (i == j) std::cout << "\033[1;32m"; // Verde na identidade
            std::cout << std::setw(8) << std::fixed << std::setprecision(1) << val;
            if (i == j) std::cout << "\033[0m";
        }
        std::cout << " ]   [ d" << (i + 1) << " ]   =   [ "
                  << "\033[1;36m" // Ciano no resultado
                  << std::setw(10) << std::fixed << std::setprecision(6) << d[i]
                  << "\033[0m" << " ]" << std::endl;
    }
}

// Configuração do sistema

void SistemaLinear::setDimensao(int dimensao) {
    n = dimensao;
    matrizA.assign(n, std::vector<double>(n, 0.0));
    vetorF.assign(n, 0.0);
}

void SistemaLinear::setMatriz(const std::vector<std::vector<double>>& A) {
    matrizA = A;
    n = A.size();
}

void SistemaLinear::setVetor(const std::vector<double>& f) {
    vetorF = f;
}

void SistemaLinear::calibrarSistema() {
    matrizA = matrizPadrao;
    vetorF = vetorPadrao;
    n = 3;
    executarCalculos();
}

void SistemaLinear::calcularDeslocamentos() {
    // LU
    resultadoLU.deslocamentos.clear();
    resultadoLU.y.clear();
    resultadoLU.sucesso = FatoracaoLU::fatorar(matrizA, resultadoLU.L, resultadoLU.U);

    if (!resultadoLU.sucesso) {
        resultadoLU.mensagemErro = "Erro na fatoração LU: matriz singular ou quase singular";
    } else {
        FatoracaoLU::substituicaoProgressiva(resultadoLU.L, vetorF, resultadoLU.y);
        FatoracaoLU::substituicaoRegressiva(resultadoLU.U, resultadoLU.y, resultadoLU.deslocamentos);
        resultadoLU.mensagemErro = "";
    }

    // LDP
    resultadoLDP.deslocamentos.clear();
    resultadoLDP.y.clear();
    resultadoLDP.sucesso = FatoracaoLDP::fatorar(matrizA, resultadoLDP.L, resultadoLDP.D, resultadoLDP.P);

    if (!resultadoLDP.sucesso) {
        resultadoLDP.mensagemErro = "Erro na fatoração LDP: matriz singular ou quase singular";
    } else {
        FatoracaoLDP::substituicaoProgressiva(resultadoLDP.L, vetorF, resultadoLDP.y);
        FatoracaoLDP::resolverDP(resultadoLDP.D, resultadoLDP.P, resultadoLDP.y, resultadoLDP.deslocamentos);
        resultadoLDP.mensagemErro = "";
    }
}

void SistemaLinear::executarCalculos() {
    calcularDeslocamentos();
}

// Entrada de dados

void SistemaLinear::lerMatriz() {
    std::cout << "Digite a dimensao da matriz (n): ";
    std::cin >> n;

    if (n <= 0) {
        std::cout << "Dimensao invalida!" << std::endl;
        return;
    }

    matrizA.assign(n, std::vector<double>(n, 0.0));

    std::cout << "\nDigite os elementos da matriz A (" << n << "x" << n << "):" << std::endl;
    std::cout << "(Digite linha por linha, separando os valores por espaco)" << std::endl;

    for (int i = 0; i < n; i++) {
        std::cout << "Linha " << (i + 1) << ": ";
        for (int j = 0; j < n; j++) {
            std::cin >> matrizA[i][j];
        }
    }
}

void SistemaLinear::lerVetor() {
    if (n == 0) {
        std::cout << "Defina a matriz primeiro!" << std::endl;
        return;
    }

    vetorF.assign(n, 0.0);

    std::cout << "\nDigite os elementos do vetor f (" << n << " elementos):" << std::endl;
    for (int i = 0; i < n; i++) {
        std::cout << "f[" << (i + 1) << "] = ";
        std::cin >> vetorF[i];
    }
}

// Quadro de resposta

void SistemaLinear::exibirQuadroRespostaLU() {
    std::cout << "=== QUADRO RESPOSTA - METODO DE FATORACAO LU ===" << std::endl;
    std::cout << std::endl;

    std::cout << "Matriz A:" << std::endl;
    for (int i = 0; i < n; i++) {
        std::cout << "[ ";
        for (int j = 0; j < n; j++) {
            std::cout << std::setw(8) << std::fixed << std::setprecision(4) << matrizA[i][j];
        }
        std::cout << " ]" << std::endl;
    }

    std::cout << "\nVetor f:" << std::endl;
    std::cout << "[ ";
    for (int i = 0; i < n; i++) {
        std::cout << std::setw(8) << std::fixed << std::setprecision(4) << vetorF[i];
    }
    std::cout << " ]" << std::endl;

    std::cout << "\n" << std::string(60, '-') << std::endl;

    if (resultadoLU.sucesso) {
        // Visual
        imprimirMatrizColorida("L (Triangular Inferior)", resultadoLU.L);
        imprimirMatrizColorida("U (Triangular Superior)", resultadoLU.U);
        imprimirMatrizFinal(resultadoLU.deslocamentos);
        std::cout << "\n" << std::string(60, '-') << std::endl;


        std::cout << "\nVetor de deslocamentos d (em cm):" << std::endl;
        std::cout << "[ ";
        for (int i = 0; i < n; i++) {
            std::cout << std::setw(10) << std::fixed << std::setprecision(6)
                      << resultadoLU.deslocamentos[i];
        }
        std::cout << " ]" << std::endl;

        std::cout << "\nDeslocamentos individuais:" << std::endl;
        for (int i = 0; i < n; i++) {
            std::cout << "d" << (i + 1) << " = " << std::fixed << std::setprecision(6)
                      << resultadoLU.deslocamentos[i] << " cm" << std::endl;
        }

        double maxDesloc = encontrarMaximoDeslocamento(resultadoLU.deslocamentos);
        std::cout << "\nDeslocamento maximo (em modulo): " << std::fixed
                  << std::setprecision(6) << maxDesloc << " cm" << std::endl;

        if (verificarSeguranca(resultadoLU.deslocamentos)) {
            std::cout << "\n\033[1;32m[SEGURO]\033[0m Nenhum deslocamento excede 2 cm." << std::endl;
        } else {
            std::cout << "\n\033[1;31m[PERIGO]\033[0m O jato pode explodir! Deslocamento maximo > 2 cm." << std::endl;
        }
    } else {
        std::cout << "\nERRO: " << resultadoLU.mensagemErro << std::endl;
    }
}

void SistemaLinear::exibirQuadroRespostaLDP() {
    std::cout << "=== QUADRO RESPOSTA - METODO DE FATORACAO LDP ===" << std::endl;
    std::cout << std::endl;

    std::cout << "Matriz A:" << std::endl;
    for (int i = 0; i < n; i++) {
        std::cout << "[ ";
        for (int j = 0; j < n; j++) {
            std::cout << std::setw(8) << std::fixed << std::setprecision(4) << matrizA[i][j];
        }
        std::cout << " ]" << std::endl;
    }

    std::cout << "\nVetor f:" << std::endl;
    std::cout << "[ ";
    for (int i = 0; i < n; i++) {
        std::cout << std::setw(8) << std::fixed << std::setprecision(4) << vetorF[i];
    }
    std::cout << " ]" << std::endl;

    std::cout << "\n" << std::string(60, '-') << std::endl;

    if (resultadoLDP.sucesso) {
        // Visual
        imprimirMatrizColorida("L (Triangular Inferior)", resultadoLDP.L);
        imprimirMatrizColorida("D (Diagonal)", resultadoLDP.D);
        imprimirMatrizColorida("P (Normalizada)", resultadoLDP.P);
        imprimirMatrizFinal(resultadoLDP.deslocamentos);
        std::cout << "\n" << std::string(60, '-') << std::endl;


        std::cout << "\nVetor de deslocamentos d (em cm):" << std::endl;
        std::cout << "[ ";
        for (int i = 0; i < n; i++) {
            std::cout << std::setw(10) << std::fixed << std::setprecision(6)
                      << resultadoLDP.deslocamentos[i];
        }
        std::cout << " ]" << std::endl;

        std::cout << "\nDeslocamentos individuais:" << std::endl;
        for (int i = 0; i < n; i++) {
            std::cout << "d" << (i + 1) << " = " << std::fixed << std::setprecision(6)
                      << resultadoLDP.deslocamentos[i] << " cm" << std::endl;
        }

        double maxDesloc = encontrarMaximoDeslocamento(resultadoLDP.deslocamentos);
        std::cout << "\nDeslocamento maximo (em modulo): " << std::fixed
                  << std::setprecision(6) << maxDesloc << " cm" << std::endl;

        if (verificarSeguranca(resultadoLDP.deslocamentos)) {
            std::cout << "\n\033[1;32m[SEGURO]\033[0m Nenhum deslocamento excede 2 cm." << std::endl;
        } else {
            std::cout << "\n\033[1;31m[PERIGO]\033[0m O jato pode explodir! Deslocamento maximo > 2 cm." << std::endl;
        }
    } else {
        std::cout << "\nERRO: " << resultadoLDP.mensagemErro << std::endl;
    }
}

void SistemaLinear::exibirQuadroComparativo() {
    std::cout << "=== QUADRO COMPARATIVO - LU vs LDP ===" << std::endl;
    std::cout << std::endl;

    std::cout << std::setw(5) << "d" << std::setw(20) << "Metodo LU (cm)"
              << std::setw(20) << "Metodo LDP (cm)" << std::setw(20) << "Diferenca" << std::endl;
    std::cout << std::string(65, '-') << std::endl;

    if (resultadoLU.sucesso && resultadoLDP.sucesso) {
        for (int i = 0; i < n; i++) {
            double diff = std::abs(resultadoLU.deslocamentos[i] - resultadoLDP.deslocamentos[i]);
            std::cout << std::setw(5) << (i + 1)
                      << std::setw(20) << std::fixed << std::setprecision(6)
                      << resultadoLU.deslocamentos[i]
                      << std::setw(20) << std::fixed << std::setprecision(6)
                      << resultadoLDP.deslocamentos[i]
                      << std::setw(20) << std::fixed << std::setprecision(6)
                      << diff << std::endl;
        }

        std::cout << std::endl;
        double maxLU = encontrarMaximoDeslocamento(resultadoLU.deslocamentos);
        double maxLDP = encontrarMaximoDeslocamento(resultadoLDP.deslocamentos);

        std::cout << "Maximo (modulo) - LU:  " << std::fixed << std::setprecision(6)
                  << maxLU << " cm" << std::endl;
        std::cout << "Maximo (modulo) - LDP: " << std::fixed << std::setprecision(6)
                  << maxLDP << " cm" << std::endl;

        std::cout << "\nStatus de Seguranca:" << std::endl;
        bool seguroLU = verificarSeguranca(resultadoLU.deslocamentos);
        bool seguroLDP = verificarSeguranca(resultadoLDP.deslocamentos);

        std::cout << "LU:  " << (seguroLU ? "\033[1;32m[SEGURO]\033[0m" : "\033[1;31m[PERIGO]\033[0m") << std::endl;
        std::cout << "LDP: " << (seguroLDP ? "\033[1;32m[SEGURO]\033[0m" : "\033[1;31m[PERIGO]\033[0m") << std::endl;
    } else {
        std::cout << "Erro nos calculos:" << std::endl;
        if (!resultadoLU.sucesso) {
            std::cout << "LU: " << resultadoLU.mensagemErro << std::endl;
        }
        if (!resultadoLDP.sucesso) {
            std::cout << "LDP: " << resultadoLDP.mensagemErro << std::endl;
        }
    }
}

// Análise detalhada

void SistemaLinear::exibirAnaliseSeguranca() {
    std::cout << "=== ANALISE DE SEGURANCA DO JATO ===" << std::endl;
    std::cout << std::endl;

    std::cout << "Limite de seguranca: 2.0 cm (em modulo)" << std::endl;
    std::cout << std::endl;

    if (resultadoLU.sucesso) {
        std::cout << "--- Resultados do Metodo LU ---" << std::endl;
        double maxDesloc = encontrarMaximoDeslocamento(resultadoLU.deslocamentos);

        std::cout << "\nDeslocamentos calculados:" << std::endl;
        for (int i = 0; i < n; i++) {
            double modulo = std::abs(resultadoLU.deslocamentos[i]);
            std::cout << "d" << (i + 1) << " = " << std::fixed << std::setprecision(6)
                      << resultadoLU.deslocamentos[i] << " cm"
                      << " (|d" << (i + 1) << "| = " << modulo << " cm)";

            if (modulo > 2.0) {
                std::cout << " \033[1;31m[PERIGO!]\033[0m" << std::endl;
            } else {
                std::cout << " \033[1;32m[OK]\033[0m" << std::endl;
            }
        }

        std::cout << "\nDeslocamento maximo: " << std::fixed << std::setprecision(6)
                  << maxDesloc << " cm" << std::endl;

        if (verificarSeguranca(resultadoLU.deslocamentos)) {
            std::cout << "\nCONCLUSAO: O jato esta \033[1;32mSEGURO\033[0m. Nenhum deslocamento excede 2 cm." << std::endl;
        } else {
            std::cout << "\nCONCLUSAO: O jato CORRE \033[1;31mRISCO DE EXPLOSAO!\033[0m" << std::endl;
            std::cout << "Pelo menos um deslocamento excede 2 cm em modulo." << std::endl;
        }
    } else {
        std::cout << "Erro no calculo LU: " << resultadoLU.mensagemErro << std::endl;
    }

    std::cout << "\n" << std::string(60, '-') << std::endl;

    if (resultadoLDP.sucesso) {
        std::cout << "\n--- Resultados do Metodo LDP ---" << std::endl;
        double maxDesloc = encontrarMaximoDeslocamento(resultadoLDP.deslocamentos);

        std::cout << "\nDeslocamentos calculados:" << std::endl;
        for (int i = 0; i < n; i++) {
            double modulo = std::abs(resultadoLDP.deslocamentos[i]);
            std::cout << "d" << (i + 1) << " = " << std::fixed << std::setprecision(6)
                      << resultadoLDP.deslocamentos[i] << " cm"
                      << " (|d" << (i + 1) << "| = " << modulo << " cm)";

            if (modulo > 2.0) {
                std::cout << " \033[1;31m[PERIGO!]\033[0m" << std::endl;
            } else {
                std::cout << " \033[1;32m[OK]\033[0m" << std::endl;
            }
        }

        std::cout << "\nDeslocamento maximo: " << std::fixed << std::setprecision(6)
                  << maxDesloc << " cm" << std::endl;

        if (verificarSeguranca(resultadoLDP.deslocamentos)) {
            std::cout << "\nCONCLUSAO: O jato esta \033[1;32mSEGURO\033[0m. Nenhum deslocamento excede 2 cm." << std::endl;
        } else {
            std::cout << "\nCONCLUSAO: O jato CORRE \033[1;31mRISCO DE EXPLOSAO!\033[0m" << std::endl;
            std::cout << "Pelo menos um deslocamento excede 2 cm em modulo." << std::endl;
        }
    } else {
        std::cout << "Erro no calculo LDP: " << resultadoLDP.mensagemErro << std::endl;
    }
}

// Funções de validação

bool SistemaLinear::verificarSeguranca(const std::vector<double>& deslocamentos) const {
    for (double d : deslocamentos) {
        if (std::abs(d) > 2.0) {
            return false;
        }
    }
    return true;
}

double SistemaLinear::encontrarMaximoDeslocamento(const std::vector<double>& deslocamentos) const {
    double max = 0.0;
    for (double d : deslocamentos) {
        double modulo = std::abs(d);
        if (modulo > max) {
            max = modulo;
        }
    }
    return max;
}
