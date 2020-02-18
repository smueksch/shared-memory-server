#include <cstdlib>    // atoi
#include <csignal>    // SIGINT, signal
#include <iostream>   // cout, endl
#include <string>     // string
#include <thread>     // sleep_for
#include <chrono>     // chrono::seconds

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
            exit(EXIT_SUCCESS);
        }
    } while(response != "n" && response != "N");
}

int main(int arg_c, char** arg_v) {
    if (arg_c < 2) {
        std::cout << "Hash table size required!" << std::endl << std::endl;

        print_usage();

        return EXIT_FAILURE;
    } else {
        int requested_size = atoi(arg_v[1]);

        std::cout << "Initialising hash table with " << requested_size << " bucket(s)" << std::endl;

        // Set up shutdown handler to terminate program correctly.
        signal(SIGINT, handle_shutdown);

        // Main loop, poll messages from clients
        while (true) {
            std::cout << "Polling clients..." << std::endl;

            // Avoid spamming the command line:
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        return EXIT_SUCCESS;
    }
}
