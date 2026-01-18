#ifndef FATORACAOLDP_H
#define FATORACAOLDP_H

#include <vector>

class FatoracaoLDP {
public:
    // Resolve o sistema Ax = b usando fatoração LDP
    // A = LDP, onde L é triangular inferior, D é diagonal, P é triangular superior com diagonal unitária
    // Retorna true se a solução foi encontrada, false caso contrário

    static bool resolver(const std::vector<std::vector<double>>& A,
                        const std::vector<double>& b,
                        std::vector<double>& x);

    // Realiza a fatoração LDP da matriz A
    // Retorna true se a fatoração foi bem-sucedida

    static bool fatorar(const std::vector<std::vector<double>>& A,
                       std::vector<std::vector<double>>& L,
                       std::vector<std::vector<double>>& D,
                       std::vector<std::vector<double>>& P);

    // Resolve Ly = b (substituição progressiva)

    static void substituicaoProgressiva(const std::vector<std::vector<double>>& L,
                                        const std::vector<double>& b,
                                        std::vector<double>& y);

    // Resolve DPx = y (primeiro D, depois P)

    static void resolverDP(const std::vector<std::vector<double>>& D,
                           const std::vector<std::vector<double>>& P,
                           const std::vector<double>& y,
                           std::vector<double>& x);
};

#endif

