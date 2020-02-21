#include "shared_memory.hpp"

Channel::Channel() : name(""), size(0), descriptor(-1) {
}

void Channel::create(std::string channel_name,
                     off_t channel_size,
                     Channel::Flags flags) {
    int fd = shm_open(channel_name.c_str(), O_CREAT | flags, 0666);

    if (fd < 0) {
        perror("Error opening connection request channel:");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(fd, channel_size) < 0) {
        perror("Error setting size of connection request channel:");
        exit(EXIT_FAILURE);
    }

    this.name = channel_name;
    this.size = size;
    this.descriptor = fd;
}

void Channel::open(std) {
    int fd = shm_open(channel_name.c_str(), O_CREAT | flags, 0666);

    if (fd < 0) {
        perror("Error opening connection request channel:");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(fd, channel_size) < 0) {
        perror("Error setting size of connection request channel:");
        exit(EXIT_FAILURE);
    }

    this.name = channel_name;
    this.descriptor = fd;
}

void Channel::close() {
    shm_unlink(this.name.c_str());
}

std::string Channel::get_name() {
    return this.name;
}
