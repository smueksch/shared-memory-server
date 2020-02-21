#include "handlers.hpp"

void handle_server_shutdown(int signal) {
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
