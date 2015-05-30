#include "porto.hh"
#include <sstream>
#include <fstream>
#include <string>

Porto::Porto(std::string input_file) {
    std::string contents;
    
    //Le arquivo pra string
    {
        std::ifstream input(input_file);
        std::string str;

        while (std::getline(input, str)) {
            contents += str;
        }
    }
    
    //Le vazao
    {
        std::string::size_type start, finish;
        start = contents.find("vazao");
        finish = contents.find(";", start);
//        std::cout << contents.substr(start + 9, finish - start - 10) << std::endl;
        std::stringstream ss(contents.substr(start + 9, finish - start - 10));
        int taxa;

        while (ss >> taxa) {
//            std::cout << "Read " << taxa << " / remaining : " << ss.str();
            Berco b;
            b.taxa = taxa;
            bercos.push_back(b);
        }

//        std::cout << "Feitos os bercos: " << std::endl;
//        for (auto b : bercos) {
//            std::cout << "Taxa: " << b.taxa << std::endl;
//        }
    }

    //Le estoque inicial
    {
        std::string::size_type start, finish;
        start = contents.find("estoque");
        finish = contents.find(";", start);
        std::stringstream ss(contents.substr(start + 11, finish - start - 12));
        int estoque;

        while (ss >> estoque) {
            Estoque e;
            e.estoque_inicial = e.estoque = estoque;
            estoques.push_back(e);
        }

//        std::cout << "Lidos estoques iniciais: " << std::endl;
//        for (auto e : estoques) {
//            std::cout << "Estoque Inicial: " << e.estoque_inicial << std::endl;
//        }
    }

    //Le consumo
    {
        std::string::size_type start, finish;
        start = contents.find("consumo");
        finish = contents.find(";", start);
        std::stringstream ss(contents.substr(start + 11, finish - start - 12));
        int consumo;
        int i = 0;

        while (ss >> consumo) {
            estoques[i].consumo = consumo;
            ++i;
        }

//        std::cout << "Lidos os consumos: " << std::endl;
//        for (auto e : estoques) {
//            std::cout << "Consumo: " << e.consumo << std::endl;
//        }
    }

    //Le chegada dos navios
    {
        std::string::size_type start, finish;
        start = contents.find("chegada");
        finish = contents.find(";", start);
        std::stringstream ss(contents.substr(start + 11, finish - start - 12));
        int chegada;

        while (ss >> chegada) {
            Navio n;
            n.chegada = chegada;
            navios.push_back(n);
        }

//        std::cout << "Lidas as chegadas: " << std::endl;
//        for (auto n : navios) {
//            std::cout << "Chegada: " << n.chegada << std::endl;
//        }
    }

    //Le a carga dos navios
    {
        std::string::size_type start, finish;
        start = contents.find("carregamento");
        start = contents.find("[", start);
        finish = contents.find(";", start);
        int i = 0;
        do {
            start = contents.find("[", start + 1);
            finish = contents.find("]", start);

            if (start == std::string::npos || finish == std::string::npos)
                break;

            std::stringstream ss(contents.substr(start + 1, finish - start - 1));
//            std::cout << "Lendo navio " << i << ": " << ss.str() << std::endl;
            int carga;
            while (ss >> carga) {
                navios[i].carga.push_back(carga);
            }
            ++i;
        } while (true);
    }

//    std::cout << "Cargas lidas: " << std::endl;
//    for (auto n : navios) {
//      std::cout << "Carga: ";
//      for (auto i : n.carga)
//        std::cout << i << " ";
//      std::cout << std::endl;
//    }
}

