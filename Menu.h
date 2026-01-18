#ifndef MENU_H
#define MENU_H

#include "SistemaLinear.h"
#include <vector>
#include <string>

class Menu {
private:
    std::vector<std::string> opcoes;
    int selecao;
    bool rodando;
    SistemaLinear* sistema;
    
    void limparTela();
    void mostrarMenu();
    int lerTecla();
    void processarTecla();
    void executarOpcao();
    
    void testePadrao();
    void configurarSistema();
    void quadroRespostaLU();
    void quadroRespostaLDP();
    void quadroComparativo();
    void analiseSeguranca();
    void variarSistema();
    void sair();
    
public:
    Menu();
    void executar();
    void setSistema(SistemaLinear* s);
};

#endif // MENU_H

