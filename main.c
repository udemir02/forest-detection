typedef signed char  S8;
typedef signed short S16;
typedef signed int   S32;
typedef signed long  S64;

typedef unsigned char  U8;
typedef unsigned short U16;
typedef unsigned int   U32;
typedef unsigned long  U64;

typedef float  F32;
typedef double F64;

typedef int B32;

#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#include "random.h"

#define ROWS 20
#define COLS 20

typedef enum {
    START,
    DEFAULT
} Print_Type;

typedef enum {
    EMPTY,
    TREE,
    IGNITED,
    BURNING,
} Tree_State;

typedef struct {
    S32 e[ROWS][COLS];

    U32 num_tree;
    U32 num_ignited;
    U32 num_burning;

    U32 ignite_start_time;
    B32 is_burning;
} Forest;

static void print_forest(Forest *forest, U32 time, Print_Type type) {
    printf("\n=================\n");

    if (type == START)
        printf("===== START =====\n");
    else if (type == DEFAULT)
        printf("==== TIME %d ====\n", time);

    printf("=================\n\n");

    printf("Number of trees: %d\n", forest->num_tree);
    if (type == DEFAULT) {
        printf("Number of ignited trees: %d\n", forest->num_ignited);
        printf("Number of burning trees: %d\n", forest->num_burning);
        printf("\nIgnite start time: %d\n", forest->ignite_start_time);
    }
    printf("\n");

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            switch (forest->e[i][j]) {
            case EMPTY:   printf(". "); break;
            case TREE:    printf("T "); break;
            case IGNITED: printf("G "); break;
            case BURNING: printf("X "); break;
            }
        }
        printf("\n");
    }
}

static void init_forest(Forest *forest) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            forest->e[i][j] = random() < 0.5 ? TREE : EMPTY;
            if (forest->e[i][j] == TREE)
                forest->num_tree++;
        }
    }

    print_forest(forest, 0, START);
}

static void sim_forest(Forest *forest, F32 prob_ignite, F32 prob_spread, U32 time) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (forest->e[i][j] == TREE) {
                if (random() < prob_ignite) {
                    forest->e[i][j] = IGNITED;
                    forest->num_ignited++;
                    if (!forest->is_burning) {
                        forest->is_burning = true;
                        forest->ignite_start_time = time;
                    }
                } else {
                    for (int di = -1; di <= 1; di++) {
                        for (int dj = -1; dj <= 1; dj++) {
                            if (di == 0 && dj == 0) continue;
                            int ni = i + di, nj = j + dj;
                            if (ni >= 0 && ni < ROWS && nj >= 0 && nj < COLS) {
                                if (forest->e[ni][nj] == BURNING && random() < prob_spread) {
                                    forest->e[i][j] = IGNITED;
                                    forest->num_ignited++;
                                    break;
                                }
                            }
                        }
                    }
                }
            } else if (forest->e[i][j] == IGNITED) {
                forest->e[i][j] = BURNING;
                forest->num_burning++;
                forest->num_ignited--;
            }
        }
    }
}

int main() {
    rseed = 123456789;

    Forest forest = {};
    init_forest(&forest);

    rseed = time(0);

    U32 index = 0;

    while (forest.num_burning < (forest.num_tree / 2)) {
        sim_forest(&forest, 0.0001, 0.1, index);
        if (index % 10 == 0 && forest.is_burning)
            print_forest(&forest, index, DEFAULT);
        index++;
    }

    print_forest(&forest, index, DEFAULT);

    return 0;
}
