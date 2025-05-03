#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

typedef struct Node {
    int value;
    struct Node* next;
} Node;

typedef struct List {
    Node* head;
    volatile LONG lock;
} List;

List* create_list() {
    List* list = (List*)malloc(sizeof(List));
    list->head = NULL;
    list->lock = 0;
    return list;
}

void acquire_lock(List* list) {
    while (InterlockedExchange(&list->lock, 1) == 1) {
    }
}


void release_lock(List* list) {
    InterlockedExchange(&list->lock, 0);
}

void insert(List* list, int value) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->value = value;
    
    acquire_lock(list);
    new_node->next = list->head;
    list->head = new_node;
    release_lock(list);
}

int remove_value(List* list, int value) {
    acquire_lock(list);
    
    Node* current = list->head;
    Node* prev = NULL;
    
    while (current != NULL && current->value != value) {
        prev = current;
        current = current->next;
    }
    
    if (current == NULL) {
        release_lock(list);
        return 0; 
    }
    
    if (prev == NULL) {
        list->head = current->next;
    } else {
        prev->next = current->next;
    }
    
    free(current);
    release_lock(list);
    return 1;  
}

void print_list(List* list) {
    acquire_lock(list);
    Node* current = list->head;
    printf("List: ");
    while (current != NULL) {
        printf("%d -> ", current->value);
        current = current->next;
    }
    printf("NULL\n");
    release_lock(list);
}

DWORD WINAPI worker(LPVOID arg) {
    List* list = (List*)arg;
    for (int i = 0; i < 1000; i++) {
        // Insert a value
        insert(list, i);
        
        // Try to remove a random value (50% of the time)
        if (rand() % 2) {
            remove_value(list, rand() % 1000);
        }
    }
    return 0;
}

int main() {
    List* list = create_list();
    HANDLE threads[3];
    
    for (int i = 0; i < 3; i++) {
        threads[i] = CreateThread(NULL, 0, worker, list, 0, NULL);
        if (threads[i] == NULL) {
            printf("Error creating thread %d\n", i);
            return 1;
        }
    }
    
    WaitForMultipleObjects(3, threads, TRUE, INFINITE);
    
    for (int i = 0; i < 3; i++) {
        CloseHandle(threads[i]);
    }
    
    printf("Final ");
    print_list(list);
    
    int count = 0;
    Node* current = list->head;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    printf("Final list contains %d nodes\n", count);
    while (list->head != NULL) {
        Node* temp = list->head;
        list->head = list->head->next;
        free(temp);
    }
    free(list);
    
    return 0;
}
