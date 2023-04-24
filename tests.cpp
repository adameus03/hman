#include <iostream>
#include "code.h"
#include "hman.h"

typedef unsigned char uchar;
typedef unsigned long long ull;
typedef dnode* dxnode; //for decoding
typedef enode* exnode; //for encoding

void printbp(const uchar* data, const size_t& bitlen){
    size_t len = bitlen / 8;
    uchar blm = (uchar)(bitlen%8);

    for(size_t i=0; i<len; i++){
        uchar* c = (uchar*)data+i;
        for(uchar b=0; b<8u; b++){
            uchar m = 0x80u >> b;
            uchar t = *c & m;
            if(t >> (7u-b)) std::cout << '1';
            else std::cout << '0';
        }
        std::cout << ' ';
    }
    if(blm != 0u){
        uchar* c = (uchar*)data+len;
        for(uchar b=0; b<blm; b++){
            uchar m = 0x80u >> b;
            uchar t = *c & m;
            if(t >> (7u-b)) std::cout << '1';
            else std::cout << '0';
        }
    }
    std::cout << std::endl;
}

void printu(ull u){
    uchar* data = new uchar[0x40];
    for(uchar i=0; i<0x40; i++){
        *data = (uchar)(u>>0x38);
        *(data+1) = (uchar)(u>>0x30) & 0xff;
        *(data+2) = (uchar)(u>>0x28) & 0xff;
        *(data+3) = (uchar)(u>>0x20) & 0xff;
        *(data+4) = (uchar)(u>>0x18) & 0xff;
        *(data+5) = (uchar)(u>>0x10) & 0xff;
        *(data+6) = (uchar)(u>>0x8) & 0xff;
        *(data+7) = (uchar)(u) & 0xff;
    }
    printbp(data, 64);
    delete[] data;
}

void checker(const uchar& ok, const char* description){
    std::cout << '[' << (ok?"OK":"!!") << "]  " << description << std::endl;
}

//void symfreq_catalogue(uchar* data_buffer, const ull& data_len, uchar* symbol_buffer, ull* freq_buffer, uchar& uniq_symbol_cnt)
uchar test_symfreq_catalogue(){
    uchar* data_buffer = new uchar[5];
    *data_buffer = 0x11;
    *(data_buffer+1) = 0x22;
    *(data_buffer+2) = 0x33;
    *(data_buffer+3) = 0x44;
    *(data_buffer+4) = 0x55;

    uchar* symbol_buffer = new uchar[0x100];
    ull* freq_buffer = new ull[0x100];
    uchar uniq_symbol_cnt;

    symfreq_catalogue(data_buffer, 0x5, symbol_buffer, freq_buffer, uniq_symbol_cnt);

    uchar retval = 0x1;
    if(uniq_symbol_cnt != 0x5) retval = 0x0;
    if(!(    *symbol_buffer == 0x11 &&
         *(symbol_buffer+1) == 0x22 &&
         *(symbol_buffer+2) == 0x33 &&
         *(symbol_buffer+3) == 0x44 &&
         *(symbol_buffer+4) == 0x55)) retval = 0x0;

    printbp(symbol_buffer, 40);

    if(!(    *freq_buffer == 0x1 &&
         *(freq_buffer+1) == 0x1 &&
         *(freq_buffer+2) == 0x1 &&
         *(freq_buffer+3) == 0x1 &&
         *(freq_buffer+4) == 0x1)) retval = 0x0;
    delete[] data_buffer;
    delete[] symbol_buffer;
    delete[] freq_buffer;
    return retval;
}

//dxnode* min_dxnode(dxnode* buffer, uchar len)
uchar test_min_dxnode(){
    dxnode* buffer = new dxnode[0x8];

    uchar* symbol_buffer = new uchar[0x3];
    *symbol_buffer = 0x33;
    *(symbol_buffer+1) = 0x66;
    *(symbol_buffer+2) = 0x44;

    *buffer = NULL;
    *(buffer+1) = NULL;
    *(buffer+2) = new dnode;
    (*(buffer+2))->symbol = symbol_buffer;
    (*(buffer+2))->freq = 0x5;
    *(buffer+3) = new dnode;
    (*(buffer+3))->symbol = symbol_buffer+1;
    (*(buffer+3))->freq = 0x4;
    *(buffer+4) = NULL;
    *(buffer+5) = NULL;
    *(buffer+6) = new dnode;
    (*(buffer+6))->symbol = symbol_buffer+2;
    (*(buffer+6))->freq = 0x6;
    *(buffer+7) = NULL;

    //std::cout << "I'm alive" << std::endl;

    dxnode* min1 = min_dxnode(buffer, 0x8);
    //std::cout << "I'm alive 2" << std::endl;
    uchar retval = 0x1;
    if(min1 != buffer+3) retval = 0x0;
    //delete[] buffer;
    return retval;
}

