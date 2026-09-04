cat << 'EOF' > README.md
# Custom Memory Allocator

A minimal dynamic memory allocator implementation in C11. It bypasses the standard library runtime (`malloc` and `free`) and requests raw memory pages directly from the operating system kernel via `mmap`.

I wrote this project to understand how memory allocation works under the hood—specifically tracking memory chunks, handling alignment, and dealing with fragmentation.

## How It Works

* **Memory Pool**: Instead of using `sbrk()`, the allocator initializes a 4 MB memory pool using `mmap()` with `MAP_ANONYMOUS | MAP_PRIVATE` flags.
* **Metadata**: Every allocated or free chunk is preceded by a 24-byte header (`struct BlockHeader`) that stores the block size, an allocation flag (`is_free`), and a pointer to the next block in a singly linked list.
* **8-Byte Alignment**: Payload sizes are rounded up to 8-byte boundaries using bitwise masking to maintain standard architecture alignment.
* **Splitting**: When an allocation request is smaller than an available free block, the block is split into two pieces: one for the requested payload and another returned to the free list.
* **Coalescing**: When memory is freed, adjacent free blocks are merged together to prevent external fragmentation.

## Project Structure

* `allocator.h`: Header definition and block metadata struct.
* `allocator.c`: Implementation of `init_memory_pool`, `my_malloc`, and `my_free`.
* `main.c`: Test program demonstrating allocation, payload verification, and coalescing.
* `Makefile`: Build configuration with strict compiler warnings.

## Build and Run

Tested on macOS (Apple Clang) and Ubuntu (GCC).

Compile the project:
make

Run the test binary:
make test

Clean build files:
make clean

Sample Output
Running make test demonstrates initial allocation, splitting, and merging of freed blocks:
Starting custom memory allocator test...
Initial Allocation State:
=== Memory Pool State ===
Block 0 | Addr: 0x10943c000 | Payload Size:       24 bytes | Status: ALLOCATED
Block 1 | Addr: 0x10943c030 | Payload Size:       64 bytes | Status: ALLOCATED
Block 2 | Addr: 0x10943c088 | Payload Size:  4194144 bytes | Status: FREE
=========================

Freeing 'text' pointer...

=== Memory Pool State ===
Block 0 | Addr: 0x10943c000 | Payload Size:       24 bytes | Status: ALLOCATED
Block 1 | Addr: 0x10943c030 | Payload Size:  4194232 bytes | Status: FREE
=========================

Freeing 'numbers' pointer...

=== Memory Pool State ===
Block 0 | Addr: 0x10943c000 | Payload Size:  4194280 bytes | Status: FREE
=========================
