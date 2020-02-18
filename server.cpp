#include <cstdlib>    // atoi, EXIT_SUCCESS, EXIT_FAILURE
#include <csignal>    // SIGINT, signal
#include <iostream>   // cin, cout, endl
#include <string>     // string
#include <thread>     // sleep_for
#include <chrono>     // chrono::seconds
#include <fcntl.h>    // O_CREAT, O_RDWR
#include <sys/mman.h> // mmap, shm_open, shm_unlink, PROT_READ
#include <unistd.h>   // ftruncate

// Number of required command-line arguments.
#define REQUIRED_CLI_ARGUMENTS 2

// Name of shared memory object.
#define SHM_NAME "comm-file"

// Size of shard memory object.
// Give enough space to store 10 signed integers.
#define SHM_SIZE 10 * sizeof(int)

void print_usage() {
    std::cout << "Usage:" << std::endl;
    std::cout << "$ server <hash table size>" << std::endl;
}

void handle_shutdown(int signal) {
    std::string response;

    std::cout << std::endl;

    do {
        std::cout << "Shutdown server? [y/n] ";
        std::cin >> response;

        if (response == "y" || response == "Y") {
            // Server is shutting down, remove shared memory object.
            shm_unlink(SHM_NAME);

            exit(EXIT_SUCCESS);
        }
    } while(response != "n" && response != "N");
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
        signal(SIGINT, handle_shutdown);

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
