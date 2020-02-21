#include <cstdlib>    // atoi, EXIT_SUCCESS, EXIT_FAILURE
#include <csignal>    // SIGINT, signal
#include <iostream>   // cin, cout, endl
#include <string>     // string
#include <thread>     // sleep_for
#include <chrono>     // chrono::seconds

#include "handlers.hpp"
#include "shared_memory.hpp"

// Number of required command-line arguments.
#define REQUIRED_CLI_ARGUMENTS 2

void print_usage() {
    std::cout << "Usage:" << std::endl;
    std::cout << "$ server <hash table size>" << std::endl;
}

int main(int arg_c, char** arg_v) {
    if (arg_c < REQUIRED_CLI_ARGUMENTS) {
        std::cout << "Hash table size required!" << std::endl << std::endl;

        print_usage();

        return EXIT_FAILURE;
    } else {
        int requested_size = atoi(arg_v[1]);

        std::cout << "Initialising hash table with " << requested_size << " bucket(s)" << std::endl;

        // Set up shutdown handler to terminate program correctly.
        signal(SIGINT, handle_server_shutdown);

        // Create shared memory object, get file descriptor.
        int shared_memory = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);

        // Configure size of shared memory object: 4KiB.
        ftruncate(shared_memory, SHM_SIZE);

        // Memory map the shared memory object.
        int* data = (int*)mmap(0, SHM_SIZE, PROT_READ, MAP_SHARED, shared_memory, 0);

        // Poll for client messages, pausing to not busy-poll.
        while(true) {
            std::cout << "Shared memory content: ";
            std::cout << data[0] << std::endl;

            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        return EXIT_SUCCESS;
    }
}
