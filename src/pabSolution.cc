#include "pabSolution.hh"

std::string pabSolution::input_file = std::string();

pabSolution::pabSolution() : p(input_file) {
    a_alocar = p.navios;
    cost = simulateCost();
}

pabSolution::pabSolution(const Porto& p, const std::vector<Porto::Navio>& alocados, const std::vector<Porto::Navio>& a_alocar) : p(p), alocados(alocados), a_alocar(a_alocar) {
    cost = simulateCost();
}

pabSolution::~pabSolution() { }

std::vector<pabSolution> pabSolution::getCandidates() {
    std::vector<pabSolution> ret;
    for (unsigned int i = 0; i < a_alocar.size(); ++i) {
        for (unsigned int j = 0; j < p.bercos.size(); ++j) {
            std::vector<Porto::Navio> new_alocados = alocados;
            std::vector<Porto::Navio> new_a_alocar = a_alocar;

            Porto::Navio n = a_alocar[i];

            n.alocacao = j;

            new_alocados.push_back(n);

            auto it = new_a_alocar.begin();
            //std::advance(it, i);
            new_a_alocar.erase(it + i);

            pabSolution pabNew(p, new_alocados, new_a_alocar);
            ret.push_back(pabNew);
        }
    }

    //TODO
    /*    
          std::cout << "Semi solution: " << std::endl << (*this);
          std::cout << "Candidate list: " << std::endl;
          for (auto s : ret)
          std::cout << s;*/

    return ret;
}

std::vector<pabSolution> pabSolution::getNeighbors() {
    std::vector<pabSolution> list;
    if (a_alocar.size() != 0) {
        std::cout << "Error on getNeighbors()" << std::endl;
        throw "Trying to get neighbors while still having free ships";
    }

    //Swaps ships

    //TODO WILL USE ONLY 5000+5000 neighbors
    int c = 0;
    for (unsigned int i = 0; (i < alocados.size()) && (c < 5000); ++i) {
        for (unsigned int j = (i + 1); j < alocados.size(); ++j) {
            auto new_alocados = alocados;
            std::iter_swap(new_alocados.begin() + i, new_alocados.begin() + j);
            pabSolution new_sol(p, new_alocados, a_alocar);

            list.push_back(new_sol);
            ++c;
        }
    }


    //Swaps allocations
    c = 0;
    for (unsigned int i = 0; (i < alocados.size()) && (c < 5000); ++i) {
        for (unsigned int j = 0; j < p.bercos.size(); ++j) {
            if (alocados[i].alocacao == j) continue;
            auto new_alocados = alocados;
            new_alocados[i].alocacao = j;
            pabSolution new_sol(p, new_alocados, a_alocar);

            list.push_back(new_sol);
        }
    }

#ifdef DEBUG_NEIGHBORS
    std::cout << "Neighbors for: " << std::endl;
    std::cout << (*this);
    std::cout << "-----------" << std::endl;
    for (auto n : list)
        std::cout << n;
#endif
    return list;
}

bool pabSolution::isComplete() {
    return a_alocar.size() == 0;
}

void pabSolution::afterGreedyBuildPhase(std::vector<pabSolution> solutions) {
    //std::cout << "After greedy phase: " << std::endl;
    float sum_avg = 0;
    std::sort(solutions.begin(), solutions.end(), [](pabSolution s1, pabSolution s2) { return s1.getCost() < s2.getCost(); });
    for (auto s : solutions) {
        //std::cout << s;
        sum_avg += s.getCost();
    }

    const float average = sum_avg/solutions.size();

    float var = 0;

    for (auto s : solutions)
        var += ((s.getCost() - average) * (s.getCost() - average));


    /*std::cout << "Average cost: " << sum_avg / solutions.size() << "\t";
    std::cout << "Variability: " << var << "\t";
    std::cout << "Best cost: " << solutions[0].getCost() << std::endl;
    std::cout << "----------------------------------------------------" << std::endl;*/
		//std::cout << "Greedy," << input_file << ","  << solutions.size() << "," << average << ","
		 //         << var << "," << solutions[0].getCost() << std::endl;
}

void pabSolution::afterLocalSearchPhase(std::vector<pabSolution> solutions) {
    //std::cout << "After Local Search Phase: " << std::endl;
    float sum_avg = 0;

    std::sort(solutions.begin(), solutions.end(), [](pabSolution s1, pabSolution s2) { return s1.getCost() < s2.getCost(); });

    for (auto s : solutions) {
        //std::cout << s;
        sum_avg += s.getCost();
    }

    const float average = sum_avg/solutions.size();
    float var = 0;

    for (auto s : solutions)
        var += ((s.getCost() - average) * (s.getCost() - average));

    /*std::cout << "Average Cost: " << sum_avg / solutions.size() << "\t";
    std::cout << "Variability: " << var << "\t";
    std::cout << "Best cost: " << solutions[0].getCost() << std::endl;*/
		std::cout << "Local," << input_file << solutions.size() << "," << average << ","
		          << var << "," << solutions[0].getCost() << std::endl;

}

