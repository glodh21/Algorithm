#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_NODES 100

//Node str
typedef struct Node{
    int vertex;
    int weight;
    struct Node* next;

}Node;

//graph structure
typedef struct Graph{
    int numvert;
    Node** adjlists;
}Graph;

//edge structure

typedef struct{
    int u,v,weight;
}Edge;

int parent[MAX_NODES],rank[MAX_NODES];
Edge edges[MAX_NODES*MAX_NODES];
int edgeCount=0;

Graph* createGraph(int vertices){
    Graph* graph=(Graph* )malloc(sizeof(Graph));
    graph->numvert=vertices;
    graph->adjlists=(Node**)malloc(vertices*sizeof(Node*));
    for(int i=0;i<vertices;i++){
        graph->adjlists[i]=NULL;
    }
    return graph;
}
Node* createNode(int v,int w){
    Node* newnode=(Node*)malloc(sizeof(Node));
    newnode->vertex=v;
    newnode->weight=w;
    newnode->next=NULL;
    return newnode;
}

void makeSet(int n){
    for(int i=0;i<n;i++){
        parent[i]=i;
        rank[i]=0;
    }
}
int findSet(int u){
    if(parent[u]!=u){
        parent[u]=findSet(parent[u]);
    }
    return parent[u];
}

void unionSets(int u,int v){
    int rootU=findSet(u);
    int rootV=findSet(v);
    if(rootU!=rootV){
        if(rank[rootU]<rank[rootV])
            parent[rootU]=rootV;
        else if(rank[rootU]>rank[rootV])
            parent[rootV]=rootU;
        else{
            parent[rootV]=rootU;
            rank[rootU]++;
        }
    }
}
void addEdge(Graph* graph,int u,int v,int weight){
    Node* newnode=createNode(v,weight);
    newnode->next=graph->adjlists[u];
    graph->adjlists[u]=newnode;

    newnode=createNode(u,weight);
    newnode->next=graph->adjlists[v];
    graph->adjlists[v]=newnode;


}
void buildEdgeList(Graph* graph,int n){

    int visited[MAX_NODES][MAX_NODES]={0};
    edgeCount=0;
    for(int u=0;u<n;u++){
        Node* temp=graph->adjlists[u];
        while(temp){
            int v=temp->vertex;
            int w=temp->weight;
           if(!visited[u][v]){
                edges[edgeCount].u=u;
                edges[edgeCount].v=v;
                edges[edgeCount].weight=w;
                edgeCount++;
                visited[u][v]=visited[v][u]=1;
            }
            temp=temp->next;
        }
    }

}
void swap(Edge* a,Edge* b){
    Edge temp=*a;
    *a=*b;
    *b=temp;
}

int random(Edge arr[],int low,int high){
    int p=low+rand()%(high-low+1);
    swap(&arr[p],&arr[high]);
    int pivot=arr[high].weight;
    int i=low-1;    
    for(int j=low;j<high;j++){
        if(arr[j].weight<=pivot){
            i++;
            swap(&arr[i],&arr[j]);
        }
    }
    swap(&arr[i+1],&arr[high]);
    return i+1;
}

void quicksort(Edge arr[],int low,int high){
    if(low<high){
        int pi=random(arr,low,high);
        quicksort(arr,low,pi-1);
        quicksort(arr,pi+1,high);
    }
}

void krusMST(Graph* graph,int n){
    
   makeSet(n);
   buildEdgeList(graph,n);
   quicksort(edges,0,edgeCount-1);
   int totalw=0;

    printf("Edges in the Minimum Spanning Tree:\n");
    printf("Vertices\tWeight\n");
    for(int i=0;i<edgeCount;i++){
       int u=edges[i].u;
       int v=edges[i].v;
       int w=edges[i].weight;
       if(findSet(u)!=findSet(v)){
              printf("%d - %d\t\t%d\n",u,v,w);
              totalw+=w;
              unionSets(u,v);
       }
    }
    printf("Total weight of MST: %d\n",totalw);
   

}


int main(){
    srand(time(NULL));
    int vertices;
    FILE* file=fopen("graph.txt","r");
    if(file==NULL){
        perror("Error opening file");
        return -1;
    }
    fscanf(file,"%d",&vertices);
    Graph* graph=createGraph(vertices);
    int a,b,c;
    while(fscanf(file,"%d%d%d",&a,&b,&c)==3){
        addEdge(graph,a,b,c);
    }
     // Free memory
    fclose(file);
    krusMST(graph,vertices);
    for(int i=0;i<vertices;i++){
        Node* temp=graph->adjlists[i];
        while(temp){
            Node* toFree=temp;
            temp=temp->next;
            free(toFree);
        }
    }
    free(graph->adjlists);
    free(graph);


    return 0;
}