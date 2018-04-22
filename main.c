#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void createSourceEdge(Node *list, int i, int j, int wLp);
void createSinkEdges(Node *list, int i, int j, int wCp);
void createHorizontalEdges(Node *list, int i, int j, int wH);
void createVerticalEdges(Node *list, int i, int j, int wV);

// aux functions
void printAdjList(Node *adjList);
void checkMaxFlow(Node *adjList);
void printImage(Node *adjList);
void freeList(Node *adjList);

int m;
int n;
int numNodes;
int maxFlow;

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
  }

  for (int i = 1; i <= m; i++) {
    for (int j = 1; j <= n; j++) {
      int wLp;
      if (scanf("%d", &wLp) > -1) {
        createSourceEdge(adjList, i, j, wLp);
      }
    }
  }

  getchar();
  maxFlow = 0;

  for (int i = 1; i <= m; i++) {
    for (int j = 1; j <= n; j++) {
      int wCp;
      if (scanf("%d", &wCp) > -1) {
        createSinkEdges(adjList, i, j, wCp);
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

  printf("\n");
  printAdjList(adjList);
  printf("\n");
  checkMaxFlow(adjList);

  printf("%d\n", maxFlow);
  // printf("\n");
  // printImage(adjList);

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

void createSinkEdges(Node *adjList, int i, int j, int wCp) {
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
