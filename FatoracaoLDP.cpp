#include "FatoracaoLDP.h"
#include <cmath>
#include <algorithm>

bool FatoracaoLDP::fatorar(const std::vector<std::vector<double>>& A,
                           std::vector<std::vector<double>>& L,
                           std::vector<std::vector<double>>& D,
                           std::vector<std::vector<double>>& P) {
    int n = A.size();

    // Inicializar L, D e P
    L.assign(n, std::vector<double>(n, 0.0));
    D.assign(n, std::vector<double>(n, 0.0));
    P.assign(n, std::vector<double>(n, 0.0));

    // Inicializar P como identidade (diagonal unitária)
    for (int i = 0; i < n; i++) {
        P[i][i] = 1.0;
    }

    // Primeiro, fazer fatoração LU tradicional
    std::vector<std::vector<double>> U(n, std::vector<double>(n, 0.0));

    // Inicializar L como identidade e U como cópia de A
    for (int i = 0; i < n; i++) {
        L[i][i] = 1.0;
        for (int j = 0; j < n; j++) {
            U[i][j] = A[i][j];
        }
    }

    // Fatoração LU usando método de Doolittle
    for (int k = 0; k < n; k++) {
        // Verificar se o pivô é zero
        if (std::abs(U[k][k]) < 1e-10) {
            return false; // Matriz singular ou quase singular
        }

        // Calcular elementos abaixo da diagonal em L
        for (int i = k + 1; i < n; i++) {
            L[i][k] = U[i][k] / U[k][k];

            // Atualizar linha i de U
            for (int j = k; j < n; j++) {
                U[i][j] -= L[i][k] * U[k][j];
            }
        }
    }

    // Agora converter LU em LDP
    // U = DP, onde D é diagonal e P tem diagonal unitária
    // D[i][i] = U[i][i]
    // P[i][j] = U[i][j] / U[i][i] para j > i

    for (int i = 0; i < n; i++) {
        // Elementos da diagonal de D
        D[i][i] = U[i][i];

        // Elementos de P (triangular superior com diagonal unitária)
        P[i][i] = 1.0;
        for (int j = i + 1; j < n; j++) {
            if (std::abs(D[i][i]) > 1e-10) {
                P[i][j] = U[i][j] / D[i][i];
            } else {
                return false; // Divisão por zero
            }
        }
    }

    return true;
}

void FatoracaoLDP::substituicaoProgressiva(const std::vector<std::vector<double>>& L,
                                           const std::vector<double>& b,
                                           std::vector<double>& y) {
    int n = L.size();
    y.assign(n, 0.0);

    for (int i = 0; i < n; i++) {
        y[i] = b[i];
        for (int j = 0; j < i; j++) {
            y[i] -= L[i][j] * y[j];
        }
        y[i] /= L[i][i];
    }
}

void FatoracaoLDP::resolverDP(const std::vector<std::vector<double>>& D,
                              const std::vector<std::vector<double>>& P,
                              const std::vector<double>& y,
                              std::vector<double>& x) {
    int n = D.size();
    std::vector<double> z(n, 0.0);

    // Resolver Dz = y (multiplicação pela diagonal)
    for (int i = 0; i < n; i++) {
        if (std::abs(D[i][i]) > 1e-10) {
            z[i] = y[i] / D[i][i];
        } else {
            z[i] = 0.0;
        }
    }

    // Resolver Px = z (substituição regressiva, já que P é triangular superior)
    x.assign(n, 0.0);
    for (int i = n - 1; i >= 0; i--) {
        x[i] = z[i];
        for (int j = i + 1; j < n; j++) {
            x[i] -= P[i][j] * x[j];
        }
        // P[i][i] = 1.0, então não precisa dividir
    }
}

bool FatoracaoLDP::resolver(const std::vector<std::vector<double>>& A,
                            const std::vector<double>& b,
                            std::vector<double>& x) {
    std::vector<std::vector<double>> L, D, P;
    std::vector<double> y;

    if (!fatorar(A, L, D, P)) {
        return false;
    }

    substituicaoProgressiva(L, b, y);
    resolverDP(D, P, y, x);

    return true;
}
