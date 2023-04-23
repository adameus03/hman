#ifndef HMAN_H_INCLUDED
#define HMAN_H_INCLUDED

#include <cstddef>

struct node {
    node* left;
    node* right;
    node* up;
    unsigned char* symbol;
    unsigned long long freq;
};



#endif // HMAN_H_INCLUDED
