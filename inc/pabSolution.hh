#ifndef PABSOLUTION_H_
#define PABSOLUTION_H_
#include <vector>
#include "porto.hh"
#include <iostream>
#include <limits>
#include <cmath>
#include <algorithm>

class pabSolution {
    public:
        pabSolution();
        ~pabSolution();

        //Partial building:
        std::vector<pabSolution> getCandidates();

        //Full Solution neighbor evaluation
        std::vector<pabSolution> getNeighbors();

        //Support functions for both
        bool isComplete();
        float getCost();

        static std::string input_file;
        friend std::ostream& operator<<(std::ostream& out, pabSolution p);

        //Package specific functions:
        static void afterGreedyBuildPhase(std::vector<pabSolution> solutions);
        static void afterLocalSearchPhase(std::vector<pabSolution> solutions);

    private:
        pabSolution(const Porto& p, const std::vector<Porto::Navio>& alocados, const std::vector<Porto::Navio>& a_alocar);
        float simulateCost();
        int simulate(std::vector<std::vector<Porto::Navio> > alocados, unsigned int& first_free);

        float cost = 0;
        Porto p;
        std::vector<Porto::Navio> alocados;
        std::vector<Porto::Navio> a_alocar;
};

inline std::ostream& operator<<(std::ostream& out, pabSolution p) {
    out << "Custo: " << p.getCost() << std::endl;
    out << "A alocar: ";
    for (auto n : p.a_alocar)
        out << n.chegada << " ";
    out << std::endl;

    out << "Alocados: ";
    for (auto n : p.alocados)
        out << n.chegada << "(" << n.alocacao << ") ";
    out << std::endl;
    return out;
}
#endif

