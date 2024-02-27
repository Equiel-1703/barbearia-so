#pragma once

#include <iostream>
#include <unistd.h>

#include "ConsoleWriter.h"

class Ferramentas
{
private:
    ConsoleWriter *console_writer;
public:
    Ferramentas(ConsoleWriter *console_writer);
    
    void usarPente();
    void usarTesoura();
};