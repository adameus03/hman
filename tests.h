#ifndef TESTS_H_INCLUDED
#define TESTS_H_INCLUDED

#include <iostream>
#include "code.h"

typedef unsigned char uchar;
typedef unsigned long long ull;

void printbp(const uchar* data, const size_t& bitlen);

void printu(ull u);

void checker(const uchar& ok, const char* description);

uchar test_symfreq_catalogue();

uchar test_min_dxnode();

uchar test_min_exnode();

uchar test_hman_dtree();

uchar test_hman_etree();

#endif // TESTS_H_INCLUDED
