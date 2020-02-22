#include "shared_memory.hpp"

/***********
 * Channel *
 ***********/

Channel::Channel() : name(""), size(0), file_descriptor(-1) {
}

Channel::~Channel() {
}

void Channel::create(std::string channel_name,
                     off_t channel_size,
                     Channel::Flags flags) {
    name = channel_name;
    size = channel_size;

    file_descriptor = shm_open(name.c_str(), O_CREAT | flags, 0666);

    if (file_descriptor < 0) {
        perror("Error opening channel:");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(file_descriptor, size) < 0) {
        perror("Error setting size of channel:");
        exit(EXIT_FAILURE);
    }

    map_to_memory();

    size = channel_size;
}

void Channel::open(std::string channel_name,
                   Channel::Flags flags) {
    name = channel_name;

    file_descriptor = shm_open(name.c_str(), flags, 0666);

    if (file_descriptor < 0) {
        perror("Error opening channel:");
        exit(EXIT_FAILURE);
    }

    map_to_memory();
}

void Channel::close() {
    shm_unlink(name.c_str());
}

std::string Channel::get_name() {
    return name;
}

/****************************
 * ConnectionRequestChannel *
 ****************************/

ConnectionRequestChannel::ConnectionRequestChannel() : Channel(), data(NULL) {
}

ConnectionRequestChannel::~ConnectionRequestChannel() {
}

void ConnectionRequestChannel::create(std::string channel_name,
                                      Channel::Flags flags) {
    Channel::create(channel_name, CONNECTION_REQUEST_CHANNEL_SIZE, flags);
}

int ConnectionRequestChannel::read() {
    return data[START_INDEX];
}

void ConnectionRequestChannel::write(int value) {
    data[START_INDEX] = value;
}

void ConnectionRequestChannel::map_to_memory() {
    data = (int*)mmap(NULL,
                      CONNECTION_REQUEST_CHANNEL_SIZE,
                      PROT_READ | PROT_WRITE,
                      MAP_SHARED,
                      file_descriptor,
                      0);

    if (data == MAP_FAILED) {
        perror("Error mapping connection requrest into memory:");
    }
}

/***************
 * DataChannel *
 ***************/

DataChannel::DataChannel() : Channel(), operation_flag(NULL), bucket_id(NULL), data(NULL) {
}

DataChannel::~DataChannel() {
}

void DataChannel::create(std::string channel_name,
                         Channel::Flags flags) {
    Channel::create(channel_name, DATA_CHANNEL_SIZE, flags);
}

DataChannel::Data DataChannel::read() {
    Data output;

    output.operation_flag = operation_flag[START_INDEX];
    output.bucket_id = bucket_id[START_INDEX];
    memcpy(output.data, data, NUM_DATA_CHANNEL_DATA_ENTRIES);

    return output;
}

void DataChannel::write(DataChannel::Data value) {
    operation_flag[START_INDEX] = value.operation_flag;
    bucket_id[START_INDEX] = value.bucket_id;
    memcpy(data, value.data, NUM_DATA_CHANNEL_DATA_ENTRIES);
}

void DataChannel::map_to_memory() {
    char* mem_ptr = (char*)mmap(NULL,
                               DATA_CHANNEL_SIZE,
                               PROT_READ | PROT_WRITE,
                               MAP_SHARED,
                               file_descriptor,
                               0);

    if (mem_ptr == MAP_FAILED) {
        perror("Error mapping connection requrest into memory:");
    }

    // Set the pointers according to the memory layout within the channel.
    operation_flag = (dc_flag_t*)mem_ptr;
    bucket_id = (dc_id_t*)(mem_ptr + sizeof(dc_flag_t));
    data = (dc_data_t*)(mem_ptr + sizeof(dc_flag_t) + sizeof(dc_data_t));
}
