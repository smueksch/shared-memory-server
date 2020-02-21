#ifndef COMMON_SHARED_MEMORY_HPP
#define COMMON_SHARED_MEMORY_HPP

#include <fcntl.h>    // O_CREAT, O_RDWR
#include <sys/mman.h> // mmap, shm_open, shm_unlink, PROT_READ
#include <unistd.h>   // ftruncate

// Name of shared memory object.
#define SHM_NAME "comm-file"

// Size of shard memory object.
// Give enough space to store 10 signed integers.
#define SHM_SIZE 10 * sizeof(int)

#endif
