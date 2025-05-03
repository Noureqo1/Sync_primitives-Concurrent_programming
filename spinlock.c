#include <stdio.h>
#include <windows.h>


volatile LONG lock_var = 0;  // 0 = unlocked, 1 = locked
volatile int count = 0;

void acquire_lock() {
    while (InterlockedExchange(&lock_var, 1) == 1) {
    }
}

void release_lock() {
    InterlockedExchange(&lock_var, 0);
}

DWORD WINAPI increment(LPVOID arg) {
    for(int i = 0; i < 1000000; i++) {
        acquire_lock();
        count++;
        release_lock();
    }
    return 0;
}

int main() {
    HANDLE thread0, thread1;
    
    printf("Initial count: %d\n", count);
    
    thread0 = CreateThread(NULL, 0, increment, NULL, 0, NULL);
    thread1 = CreateThread(NULL, 0, increment, NULL, 0, NULL);
    
    if (thread0 == NULL || thread1 == NULL) {
        printf("Error creating threads\n");
        return 1;
    }
    
    WaitForSingleObject(thread0, INFINITE);
    WaitForSingleObject(thread1, INFINITE);
    
    CloseHandle(thread0);
    CloseHandle(thread1);
    
    printf("Final count: %d\n", count);
    printf("Expected count: %d\n", 2000000);
    
    return 0;
}
