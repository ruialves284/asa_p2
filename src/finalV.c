#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef struct node {
  int id;
  char type;
  struct edge *edgeListHead;
  struct edge *edgeListTail;
} Node;

typedef struct edge {
  int from;
  int to;
  int capacity;
  int flow;
  struct edge *reverse;
  struct edge *next;
} Edge;

typedef struct queue{
	int front;
	int back;
	int size;
	int capacity;
	int *vertices;
} Queue;

/* Read input functions. */
void createSourceEdge(Node *list, int i, int j, int wLp);
void createSinkEdge(Node *list, int i, int j, int wCp);
void createHorizontalEdge(Node *list, int i, int j, int wH);
void createVerticalEdge(Node *list, int i, int j, int wV);

/* Main behavior functions. */
void EdmondsKarp(Node *adjList);
int augmentingPathBFS(Node *adjList, int P[], int M[]);
void minCutMappingBFS(Node *adjList);
void printImage(Node *adjList);
void freeList(Node *adjList);

/* Queue auxiliary functions. */
void initQueue(int size);
void enqueue(int v);
int dequeue();

int m;
int n;
int numNodes;
int maxFlow;

Queue BFSQueue;

int main() {
  if (scanf("%d", &m) != 1) {
		printf("Erro ao ler o numero de linhas\n");
		return 1;
	}

  getchar();

  if (scanf("%d", &n) != 1) {
		printf("Erro ao ler o numero de colunas\n");
		return 1;
	}

  getchar();

  int i, j;
  numNodes = m * n + 1;
  Node *adjList = (Node*) malloc(sizeof(Node)*(numNodes+1));

  /* Init adjacency list*/
  for (i = 0; i <= numNodes; i++) {
    adjList[i].id = i;
    adjList[i].edgeListHead = NULL;
    adjList[i].edgeListTail = NULL;
    /* Init all pixels as foreground ('Primeiro Plano')*/
    adjList[i].type = 'P';
  }

  for (i = 1; i <= m; i++) {
    for (j = 1; j <= n; j++) {
      int wLp;
      if (scanf("%d", &wLp) > -1) {
        createSourceEdge(adjList, i, j, wLp);
      }
    }
  }

  getchar();
  maxFlow = 0;

  for (i = 1; i <= m; i++) {
    for (j = 1; j <= n; j++) {
      int wCp;
      if (scanf("%d", &wCp) > -1) {
        createSinkEdge(adjList, i, j, wCp);
      }
    }
  }

  getchar();

  for (i = 1; i <= m; i++) {
    for (j = 1; j <= n-1; j++) {
      int wH;
      if (scanf("%d", &wH) > -1) {
        createHorizontalEdge(adjList, i, j, wH);
      }
    }
  }

  getchar();

  for (i = 1; i <= m-1; i++) {
    for (j = 1; j <= n; j++) {
      int wV;
      if (scanf("%d", &wV) > -1) {
        createVerticalEdge(adjList, i, j, wV);
      }
    }
  }

  EdmondsKarp(adjList);

  printf("%d\n\n",maxFlow);

  minCutMappingBFS(adjList);

  printImage(adjList);

  freeList(adjList);

  return 0;
}

void createSourceEdge(Node *adjList, int i, int j, int wLp) {
  int nodeId = (i - 1) * n + j;

  Edge *srcToPixel = (Edge*) malloc(sizeof(Edge));
  srcToPixel->from = 0;
  srcToPixel->to = nodeId;
  srcToPixel->flow = wLp;

  srcToPixel->capacity = wLp;
  srcToPixel->next = NULL;

  Edge *pixelToSrc = (Edge*) malloc(sizeof(Edge));
  pixelToSrc->from = nodeId;
  pixelToSrc->to = 0;
  pixelToSrc->flow = wLp;
  pixelToSrc->capacity = wLp;
  pixelToSrc->next = NULL;

  srcToPixel->reverse = pixelToSrc;
  pixelToSrc->reverse = srcToPixel;

  if (adjList[0].edgeListHead == NULL) {
    adjList[0].edgeListHead = srcToPixel;
    adjList[0].edgeListTail = srcToPixel;
  } else {
    adjList[0].edgeListTail->next = srcToPixel;
    adjList[0].edgeListTail = srcToPixel;
  }

  adjList[nodeId].edgeListHead = pixelToSrc;
  adjList[nodeId].edgeListTail = pixelToSrc;
}

