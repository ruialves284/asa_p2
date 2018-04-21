#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
  int id;
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

void createSourceEdges(Node *list, int i, int j, int wLp);
void createSinkEdges(Node *list, int i, int j, int wCp);
void createHorizontalEdges(Node *list, int i, int j, int wH);
void createVerticalEdges(Node *list, int i, int j, int wV);
void printAdjList(Node *adjList);

int m;
int n;
int numNodes;

int main() {
  int maxFlow;

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

  Node *adjList = (Node*) malloc(sizeof(Edge)*(numNodes));

  for (int i = 0; i <= numNodes; i++) {
    adjList[i].id = i;
    adjList[i].edgeListHead = NULL;
    adjList[i].edgeListTail = NULL;
  }

  for (int i = 1; i <= m; i++) {
    for (int j = 1; j <= n; j++) {
      int wLp;
      if (scanf("%d", &wLp) > 0) {
        createSourceEdges(adjList, i, j, wLp);
      }
    }
  }

  printf("\n");
  getchar();
  printAdjList(adjList);

  for (int i = 1; i <= m; i++) {
    for (int j = 1; j <= n; j++) {
      int wCp;
      if (scanf("%d", &wCp) > 0) {
        createSinkEdges(adjList, i, j, wCp);
      }
    }
  }

  printf("\n");
  getchar();
  printAdjList(adjList);

  for (int i = 1; i <= m; i++) {
    for (int j = 1; j <= n-1; j++) {
      int wH;
      if (scanf("%d", &wH) > 0) {
        createHorizontalEdges(adjList, i, j, wH);
      }
    }
  }

  printf("\n");
  getchar();
  printAdjList(adjList);

  for (int i = 1; i <= m-1; i++) {
    for (int j = 1; j <= n; j++) {
      int wV;
      if (scanf("%d", &wV) > 0) {
        createVerticalEdges(adjList, i, j, wV);
      }
    }
  }

  printf("\n");
  printAdjList(adjList);

  return 0;
}

void createSourceEdges(Node *adjList, int i, int j, int wLp) {
  int nodeId = (i - 1) * n + j;

  Edge *fromSrcEdge = (Edge*) malloc(sizeof(Edge));
  fromSrcEdge->from = 0;
  fromSrcEdge->to = nodeId;
  fromSrcEdge->flow = 0;
  fromSrcEdge->capacity = wLp;
  fromSrcEdge->next = NULL;

  if (adjList[0].edgeListHead == NULL) {
    adjList[0].edgeListHead = fromSrcEdge;
    adjList[0].edgeListTail = fromSrcEdge;
  } else {
    adjList[0].edgeListTail->next = fromSrcEdge;
    adjList[0].edgeListTail = fromSrcEdge;
  }

  Edge *toSrcEdge = (Edge*) malloc(sizeof(Edge));
  toSrcEdge->from = nodeId;
  toSrcEdge->to = 0;
  toSrcEdge->flow = 0;
  toSrcEdge->capacity = wLp;
  toSrcEdge->next = NULL;
  toSrcEdge->reverse = fromSrcEdge;

  adjList[nodeId].edgeListHead = toSrcEdge;
  adjList[nodeId].edgeListTail = toSrcEdge;

  fromSrcEdge->reverse = toSrcEdge;
  printf("\n");
}

void createSinkEdges(Node *adjList, int i, int j, int wCp) {
  int nodeId = (i - 1) * n + j;

  Edge *fromSinkEdge = (Edge*) malloc(sizeof(Edge));
  fromSinkEdge->from = numNodes;
  fromSinkEdge->to = nodeId;
  fromSinkEdge->flow = 0;
  fromSinkEdge->capacity = wCp;
  fromSinkEdge->next = NULL;

  if (adjList[numNodes].edgeListHead == NULL) {
    adjList[numNodes].edgeListHead = fromSinkEdge;
    adjList[numNodes].edgeListTail = fromSinkEdge;
  } else {
    adjList[numNodes].edgeListTail->next = fromSinkEdge;
    adjList[numNodes].edgeListTail = fromSinkEdge;
  }

  Edge *toSinkEdge = (Edge*) malloc(sizeof(Edge));
  toSinkEdge->from = nodeId;
  toSinkEdge->to = numNodes;
  toSinkEdge->flow = 0;
  toSinkEdge->capacity = wCp;
  toSinkEdge->next = NULL;
  toSinkEdge->reverse = fromSinkEdge;

  adjList[nodeId].edgeListHead->next = toSinkEdge;
  adjList[nodeId].edgeListTail = toSinkEdge;

  fromSinkEdge->reverse = toSinkEdge;
}

void createHorizontalEdges(Node *adjList, int i, int j, int wH) {
  int leftId = (i - 1) * n + j;
  int rightId = (i - 1) * n + j + 1;

  Edge *fromLeftEdge = (Edge*) malloc(sizeof(Edge));
  fromLeftEdge->from = leftId;
  fromLeftEdge->to = rightId;
  fromLeftEdge->flow = 0;
  fromLeftEdge->capacity = wH;
  fromLeftEdge->next = NULL;

  Edge *fromRightEdge = (Edge*) malloc(sizeof(Edge));
  fromRightEdge->from = rightId;
  fromRightEdge->to = leftId;
  fromRightEdge->flow = 0;
  fromRightEdge->capacity = wH;
  fromRightEdge->next = NULL;

  fromLeftEdge->reverse = fromRightEdge;
  fromRightEdge->reverse = fromLeftEdge;

  adjList[leftId].edgeListTail->next = fromLeftEdge;
  adjList[leftId].edgeListTail = fromLeftEdge;

  adjList[rightId].edgeListTail->next = fromRightEdge;
  adjList[rightId].edgeListTail = fromRightEdge;
}

void createVerticalEdges(Node *adjList, int i, int j, int wV) {
  int upId = (i - 1) * n + j;
  int downId = (i - 1) * n + j + n;

  Edge *fromUpEdge = (Edge*) malloc(sizeof(Edge));
  fromUpEdge->from = upId;
  fromUpEdge->to = downId;
  fromUpEdge->flow = 0;
  fromUpEdge->capacity = wV;
  fromUpEdge->next = NULL;

  Edge *fromDownEdge = (Edge*) malloc(sizeof(Edge));
  fromDownEdge->from = downId;
  fromDownEdge->to = upId;
  fromDownEdge->flow = 0;
  fromDownEdge->capacity = wV;
  fromDownEdge->next = NULL;

  fromUpEdge->reverse = fromDownEdge;
  fromDownEdge->reverse = fromUpEdge;

  adjList[upId].edgeListTail->next = fromUpEdge;
  adjList[upId].edgeListTail = fromUpEdge;

  adjList[downId].edgeListTail->next = fromDownEdge;
  adjList[downId].edgeListTail = fromDownEdge;
}

void printAdjList(Node *adjList) {
  for (int i = 0; i <= numNodes; i++) {
    Edge *edge = adjList[i].edgeListHead;
    while (edge != NULL) {
      printf("%d ", edge->from); //pick property to print
      edge = edge->next;
    }
    printf("\n");
  }
}