//exnode* min_exnode(exnode* buffer, uchar len)
uchar test_min_exnode(){
    exnode* buffer = new exnode[0x8];

    uchar* symbol_buffer = new uchar[0x3];
    *symbol_buffer = 0x33;
    *(symbol_buffer+1) = 0x66;
    *(symbol_buffer+2) = 0x44;

    *buffer = NULL;
    *(buffer+1) = NULL;
    *(buffer+2) = new enode;
    (*(buffer+2))->symbol = symbol_buffer;
    (*(buffer+2))->freq = 0x5;
    *(buffer+3) = new enode;
    (*(buffer+3))->symbol = symbol_buffer+1;
    (*(buffer+3))->freq = 0x4;
    *(buffer+4) = NULL;
    *(buffer+5) = NULL;
    *(buffer+6) = new enode;
    (*(buffer+6))->symbol = symbol_buffer+2;
    (*(buffer+6))->freq = 0x6;
    *(buffer+7) = NULL;

    //std::cout << "I'm alive" << std::endl;

    exnode* min1 = min_exnode(buffer, 0x8);
    //std::cout << "I'm alive 2" << std::endl;
    uchar retval = 0x1;
    if(min1 != buffer+3) retval = 0x0;
    //delete[] buffer;
    return retval;
}

//dnode hman_dtree(uchar* symbol_buffer, ull* freq_buffer, const uchar& n)
uchar test_hman_dtree(){
    uchar* symbol_buffer = new uchar[0x3];
    *symbol_buffer = 0x44;
    *(symbol_buffer+1) = 0x88;
    *(symbol_buffer+2) = 0x66;
    ull* freq_buffer = new ull[0x3];
    *freq_buffer = 0x1;
    *(freq_buffer+1) = 0x2;
    *(freq_buffer+2) = 0x3;
    uchar n = 0x3;

    //std::cout << "bef call" << std::endl;
    dnode root = hman_dtree(symbol_buffer, freq_buffer, n);
    //std::cout << "aft call" << std::endl;


    uchar retval = 0x1;

    //printu(root.freq);


    if(root.freq != 0x6) retval = 0x0;
    if(root.symbol != NULL) retval = 0x0;

    if(root.left->freq != 0x3) retval = 0x0;
    if(root.left->symbol != NULL) retval = 0x0;

    if(root.right->freq != 0x3) retval = 0x0;
    if(root.right->symbol != symbol_buffer+2) retval=0x0;

    if(root.left->left->freq != 0x1) retval = 0x0;
    if(root.left->left->symbol != symbol_buffer) retval=0x0;

    if(root.left->right->freq != 0x2) retval = 0x0;
    if(root.left->right->symbol != symbol_buffer+1) retval = 0x0;

    return retval;
}

//void hman_etree(uchar* symbol_buffer, ull* freq_buffer, const uchar& n, exnode* leaf_buffer)

uchar test_hman_etree(){
    uchar* symbol_buffer = new uchar[0x3];
    *symbol_buffer = 0x44;
    *(symbol_buffer+1) = 0x88;
    *(symbol_buffer+2) = 0x66;
    ull* freq_buffer = new ull[0x3];
    *freq_buffer = 0x1;
    *(freq_buffer+1) = 0x2;
    *(freq_buffer+2) = 0x3;
    uchar n = 0x3;

    /*for(uchar i=0x0; i<n; i++){
        printu(*(freq_buffer+i));
    }*/

    //exnode* leaf_buffer = new exnode[n];
    exnode* leaf_buffer = new exnode[0x100]; // !

    std::cout << "Bef ecall" << std::endl;
    hman_etree(symbol_buffer, freq_buffer, n, leaf_buffer);
    std::cout << "Aft ecall" << std::endl;

    uchar retval = 0x1;
    /*if((*leaf_buffer)->freq != 0x1) retval = 0x0;
    if((*(leaf_buffer+1))->freq != 0x2) retval = 0x0;
    if((*(leaf_buffer+2))->freq != 0x3) retval = 0x0;

    if((*leaf_buffer)->symbol != symbol_buffer) retval = 0x0;
    if((*(leaf_buffer+1))->symbol != symbol_buffer+1) retval = 0x0;
    if((*(leaf_buffer+2))->symbol != symbol_buffer+2) retval = 0x0;

    if((*leaf_buffer)->up->freq != 0x3) retval = 0x0;
    if((*leaf_buffer)->up->symbol != NULL) retval = 0x0;
    if((*leaf_buffer)->up->up->freq != 0x6) retval = 0x0;
    if((*leaf_buffer)->up->up->up != NULL) retval = 0x0;

    if((*leaf_buffer)->up != (*(leaf_buffer+1))->up) retval = 0x0;
    if((*(leaf_buffer+2))->up != (*leaf_buffer)->up->up) retval = 0x0;*/

    return retval;
}

