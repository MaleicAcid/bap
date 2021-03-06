#include <iostream>
#include "porto.hh"
#include "bnb.hh"
#include "pabSolution.hh"
#include "grasp.hh"
#include <dirent.h>
#include <ctime>
#include <cstring>

int main(int argc, char* argv[]) {
    //doBnB(argv[1]);
    //pabSolution::input_file = std::string(argv[1]);
    //    acmo::grasp<pabSolution, 100> g;

    //g.solve();

    /*pabSolution p;

      while (!p.isComplete()) {
      p = p.getCandidates()[0];
      }

      std::cout << "Solution: " << std::endl << p; */

    DIR *dir;
    struct dirent *ent;
    time_t start_time;

    dir = opendir("./dat");
    if (dir != NULL) { //Null for C-style NULL
        while ((ent = readdir(dir)) != NULL) {
				    if (strcmp(ent->d_name,".") == 0 || strcmp(ent->d_name,"..") == 0)
						    continue;
            std::string dirpath = "./dat/";
            dirpath += ent->d_name;
            pabSolution::input_file = dirpath;

            std::cout << "Problem: " << dirpath << std::endl;

            start_time = time(NULL);
            for (unsigned char c = 0; c < 1000; ++c) {
                //std::cout << "Grasp with 1 solution, try " << static_cast<unsigned>(c) << "/30." << std::endl;
                acmo::grasp<pabSolution, 1> g;
                g.solve();
            }

            time_t cur_time;
            cur_time = std::time(NULL);

            std::cout << "Time: " << std::difftime(cur_time, start_time) << " seconds used." << std::endl;
/*            std::cout << "---------------------------------------------------------------------------------" << std::endl;

            start_time = std::time(NULL);

            for (unsigned char c = 0; c < 30; ++c) {
                //std::cout << "Grasp with 5 solutions, try " << static_cast<unsigned>(c) << "/30." << std::endl;
                acmo::grasp<pabSolution, 5> g;
                g.solve();
            }

            cur_time = std::time(NULL);
            std::cout << "Time: " << std::difftime(cur_time, start_time) << " seconds used." << std::endl;
            std::cout << "---------------------------------------------------------------------------------" << std::endl;

            start_time = std::time(NULL);

            for (unsigned char c = 0; c < 30; ++c) {
                //std::cout << "Grasp with 10 solutions, try " << static_cast<unsigned>(c) << "/30." << std::endl;
                acmo::grasp<pabSolution, 10> g;
                g.solve();
            }
            cur_time = std::time(NULL);
            std::cout << "Time: " << std::difftime(cur_time, start_time) << " seconds used." << std::endl;*/
        }
        closedir(dir);
    } else {
        std::cout << "Error to read directory." << std::endl;
    }

    return 0;
}
