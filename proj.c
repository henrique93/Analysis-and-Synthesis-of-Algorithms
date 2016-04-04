#include <stdio.h>
#include <stdlib.h>

/*							    */
/*			ESTRUTURAS			*/
/*							    */

/*
    Estrutura para nos das filas (arcos do grafo)
        Tem um ponteiro para o vertice ao qual esta ligado
        Tem um ponteiro para o no seguinte
*/
typedef struct node {
	struct vertex* vertex;
    struct node *next;
}*link;

/*
    Estrutura para a fila de arcos (todos os arcos de um vertice)
        Tem um ponteiro para o no inicial da fila
        Tem um ponteiro para o no final da fila
*/
typedef struct queue {
    struct node* begin;
    struct node* end;
}*queue;

/*
    Estrutura para os vertices do grafo
        Tem um inteiro que indica a cor a que o vertice esta
        Tem um inteiro que indica a distancia a que esta do vertice inicial
        Tem um ponteiro para a fila que contem os seus arcos
*/
typedef struct vertex {
    int color; /* 0 - white ; 1 - gray ; 2 - black; */
    int distance;
    struct queue* edges;
}*vertex;


/*							    */
/*		FUNCOES AUXILIARES		*/
/*							    */

/*
    Funcao que cria um novo no com um ponteiro para um determinado vertice
        Recebe um ponteiro para um vertice
        Retorna um ponteiro para um no
*/
link newlink(vertex v) {
    link novo = (link) malloc(sizeof(struct node));
    novo -> vertex = v;
    novo -> next = NULL;
    return novo;
}


/*
    Funcao que insere um no na fila de um determinado vertice
        Recebe um ponteiro para um vertice
        Recebe um ponteiro para o no a inserir
*/
void insertEnd(vertex v, link edge) {
    if (v -> edges -> begin == NULL) {
        v -> edges -> begin = edge;
        v -> edges -> end = edge;
    }
    else {
        v -> edges -> end -> next = edge;
        v -> edges -> end = edge;
    }
}

/*
    Funcao que insere um vertice no fim da fila da priority queue
        Recebe um ponteiro para a priority queue
        Recebe um ponteiro para o vertice a inserir
*/
void insertEndPQueue(queue priorityQueue, vertex v) {
    link novo = newlink(v);
    if (priorityQueue -> begin == NULL) {
        priorityQueue -> begin = novo;
        priorityQueue -> end = novo;
    }
    else {
        priorityQueue -> end -> next = novo;
        priorityQueue -> end = novo;
    }
}

/*
    Funcao que retira o vertice inicial da priority queue
        Recebe um ponteiro para a priority queue
*/
void deleteBeginPQueue(queue priorityQueue) {
    priorityQueue -> begin = priorityQueue -> begin -> next;
}

/*
    Funcao que cria um novo vertice
        Retorna uma nova instancia de vertice
*/
struct vertex newVertex() {
    struct vertex novo;
    novo.color = 0;
    novo.distance = 0;
    novo.edges = (queue) malloc(sizeof(struct queue));
    novo.edges -> begin = NULL;
    novo.edges -> end = NULL;
    return novo;
}

/*							    */
/*		FUNCAO PRINCIPAL		*/
/*							    */

/*
    Funcao que implementa o algoritmo Breadth-First-Search
        Recebe um array de vertices (graph)
        Recebe um ponteiro para o vertice inicial (start)
*/
void BFS(vertex graph, vertex start) {
    vertex v;
    link adj;
    queue priorityQueue = (queue) malloc(sizeof(struct queue));
    priorityQueue -> begin = NULL;
    priorityQueue -> end = NULL;
    start -> color = 1;
    insertEndPQueue(priorityQueue, start);
    while (priorityQueue -> begin != NULL) {
        v = priorityQueue -> begin -> vertex;
        deleteBeginPQueue(priorityQueue);
        for (adj = v -> edges -> begin; adj != NULL; adj = adj -> next) {
            if (adj -> vertex -> color == 0) {
                adj -> vertex -> color = 1;
                adj -> vertex -> distance = v -> distance + 1;
                insertEndPQueue(priorityQueue, adj -> vertex);
            }
        }
        v -> color = 2;
    }
}

/*							    */
/*			    MAIN			*/
/*							    */
int main() {
    int max_vertex, max_edge, start, v1, edge, max = 0, i, v;
    vertex graph;
    int* out;
    scanf("%d %d %d", &max_vertex, &max_edge, &start);
    graph = (vertex) calloc(sizeof(struct vertex), max_vertex);
    for (i = 0; i < max_vertex; i++) {
        graph[i] = newVertex();
    }
    for(i = 0; i < max_edge; i++) {
        scanf("%d %d", &v1, &edge);
        insertEnd(&graph[v1 - 1], newlink(&graph[edge - 1]));
        insertEnd(&graph[edge - 1], newlink(&graph[v1 - 1]));
    }
    BFS(graph, &graph[start - 1]);
    for (v = 0; v < max_vertex; v++) {
        if (graph[v].distance > max) {
            max = graph[v].distance;
        }
    }
    printf("%d\n", max);
    out = (int*) malloc(sizeof(int)*max);
    for(v = 0; v < max_vertex; v++) {
        if (graph[v].distance != 0) {
            out[graph[v].distance - 1]++;
        }
    }
    for (i = 0; i < max; i++) {
        printf("%d\n", out[i]);
    }
    return 0;
}