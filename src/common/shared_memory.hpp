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

// Offset to start reading from in channels.
constexpr int START_INDEX = 0;

// Number of characters that a DataChannel can hold.
constexpr int NUM_DATA_CHANNEL_DATA_ENTRIES = 59;

// Typedefs for DataChannel types.
// Allows for calculations relating to size of DataChannel fields to be
// easier maintainable.
typedef char dc_flag_t;
typedef int dc_id_t;
typedef char dc_data_t;

// Size of the data channel.
//
// Calculation: 1 byte for flag + Size of ID in byte + 1 byte for each data
// entry possible in a data channel.
//
// In theory, this should be 64 Byte (chosen arbitrarily).
constexpr int DATA_CHANNEL_SIZE = sizeof(dc_flag_t) + 
                                  sizeof(dc_id_t) +
                                  NUM_DATA_CHANNEL_DATA_ENTRIES * sizeof(dc_data_t);

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

    protected:
        // Map the shared memory file into memory.
        virtual void map_to_memory();

        std::string name;
        off_t size;
        file_t descriptor;
};

// Channel which is used to establish a connection between the server and the
// client.
class ConnectionRequestChannel : Channel {
    public:
        // Overwrite function with predetermined size.
        void create(std::string channel_name,
                    Channel::Flags flags);

        int read();
        void write(int value);

    private:
        int* data;

        void map_to_memory();
};

// Channel representing connection between server and client. Both server and
// client will have one instance of these.
class DataChannel : Channel {
    public:
        struct Data {
            dc_flag_t operation_flag;
            dc_id_t bucket_id;
            dc_data_t data[NUM_DATA_CHANNEL_DATA_ENTRIES];
        };

        // Overwrite function with predetermined size.
        void create(std::string channel_name,
                    Channel::Flags flags);

        DataChannel::Data read();
        void write(DataChannel::Data value);

    private:
        dc_flag_t* operation_flag;
        dc_id_t* bucket_id;
        dc_data_t* data;

        void map_to_memory();
};

#endif
