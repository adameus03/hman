#ifndef TESTS_H_INCLUDED
#define TESTS_H_INCLUDED

#include <iostream>
#include "code.h"
#include "hman.h"

//enum NODEDIR;

struct dnode;

struct enode;

typedef unsigned char uchar;
typedef unsigned long long ull;
typedef enode* exnode;
typedef dnode* dxnode;


void printbp(const uchar* data, const size_t& bitlen);

void printu(ull u);

void checker(const uchar& ok, const char* description);

void inside_checker(const uchar& ok, const char* description);

void conditional_inside_checker(const uchar& toggle, const uchar& ok, const char* description);

void expected_conditional_inside_checker(const uchar& toggle, const uchar& ok, const char* expected, const char* actual, const char* description);

void print_dxnode(dxnode node);

void print_exnode(exnode node);




uchar test_symfreq_catalogue();

uchar test_min_dxnode();

uchar test_min_exnode();

uchar test_hman_dtree();

uchar test_hman_etree();


uchar test_encodec();

uchar test_decodec();

#endif // TESTS_H_INCLUDED
