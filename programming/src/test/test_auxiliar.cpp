
#include "../libraries/raider/raider.h"
#include "../libraries/debug/debug.h"
#include <iostream>

int main()
{
    report(INFO,"Mensaje de informacion de color azul :|");
    report(ERROR,"Mensaje de error de color rojo :(");
    report(OK,"Mensaje de ok de color verde :)");
    report(WARNING,"Mensaje de warning de color amarillo :O");
    report(RAIDER,"Mensaje de Raider");
    report(STATE,"Estado actual");
    report(MOVE,"Me muevo mazo");
    report("mensaje comun y moliente");

}

