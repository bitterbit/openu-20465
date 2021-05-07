#include <stdlib.h>

#ifndef _SET_H_
#define _SET_H_

struct Set {
    // low
    uint32_t a;
    uint32_t b;
    uint32_t c;
    uint32_t d;
    // high
};

typedef struct Set Set;

/* void read_set(Set *set, uint8_t i); */ 
void reset_set(Set *set);
void put_set(Set *set, uint8_t i);
void print_set(Set *set); 
void union_set(Set *set_a, Set *set_b, Set *set_c); 
void intersect_set(Set *set_a, Set *set_b, Set *set_c);
void sub_set(Set *set_a, Set *set_b, Set *set_c);
void symdiff_set(Set *set_a, Set *set_b, Set *set_c);

#endif