void createSinkEdge(Node *adjList, int i, int j, int wCp) {
  int nodeId = (i - 1) * n + j;

  int shortPathFlow;

  /* Optimization. For each augmenting path consisting of only 2 edges,
  saturate one of them (the one with least capacity)  when reading the input.*/
  Edge *pixelToSrc = adjList[nodeId].edgeListHead;
  if (pixelToSrc->flow > wCp) {
    shortPathFlow = wCp;
    pixelToSrc->reverse->flow = shortPathFlow;
  } else {
    shortPathFlow = pixelToSrc->flow;
  }

  pixelToSrc->reverse->reverse = NULL;

  /* the pixelToSrc edges were only created so that the access to
  their reverse could be done in O(1) in this step. No longer needed, free it.*/
  free(pixelToSrc);

  maxFlow = maxFlow + shortPathFlow;

  Edge *pixelToSink = (Edge*) malloc(sizeof(Edge));
  pixelToSink->from = nodeId;
  pixelToSink->to = numNodes;
  pixelToSink->flow = shortPathFlow;
  pixelToSink->capacity = wCp;
  pixelToSink->next = NULL;
  pixelToSink->reverse = NULL;

  adjList[nodeId].edgeListHead = pixelToSink;
  adjList[nodeId].edgeListTail = pixelToSink;
}

void createHorizontalEdge(Node *adjList, int i, int j, int wH) {
  int leftId = (i - 1) * n + j;
  int rightId = (i - 1) * n + j + 1;

  Edge *leftToRight = (Edge*) malloc(sizeof(Edge));
  leftToRight->from = leftId;
  leftToRight->to = rightId;
  leftToRight->flow = 0;
  leftToRight->capacity = wH;
  leftToRight->next = NULL;

  Edge *rightToLeft = (Edge*) malloc(sizeof(Edge));
  rightToLeft->from = rightId;
  rightToLeft->to = leftId;
  rightToLeft->flow = 0;
  rightToLeft->capacity = wH;
  rightToLeft->next = NULL;

  leftToRight->reverse = rightToLeft;
  rightToLeft->reverse = leftToRight;

  adjList[leftId].edgeListTail->next = leftToRight;
  adjList[leftId].edgeListTail = leftToRight;

  adjList[rightId].edgeListTail->next = rightToLeft;
  adjList[rightId].edgeListTail = rightToLeft;
}

void createVerticalEdge(Node *adjList, int i, int j, int wV) {
  int upId = (i - 1) * n + j;
  int downId = (i - 1) * n + j + n;

  Edge *upToDown = (Edge*) malloc(sizeof(Edge));
  upToDown->from = upId;
  upToDown->to = downId;
  upToDown->flow = 0;
  upToDown->capacity = wV;
  upToDown->next = NULL;

  Edge *downToUp = (Edge*) malloc(sizeof(Edge));
  downToUp->from = downId;
  downToUp->to = upId;
  downToUp->flow = 0;
  downToUp->capacity = wV;
  downToUp->next = NULL;

  upToDown->reverse = downToUp;
  downToUp->reverse = upToDown;

  adjList[upId].edgeListTail->next = upToDown;
  adjList[upId].edgeListTail = upToDown;

  adjList[downId].edgeListTail->next = downToUp;
  adjList[downId].edgeListTail = downToUp;
}

void printImage(Node *adjList) {
  int i, j;
  for (i = 1; i <= m; i++) {
    for (j = 1; j <= n; j++) {
      int nodeId = (i - 1) * n + j;
      printf("%c ", adjList[nodeId].type);
    }
    printf("\n");
  }
}

