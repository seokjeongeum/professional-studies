#define MISS        0
#define CARRIER        1
#define BATTLESHIP    2
#define CRUISER        3
#define SUBMARINE    4
#define DESTROYER    5

extern int fire(int r, int c);

void init(int limit) {}

static int hit;
#define UNKNOWN -1
#define SIZE 10
int ships[SIZE][SIZE];
int probabilities[SIZE][SIZE];
int length[5 + 1];
int appearances[5 + 1];

bool is_outside(int r, int c) {
    return r < 0 || r >= SIZE || c < 0 || c >= SIZE;
}

void play() {
    length[CARRIER] = 5;
    length[BATTLESHIP] = 4;
    length[CRUISER] = 3;
    length[SUBMARINE] = 3;
    length[DESTROYER] = 2;
    for (int ship = CARRIER; ship <= DESTROYER; ++ship) {
        appearances[ship] = 0;
        hit += length[ship];
    }
    for (int r = 0; r < 10; ++r) {
        for (int c = 0; c < 10; ++c) {
            ships[r][c] = UNKNOWN;
        }
    }

    while (hit != 0) {
        for (int r = 0; r < 10; ++r) {
            for (int c = 0; c < 10; ++c) {
                probabilities[r][c] = 0;
            }
        }

        for (int ship = CARRIER; ship <= DESTROYER; ++ship) {
            if (appearances[ship] == 0) {
                for (int r = 0; r < 10; ++r) {
                    for (int c = 0; c < 10; ++c) {
                        if (ships[r][c] != UNKNOWN) {
                            continue;
                        }

                        bool is_probable = true;
                        for (int i = 1; i < length[ship] && is_probable; ++i) {
                            if (is_outside(r - i, c) || ships[r - i][c] != UNKNOWN) {
                                is_probable = false;
                            }
                        }
                        if (is_probable) {
                            ++probabilities[r][c];
                        }

                        is_probable = true;
                        for (int i = 1; i < length[ship] && is_probable; ++i) {
                            if (is_outside(r + i, c) || ships[r + i][c] != UNKNOWN) {
                                is_probable = false;
                            }
                        }
                        if (is_probable) {
                            ++probabilities[r][c];
                        }

                        is_probable = true;
                        for (int i = 1; i < length[ship] && is_probable; ++i) {
                            if (is_outside(r, c - i) || ships[r][c - i] != UNKNOWN) {
                                is_probable = false;
                            }
                        }
                        if (is_probable) {
                            ++probabilities[r][c];
                        }

                        is_probable = true;
                        for (int i = 1; i < length[ship] && is_probable; ++i) {
                            if (is_outside(r, c + i) || ships[r][c + i] != UNKNOWN) {
                                is_probable = false;
                            }
                        }
                        if (is_probable) {
                            ++probabilities[r][c];
                        }
                    }
                }
            } else if (appearances[ship] == 1) {
                bool not_found = true;
                for (int r = 0; r < 10 && not_found; ++r) {
                    for (int c = 0; c < 10 && not_found; ++c) {
                        if (ships[r][c] == ship) {
                            not_found = false;

                            int probability = length[ship] - 1;
                            for (int i = 1; i < length[ship]; ++i) {
                                if (is_outside(r - i, c) || ships[r - i][c] != UNKNOWN) {
                                    break;
                                }
                                probabilities[r - i][c] += probability;
                                --probability;
                            }

                            probability = length[ship] - 1;
                            for (int i = 1; i < length[ship]; ++i) {
                                if (is_outside(r + i, c) || ships[r + i][c] != UNKNOWN) {
                                    break;
                                }
                                probabilities[r + i][c] += probability;
                                --probability;
                            }

                            probability = length[ship] - 1;
                            for (int i = 1; i < length[ship]; ++i) {
                                if (is_outside(r, c - i) || ships[r][c - i] != UNKNOWN) {
                                    break;
                                }
                                probabilities[r][c - i] += probability;
                                --probability;
                            }

                            probability = length[ship] - 1;
                            for (int i = 1; i < length[ship]; ++i) {
                                if (is_outside(r, c + i) || ships[r][c + i] != UNKNOWN) {
                                    break;
                                }
                                probabilities[r][c + i] += probability;
                                --probability;
                            }
                        }
                    }
                }
            } else if (appearances[ship] >= 2) {
                int minimum_row = 9;
                int minimum_col = 9;
                int maximum_row = 0;
                int maximum_col = 0;
                for (int r = 0; r < 10; ++r) {
                    for (int c = 0; c < 10; ++c) {
                        if (ships[r][c] == ship) {
                            minimum_row = minimum_row < r ? minimum_row : r;
                            minimum_col = minimum_col < c ? minimum_col : c;
                            maximum_row = maximum_row > r ? maximum_row : r;
                            maximum_col = maximum_col > c ? maximum_col : c;
                        }
                    }
                }
                if (minimum_row == maximum_row) {
                    int r = minimum_row = maximum_row;
                    int current_length = maximum_col - minimum_col + 1;
                    int probability = 1;
                    for (int c = minimum_col - length[ship] + current_length; c < minimum_col; ++c) {
                        if (is_outside(r, c) || ships[r][c] != UNKNOWN) {
                            continue;
                        }
                        probabilities[r][c] += probability;
                        ++probability;
                    }
                    for (int c = minimum_col + 1; c < maximum_col; ++c) {
                        if (is_outside(r, c) || ships[r][c] != UNKNOWN) {
                            continue;
                        }
                        probabilities[r][c] = 987654321;
                    }
                    probability = length[ship] - current_length;
                    for (int c = maximum_col + 1; c <= maximum_col + length[ship] - current_length; ++c) {
                        if (is_outside(r, c) || ships[r][c] != UNKNOWN) {
                            continue;
                        }
                        probabilities[r][c] += probability;
                        --probability;
                    }
                }
                if (minimum_col == maximum_col) {
                    int c = minimum_col = maximum_col;
                    int current_length = maximum_row - minimum_row + 1;
                    int probability = 1;
                    for (int r = minimum_row - length[ship] + current_length; r < minimum_row; ++r) {
                        if (is_outside(r, c) || ships[r][c] != UNKNOWN) {
                            continue;
                        }
                        probabilities[r][c] += probability;
                        ++probability;
                    }
                    for (int r = minimum_row + 1; r < maximum_row; ++r) {
                        if (is_outside(r, c) || ships[r][c] != UNKNOWN) {
                            continue;
                        }
                        probabilities[r][c] = 987654321;
                    }
                    probability = length[ship] - current_length;
                    for (int r = maximum_row + 1; r <= maximum_row + length[ship] - current_length; ++r) {
                        if (is_outside(r, c) || ships[r][c] != UNKNOWN) {
                            continue;
                        }
                        probabilities[r][c] += probability;
                        --probability;
                    }
                }
            }
        }

        int maximum_probability = 0;
        int row = 0;
        int col = 0;
        for (int r = 0; r < 10; ++r) {
            for (int c = 0; c < 10; ++c) {
                if (maximum_probability < probabilities[r][c]) {
                    maximum_probability = probabilities[r][c];
                    row = r;
                    col = c;
                }
            }
        }
        int ship = fire(row, col);
        ships[row][col] = ship;
        ++appearances[ship];
        if (ship != 0) {
            --hit;
        }
    }
}