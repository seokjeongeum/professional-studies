#include <cstdio>

#define MAX_KEY 20
#define MAX_TABLE 200000

typedef struct {
    char key[MAX_KEY + 1];
    int data;
} Hash;
Hash tb[MAX_TABLE];

unsigned long hash(const char *str) {
    unsigned long hash = 5381;
    int c;

    while (c = *str++) {
        hash = (((hash << 5) + hash) + c) % MAX_TABLE;
    }

    return hash % MAX_TABLE;
}

int strcmp(char key[21], const char *key1) {
    int i = 0;
    while (key[i]) {
        if (key[i] != key1[i]) {
            return key[i] - key1[i];
        }
        ++i;
    }
    return key[i] - key1[i];
}

void strcpy(char key[21], const char *key1) {
    int i = 0;
    while (key1[i]) {
        key[i] = key1[i];
        ++i;
    }
    key[i] = key1[i];
}

int find(const char *key) {
    unsigned long h = hash(key);
    int cnt = MAX_TABLE;

    while (tb[h].key[0] != 0 && cnt--) {
        if (strcmp(tb[h].key, key) == 0) {
            return h;
        }
        h = (h + 1) % MAX_TABLE;
    }
    return -1;
}


int add(const char *key) {
    unsigned long h = hash(key);

    while (tb[h].key[0] != 0) {
        if (strcmp(tb[h].key, key) == 0) {
            return 0;
        }

        h = (h + 1) % MAX_TABLE;
    }
    strcpy(tb[h].key, key);
    tb[h].data = h;
    return 1;
}

using namespace std;
char name1[MAX_KEY + 1], name2[MAX_KEY + 1];
int root[MAX_TABLE], counts[MAX_TABLE];

int find_set(int x) {
    if (root[x] == x) {
        return x;
    } else {
        return root[x] = find_set(root[x]);
    }
}

void union_set(int x, int y) {
    x = find_set(x);
    y = find_set(y);
    if (x != y) {
        counts[x] = counts[y] = counts[x] + counts[y];
    }
    root[y] = x;
}

int main() {
    int t;
    scanf(" %d", &t);
    for (int i = 1; i <= t; ++i) {
        for (int j = 0; j < MAX_TABLE; ++j) {
            root[j] = j;
            counts[j] = 1;
        }
        int f;
        scanf(" %d", &f);
        for (int j = 0; j < f; ++j) {
            scanf(" %s%s", name1, name2);
            add(name1);
            add(name2);
            int x = find(name1);
            int y = find(name2);
            union_set(x, y);
            printf("%d\n", counts[find_set(x)]);
        }
    }
    return 0;
}