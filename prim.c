#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#define MAX_NODES 50

struct node {
    int vertex;
    int weight;
    struct node* next;
};

struct Graph {
    int numVertices;
    struct node** adjLists;
};

struct node* createNode(int v, int weight) {
    struct node* newNode = (struct node*)malloc(sizeof(struct node));
    newNode->vertex = v;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

struct Graph* createGraph(int vertices) {
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->numVertices = vertices;
    graph->adjLists = (struct node**)malloc(vertices * sizeof(struct node*));
    for (int i = 0; i < vertices; i++) {
        graph->adjLists[i] = NULL;
    }
    return graph;
}

void addEdge(struct Graph* graph, int src, int dest, int weight) {
    struct node* newNode = createNode(dest, weight);
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;
    newNode = createNode(src, weight);
    newNode->next = graph->adjLists[dest];
    graph->adjLists[dest] = newNode;
}

typedef struct {
    int vertex, weight;
} HeapNode;


typedef struct {
    HeapNode heap[MAX_NODES];
    int size;
} MinHeap;

void swap(HeapNode* a, HeapNode* b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void heapifyUp(MinHeap* h, int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (h->heap[parent].weight > h->heap[index].weight) {
            swap(&h->heap[parent], &h->heap[index]);
            index = parent;
        } else {
            break;
        }
    }
}

void heapifyDown(MinHeap* h, int index) {
    int left, right, smallest;
    while (1) {
        left = 2 * index + 1;
        right = 2 * index + 2;
        smallest = index;
        if (left < h->size && h->heap[left].weight < h->heap[smallest].weight) {
            smallest = left;
        }
        if (right < h->size && h->heap[right].weight < h->heap[smallest].weight) {
            smallest = right;
        }
        if (smallest != index) {
            swap(&h->heap[index], &h->heap[smallest]);
            index = smallest;
        } else {
            break;
        }
    }
}

void push(MinHeap* h, int v, int w) {
    h->heap[h->size].vertex = v;
    h->heap[h->size].weight = w;
    heapifyUp(h, h->size);
    h->size++;
}

HeapNode pop(MinHeap* h) {
    HeapNode minNode = h->heap[0];
    h->heap[0] = h->heap[h->size - 1];
    h->size--;
    heapifyDown(h, 0);
    return minNode;
}

int isEmpty(MinHeap* h) {
    return h->size == 0;
}

void primMST(struct Graph* graph) {
    int V = graph->numVertices;
    int parent[V]; 
    int weight[V]; 
    int inMST[V]; 
    MinHeap pq = {0};

    for (int i = 0; i < V; i++) {
        weight[i] = INT_MAX;
        inMST[i] = 0;
    }

    srand(time(NULL));
    int x = rand() % V;
    printf("starting vertex %d\n", x);

    weight[x] = 0; 
    parent[x] = -1;
    push(&pq, x, weight[x]);

    while (!isEmpty(&pq)) {
        HeapNode minNode = pop(&pq);
        int u = minNode.vertex;
        if (inMST[u]) {
            continue;
        }
        inMST[u] = 1;
        struct node* temp = graph->adjLists[u];

        while (temp) {
            int v = temp->vertex;
            int w = temp->weight;
            if (!inMST[v] && w < weight[v]) {
                weight[v] = w;
                parent[v] = u;
                push(&pq, v, weight[v]);
            }
            temp = temp->next;
        }
    }

    printf("Edge    Weight\n");
    for (int i = 0; i < V; i++) {
        if (weight[i] != INT_MAX && parent[i] != -1) {
            printf("%d - %d     %d\n", parent[i], i, weight[i]);
        }
    }
}

int main() {
    int V; 
    FILE* fptr = fopen("graph.txt", "r");
    fscanf(fptr, "%d", &V);

    struct Graph* graph = createGraph(V);
    while (!feof(fptr)) {
        int a, b, c;
        fscanf(fptr, "%d %d %d", &a, &b, &c);
        addEdge(graph, a, b, c);
    }

    primMST(graph);
    return 0;
}
