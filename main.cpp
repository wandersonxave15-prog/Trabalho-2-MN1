#include <iostream>
#include "SistemaLinear.h"
#include "Menu.h"

int main() {
    SistemaLinear sistema;
    Menu menu;
    menu.setSistema(&sistema);
    menu.executar();
    return 0;
}

