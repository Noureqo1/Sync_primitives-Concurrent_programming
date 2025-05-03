#include <stdio.h>
#include <windows.h>

volatile int count = 0;
volatile int request[2] = {0, 0};
volatile int turn = 0;

void lock(int id);
void unlock(int id);

// Lock implementation using Peterson's algorithm
void lock(int id) {
    int other = 1 - id;
    request[id] = 1;
    turn = other;
    while(request[other] && turn == other);
}

void unlock(int id) {
    request[id] = 0;
}

DWORD WINAPI increment(LPVOID arg) {
    int id = *(int*)arg;
    for(int i = 0; i < 1000000; i++) {
        lock(id);
        count++;
        unlock(id);
    }
    return 0;
}

int main() {
    HANDLE thread0, thread1;
    int id0 = 0, id1 = 1;
    
    printf("Initial count: %d\n", count);
    
    thread0 = CreateThread(NULL, 0, increment, &id0, 0, NULL);
    thread1 = CreateThread(NULL, 0, increment, &id1, 0, NULL);
    
    WaitForSingleObject(thread0, INFINITE);
    WaitForSingleObject(thread1, INFINITE);
    
    CloseHandle(thread0);
    CloseHandle(thread1);
    
    printf("Final count: %d\n", count);
    printf("Expected count: %d\n", 2000000);
    
    return 0;
}
