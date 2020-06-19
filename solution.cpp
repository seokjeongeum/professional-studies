#define MAXL    5
#define MAXF    10

#include <malloc.h>

typedef struct adjlistNode {
    int vertex;
    adjlistNode *next;
} AdjlistNode;

typedef struct {
    int num_members;
    AdjlistNode *head;
    AdjlistNode *tail;
} AdjList;

typedef struct {
    int num_vertices;
    AdjList *adjListArr;
} Graph;

AdjlistNode *createNode(int v) {
    AdjlistNode *newNode = (AdjlistNode *) malloc(sizeof(AdjlistNode));

    newNode->vertex = v;
    newNode->next = NULL;

    return newNode;
}

Graph *createGraph(int n) {

    Graph *graph = (Graph *) malloc(sizeof(Graph));
    graph->num_vertices = n;

    graph->adjListArr = (AdjList *) malloc((n + 1) * sizeof(AdjList));

    for (int i = 1; i <= n; i++) {
        graph->adjListArr[i].head = graph->adjListArr[i].tail = NULL;
        graph->adjListArr[i].num_members = 0;
    }

    return graph;
}

void addEdge(Graph *graph, int src, int dest) {
    AdjlistNode *newNode = createNode(dest);
    if (graph->adjListArr[src].tail != NULL) {
        graph->adjListArr[src].tail->next = newNode;
        graph->adjListArr[src].tail = newNode;
    } else {
        graph->adjListArr[src].head = graph->adjListArr[src].tail = newNode;
    }
    graph->adjListArr[src].num_members++;

    newNode = createNode(src);
    if (graph->adjListArr[dest].tail != NULL) {
        graph->adjListArr[dest].tail->next = newNode;
        graph->adjListArr[dest].tail = newNode;
    } else {
        graph->adjListArr[dest].head = graph->adjListArr[dest].tail = newNode;
    }
    graph->adjListArr[dest].num_members++;
}

struct pair {
    int id;
    int count;

    bool operator<(pair other) {
        if (count != other.count) {
            return count > other.count;
        }
        return id < other.id;
    }
};

#define MAX_SIZE 10000

pair heap[MAX_SIZE];
int heapSize = 0;

int heapPush(pair value) {
    if (heapSize + 1 > MAX_SIZE) {
        printf("queue is full!");
        return 0;
    }

    heap[heapSize] = value;

    int current = heapSize;
    while (current > 0 && heap[current] < heap[(current - 1) / 2]) {
        pair temp = heap[(current - 1) / 2];
        heap[(current - 1) / 2] = heap[current];
        heap[current] = temp;
        current = (current - 1) / 2;
    }

    heapSize = heapSize + 1;

    return 1;
}

int heapPop(pair *value) {
    if (heapSize <= 0) {
        return -1;
    }

    *value = heap[0];
    heapSize = heapSize - 1;

    heap[0] = heap[heapSize];

    int current = 0;
    while (current * 2 + 1 < heapSize) {
        int child;
        if (current * 2 + 2 == heapSize) {
            child = current * 2 + 1;
        } else {
            child = heap[current * 2 + 1] < heap[current * 2 + 2] ? current * 2 + 1 : current * 2 + 2;
        }

        if (heap[current] < heap[child]) {
            break;
        }

        pair temp = heap[current];
        heap[current] = heap[child];
        heap[child] = temp;

        current = child;
    }
    return 1;
}

Graph *graph;
int n;

void init(int N) {
    n = N;
    graph = createGraph(N);
}

void add(int id, int F, int ids[MAXF]) {
    for (int i = 0; i < F; ++i) {
        addEdge(graph, id, ids[i]);
    }
}

void del(int id1, int id2) {
    AdjlistNode *previous = NULL;
    AdjlistNode *current = graph->adjListArr[id1].head;
    while (current) {
        if (current->vertex == id2) {
            if (graph->adjListArr[id1].head == current) {
                graph->adjListArr[id1].head = current->next;
            } else {
                previous->next = current->next;
            }
            if (graph->adjListArr[id1].tail == current) {
                graph->adjListArr[id1].tail = previous;
            }
            break;
        }
        previous = current;
        current = current->next;
    }

    previous = NULL;
    current = graph->adjListArr[id2].head;
    while (current) {
        if (current->vertex == id1) {
            if (graph->adjListArr[id2].head == current) {
                graph->adjListArr[id2].head = current->next;
            } else {
                previous->next = current->next;
            }
            if (graph->adjListArr[id2].tail == current) {
                graph->adjListArr[id2].tail = previous;
            }
            break;
        }
        previous = current;
        current = current->next;
    }
}

int is_friends[MAX_SIZE + 1];

int yes = 1;

int recommend(int id, int list[MAXL]) {
    is_friends[id] = yes;
    AdjlistNode *current = graph->adjListArr[id].head;
    while (current) {
        is_friends[current->vertex] = yes;
        current = current->next;
    }
    for (int i = 1; i <= n; ++i) {
        if (is_friends[i] != yes) {
            current = graph->adjListArr[i].head;
            int common_friend_count = 0;
            while (current) {
                if (is_friends[current->vertex] == yes) {
                    ++common_friend_count;
                }
                current = current->next;
            }
            if (common_friend_count != 0) {
                heapPush({i, common_friend_count});
            }
        }
    }
    int r = heapSize > MAXL ? MAXL : heapSize;
    pair s{};
    for (int i = 0; i < r; ++i) {
        heapPop(&s);
        list[i] = s.id;
    }
    while (heapSize) {
        heapPop(&s);
    }
    ++yes;
    return r;
}