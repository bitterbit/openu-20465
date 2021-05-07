#include <stdio.h>
#include <stdbool.h>
#include "set.h"

/* #define MAX_64 0xffffffffffffffff */

#define MAX_A 32 
#define MAX_B 64
#define MAX_C 96 
#define MAX_D 128

void reset_set(Set *set) {
    set->a = 0;
    set->b = 0;
    set->c = 0;
    set->d = 0;
}

void debug_set(Set *set) {
    /* printf("set %p %u %u %u %u\n", set, set->a, set->b, set->c, set->d); */
}

void put_set(Set *set, uint8_t i) {
    if (i < MAX_A) {
        set->a |= (1 << i);
    } else if(i < MAX_B) {
        i -= MAX_A;
        set->b |= 1 << i;
    } else if (i < MAX_C) {
        i -= MAX_B;
        set->c |= 1 << i;
    } else {
        i -= MAX_C;
        set->d |= 1 << i;
    }
}

void print_set_item(int val, bool *first_item) {
    if (*first_item == false) {
        printf(",");
    } else {
        *first_item = false;
    }

    printf("%d", val);
}

void print_set(Set *set) {
    debug_set(set);

    int i;
    bool first_item = true;

    for(i=0; i<MAX_A; i++) {
        uint32_t mask = 1 << i;
        if (mask & set->a) {
            print_set_item(i, &first_item);
        }
    }

    for(i=MAX_A; i<MAX_B; i++) {
        uint32_t mask = 1 << (i - MAX_A);
        if (mask & set->b) {
            print_set_item(i, &first_item);
        }
    }

    for(i=MAX_B; i<MAX_C; i++) {
        uint32_t mask = 1 << (i - MAX_B);
        if (mask & set->c) {
            print_set_item(i, &first_item);
        }
    }

    for(i=MAX_C; i<MAX_D; i++) {
        uint32_t mask = 1 << (i - MAX_C);
        if (mask & set->d) {
            print_set_item(i, &first_item);
        }
    }
}

#define UNION_SET(x) set_c->x = set_a->x | set_b->x
void union_set(Set *set_a, Set *set_b, Set *set_c) {
    UNION_SET(a);
    UNION_SET(b);
    UNION_SET(c);
    UNION_SET(d);
}

#define INTERSECT_SET(x) set_c->x = set_a->x & set_b->x
void intersect_set(Set *set_a, Set *set_b, Set *set_c) {
    INTERSECT_SET(a);
    INTERSECT_SET(b);
    INTERSECT_SET(c);
    INTERSECT_SET(d);
}

void sub_set(Set *set_a, Set *set_b, Set *set_c) {
    printf("sub_set\n");
}

void symdiff_set(Set *set_a, Set *set_b, Set *set_c) {
    printf("symdiff_set\n");
}
