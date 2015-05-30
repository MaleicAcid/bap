#include "porto.hh"
#include <iostream>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <limits>

void pr(std::vector<Porto::Navio> lista) {
    std::cout << "Lista de alocacoes: ";
    for (auto n : lista) {
        std::cout << n.alocacao;
    }
    std::cout << std::endl;
}

bool comp(Porto::Berco b1, Porto::Berco b2) { return b1.taxa < b2.taxa; };

int custo_com_filas(std::vector<std::vector<Porto::Navio> > alocados, Porto p, unsigned int& tempos_min) {
    unsigned int t = 1;
    std::vector<unsigned int> hora_a_liberar;
    std::vector<Porto::Navio> proc;
    int processados = 0;

    int numNavios = 0;

    for (auto v : alocados) {
        numNavios += v.size();
    }

    for (unsigned int i = 0; i < p.bercos.size(); ++i) {
        p.bercos[i].is_loading = false;
    }

    for (unsigned int i = 0; i < p.bercos.size(); ++i)
        hora_a_liberar.push_back(0);
    while (processados != numNavios) {
        for (unsigned int i = 0; i < alocados.size(); ++i) {
            if (alocados[i].size() == 0 && !p.bercos[i].is_loading) continue;
            if (p.bercos[i].is_loading) {

                //Se esta carregando agrega a carga
                auto n = alocados[i].back();
                unsigned int max = n.carga.size();
                for (unsigned int c = 0; c < max; ++i) {
                    if (n.carga[c] != 0) {
                        int descarregados = (n.carga[c] > p.bercos[i].taxa ? p.bercos[i].taxa : n.carga[c]);
                        p.estoques[c].estoque += descarregados;
                        n.carga[c] -= descarregados;
                    }
                }

                int c_tot = 0;
                for (int i : n.carga)
                    c_tot += i;
                if (c_tot == 0) p.bercos[i].is_loading = false;
            }

            if (hora_a_liberar[i] < t && alocados[i].back().chegada <= t) {
                Porto::Navio n = alocados[i].back();
                n.inicio = t;
                int c_total = 0;
                for (int c : n.carga)
                    c_total += c;
                n.saida = hora_a_liberar[i] = t + std::ceil(c_total/p.bercos[i].taxa);
                alocados[i].pop_back();
                proc.push_back(n);
                ++processados;
            }
        }

        for (auto e : p.estoques) {
            e.estoque -= e.consumo;
            if (e.estoque < 0)
                return std::numeric_limits<int>::max();
        }
        ++t;
    }
    int ret = 0;
    
    tempos_min = hora_a_liberar[0];
    for (unsigned int i = 1; i < hora_a_liberar.size(); ++i)
        if (hora_a_liberar[i] < tempos_min)
            tempos_min = hora_a_liberar[i];

    //std::cout << "Resultados -----------------" << std::endl;
    for (auto n : proc) {
        ret += (n.saida - n.chegada + 1);
        //std::cout << "Tirando resultado: " << n.saida << " / " << n.chegada << std::endl;
    }
    return ret;
}

int custo(std::vector<Porto::Navio> alocados, std::vector<Porto::Navio> a_alocar, Porto p) {
    int custo_a_alocar = 0;
    int custo_alocado = 0;
    static int calls = 1;


    std::vector<std::vector<Porto::Navio> > filas;
    for (unsigned int i = 0; i < p.bercos.size(); ++i)
        filas.emplace_back();

    for (auto n : alocados)
        filas[n.alocacao].push_back(n);

    unsigned int t_minimo;
    custo_alocado = custo_com_filas(filas, p, t_minimo);
    
    for (auto n : a_alocar) {
        custo_a_alocar += n.t_min;
        if (n.chegada > t_minimo)
            custo_a_alocar += (t_minimo - n.chegada);
    }

    std::cout << "Calls: " << ++calls << std::endl;
    return custo_a_alocar + custo_alocado;
}

int best;
int BnB(std::vector<Porto::Navio> alocados, std::vector<Porto::Navio> a_alocar, Porto p);

void doBnB(char* input_file) {
    Porto p(input_file);

    best = std::numeric_limits<int>::max();
    std::vector<Porto::Navio> a_alocar = p.navios;
    std::vector<Porto::Navio> alocados;
    int i = BnB(alocados, a_alocar, p);

    std::cout << "Min cost: " << i << std::endl;
    std::cout << "Best: " << best << std::endl;
}

int BnB(std::vector<Porto::Navio> alocados, std::vector<Porto::Navio> a_alocar, Porto p) {
    static int bound = 0;
    if (a_alocar.size() == 0) return custo(alocados, a_alocar, p);

    auto n = a_alocar.back();
    a_alocar.pop_back();
    int min_pra_baixo = std::numeric_limits<int>::max();
    for (unsigned int i = 0; i < p.bercos.size(); ++i) { //Bound pegando o proximo berco TODO: variar o trem
        n.alocacao = i;
        alocados.push_back(n);
        int c = custo(alocados, a_alocar, p);
        std::cout << "Pre_avaliacao: " << std::endl;
        pr(alocados);
        pr(a_alocar);
        if (c > best) { 
            alocados.pop_back();
            std::cout << "Bound: " << ++bound << std::endl;
            continue; //Bound
        }
        c = BnB(alocados, a_alocar, p); //Brancheia
        if (c < min_pra_baixo) min_pra_baixo = c;
        alocados.pop_back();
    }

    if (a_alocar.size() == 0) {
        if (best > min_pra_baixo) {
            best = min_pra_baixo;
            std::cout << "_______________NEW BEST _______________" << std::endl;
        }
    }

    return min_pra_baixo;
}

void Evaluate(char* input_file) {
    Porto p(input_file);

    int taxa_max = (*std::max_element(p.bercos.begin(), p.bercos.end(), comp)).taxa;

    for (auto n : p.navios) {
        int carga_total = 0;
        for (int i : n.carga)
            carga_total += i;
        n.t_min = carga_total/taxa_max;
    }

    std::vector<Porto::Navio> alocados;
    std::vector<Porto::Navio> a_alocar = p.navios;

    //while (true) {
        while (a_alocar.size() != 0) {
            Porto::Navio n = a_alocar.back();
            a_alocar.pop_back();

            n.alocacao = 1;
            alocados.push_back(n);
        }

        std::cout << "Avaliar: ";
        pr(alocados);
        int custo_min;
        custo_min = custo(alocados, a_alocar, p);

        std::cout << "Custo inicial: " << custo_min << std::endl;

        bool repeat = false;
        do {
            //std::cout << "Tamanho alocados: " << alocados.size() << std::endl;
            if (alocados.size() == 0) {
                std::cout << "Terminou as permutacoes! " << std::endl;
                return;
            }
            auto n = alocados.back();
            alocados.pop_back();
            n.alocacao += 1;
            if (n.alocacao == p.bercos.size()) {
                n.alocacao = 1;
                repeat = true;
                a_alocar.push_back(n);
            } else {
                alocados.push_back(n);
                while (a_alocar.size() != 0) {
                    auto temp = a_alocar.back();
                    a_alocar.pop_back();
                    alocados.push_back(temp);
                    //avalia se faz o bound
                }
                //std::cout << "Avaliacao2";
                //pr(alocados);
                //TODO:Avaliar
            }
        } while (repeat);
    //}
}
