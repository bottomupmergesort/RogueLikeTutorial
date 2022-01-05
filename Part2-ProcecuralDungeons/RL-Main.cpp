#include <iostream>
#include <BearLibTerminal.h>
#include "RL-map.h"
#include "RL-Actor.h"
#include "RL-Engine.h"
using namespace std;
void initBearLib(int wx, int wy)
{
    terminal_open();
    terminal_set("window: size=80x40;");
    terminal_refresh();
}

int main()
{
    int wy = 40, wx = 80; //window height and width
    initBearLib(wx, wy);
    Engine eng(wx, wy);
    eng.gameLoop();
    return 0;
}