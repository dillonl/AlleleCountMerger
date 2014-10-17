#include "AlleleCountManager.h"

#include <string>
#include <algorithm>
#include <time.h>
#include <iostream>

int main(int argc, char** argv)
{
    // clock_t begin = clock(); // for timing
    std::vector<std::string> file_names;
    for (unsigned int i = 1; i < argc; ++i)
    {
        file_names.push_back(std::string(argv[i]));
    }
    AlleleCount::AlleleCountManager* allele_count_manager = AlleleCount::AlleleCountManager::Instance();
    std::for_each(file_names.begin(), file_names.end(), [allele_count_manager](std::string& file_name){allele_count_manager->addAlleleCountFile(file_name);});
    allele_count_manager->run();

    /* // for timing
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    std::cout << "Run time in seconds: " << elapsed_secs << std::endl;
    */
    return 0;
}
