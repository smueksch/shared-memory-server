#include <cstdlib>   // atoi
#include <iostream>  // cout, endl

void print_usage() {
    std::cout << "Usage:" << std::endl;
    std::cout << "$ server <hash table size>" << std::endl;
}

int main(int arg_c, char** arg_v) {
    if (arg_c < 2) {
        std::cout << "Hash table size required!" << std::endl << std::endl;

        print_usage();

        return 1;
    } else {
        int requested_size = atoi(arg_v[1]);

        std::cout << "Initialising hash table with " << requested_size << " bucket(s)" << std::endl;

        return 0;
    }
}
