#ifndef GRASP_H_
#define GRASP_H_
#include <vector>
#include <algorithm>
#include <random>
#include <cmath>

namespace acmo {
    //Problem must have a partially constructible solution
    //Implement Later : selector, second phase heuristics
    //Problem instance must provide at least the following methods:
    //getCost()
    //std::vector<solution> getCandidates() for next step in building solution
    //isComplete()

    //HC Phase:
    //getNeighbors
    //getCost()

    //Reporting: (put null functions inline to 0 impact on running time
    //static void afterGreedyBuildPhase(std::vector<solution>)
    //static void afterLocalSearchPhase(std::vector<solution>)
    
    template <typename solution, unsigned int numSolutions = 1000>
    class grasp {
            public:
                grasp();
                ~grasp();
                void solve();

                std::vector<solution> solutions;
            private:
                float alpha = 0.9; //TODO: argument
                void greedyRandomPhase();
                void HCPhase();

                solution chooseCandidate(std::vector<solution> candidate_list);
        };
}

#include "../src/grasp.cc"

#endif
