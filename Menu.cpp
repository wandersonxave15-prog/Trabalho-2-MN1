#include "Menu.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <termios.h>
#include <unistd.h>

Menu::Menu() {
    selecao = 0;
    rodando = true;
    sistema = nullptr;
    opcoes.push_back("1. Executar Teste Padrao (Calibracao)");
    opcoes.push_back("2. Configurar Sistema (Matriz A e Vetor f)");
    opcoes.push_back("3. Exibir Quadro Resposta - Metodo LU");
    opcoes.push_back("4. Exibir Quadro Resposta - Metodo LDP");
    opcoes.push_back("5. Exibir Quadro Comparativo (LU vs LDP)");
    opcoes.push_back("6. Analise de Seguranca do Jato");
    opcoes.push_back("7. Variar Sistema (Testar diferentes valores)");
    opcoes.push_back("8. Sair");
}

void Menu::setSistema(SistemaLinear* s) {
    sistema = s;
}

void Menu::limparTela() {
    std::cout << "\033[2J\033[H";
}

void Menu::mostrarMenu() {
    limparTela();
    std::cout << "=== SISTEMA DE CALCULO DE DESLOCAMENTO DE JATO SUPERSONICO ===" << std::endl;
    std::cout << "=== Metodos: Fatoracao LU e Fatoracao LDP ===" << std::endl;
    std::cout << std::endl;
    std::cout << "Use as setas para navegar, ENTER para selecionar, ESC para sair" << std::endl;
    std::cout << std::endl;
    
    for (int i = 0; i < (int)opcoes.size(); i++) {
        if (i == selecao) {
            std::cout << "> " << opcoes[i] << std::endl;
        } else {
            std::cout << "  " << opcoes[i] << std::endl;
        }
    }
    std::cout << std::endl;
}

int Menu::lerTecla() {
    struct termios antigo, novo;
    int tecla;
    
    tcgetattr(STDIN_FILENO, &antigo);
    novo = antigo;
    novo.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &novo);
    
    tecla = getchar();
    
    if (tecla == 27) {
        tecla = getchar();
        if (tecla == 91) {
            tecla = getchar();
            tcsetattr(STDIN_FILENO, TCSANOW, &antigo);
            if (tecla == 65) return 1; // Seta para cima
            if (tecla == 66) return 2; // Seta para baixo
        } else {
            tcsetattr(STDIN_FILENO, TCSANOW, &antigo);
            return 6; // ESC
        }
    }
    
    tcsetattr(STDIN_FILENO, TCSANOW, &antigo);
    
    if (tecla == 10 || tecla == 13) {
        return 5; // ENTER
    }
    
    return 0;
}

void Menu::processarTecla() {
    int tecla = lerTecla();
    
    if (tecla == 1) {
        selecao--;
        if (selecao < 0) selecao = opcoes.size() - 1;
    }
    if (tecla == 2) {
        selecao++;
        if (selecao >= (int)opcoes.size()) selecao = 0;
    }
    if (tecla == 5) {
        executarOpcao();
    }
    if (tecla == 6) {
        sair();
    }
}

void Menu::executarOpcao() {
    limparTela();
    
    if (selecao == 0) testePadrao();
    else if (selecao == 1) configurarSistema();
    else if (selecao == 2) quadroRespostaLU();
    else if (selecao == 3) quadroRespostaLDP();
    else if (selecao == 4) quadroComparativo();
    else if (selecao == 5) analiseSeguranca();
    else if (selecao == 6) variarSistema();
    else if (selecao == 7) {
        sair();
        return;
    }
    
    std::cout << std::endl;
    std::cout << "Pressione ENTER para voltar ao menu...";
    std::cin.ignore();
    std::cin.get();
}

void Menu::testePadrao() {
    if (!sistema) return;
    
    std::cout << "=== TESTE PADRAO (CALIBRACAO) ===" << std::endl;
    std::cout << std::endl;
    std::cout << "Matriz A padrao:" << std::endl;
    std::cout << "[3, -2,  1]" << std::endl;
    std::cout << "[1, -3,  4]" << std::endl;
    std::cout << "[9,  4, -5]" << std::endl;
    std::cout << std::endl;
    std::cout << "Vetor f padrao: [8, 6, 11]" << std::endl;
    std::cout << std::endl;
    
    sistema->calibrarSistema();
    
    std::cout << "Sistema calibrado e calculos executados!" << std::endl;
    std::cout << std::endl;
    std::cout << "Use as opcoes 3, 4, 5 ou 6 para ver os resultados." << std::endl;
}

void Menu::configurarSistema() {
    if (!sistema) return;
    
    std::cout << "=== CONFIGURACAO DO SISTEMA ===" << std::endl;
    std::cout << std::endl;
    
    sistema->lerMatriz();
    sistema->lerVetor();
    
    std::cout << std::endl;
    std::cout << "Processando sistema..." << std::endl;
    sistema->executarCalculos();
    
    std::cout << std::endl;
    std::cout << "Sistema configurado e calculos executados!" << std::endl;
}

void Menu::quadroRespostaLU() {
    if (!sistema) return;
    if (sistema->getDimensao() == 0) {
        std::cout << "Configure o sistema primeiro (opcao 1 ou 2)." << std::endl;
        return;
    }
    sistema->exibirQuadroRespostaLU();
}

void Menu::quadroRespostaLDP() {
    if (!sistema) return;
    if (sistema->getDimensao() == 0) {
        std::cout << "Configure o sistema primeiro (opcao 1 ou 2)." << std::endl;
        return;
    }
    sistema->exibirQuadroRespostaLDP();
}

void Menu::quadroComparativo() {
    if (!sistema) return;
    if (sistema->getDimensao() == 0) {
        std::cout << "Configure o sistema primeiro (opcao 1 ou 2)." << std::endl;
        return;
    }
    sistema->exibirQuadroComparativo();
}

void Menu::analiseSeguranca() {
    if (!sistema) return;
    if (sistema->getDimensao() == 0) {
        std::cout << "Configure o sistema primeiro (opcao 1 ou 2)." << std::endl;
        return;
    }
    sistema->exibirAnaliseSeguranca();
}

void Menu::variarSistema() {
    if (!sistema) return;
    
    std::cout << "=== VARIACAO DO SISTEMA ===" << std::endl;
    std::cout << std::endl;
    std::cout << "Esta opcao permite testar diferentes valores de A e f." << std::endl;
    std::cout << "Digite o numero de variacoes que deseja testar: ";
    
    int numVariacoes;
    std::cin >> numVariacoes;
    
    if (numVariacoes <= 0) {
        std::cout << "Numero invalido!" << std::endl;
        return;
    }
    
    std::cout << std::endl;
    
    for (int v = 0; v < numVariacoes; v++) {
        std::cout << "\n=== Variacao " << (v + 1) << " ===" << std::endl;
        sistema->lerMatriz();
        sistema->lerVetor();
        sistema->executarCalculos();
        
        std::cout << std::endl;
        std::cout << "Resultados da variacao " << (v + 1) << ":" << std::endl;
        sistema->exibirQuadroComparativo();
        
        if (v < numVariacoes - 1) {
            std::cout << "\nPressione ENTER para continuar...";
            std::cin.ignore();
            std::cin.get();
        }
    }
}

void Menu::sair() {
    limparTela();
    std::cout << "Encerrando..." << std::endl;
    rodando = false;
}

void Menu::executar() {
    while (rodando) {
        mostrarMenu();
        processarTecla();
    }
}

