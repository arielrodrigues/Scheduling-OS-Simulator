#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <tuple>

#ifndef SIMPLE_OS_SIMULATOR_SIMULATOR_H
#define SIMPLE_OS_SIMULATOR_SIMULATOR_H


class Simulator {

public:
    Simulator();

private:
    //camelCase??
    float tempoRetornoMedio, tempoServicoMedio, utilizacaoProcessador,
            tempoRespostaMedio, throughput, tempoEsperaMedio, duracaoSimulacao;

};


#endif //SIMPLE_OS_SIMULATOR_SIMULATOR_H
