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
    // Set up shutdown handler to terminate program correctly.
    signal(SIGINT, handle_server_shutdown);

    if (arg_c < REQUIRED_CLI_ARGUMENTS) {
        std::cout << "Hash table size required!" << std::endl << std::endl;

        print_usage();

        return EXIT_FAILURE;
    } else {
        int requested_size = atoi(arg_v[1]);

        std::cout << "Initialising hash table with ";
        std::cout << requested_size << " bucket(s)" << std::endl;

        ConnectionRequestChannel cr_channel;

        cr_channel.create(CONNECTION_REQUEST_CHANNEL_NAME,
                          Channel::Flags::READ_WRITE);

        cr_channel.close();

        return EXIT_SUCCESS;
    }
}
