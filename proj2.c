#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/*							    */
/*			ESTRUTURAS			*/
/*							    */

/*
    Estrutura para nos das filas (arcos do grafo)
        Tem um inteiro que indica o peso do arco
        Tem um ponteiro para o vertice ao qual esta ligado
        Tem um ponteiro para o no seguinte
*/
typedef struct node {
    int cost;
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
        Tem um inteiro que indica se o vertice esta num ciclo de peso negativo
        Tem um inteiro que indica se o vertice pode ser alterado na iteracao actual
        Tem um ponteiro para a fila que contem os seus arcos
*/
typedef struct vertex {
    int color; /* 0 - white ; 1 - gray ; 2 - black; */
    int distance;
    int cycle; /* 1 se for parte de um ciclo de peso negativo, 0 caso contrario*/
    int change; /* 0 se pode alterar nesta iteracao, 1 caso contrario */
    struct queue* edges;
}*vertex;


/*							    */
/*		FUNCOES AUXILIARES		*/
/*							    */

/*
    Funcao que cria um novo no com um ponteiro para um determinado vertice
        Recebe um ponteiro para um vertice
        Recebe o peso do arco a criar
        Retorna um ponteiro para um no
*/
link newlink(vertex v, int weight) {
    link novo = (link) malloc(sizeof(struct node));
    novo -> cost = weight;
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
    link novo = newlink(v, 0);
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
    novo.distance = INT_MAX;
    novo.cycle = 0;
    novo.change = 1;
    novo.edges = (queue) malloc(sizeof(struct queue));
    novo.edges -> begin = NULL;
    novo.edges -> end = NULL;
    return novo;
}

/*                      <<<< BFS >>>
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
                adj -> vertex -> cycle = 1;
                insertEndPQueue(priorityQueue, adj -> vertex);
            }
        }
        v -> color = 2;
    }
}

/*
   Funcao que faz o relaxamento de um vertice
        Recebe um vertice u
        Recebe um vertice v
        Recebe um arco w que liga u a v
        Retorna 1 caso tenha feito relax, 0 caso contrario
*/
int relax(struct vertex u, vertex v, link w) {
    if ((u.distance != INT_MAX) && (v -> distance > u.distance + w -> cost)) {
        v -> distance = u.distance + w -> cost;
        v -> change = 0;
        return 1;
    }
    return 0;
}

/*							    */
/*		FUNCAO PRINCIPAL		*/
/*							    */

/*
   Funcao que implementa o algoritmo Bellman-Ford
        Recebe um array de vertices (um grafo)
        Recebe um inteiro que indica o numero de vertices existentes
*/
void BellmanFord(vertex graph, int max_vertex) {
    int i, v, changed;
    link adj;
    for (i = 1; i < max_vertex - 1; i++) {
        changed = 0;
        for (v = 0; v < max_vertex; v++) {
            if (graph[v].change != 1) {
                for (adj = graph[v].edges -> begin; adj != NULL; adj = adj -> next) {
                    relax(graph[v], adj -> vertex, adj);
                    changed = 1;
                }
                graph[v].change = 1;
            }
        }
        if (changed == 0) {break;}
    }
    for (v = 0; v < max_vertex; v++) {
        for (adj = graph[v].edges -> begin; adj != NULL; adj = adj -> next) {
            if ((relax(graph[v], adj -> vertex, adj) == 1) && (graph[v].cycle != 1)) {
                    graph[v].cycle = 1;
                    BFS(graph, graph + v);
            }
        }
    }
}

/*							    */
/*			    MAIN			*/
/*							    */
int main() {
    int max_vertex, max_edge, start, v1, v2, edge, i, v;
    vertex graph;
    if (scanf("%d %d %d", &max_vertex, &max_edge, &start) == 1);
    graph = (vertex) calloc(sizeof(struct vertex), max_vertex);
    for (i = 0; i < max_vertex; i++) {
        graph[i] = newVertex();
    }
    for(i = 0; i < max_edge; i++) {
        if (scanf("%d %d %d", &v1, &v2, &edge) == 1);
        insertEnd(&graph[v1 - 1], newlink(&graph[v2 - 1], edge));
    }
    graph[start - 1].distance = 0;
    graph[start - 1].change = 0;
    BellmanFord(graph, max_vertex);
    for(v = 0; v < max_vertex; v++) {
        if (graph[v].cycle == 1) {
            printf("I\n");
        }
        else if (graph[v].distance == INT_MAX) {
            printf("U\n");
        }
        else {
            printf("%d\n", graph[v].distance);
        }
    }
    return 0;
}