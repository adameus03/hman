#ifndef HMAN_H_INCLUDED
#define HMAN_H_INCLUDED

#include <cstddef>


enum NODEDIR{
    LEFT = 0x0,
    RIGHT = 0x1
};

struct dnode {
    dnode* left;
    dnode* right;
    unsigned char* symbol;
    unsigned long long freq;
};

struct enode {
    enode* up;
    unsigned char* symbol;
    unsigned long long freq;
    NODEDIR polarity;
};

typedef unsigned char uchar;
typedef unsigned long long ull;
typedef dnode* dxnode; //for decoding
typedef enode* exnode; //for encoding

//void symfreq_catalogue(uchar* data_buffer, const ull& data_len, uchar* symbol_buffer, ull* freq_buffer, uchar& uniq_symbol_cnt);

dxnode* min_dxnode(dxnode* buffer, uchar len);

exnode* min_exnode(exnode* buffer, uchar len);

dnode hman_dtree(uchar* symbol_buffer, ull* freq_buffer, const uchar& n);

void hman_etree(uchar* symbol_buffer, ull* freq_buffer, const uchar& n, exnode* leaf_buffer);



#endif // HMAN_H_INCLUDED
