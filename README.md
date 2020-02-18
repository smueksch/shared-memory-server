# Overview

This is a small server-client project to demonstrate communication using a shared memory buffer with [POSIX shm](http://man7.org/linux/man-pages/man7/shm_overview.7.html) and concurrent operations on a server-side hash table.

# Compilation

In the root of this project, execute the following:

```console
$ cmake .
$ cmake --build .
```

# Usage

The ```server``` executable takes one argument, the size for the internal hash table:

```console
$ server <hash table size>
```

Example usage:

```console
$ server 42
```

# Resources Used

- [CMake Tutorial](https://cmake.org/cmake/help/latest/guide/tutorial/index.html)
- [Shutdown Signal Handling](https://stackoverflow.com/questions/1641182/how-can-i-catch-a-ctrl-c-event)
