#include <cstdlib>    // EXIT_SUCCESS
#include <csignal>    // SIGINT, signal
#include <iostream>   // cin, cout, endl
#include <string>     // string
#include <fcntl.h>    // O_WRONLY
#include <sys/mman.h> // mmap, shm_open, PROT_WRITE
#include <unistd.h>   // ftruncate

// Name of shared memory object.
#define SHM_NAME "comm-file"

// Size of shard memory object.
// Give enough space to store 10 signed integers.
#define SHM_SIZE 10 * sizeof(int)

void handle_shutdown(int signal) {
    std::string response;

    std::cout << std::endl;

    do {
        std::cout << "Shutdown client? [y/n] ";
        std::cin >> response;

        if (response == "y" || response == "Y") {
            exit(EXIT_SUCCESS);
        }
    } while(response != "n" && response != "N");
}

int main(int arg_c, char** arg_v) {
    // Set up shutdown handler to terminate program correctly.
    signal(SIGINT, handle_shutdown);

    // Create shared memory object, get file descriptor.
    // TODO: Check that a server is active and has created the shared memory!
    int shared_memory = shm_open(SHM_NAME, O_WRONLY, 0666);

    // Memory map the shared memory object.
    int* data = (int*)mmap(0, SHM_SIZE, PROT_WRITE, MAP_SHARED, shared_memory, 0);

    // Poll for new messages to send and store in shared memory.
    while(true) {
        std::cout << "Message to server: ";
        // TODO: Put checks that what is read from console is actually an
        // integer!
        std::cin >> data[0];
    }

    return EXIT_SUCCESS;
}
