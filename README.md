# Overview

This is a small server-client project to demonstrate communication using a shared memory buffer with [POSIX shm](http://man7.org/linux/man-pages/man7/shm_overview.7.html) and concurrent operations on a server-side hash table.

# Compilation

In the root of this project, execute the following:

```
$ cmake .
$ cmake --build .
```

The compiled binaries will be places in a newly generated ```bin``` folder in the project root directory.

# Usage

## Server

The ```server``` executable takes one argument, the size for the internal hash table:

```
$ server <hash table size>
```

Example usage:

```
$ server 42
```

After this, the ```server``` executable will poll messages from the shared memory buffer sent by clients and display the current state of the shared memory buffer.

If you wish to shut down the ```server``` gracefully, simply press ```Ctrl + C``` and follow the prompt.

## Client

The ```client``` executable takes no arguments and continuously asks for messages to be sent to the server via the shared memory buffer:

```
$ cient
```

If you wish to shut down the ```client``` gracefully, simply press ```Ctrl + C``` and follow the prompt.

# Resources Used

- [CMake Tutorial](https://cmake.org/cmake/help/latest/guide/tutorial/index.html)
- [Shutdown Signal Handling](https://stackoverflow.com/questions/1641182/how-can-i-catch-a-ctrl-c-event)
- [POSIX shared memory](https://www.geeksforgeeks.org/posix-shared-memory-api/)
