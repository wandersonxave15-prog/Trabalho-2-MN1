#include "FatoracaoLU.h"
#include <cmath>
#include <algorithm>

bool FatoracaoLU::fatorar(const std::vector<std::vector<double>>& A,
                          std::vector<std::vector<double>>& L,
                          std::vector<std::vector<double>>& U) {
    int n = A.size();
    
    // Inicializar L como identidade e U como cópia de A
    L.assign(n, std::vector<double>(n, 0.0));
    U.assign(n, std::vector<double>(n, 0.0));
    
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
    
    return true;
}

void FatoracaoLU::substituicaoProgressiva(const std::vector<std::vector<double>>& L,
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

void FatoracaoLU::substituicaoRegressiva(const std::vector<std::vector<double>>& U,
                                         const std::vector<double>& y,
                                         std::vector<double>& x) {
    int n = U.size();
    x.assign(n, 0.0);
    
    for (int i = n - 1; i >= 0; i--) {
        x[i] = y[i];
        for (int j = i + 1; j < n; j++) {
            x[i] -= U[i][j] * x[j];
        }
        x[i] /= U[i][i];
    }
}

bool FatoracaoLU::resolver(const std::vector<std::vector<double>>& A,
                          const std::vector<double>& b,
                          std::vector<double>& x) {
    std::vector<std::vector<double>> L, U;
    std::vector<double> y;
    
    if (!fatorar(A, L, U)) {
        return false;
    }
    
    substituicaoProgressiva(L, b, y);
    substituicaoRegressiva(U, y, x);
    
    return true;
}

