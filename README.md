# Synchronization Primitives and Concurrent Programming

This project demonstrates fundamental concepts in synchronization and concurrent programming through three progressive implementations. Each implementation builds upon the lessons learned from the previous one.

## Project Components

### 1. Peterson's Algorithm (`peterson.c`)
Demonstrates why regular read/write operations are insufficient for synchronization on modern processors.
- Shows race conditions in action
- Illustrates memory reordering effects
- Demonstrates why stronger synchronization primitives are needed

### 2. Spinlock Implementation (`spinlock.c`)
Implements proper synchronization using atomic test-and-set operations.
- Uses Windows' `InterlockedExchange` for atomic operations
- Demonstrates reliable mutual exclusion
- Shows how hardware support enables correct synchronization

### 3. Concurrent Linked List (`concurrent_list.c`)
Implements a thread-safe data structure using the synchronization concepts.
- Protected critical sections
- Safe memory management
- Concurrent insert and remove operations
- Multiple threads operating simultaneously

## Requirements
- Windows operating system
- GCC compiler (MinGW or similar)
- Command line terminal

## Output

### Peterson's Algorithm
![Peterson's Algorithm](Output/Screenshot%202025-05-03%20213830.png)
Output: The final count is less than 2,000,000, demonstrating synchronization failure.

### Spinlock Implementation
![Spinlock Implementation](Output/Screenshot%202025-05-03%20213741.png)
Output: The final count is exactly 2,000,000, showing correct synchronization.

### Concurrent Linked List
![Peterson's Algorithm](Output/Screenshot%202025-05-03%20213514.png)
Output: Shows the final list state and node count after concurrent operations.

## Understanding the Results

1. **Peterson's Algorithm**: 
   - If you see a final count less than 2,000,000, this demonstrates that regular read/write operations aren't sufficient for synchronization
   - The discrepancy occurs due to modern CPU optimizations and memory reordering

2. **Spinlock**:
   - The final count should be exactly 2,000,000
   - This demonstrates that atomic operations provide reliable synchronization

3. **Concurrent List**:
   - The final list will contain a variable number of nodes
   - All operations are thread-safe thanks to proper synchronization
   - The list remains consistent despite concurrent modifications
