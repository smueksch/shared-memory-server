#include "shared_memory.hpp"

/***********
 * Channel *
 ***********/

Channel::Channel() : name(""), size(0), descriptor(-1) {
}

void Channel::create(std::string channel_name,
                     off_t channel_size,
                     Channel::Flags flags) {
    int fd = shm_open(channel_name.c_str(), O_CREAT | flags, 0666);

    if (fd < 0) {
        perror("Error opening channel:");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(fd, channel_size) < 0) {
        perror("Error setting size of channel:");
        exit(EXIT_FAILURE);
    }

    map_to_memory();

    if ((shared_mem_ptr = mmap (NULL, sizeof (struct shared_memory), PROT_READ | PROT_WRITE, MAP_SHARED,
            fd_shm, 0)) == MAP_FAILED)
       error ("mmap");

    this.name = channel_name;
    this.size = size;
    this.descriptor = fd;
}

void Channel::open(std) {
    int fd = shm_open(channel_name.c_str(), O_CREAT | flags, 0666);

    if (fd < 0) {
        perror("Error opening channel:");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(fd, channel_size) < 0) {
        perror("Error setting size of channel:");
        exit(EXIT_FAILURE);
    }

    map_to_memory();

    this.name = channel_name;
    this.descriptor = fd;
}

void Channel::close() {
    shm_unlink(this.name.c_str());
}

std::string Channel::get_name() {
    return this.name;
}

/****************************
 * ConnectionRequestChannel *
 ****************************/

void ConnectionRequestChannel::create(std::string channel_name,
                                      Channel::Flags flags) {
    base::create(channel_name, CONNECTION_REQUEST_CHANNEL_SIZE, flags);
}

int ConnectionRequestChannel::read() {
    return this.data[START_INDEX];
}

void ConnectionRequestChannel::write(int value) {
    this.data[START_INDEX] = value;
}

void ConnectionRequestChannel::map_to_memory() {
    this.data = (int*)mmap(NULL,
                           CONNECTION_REQUEST_CHANNEL_SIZE,
                           PROT_READ | PROT_WRITE,
                           MAP_SHARED,
                           this.descriptor,
                           0);

    if (this.data == MAP_FAILED) {
        perror("Error mapping connection requrest into memory:");
    }
}

/***************
 * DataChannel *
 ***************/

void DataChannel::create(std::string channel_name,
                         Channel::Flags flags) {
    base::create(channel_name, DATA_CHANNEL_SIZE, flags);
}

Data DataChannel::read() {
    Data output;

    output.operation_flag = this.operation_flag[START_INDEX];
    output.bucket_id = this.bucket_id[START_INDEX];
    memcpy(output.data, this.data, NUM_DATA_CHANNEL_DATA_ENTRIES);

    return output;
}

void DataChannel::write(Data value) {
    this.operation_flag[START_INDEX] = value.operation_flag;
    this.bucket_id[START_INDEX] = value.bucket_id;
    memcpy(this.data, value.data, NUM_DATA_CHANNEL_DATA_ENTRIES);
}

void DataChannel::map_to_memory() {
    void* mem_ptr = (int*)mmap(NULL,
                               DATA_CHANNEL_SIZE,
                               PROT_READ | PROT_WRITE,
                               MAP_SHARED,
                               this.descriptor,
                               0);

    if (mem_ptr == MAP_FAILED) {
        perror("Error mapping connection requrest into memory:");
    }

    // Set the pointers according to the memory layout within the channel.
    this.operation_flag = (dc_flag_t*)mem_ptr;
    this.bucket_id = (dc_id_t*)(mem_ptr + sizeof(dc_flag_t));
    this.data = (dc_data_t*)(mem_ptr + sizeof(dc_flag_t) + sizeof(dc_data_t));
}
