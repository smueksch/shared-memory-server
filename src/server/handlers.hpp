#ifndef SERVER_HANDLERS_HPP
#define SERVER_HANDLERS_HPP

#include <cstdio>
#include <string>
#include <iostream>

#include "shared_memory.hpp"

// Ask user if they really want to shut down the running process after
// sending the appropriate signal through key combinations.
//
// If the user declines shutting down the process it continues as normal.
void handle_server_shutdown(int signal);

#endif
