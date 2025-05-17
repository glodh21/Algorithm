#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#define MAX_NODES 50

typedef struct Node {
    int vertex;
    int weight;
    struct Node* next;
} Node;

typedef struct Graph {
    int numvertices;
    Node** adjlists;
} Graph;

Node* createNode(int v, int weight) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->vertex = v;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

Graph* createGraph(int vertices) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->numvertices = vertices;
    graph->adjlists = (Node**)malloc(vertices * sizeof(Node*));
    for (int i = 0; i < vertices; i++) {
        graph->adjlists[i] = NULL;
    }
    return graph;
}

void addEdge(Graph* graph, int src, int dest, int weight) {
    Node* newNode = createNode(dest, weight);
    newNode->next = graph->adjlists[src];
    graph->adjlists[src] = newNode;

    newNode = createNode(src, weight);
    newNode->next = graph->adjlists[dest];
    graph->adjlists[dest] = newNode;
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

void primMST(Graph* graph) {
    int V = graph->numvertices;
    int parent[V];
    int weight[V];
    int inMST[V];
    MinHeap pq = {0};
    int totalw=0;

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
       
        Node* temp = graph->adjlists[u];
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
            totalw+=weight[i];
        }
    }
    printf("Total weight of MST: %d\n", totalw);
}

int main() {
    int V;
    FILE* fptr = fopen("graph.txt", "r");
    fscanf(fptr, "%d", &V);

    Graph* graph = createGraph(V);
    while (!feof(fptr)) {
        int a, b, c;
        fscanf(fptr, "%d %d %d", &a, &b, &c);
        addEdge(graph, a, b, c);
    }
    fclose(fptr);

    primMST(graph);
    return 0;
}
