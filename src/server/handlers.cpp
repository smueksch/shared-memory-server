#include "handlers.hpp"

void handle_server_shutdown(int signal) {
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
