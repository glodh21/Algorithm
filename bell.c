#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Structure to represent an edge in the graph
struct Edge {
    int dest;
    int weight;
    struct Edge* next;
};

// Structure to represent the graph
struct Graph {
    struct Edge** edges;
    int vertices;
};

// Create a new graph with V vertices
struct Graph* createGraph(int V) {
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->vertices = V;
    graph->edges = (struct Edge**)malloc(V * sizeof(struct Edge*));
    for (int i = 0; i < V; i++)
        graph->edges[i] = NULL;
    return graph;
}

// Add an edge to the graph
void addEdge(struct Graph* graph, int src, int dest, int weight) {
    struct Edge* newEdge = (struct Edge*)malloc(sizeof(struct Edge));
    newEdge->dest = dest;
    newEdge->weight = weight;
    newEdge->next = graph->edges[src];
    graph->edges[src] = newEdge;
}

// Bellman-Ford algorithm implementation
int bellmanFord(struct Graph* graph, int src, int dist[]) {
    int V = graph->vertices;
    
    // Initialize distances
    for (int i = 0; i < V; i++)
        dist[i] = INT_MAX;
    dist[src] = 0;

    // Relax all edges V-1 times
    for (int i = 1; i <= V-1; i++) {
        for (int u = 0; u < V; u++) {
            struct Edge* edge = graph->edges[u];
            while (edge != NULL) {
                int v = edge->dest;
                int weight = edge->weight;
                
                if (dist[u] != INT_MAX && dist[u] + weight < dist[v])
                    dist[v] = dist[u] + weight;
                
                edge = edge->next;
            }
        }
    }

    // Check for negative-weight cycles
    for (int u = 0; u < V; u++) {
        struct Edge* edge = graph->edges[u];
        while (edge != NULL) {
            int v = edge->dest;
            int weight = edge->weight;
            
            if (dist[u] != INT_MAX && dist[u] + weight < dist[v])
                return 0; // Negative cycle exists
                
            edge = edge->next;
        }
    }

    return 1; // No negative cycle
}

int main() {
    int V = 5; // Number of vertices
    struct Graph* graph = createGraph(V);

    // Add edges
    addEdge(graph, 0, 1, -1);
    addEdge(graph, 0, 2, 4);
    addEdge(graph, 1, 2, 3);
    addEdge(graph, 1, 3, 2);
    addEdge(graph, 1, 4, 2);
    addEdge(graph, 3, 2, 5);
    addEdge(graph, 3, 1, 1);
    addEdge(graph, 4, 3, -3);

    int source = 0;
    int dist[V];
    
    if (bellmanFord(graph, source, dist)) {
        printf("Vertex\tDistance from Source\n");
        for (int i = 0; i < V; i++)
            printf("%d\t\t%d\n", i, dist[i]);
    } else {
        printf("Graph contains negative weight cycle\n");
    }

    return 0;
}