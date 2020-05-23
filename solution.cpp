#define MISS 0
#define CARRIER 1
#define BATTLESHIP 2
#define CRUISER 3
#define SUBMARINE 4
#define DESTROYER 5

#define SIZE 10
int remaining_parts[5 + 1];
bool is_checked[SIZE][SIZE];

extern int fire(int r, int c);

void init(int limit) {
    remaining_parts[CARRIER] = 5;
    remaining_parts[BATTLESHIP] = 4;
    remaining_parts[CRUISER] = 3;
    remaining_parts[SUBMARINE] = 3;
    remaining_parts[DESTROYER] = 2;
    for (auto &row : is_checked) {
        for (bool &checked : row) {
            checked = false;
        }
    }
}

bool is_outside(int r, int c) {
    return r < 0 || r >= SIZE || c < 0 || c >= SIZE;
}

void check(int row, int column, int ship, int is_horizontal, int is_vertical, int direction) {
    int length = remaining_parts[ship];
    for (int i = 1; i <= length; ++i) {
        int new_row = row + i * direction * is_vertical;
        int new_col = column + i * direction * is_horizontal;
        if (is_outside(new_row, new_col) || is_checked[new_row][new_col]) {
            continue;
        }
        int new_ship = fire(new_row, new_col);
        is_checked[new_row][new_col] = true;
        if (new_ship == MISS) {
            break;
        }
        --remaining_parts[new_ship];
        if (ship != new_ship) {
            check(new_row, new_col, new_ship, 0, 1, -1);
            check(new_row, new_col, new_ship, 0, 1, 1);
            check(new_row, new_col, new_ship, 1, 0, -1);
            check(new_row, new_col, new_ship, 1, 0, 1);
        }
    }
}

void play() {
    init(0);
    for (int row = 0; row < SIZE; ++row) {
        for (int column = 0; column < SIZE; ++column) {
            if (is_checked[row][column] || (row + column) % 2 == 0) {
                continue;
            }
            int ship = fire(row, column);
            is_checked[row][column] = true;
            if (ship == MISS) {
                continue;
            }
            --remaining_parts[ship];
            check(row, column, ship, 0, 1, -1);
            check(row, column, ship, 0, 1, 1);
            check(row, column, ship, 1, 0, -1);
            check(row, column, ship, 1, 0, 1);
        }
    }
}