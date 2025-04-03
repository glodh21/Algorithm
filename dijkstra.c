#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Global counter to count heap comparisons.
unsigned long long c = 0;

typedef struct {
    int v;      
    int weight;
} Edge;

typedef struct {
    int distance;
    int vertex;  
} HeapNode;

typedef struct {
    HeapNode *arr;
    int size;
    int capacity;
} Heap;

Heap* createHeap(int capacity) {
    Heap *heap = malloc(sizeof(Heap));
    if (!heap) {
        perror("Unable to allocate heap");
        exit(EXIT_FAILURE);
    }
    heap->arr = malloc(capacity * sizeof(HeapNode));
    if (!heap->arr) {
        perror("Unable to allocate heap array");
        free(heap);
        exit(EXIT_FAILURE);
    }
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void freeHeap(Heap *heap) {
    if (heap) {
        free(heap->arr);
        free(heap);
    }
}

void swap(HeapNode *a, HeapNode *b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void push(Heap *heap, HeapNode element) {
    if (heap->size >= heap->capacity) {
        heap->capacity = (heap->capacity == 0) ? 1 : heap->capacity * 2;
        HeapNode *newArr = realloc(heap->arr, heap->capacity * sizeof(HeapNode));
        if (!newArr) {
            perror("Realloc failed in push");
            freeHeap(heap);
            exit(EXIT_FAILURE);
        }
        heap->arr = newArr;
    }
    int i = heap->size;
    heap->arr[i] = element;
    heap->size++;

    // Bubble-up to maintain the heap property.
    while (i > 0) {
        int parent = (i - 1) / 2;
        c++;  // Increment counter for each comparison.
        if (heap->arr[i].distance < heap->arr[parent].distance) {
            swap(&heap->arr[i], &heap->arr[parent]);
            i = parent;
        } else {
            break;
        }
    }
}

HeapNode pop(Heap *heap) {
    if (heap->size == 0) {
        HeapNode dummy = {0, -1};
        return dummy;
    }
    HeapNode result = heap->arr[0];
    heap->arr[0] = heap->arr[heap->size - 1];
    heap->size--;

    int i = 0;
    // Bubble-down to restore the heap property.
    while (1) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;
        if (left < heap->size) {
            c++;  // Increment counter for left child comparison.
            if (heap->arr[left].distance < heap->arr[smallest].distance)
                smallest = left;
        }
        if (right < heap->size) {
            c++;  // Increment counter for right child comparison.
            if (heap->arr[right].distance < heap->arr[smallest].distance)
                smallest = right;
        }
        if (smallest != i) {
            swap(&heap->arr[i], &heap->arr[smallest]);
            i = smallest;
        } else {
            break;
        }
    }
    return result;
}

int getIndex(int *labels, int n, int label) {
    for (int i = 0; i < n; i++) {
        if (labels[i] == label)
            return i;
    }
    return -1;
}

int* dijkstra(int n, Edge **adj, int *adj_sizes, int start) {
    int *dist = malloc(n * sizeof(int));
    if (!dist) {
        perror("Failed to allocate distance array");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++)
        dist[i] = INT_MAX;
    dist[start] = 0;

    Heap *heap = createHeap(10);
    push(heap, (HeapNode){0, start});

    while (heap->size > 0) {
        HeapNode node = pop(heap);
        int u = node.vertex;
        int current_dist = node.distance;

        if (current_dist > dist[u])
            continue;

        for (int i = 0; i < adj_sizes[u]; i++) {
            Edge e = adj[u][i];
            int v = e.v;
            int weight = e.weight;
            if (dist[u] != INT_MAX && dist[v] > dist[u] + weight) {
                dist[v] = dist[u] + weight;
                push(heap, (HeapNode){dist[v], v});
            }
        }
    }

    freeHeap(heap);
    return dist;
}

int main() {
    int n, m;
    printf("Enter the number of vertices: ");
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error reading number of vertices.\n");
        exit(EXIT_FAILURE);
    }
    int *labels = malloc(n * sizeof(int));
    if (!labels) {
        perror("Failed to allocate vertex labels array");
        exit(EXIT_FAILURE);
    }
    printf("Enter the vertex labels:\n");
    for (int i = 0; i < n; i++) {
        if (scanf("%d", &labels[i]) != 1) {
            fprintf(stderr, "Error reading vertex label.\n");
            exit(EXIT_FAILURE);
        }
    }

    printf("Enter the number of edges: ");
    if (scanf("%d", &m) != 1) {
        fprintf(stderr, "Error reading number of edges.\n");
        exit(EXIT_FAILURE);
    }

    Edge **adj = calloc(n, sizeof(Edge *));
    int *adj_sizes = calloc(n, sizeof(int));
    if (!adj || !adj_sizes) {
        perror("Memory allocation failed for adjacency lists");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < m; i++) {
        int u_label, v_label, w;
        printf("Enter edge (u v w) using vertex labels: ");
        if (scanf("%d %d %d", &u_label, &v_label, &w) != 3) {
            fprintf(stderr, "Invalid input for edge.\n");
            exit(EXIT_FAILURE);
        }
        int u = getIndex(labels, n, u_label);
        int v = getIndex(labels, n, v_label);
        if (u == -1 || v == -1) {
            fprintf(stderr, "Invalid vertex label in edge (%d, %d).\n", u_label, v_label);
            exit(EXIT_FAILURE);
        }
        adj_sizes[u]++;
        Edge *newList = realloc(adj[u], adj_sizes[u] * sizeof(Edge));
        if (!newList) {
            perror("Realloc failed for adjacency list");
            exit(EXIT_FAILURE);
        }
        adj[u] = newList;
        adj[u][adj_sizes[u] - 1] = (Edge){v, w};
    }

    int start_label;
    printf("Enter the starting vertex label: ");
    if (scanf("%d", &start_label) != 1) {
        fprintf(stderr, "Error reading starting vertex label.\n");
        exit(EXIT_FAILURE);
    }
    int start = getIndex(labels, n, start_label);
    if (start == -1) {
        fprintf(stderr, "Starting vertex label %d not found.\n", start_label);
        exit(EXIT_FAILURE);
    }

    int *dist = dijkstra(n, adj, adj_sizes, start);

    printf("\nShortest distances from vertex %d:\n", start_label);
    for (int i = 0; i < n; i++) {
        printf("Vertex %d: ", labels[i]);
        if (dist[i] == INT_MAX)
            printf("INF\n");
        else
            printf("%d\n", dist[i]);
    }

    // Display the global counter value.
    printf("\nGlobal counter c (approx O(E log V)): %llu\n", c);

    // Clean up allocated memory.
    for (int i = 0; i < n; i++)
        free(adj[i]);
    free(adj);
    free(adj_sizes);
    free(labels);
    free(dist);

    return 0;
}
