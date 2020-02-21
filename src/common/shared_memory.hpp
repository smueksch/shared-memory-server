#ifndef COMMON_SHARED_MEMORY_HPP
#define COMMON_SHARED_MEMORY_HPP

#include <cstdio>     // perror
#include <fcntl.h>    // O_CREAT, O_RDWR
#include <sys/mman.h> // mmap, shm_open, shm_unlink, PROT_READ
#include <unistd.h>   // ftruncate
#include <string>

/*********************
 * Types & Constants *
 *********************/

// Type for file descriptors.
typedef int file_t;

// Name of shared memory object where clients request connections to
// server.
constexpr char CONNECTION_REQUEST_CHANNEL_NAME[] = "connection-requests";

// Size of the connections channel.
//
// New clients request a connection with the server by providing a unique
// integer ID that will be used to identify the shared memory object through
// which the server interacts with the client in question.
constexpr off_t CONNECTION_REQUEST_CHANNEL_SIZE = sizeof(int);

/***********
 * Classes *
 ***********/

// Class wrapping POSIX shared memory into a channel interface for server
// and client to use.
class Channel {
    public:
        // Channel access flags.
        enum Flags {
            READ_ONLY = O_RDONLY,
            READ_WRITE = O_RDWR
        };

        Channel();

        void create(std::string channel_name,
                    off_t channel_size,
                    Channel::Flags flags);
        void open(std::string channel_name,
                  Channel::Flags flags);
        void close();

        std::string get_name();

    private:
        std::string name;
        off_t size;
        file_t descriptor;
};

#endif
