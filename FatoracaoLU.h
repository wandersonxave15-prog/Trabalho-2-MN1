#ifndef FATORACAOLU_H
#define FATORACAOLU_H

#include <vector>

class FatoracaoLU {
public:
    // Resolve o sistema Ax = b usando fatoração LU
    // Retorna true se a solução foi encontrada, false caso contrário

    static bool resolver(const std::vector<std::vector<double>>& A,
                        const std::vector<double>& b,
                        std::vector<double>& x);

    // Realiza a fatoração LU da matriz A
    // A = LU, onde L é triangular inferior e U é triangular superior
    // Retorna true se a fatoração foi bem-sucedida

    static bool fatorar(const std::vector<std::vector<double>>& A,
                       std::vector<std::vector<double>>& L,
                       std::vector<std::vector<double>>& U);

    // Resolve Ly = b (substituição progressiva)

    static void substituicaoProgressiva(const std::vector<std::vector<double>>& L,
                                        const std::vector<double>& b,
                                        std::vector<double>& y);

    // Resolve Ux = y (substituição regressiva)

    static void substituicaoRegressiva(const std::vector<std::vector<double>>& U,
                                       const std::vector<double>& y,
                                       std::vector<double>& x);
};

#endif

