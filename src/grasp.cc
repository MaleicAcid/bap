#include "grasp.hh"
#include <iostream>

#ifndef GRASP_CC_
#define GRASP_CC_

template <typename solution, unsigned int numSolution>
acmo::grasp<solution, numSolution>::grasp() : solutions(numSolution) { };

template <typename solution, unsigned int numSolution>
acmo::grasp<solution, numSolution>::~grasp() { };

template <typename solution, unsigned int numSolution>
void acmo::grasp<solution, numSolution>::solve() {
    greedyRandomPhase();

    solution::afterGreedyBuildPhase(solutions); //Suggestion: use statics to gather data

    HCPhase();

    solution::afterLocalSearchPhase(solutions); //Suggestion: statics to aggregate data
}

template <typename solution, unsigned int numSolution>
void acmo::grasp<solution, numSolution>::greedyRandomPhase() {
    std::vector<solution> finalSolutions;
  //  int i = 0;
    for (auto s : solutions) {
        while (! s.isComplete()) {
            std::vector<solution> candidate_list = s.getCandidates(); //TODO: int as amount of neighbors?

            s = chooseCandidate(candidate_list);
        }

        finalSolutions.push_back(s);
//        if (((++i) % 10) == 0)
 //           std::cout << "Built " << i << " solutions" << std::endl;
    }

    solutions = finalSolutions;
}

template <typename solution, unsigned int numSolution>
inline
solution acmo::grasp<solution, numSolution>::chooseCandidate(std::vector<solution> candidate_list) {
    std::sort(candidate_list.begin(), candidate_list.end(), [](solution s1, solution s2) { return s1.getCost() < s2.getCost(); });

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0, alpha);

    unsigned int index = std::floor(dist(gen) * candidate_list.size());

    return candidate_list[index];
}

template <typename solution, unsigned int numSolution>
void acmo::grasp<solution, numSolution>::HCPhase() {
   // int c = 0;
/*    for (auto s : solutions) {
        std::vector<solution> neighbors = s.getNeighbors();
        std::sort(neighbors.begin(), neighbors.end(), [](solution s1, solution s2) { return s1.getCost() < s2.getCost(); });
        while (s.getCost() > neighbors[0].getCost()) {
            s = neighbors[0];
            neighbors = s.getNeighbors();
            std::sort(neighbors.begin(), neighbors.end(), [](solution s1, solution s2) { return s1.getCost() < s2.getCost(); });
        } */
    for (unsigned int i = 0; i < solutions.size(); ++i) {
        std::vector<solution> neighbors = solutions[i].getNeighbors();
        std::sort(neighbors.begin(), neighbors.end(), [](solution s1, solution s2) { return s1.getCost() < s2.getCost(); });

        while (solutions[i].getCost() > neighbors[0].getCost()) {
            solutions[i] = neighbors[0];
            neighbors = solutions[i].getNeighbors();
            std::sort(neighbors.begin(), neighbors.end(), [](solution s1, solution s2) { return s1.getCost() < s2.getCost(); });
        }

   //     if (((++c) % 1) == 0)
    //        std::cout << "Improved " << c << " solutions." << std::endl;
    }
}
#endif

