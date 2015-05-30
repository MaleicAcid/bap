#ifndef PORTO_H_
#define PORTO_H_

#include <vector>
#include <iostream>

class Porto {
    public:
        struct Berco {
            unsigned int taxa;
            bool is_loading;
        };

        std::vector<Berco> bercos;

        struct Navio {
            unsigned int chegada;
            std::vector<unsigned int> carga;
            int inicio;
            int saida;
            unsigned int alocacao;
            unsigned int t_min;
            bool ja_alocou;
        };

        std::vector<Navio> navios;

        struct Estoque {
            int estoque_inicial;
            int estoque;
            int consumo;
        };

        std::vector<Estoque> estoques;

        Porto(std::string input_file);

        //void printOn(std::ostream& stream);
};

#endif

