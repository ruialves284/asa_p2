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

void createSourceEdge(Node *list, int i, int j, int wLp, int (*F)[]);
void createSinkEdges(Node *list, int i, int j, int wCp, int (*F)[]);
void createHorizontalEdges(Node *list, int i, int j, int wH);
void createVerticalEdges(Node *list, int i, int j, int wV);

int EdmondsKarp(Node *adjList, int numNodes, int (*F)[numNodes+1]);
int BFS(Node *adjList, int nV, int (*F)[nV], int P[], int M[]);
void bfs2(Node *adjList, int numNodes, int (*F)[numNodes+1]);

// aux functions
void initQueue(int size);
void enqueue(int v);
int dequeue();

void printResMatrix(int numNodes, int (*F)[numNodes+1]);

void printAdjList(Node *adjList);
void checkMaxFlow(Node *adjList);
void printImage(Node *adjList);
void freeList(Node *adjList);

int m;
int n;
int numNodes;
int maxFlow;

Queue BFSQueue;

int main() {
  if (scanf("%d", &m) != 1) {
		printf("Erro ao ler o numero de linhas");
		return 1;
	}

  getchar();

  if (scanf("%d", &n) != 1) {
		printf("Erro ao ler o numero de colunas");
		return 1;
	}

  getchar();

  numNodes = m * n + 1;
  Node *adjList = (Node*) malloc(sizeof(Node)*(numNodes+1));

  for (int i = 0; i <= numNodes; i++) {
    adjList[i].id = i;
    adjList[i].edgeListHead = NULL;
    adjList[i].edgeListTail = NULL;
    adjList[i].type = 'P';
  }

  int (*F)[numNodes+1] = malloc(sizeof(int[numNodes+1][numNodes+1]));

  int i, j;

  for(i = 0; i<numNodes+1; i++){
  	for(j = 0; j<numNodes+1; j++){
      F[i][j]=0;
    }
  }

  for (i = 1; i <= m; i++) {
    for (j = 1; j <= n; j++) {
      int wLp;
      if (scanf("%d", &wLp) > -1) {
        createSourceEdge(adjList, i, j, wLp, F);
      }
    }
  }

  getchar();
  maxFlow = 0;

  for (int i = 1; i <= m; i++) {
    for (int j = 1; j <= n; j++) {
      int wCp;
      if (scanf("%d", &wCp) > -1) {
        createSinkEdges(adjList, i, j, wCp, F);
      }
    }
  }

  getchar();

  for (int i = 1; i <= m; i++) {
    for (int j = 1; j <= n-1; j++) {
      int wH;
      if (scanf("%d", &wH) > -1) {
        createHorizontalEdges(adjList, i, j, wH);
      }
    }
  }

  getchar();

  for (int i = 1; i <= m-1; i++) {
    for (int j = 1; j <= n; j++) {
      int wV;
      if (scanf("%d", &wV) > -1) {
        createVerticalEdges(adjList, i, j, wV);
      }
    }
  }

  // printf("\n");
  // printAdjList(adjList);
  // printf("\n");
  // checkMaxFlow(adjList);
  //
  // printf("%d\n", maxFlow);


  int u = EdmondsKarp(adjList, numNodes, F);
  printf("%d\n",u);

  // printf("Matriz F\n");
  // printResMatrix(numNodes, F);

  bfs2(adjList, numNodes, F);
  printf("\n");
  printImage(adjList);

  freeList(adjList);

  return 0;
}