float pabSolution::getCost() {
    return cost;
}

float pabSolution::simulateCost() {
    int custo_a_alocar = 0;
    int custo_alocado = 0;

    std::vector<std::vector<Porto::Navio> > filas;
    for (unsigned int i = 0; i < p.bercos.size(); ++i)
        filas.emplace_back();

    for (auto n : alocados)
        filas[n.alocacao].push_back(n);

    unsigned int t_minimo;
    custo_alocado = simulate(filas, t_minimo);

    //Calcula t_minimo
    unsigned int taxa_max = 0;
    for (auto b : p.bercos)
        if (b.taxa > taxa_max)
            taxa_max = b.taxa;

    for (auto n : a_alocar) {
        int c_total = 0;
        for (auto c : n.carga)
            c_total += c;
        n.t_min = c_total / taxa_max;
    }

    for (auto n : a_alocar) {
        custo_a_alocar += n.t_min;
        if (n.chegada < t_minimo)
            custo_a_alocar += (t_minimo - n.chegada);
    }

    return custo_a_alocar + custo_alocado;
}

int pabSolution::simulate(std::vector<std::vector<Porto::Navio> > alocados, unsigned int& first_free) {
    unsigned int t = 1;
    std::vector<unsigned int> hora_a_liberar;
    std::vector<Porto::Navio> proc;
    Porto p_local = p;

    int processados = 0;

    int numNavios = 0;

    for (auto v : alocados) {
        numNavios += v.size();
    }

    for (unsigned int i = 0; i < p_local.bercos.size(); ++i) {
        p_local.bercos[i].is_loading = false;
    }

    for (unsigned int i = 0; i < p_local.bercos.size(); ++i)
        hora_a_liberar.push_back(0);
    while (processados != numNavios) {

#ifdef DEBUG_SIMULATION
        std::cout << "Simulation time: " << t << std::endl;
        std::cout << "Berth Status: ";
        for (auto ba : p_local.bercos) {
            std::cout << (ba.is_loading ? "L" : "N");
        }
        std::cout << std::endl;
        std::cout << "First ships: ";
        for (auto v : alocados) {
            std::cout << (v.size() == 0 ? 0 : v.back().chegada) << " ";
        }
        std::cout << std::endl;
        std::cout << "Stocks: ";
        for (auto e : p_local.estoques)
            std::cout << e.estoque << " ";
        std::cout << std::endl;
#endif

        for (unsigned int i = 0; i < alocados.size(); ++i) {
            if (alocados[i].size() == 0 && !p_local.bercos[i].is_loading) continue;  //if no ship and is not loading
            if (p_local.bercos[i].is_loading) {
                //Se esta carregando agrega a carga
                auto n = alocados[i].back();
                unsigned int max = n.carga.size();
                for (unsigned int c = 0; c < max; ++c) {
                    if (n.carga[c] != 0) {
                        int descarregados = (n.carga[c] > p_local.bercos[i].taxa ? p_local.bercos[i].taxa : n.carga[c]);
                        p_local.estoques[c].estoque += descarregados;
                        n.carga[c] -= descarregados;
                    }
                }

                if (n.saida == t) {
                    p_local.bercos[i].is_loading = false;
                    alocados[i].pop_back();
                }
            }

            if (alocados[i].size() == 0) continue;

            if (!p_local.bercos[i].is_loading && alocados[i].back().chegada <= t) {
                Porto::Navio n = alocados[i].back();
                n.inicio = t;
                int c_total = 0;
                for (int c : n.carga)
                    c_total += c;
                n.saida = hora_a_liberar[i] = t + std::ceil((float)c_total/p_local.bercos[i].taxa);
                alocados[i].back().saida = n.saida;
                proc.push_back(n);
                p_local.bercos[i].is_loading = true;
#ifdef DEBUG_SIMULATION
                std::cout << "Alocou navio: Chegada("; //XXX
                std::cout << n.chegada << "), inicio(" << n.inicio << "), saida (" << n.saida << "), carga_total(" << c_total << "), taxa(" << p_local.bercos[i].taxa << ")" << std::endl;
#endif
                ++processados;
            }
        }

        for (auto e : p_local.estoques) {
            e.estoque -= e.consumo;
            if (e.estoque < 0)
                return std::numeric_limits<int>::max();
        }
        ++t;
    }
    int ret = 0;

    first_free = hora_a_liberar[0];
    for (unsigned int i = 1; i < hora_a_liberar.size(); ++i)
        if (hora_a_liberar[i] < first_free)
            first_free = hora_a_liberar[i];

    //std::cout << "Resultados -----------------" << std::endl;
    for (auto n : proc) {
        ret += (n.saida - n.chegada/* + 1*/);
#ifdef DEBUG_SIMULATION
        std::cout << "Tirando resultado: " << n.saida << " / " << n.chegada << std::endl;
#endif
    }

    //std::cout << "Mares: " << t << "\n";
    return ret;
}
