#ifndef SISTEMALINEAR_H
#define SISTEMALINEAR_H

#include "FatoracaoLU.h"
#include "FatoracaoLDP.h"
#include <vector>
#include <string>

// Estrutura para armazenar resultados, vetores intermediários e matrizes de transição
struct ResultadoMetodo {
    std::vector<double> deslocamentos;
    std::vector<double> y;
    std::vector<std::vector<double>> L, U, D, P;
    bool sucesso;
    std::string mensagemErro;
};

class SistemaLinear {
private:
    std::vector<std::vector<double>> matrizA;
    std::vector<double> vetorF;
    int n;

    // Constantes para o teste de calibração
    static const std::vector<std::vector<double>> matrizPadrao;
    static const std::vector<double> vetorPadrao;

    // Resultados detalhados de cada método
    ResultadoMetodo resultadoLU;
    ResultadoMetodo resultadoLDP;

    // Lógica interna de processamento
    void calcularDeslocamentos();
    bool verificarSeguranca(const std::vector<double>& deslocamentos) const;
    double encontrarMaximoDeslocamento(const std::vector<double>& deslocamentos) const;

    // Funções auxiliares de exibição para os quadros de resposta
    void imprimirMatrizColorida(const std::string& nome, const std::vector<std::vector<double>>& matriz);
    void imprimirMatrizFinal(const std::vector<double>& d);

public:
    SistemaLinear();

    // Configuração do sistema
    void setMatriz(const std::vector<std::vector<double>>& A);
    void setVetor(const std::vector<double>& f);
    void setDimensao(int dimensao);

    // Execução
    void calibrarSistema();
    void executarCalculos();

    // Exibição de Quadros (Menus 3, 4, 5 e 6)
    void exibirQuadroRespostaLU();
    void exibirQuadroRespostaLDP();
    void exibirQuadroComparativo();
    void exibirAnaliseSeguranca();

    // Entrada de dados via console
    void lerMatriz();
    void lerVetor();

    // Getters para consulta externa, se necessário
    int getDimensao() const { return n; }
    std::vector<std::vector<double>> getMatriz() const { return matrizA; }
    std::vector<double> getVetor() const { return vetorF; }
    ResultadoMetodo getResultadoLU() const { return resultadoLU; }
    ResultadoMetodo getResultadoLDP() const { return resultadoLDP; }
};

#endif