void createSourceEdge(Node *adjList, int i, int j, int wLp, int (*F)[]) {
  int nodeId = (i - 1) * n + j;

  Edge *srcToPixel = (Edge*) malloc(sizeof(Edge));
  srcToPixel->from = 0;
  srcToPixel->to = nodeId;
  srcToPixel->flow = wLp;
  // srcToPixel->flow = 0;

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

void createSinkEdges(Node *adjList, int i, int j, int wCp, int (*F)[]) {
  int nodeId = (i - 1) * n + j;

  int shortPathFlow;

  Edge *pixelToSrc = adjList[nodeId].edgeListHead;
  if (pixelToSrc->flow > wCp) {
    shortPathFlow = wCp;
    pixelToSrc->flow = shortPathFlow;
    pixelToSrc->reverse->flow = shortPathFlow;
    // adjList[nodeId].type = 'C';
  } else {
    shortPathFlow = pixelToSrc->flow;
    // adjList[nodeId].type = 'P';
  }
  maxFlow = maxFlow + shortPathFlow;

  // shortPathFlow = 0;

  Edge *sinkToPixel = (Edge*) malloc(sizeof(Edge));
  sinkToPixel->from = numNodes;
  sinkToPixel->to = nodeId;
  sinkToPixel->flow = shortPathFlow;
  sinkToPixel->capacity = wCp;
  sinkToPixel->next = NULL;

  Edge *pixelToSink = (Edge*) malloc(sizeof(Edge));
  pixelToSink->from = nodeId;
  pixelToSink->to = numNodes;
  pixelToSink->flow = shortPathFlow;
  pixelToSink->capacity = wCp;
  pixelToSink->next = NULL;

  sinkToPixel->reverse = pixelToSink;
  pixelToSink->reverse = sinkToPixel;

  if (adjList[numNodes].edgeListHead == NULL) {
    adjList[numNodes].edgeListHead = sinkToPixel;
    adjList[numNodes].edgeListTail = sinkToPixel;
  } else {
    adjList[numNodes].edgeListTail->next = sinkToPixel;
    adjList[numNodes].edgeListTail = sinkToPixel;
  }

  adjList[nodeId].edgeListHead->next = pixelToSink;
  adjList[nodeId].edgeListTail = pixelToSink;
}

void createHorizontalEdges(Node *adjList, int i, int j, int wH) {
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

void createVerticalEdges(Node *adjList, int i, int j, int wV) {
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

void printAdjList(Node *adjList) {
  for (int i = 0; i <= numNodes; i++) {
    Edge *edge = adjList[i].edgeListHead;
    while (edge != NULL) {
      printf("%d ", edge->flow); //pick property to print
      edge = edge->next;
    }
    printf("\n");
  }
}

void checkMaxFlow(Node *adjList) {
  int srcSum = 0;
  int sinkSum = 0;

  for (int i = 1; i < numNodes; i++) {
    srcSum += adjList[i].edgeListHead->flow;
  }

  for (int i = 1; i < numNodes; i++) {
    sinkSum += adjList[i].edgeListHead->next->flow;
  }

  printf("srcSum = %d\n", srcSum);
  printf("sinkSum = %d\n", sinkSum);
}

void printImage(Node *adjList) {
  for (int i = 1; i <= m; i++) {
    for (int j = 1; j <= n; j++) {
      int nodeId = (i - 1) * n + j;
      printf("%c ", adjList[nodeId].type);
    }
    printf("\n");
  }
}

void freeList(Node *adjList) {
  for (int i = 0; i <= numNodes; i++) {
    Node *head = &adjList[i];
    while (head->edgeListHead != NULL) {
      Edge *tmp = head->edgeListHead;
      head->edgeListHead = tmp->next;
      free(tmp);
    }
  }
  free(adjList);
}

void initQueue(int numberOfVertices){
	BFSQueue.capacity = numberOfVertices;
	BFSQueue.size = 0;
	BFSQueue.front = 0;
	BFSQueue.back = -1;
	BFSQueue.vertices = (int*) malloc(sizeof(int) * numberOfVertices);
}

void enqueue(int v){
	if(BFSQueue.size == BFSQueue.capacity){
    printf("erro no enqueue\n");
		return;
	}

  // if (BFSQueue.back == BFSQueue.capacity - 1) {
  //   BFSQueue.back = -1;
  // }

	BFSQueue.vertices[++BFSQueue.back] = v;
	BFSQueue.size++;
  // BFSQueue.vertices[++BFSQueue.back] = v;
}

int dequeue(){
	int vertex = BFSQueue.vertices[BFSQueue.front++];

	// if(BFSQueue.front == BFSQueue.capacity){
	// 	BFSQueue.front = 0;
	// }

	BFSQueue.size = BFSQueue.size - 1;

	return vertex;
  // return BFSQueue.vertices[BFSQueue.front++];
}

int EdmondsKarp(Node *adjList, int numNodes, int (*F)[numNodes+1]){
	int nV = m*n + 2;
	int flow = 0;

	// int (*F)[nV] = malloc(sizeof(int[nV][nV])); //matriz capacidade residual
  //
  // //init cap res a 0
  int i, j;
	// for(i = 0; i<nV; i++){
	// 	for(j = 0; j<nV; j++){
	// 		F[i][j]=0;
	// 	}
	// }

	// printf("ANTES DO WHILE\n");
	while(1){
		// printf("entrei no while");
		int *M = (int*) malloc(sizeof(int)*nV);
		int *P = (int*) malloc(sizeof(int)*nV);

		int pathFlow; // values[0]: pathFlow ; values[1] = P.

		for(i = 0; i < nV; i++){
			P[i] = -1;
			M[i] = 0;
		}

		P[0] = -2; //SOURCE : 0
		M[0] = INT_MAX;

		initQueue(nV);
		enqueue(0); //mete source na queue

		// printf("DEI ENQUEUE NO SINK: %d\n",BFSQueue.vertices[0]);
		pathFlow = BFS(adjList, nV, F, P, M);
		// printf("PATHFLOW DA BFS: %d\n",pathFlow);

		if (pathFlow == 0){
			break;
		}

		flow = flow + pathFlow;
		int v = nV - 1;
		while(v != 0){
			int u = P[v];
			F[u][v] = F[u][v] + pathFlow;
			F[v][u] = F[v][u] - pathFlow;
			v = u;
		}
	}
	// printf("flow = %d\n",flow);
	return flow;
}

int BFS(Node *adjList, int nV, int (*F)[nV],int *P,int *M){
	int pathFlow;

	while(BFSQueue.size > 0){
		// printf("A FAZER BFS\n");
		int u = dequeue(); //vai buscar primeiro elemento da fila
		// printf("DEI DEQUEUE: %d\n", u);

		Edge *edge = adjList[u].edgeListHead;
		// printf("De u para %d", edge->to); //Esta cena de adjList[u].edgelistHead -> to da core dump!!! (adjList[u].....->to SIGSEGV

    while (edge != NULL) {
			//ERRO ESTA AQUI, ELE N TA ENTRAR PARA AQUI
			// printf("ENTREI NO WHILE 2 DA BFS\n");

			if(edge->capacity - F[u][edge->to] > 0 && P[edge->to] == -1){
				// printf("ENTREI NO IF: %d %d %d\n", edge->capacity, F[u][edge->to], P[edge->to]);
				P[edge->to] = u;
				M[edge->to] = (M[u] < edge->capacity - F[u][edge->to]) ? M[u] : edge->capacity - F[u][edge->to];

        if(edge->to != nV - 1){ //v not sink
					enqueue(edge->to);
					// printf("VOU DEI DEQUEUE DO EDGE->TO: %d\n", edge->to);
				} else{
					// printf("NAO DEI DEQUEUE\n");
					pathFlow = M[nV-1];
					return pathFlow;

				}
			}
		edge = edge->next;
		}
	}

	return 0;
}

void printQueue() {
  int front = BFSQueue.front;
  int back = BFSQueue.back;

  for (; front < back; front++) {
    printf("%d \n", BFSQueue.vertices[front]);
  }
}

void printResMatrix(int numNodes, int (*F)[numNodes+1]) {
  for (int i = 0; i < numNodes+1; i++) {
    for (int j = 0; j < numNodes+1; j++) {
      printf("%d ", F[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}

void bfs2(Node *adjList, int numNodes, int (*F)[numNodes+1]) {
    initQueue(numNodes+1);
    // int visited[8] = {0, 1, 2, 3, 4, 5, 6, 1000};
    int *visited = malloc(sizeof(int)*(numNodes+1));
    memset(visited, 0, numNodes+1);

    // printf("\n");
    visited[numNodes] = 1;
    // printf("numNodes %d\n", numNodes);
    // printf("array visited\n");
    //
    // for (int i = 0; i <= numNodes; i++) {
    //   printf("%d ", visited[i]);
    // }
    // printf("\n");
    // printf("\n");

    enqueue(numNodes);

    while(BFSQueue.size > 0){
        // printQueue();
        int currentVertex = dequeue();
        // printf("Visit and dequeue %d\n", currentVertex);

       Edge *edge = adjList[currentVertex].edgeListHead;

       while(edge != NULL) {
            int adjVertex = edge->to;

            // printf("adjVertex %d\n", adjVertex);
            // printf("visited? %d\n", visited[adjVertex]);

            if(visited[adjVertex] == 0 && (F[currentVertex][adjVertex] < 0 && F[currentVertex][adjVertex]+edge->capacity > 0)){
                visited[adjVertex] = 1;
                enqueue(adjVertex);
                adjList[adjVertex].type = 'C';
                // printf("\n");
                // printf("enqueue %d\n", adjVertex);
                // printf("\n");
            }
            edge = edge->next;
       }
    }
}