void freeList(Node *adjList) {
  int i;
  for (i = 0; i <= numNodes; i++) {
    Node *head = &adjList[i];
    while (head->edgeListHead != NULL) {
      Edge *tmp = head->edgeListHead;
      head->edgeListHead = tmp->next;
      free(tmp);
    }
  }
  free(adjList);
}

void initQueue(int size){
	BFSQueue.capacity = size;
	BFSQueue.size = 0;
	BFSQueue.front = 0;
	BFSQueue.back = -1;
	BFSQueue.vertices = (int*) malloc(sizeof(int) * size);
}

void enqueue(int v){
	if(BFSQueue.size == BFSQueue.capacity){
    printf("Erro no enqueue\n");
		return;
	}
	BFSQueue.vertices[++BFSQueue.back] = v;
	BFSQueue.size++;
}

int dequeue(){
	int vertex = BFSQueue.vertices[BFSQueue.front++];
	BFSQueue.size = BFSQueue.size - 1;
	return vertex;
}

void EdmondsKarp(Node *adjList){
  int i;

	while(1) {
    /* Edmonds-Karp flow array. For each iteration saves the flow in each edge. */
		int *M = (int*) malloc(sizeof(int)*(numNodes+1));

    /* BFS parent array. Stores the previously visited pixel for each pixel. */
		int *P = (int*) malloc(sizeof(int)*(numNodes+1));

		int pathFlow;

		for(i = 0; i <= numNodes; i++){
			P[i] = -1;
			M[i] = 0;
		}

		P[0] = -2;
		M[0] = INT_MAX;

		initQueue(numNodes+1);

    /* Start in the source. */
		enqueue(0);

		pathFlow = augmentingPathBFS(adjList, P, M);

    /* No more augmenting paths exist. */
		if (pathFlow == 0){
      free(M);
      free(P);
      free(BFSQueue.vertices);
			break;
		}

		maxFlow = maxFlow + pathFlow;
		int v = numNodes;

    /* Update flow on each augmenting path's edge on the adjacency list.*/
		while(v != 0){
      int u = P[v];
      Edge *edge = adjList[u].edgeListHead;
      while (edge->to != v) {
        edge = edge->next;
      }
      edge->flow+=pathFlow;
			v = u;
		}

    free(M);
    free(P);
    free(BFSQueue.vertices);
	}
}

int augmentingPathBFS(Node *adjList, int *P, int *M){
	int pathFlow;
  int arrivedToSink = 0;

	while(BFSQueue.size > 0){
		int u = dequeue();

		Edge *edge = adjList[u].edgeListHead;

    while (edge != NULL) {
			if(P[edge->to] == -1 && edge->capacity > edge->flow){
				P[edge->to] = u;
				M[edge->to] = (M[u] < edge->capacity - edge->flow) ? M[u] : edge->capacity - edge->flow;

        if(edge->to != numNodes){
					enqueue(edge->to);
				} else {
          arrivedToSink = 1;
          break;
				}

			}
		edge = edge->next;
		}
    if (arrivedToSink) {
      break;
    }
	}
  pathFlow = M[numNodes];
  return pathFlow;
}

void minCutMappingBFS(Node *adjList) {
    initQueue(numNodes+1);

    /* BFS array to mark visited edges. */
    int *visited = malloc(sizeof(int)*(numNodes+1));

    int i;
    for (i = 0; i <= numNodes; i++) {
      visited[i] = 0;
    }

    /* Start in the source. */
    visited[0] = 1;
    enqueue(0);

    while(BFSQueue.size > 0){
        int currentVertex = dequeue();

       Edge *edge = adjList[currentVertex].edgeListHead;

       while(edge != NULL) {
            int adjVertex = edge->to;
            if(visited[adjVertex] == 0 && edge->capacity-edge->flow > 0){

                visited[adjVertex] = 1;
                enqueue(adjVertex);
                adjList[adjVertex].type = 'C';
            }
            edge = edge->next;
       }
    }
    free(BFSQueue.vertices);
    free(visited);
}
