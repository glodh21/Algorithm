#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#define MAX_LEVEL 6   // Maximum number of levels in the skip list
#define P 0.5         // Probability for increasing level

// Special keys (sentinel values)
#define NEG_INF INT_MIN
#define POS_INF INT_MAX

typedef struct Node {
    int key;
    struct Node **forward; // Array of forward pointers
} Node;

typedef struct SkipList {
    int level;    // Current highest level in the skip list
    Node *header; // Header node (with NEG_INF)
    Node *tail;   // Tail sentinel node (with POS_INF)
} SkipList;

unsigned long comparisons = 0;  // Global counter for comparisons

// Create a new node with a given level and key.
Node* createNode(int level, int key) {
    Node *n = (Node *)malloc(sizeof(Node));
    n->key = key;
    n->forward = (Node **)malloc(sizeof(Node*) * (level + 1));
    for (int i = 0; i <= level; i++) {
        n->forward[i] = NULL;
    }
    return n;
}

// Create an empty skip list with sentinel nodes.
SkipList* createSkipList() {
    SkipList *list = (SkipList *)malloc(sizeof(SkipList));
    list->level = 0;
    list->header = createNode(MAX_LEVEL, NEG_INF);
    list->tail = createNode(MAX_LEVEL, POS_INF);
    
    for (int i = 0; i <= MAX_LEVEL; i++) {
        list->header->forward[i] = list->tail;
    }
    return list;
}

// Free all memory used by the skip list.
void freeSkipList(SkipList *list) {
    Node *current = list->header->forward[0];
    while (current != list->tail) {
        Node *next = current->forward[0];
        free(current->forward);
        free(current);
        current = next;
    }
    free(list->header->forward);
    free(list->header);
    free(list->tail->forward);
    free(list->tail);
    free(list);
}

// Random level generator using coin tosses.
int randomLevel() {
    int lvl = 0;
    while (((double)rand() / RAND_MAX) < P && lvl < MAX_LEVEL)
        lvl++;
    return lvl;
}

// Insert a key into the skip list.
void insert(SkipList *list, int key) {
    Node *update[MAX_LEVEL + 1];
    Node *current = list->header;
    
    for (int i = list->level; i >= 0; i--) {
        while (current->forward[i] != list->tail && 
               (++comparisons && current->forward[i]->key < key))
            current = current->forward[i];
        update[i] = current;
    }
    
    current = current->forward[0];
    if (current != list->tail && current->key == key) {
        comparisons++;
        printf("Key %d already exists.\n", key);
        printf("Comparisons: %lu\n", comparisons);
        comparisons = 0;
        return;
    }
    
    int lvl = randomLevel();
    if (lvl > list->level) {
        for (int i = list->level + 1; i <= lvl; i++) {
            update[i] = list->header;
        }
        list->level = lvl;
    }
    
    Node *n = createNode(lvl, key);
    for (int i = 0; i <= lvl; i++) {
        n->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = n;
    }
    printf("Inserted key %d at level %d.\n", key, lvl);
    printf("Comparisons: %lu\n", comparisons);
    comparisons = 0;
}

// Search for a key in the skip list.
Node* search(SkipList *list, int key) {
    Node *current = list->header;
    for (int i = list->level; i >= 0; i--) {
        while (current->forward[i] != list->tail && 
               (++comparisons && current->forward[i]->key < key))
            current = current->forward[i];
    }
    current = current->forward[0];
    if (current != list->tail && current->key == key) {
        comparisons++;
        printf("Found key %d.\n", key);
        printf("Comparisons: %lu\n", comparisons);
        comparisons = 0;
        return current;
    }
    printf("Key %d not found.\n", key);
    printf("Comparisons: %lu\n", comparisons);
    comparisons = 0;
    return NULL;
}

// Delete a key from the skip list.
void delete(SkipList *list, int key) {
    Node *update[MAX_LEVEL + 1];
    Node *current = list->header;
    
    for (int i = list->level; i >= 0; i--) {
        while (current->forward[i] != list->tail && 
               (++comparisons && current->forward[i]->key < key))
            current = current->forward[i];
        update[i] = current;
    }
    
    current = current->forward[0];
    if (current == list->tail || current->key != key) {
        printf("Key %d not found.\n", key);
        printf("Comparisons: %lu\n", comparisons);
        comparisons = 0;
        return;
    }
    
    for (int i = 0; i <= list->level; i++) {
        if (update[i]->forward[i] != current)
            break;
        update[i]->forward[i] = current->forward[i];
    }
    
    free(current->forward);
    free(current);
    
    while (list->level > 0 && list->header->forward[list->level] == list->tail)
        list->level--;
    
    printf("Deleted key %d.\n", key);
    printf("Comparisons: %lu\n", comparisons);
    comparisons = 0;
}

// Display the skip list.
void display(SkipList *list) {
    printf("\n***** Skip List *****\n");
    for (int i = list->level; i >= 0; i--) {
        Node *node = list->header->forward[i];
        printf("Level %d: -infinity", i);
        while (node != list->tail) {
            printf(" -> %d", node->key);
            node = node->forward[i];
        }
        printf(" -> +infinity\n");
    }
}

int main() {
    srand(time(NULL));
    SkipList *list = createSkipList();
    int choice, key;
    
    while (1) {
        printf("\nMenu:\n1. Insert\n2. Delete\n3. Search\n4. Display\n5. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                printf("Key to insert: ");
                scanf("%d", &key);
                if (key == NEG_INF || key == POS_INF) {
                    printf("Cannot insert special keys.\n");
                    break;
                }
                insert(list, key);
                display(list);
                break;
            case 2:
                printf("Key to delete: ");
                scanf("%d", &key);
                if (key == NEG_INF || key == POS_INF) {
                    printf("Cannot delete special keys.\n");
                    break;
                }
                delete(list, key);
                display(list);
                break;
            case 3:
                printf("Key to search: ");
                scanf("%d", &key);
                search(list, key);
                break;
            case 4:
                display(list);
                break;
            case 5:
                freeSkipList(list);
                exit(0);
            default:
                printf("Invalid choice.\n");
        }
    }
    
    return 0;
}