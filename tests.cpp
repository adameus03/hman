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

void inside_checker(const uchar& ok, const char* description){
    std::cout << '{' << (ok?"good":"!!") << "}  " << description << std::endl;
}


void conditional_inside_checker(const uchar& toggle, const uchar& ok, const char* description){
    if(toggle){
        inside_checker(ok, description);
    }
}
void expected_conditional_inside_checker(const uchar& toggle, const uchar& ok, const char* expected, const char* actual, const char* description){
    if(toggle){
        std::cout << '{' << (ok?"good":"!!") << "}  ( E: " << expected << ", A: " << actual << ") " << description << std::endl;
    }
}

void print_dxnode(dxnode node){
    if(node){
        std::cout << "[dnode symbol: ";
        if(node->symbol) printbp(node->symbol, 8);
        else std::cout << "null";
        std::cout << "freq: " << node->freq << "]" << std::endl;
    }
    else std::cout << "NULL DXNODE" << std::endl;
}

void print_exnode(exnode node){
    if(node){
        std::cout << "[enode symbol: ";
        if(node->symbol) printbp(node->symbol, 8);
        else std::cout << "null";
        std::cout << "freq: " << node->freq << "]" << std::endl;
    }
    else std::cout << "NULL EXNODE" << std::endl;
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

// //exnode* min_exnode(exnode* buffer, uchar len)
//uchar min_exnode(exnode* buffer, exnode*& minptr, uchar len)
uchar test_min_exnode(){
    /* TEST 1 */

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

    uchar minret;

    //std::cout << "I'm alive" << std::endl;
    exnode* min1;
    minret = min_exnode(buffer, min1, 0x8);
    //std::cout << "I'm alive 2" << std::endl;
    uchar retval = 0x1;
    if(min1 != buffer+3) retval = 0x0;
    if(minret) retval = 0x0;

    delete[] buffer;
    delete[] symbol_buffer;

    /* TEST 2 */

    /*symbol_buffer = new uchar[0x3];
    *symbol_buffer = 0x44;
    *(symbol_buffer+1) = 0x88;
    *(symbol_buffer+2) = 0x66;*/

    buffer = new exnode[0x3];
    *buffer = NULL;
    *(buffer+1) = NULL;
    *(buffer+2) = new enode;
    (*(buffer+2))->symbol = NULL;
    (*(buffer+2))->freq = 0x6;

    minret = min_exnode(buffer, min1, 0x3);
    if(min1 != buffer+2) retval = 0x0;
    if(!minret) retval = 0x0;

    delete[] buffer;


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

    /* TEST 1 */

    /*uchar* symbol_buffer = new uchar[0x3];
    *symbol_buffer = 0x44;
    *(symbol_buffer+1) = 0x88;
    *(symbol_buffer+2) = 0x66;
    ull* freq_buffer = new ull[0x3];
    *freq_buffer = 0x1;
    *(freq_buffer+1) = 0x2;
    *(freq_buffer+2) = 0x3;
    uchar n = 0x3;

    exnode* leaf_buffer = new exnode[0x100]; // !

    hman_etree(symbol_buffer, freq_buffer, n, leaf_buffer);

    uchar retval = 0x1;

    if(leaf_buffer[0x44]->freq != 0x1) retval = 0x0;
    if(leaf_buffer[0x88]->freq != 0x2) retval = 0x0;
    if(leaf_buffer[0x66]->freq != 0x3) retval = 0x0;

    if(leaf_buffer[0x44]->symbol != symbol_buffer) retval = 0x0;
    if(leaf_buffer[0x88]->symbol != symbol_buffer+1) retval = 0x0;
    if(leaf_buffer[0x66]->symbol != symbol_buffer+2) retval = 0x0;

    if(leaf_buffer[0x44]->up->freq != 0x3) retval = 0x0;
    if(leaf_buffer[0x44]->up->symbol != NULL) retval = 0x0;
    if(leaf_buffer[0x44]->up->up->freq != 0x6) retval = 0x0;
    if(leaf_buffer[0x44]->up->up->up != NULL){
        retval = 0x0;

        print_exnode(leaf_buffer[0x44]);
        print_exnode(leaf_buffer[0x44]->up);
        print_exnode(leaf_buffer[0x44]->up->up);
        std::cout << "!!!!!!!!!!!!!" << std::endl;
        print_exnode(leaf_buffer[0x44]->up->up->up);
        std::cout << "!!!!!!!!!!!!!" << std::endl;
    }



    if(leaf_buffer[0x44]->up != leaf_buffer[0x88]->up) retval = 0x0;
    if(leaf_buffer[0x66]->up != leaf_buffer[0x44]->up->up) retval = 0x0;

    delete[] symbol_buffer;
    delete[] freq_buffer;
    delete[] leaf_buffer;*/

    std::cout << "etree test 2" << std::endl;

    /* TEST 2 */ uchar retval = 0x1;

    uchar* symbol_buffer = new uchar[0x3];
    *symbol_buffer = 0x61;
    *(symbol_buffer+1) = 0x62;
    *(symbol_buffer+2) = 0x63;
    ull* freq_buffer = new ull[0x3];
    *freq_buffer = 0x1;
    *(freq_buffer+1) = 0x1;
    *(freq_buffer+2) = 0x1;
    uchar n = 0x3;

    /*for(uchar i=0x0; i<n; i++){
        printu(*(freq_buffer+i));
    }*/

    //exnode* leaf_buffer = new exnode[n];
    exnode* leaf_buffer = new exnode[0x100]; // !

    //std::cout << "Bef ecall" << std::endl;
    hman_etree(symbol_buffer, freq_buffer, n, leaf_buffer);
    //std::cout << "Aft ecall" << std::endl;

    if(leaf_buffer[0x61]->freq != 0x1) retval = 0x0;
    if(leaf_buffer[0x62]->freq != 0x1) retval = 0x0;
    if(leaf_buffer[0x63]->freq != 0x1) retval = 0x0;

    if(leaf_buffer[0x61]->symbol != symbol_buffer) retval = 0x0;
    if(leaf_buffer[0x62]->symbol != symbol_buffer+1) retval = 0x0;
    if(leaf_buffer[0x63]->symbol != symbol_buffer+2) retval = 0x0;

    if(leaf_buffer[0x61]->up->freq != 0x2) retval = 0x0;
    if(leaf_buffer[0x61]->up->symbol != NULL) retval = 0x0;
    if(leaf_buffer[0x61]->up->up->freq != 0x3) retval = 0x0;
    if(leaf_buffer[0x61]->up->up->up != NULL){
        retval = 0x0;

        print_exnode(leaf_buffer[0x61]);
        print_exnode(leaf_buffer[0x61]->up);
        print_exnode(leaf_buffer[0x61]->up->up);
        std::cout << "!!!!!!!!!!!!!" << std::endl;
        print_exnode(leaf_buffer[0x61]->up->up->up);
        std::cout << "!!!!!!!!!!!!!" << std::endl;
    }



    if(leaf_buffer[0x61]->up != leaf_buffer[0x62]->up) retval = 0x0;
    if(leaf_buffer[0x63]->up != leaf_buffer[0x61]->up->up) retval = 0x0;

    delete[] symbol_buffer;
    delete[] freq_buffer;
    delete[] leaf_buffer;


    return retval;
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

    //printbp(symbol_buffer, 40);

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

//void encodec(uchar* input_data, const size_t& input_len, uchar* symbol_buffer, ull* freq_buffer, const uchar& n, uchar* output_data, size_t& output_len);
uchar test_encodec(){ //return 0x0;
    /* TEST 1 */

    uchar* input_data = new uchar[0x3];
    *input_data = 0x44;
    *(input_data+1) = 0x88;
    *(input_data+2) = 0x66;

    size_t input_len = 0x3;
    uchar* symbol_buffer  = new uchar[0x3];
    *symbol_buffer = 0x44;
    *(symbol_buffer+1) = 0x88;
    *(symbol_buffer+2) = 0x66;

    ull* freq_buffer = new ull[0x3];
    *freq_buffer = 0x1;
    *(freq_buffer+1) = 0x2;
    *(freq_buffer+2) = 0x3;

    uchar n = 0x3;
    uchar* output_data = new uchar[input_len];
    size_t output_len = 0x0;

    std::cout << "Bef encodec call" << std::endl;
    encodec(input_data, input_len, symbol_buffer, freq_buffer, n, output_data, output_len);
    std::cout << "Aft encodec call" << std::endl;

    uchar retval = 0x1;
    std::cout << "Conditions" << std::endl;
    std::cout << output_len << std::endl;
    if(output_len != 0x1) retval = 0x0;
    else if(((*output_data) & 0xf8) != 0x18) retval = 0x0;

    printbp(output_data, 8);

    delete[] input_data;
    delete[] symbol_buffer;
    delete[] freq_buffer;
    delete[] output_data;

    /* TEST 2 */

    std::cout << "--Test 2--" << std::endl;

    input_data = new uchar[0x3];
    *input_data = 0x61;
    *(input_data+1) = 0x62;
    *(input_data+2) = 0x63;

    input_len = 0x3;
    symbol_buffer = new uchar[0x3];
    *symbol_buffer = 0x61;
    *(symbol_buffer+1) = 0x62;
    *(symbol_buffer+2) = 0x63;

    freq_buffer = new ull[0x3];
    *freq_buffer = 0x1;
    *(freq_buffer+1) = 0x1;
    *(freq_buffer+2) = 0x1;

    n = 0x3;
    output_data = new uchar[input_len];
    output_len = 0x0;
    encodec(input_data, input_len, symbol_buffer, freq_buffer, n, output_data, output_len);
    std::cout << "Conditions (II)" << std::endl;
    if(output_len != 0x1) retval = 0x0;
    else if(((*output_data)&0xf8) != 0xb0) retval = 0x0;

    std::cout << "output2" << std::endl;
    printbp(output_data, 8);

    delete[] input_data;
    delete[] symbol_buffer;
    delete[] freq_buffer;
    delete[] output_data;

    return retval;

}

//void decodec(uchar* input_data, const size_t& input_len, uchar* symbol_buffer, ull* freq_buffer, const uchar& n, uchar* output_data, size_t& output_len)
uchar test_decodec(){
    std::cout << "Test decodec" << std::endl;
    uchar* input_data = new uchar[0x1];

    size_t input_len = 0x1;
    uchar* symbol_buffer = new uchar[0x3];
    ull* freq_buffer = new ull[0x3];
    uchar n = 0x3;
    uchar* output_data = new uchar[n];
    size_t output_len = 0x0;

    decodec(input_data, input_len, symbol_buffer, freq_buffer, n, output_data, output_len);

    uchar retval = 0x1;
    if(output_len != 0x3) retval = 0x0;
    else if(*output_data != 0x44) retval = 0x0;
    else if(*(output_data+1) != 0x88) retval = 0x0;
    else if(*(output_data+2) != 0x66) retval = 0x0;

    delete[] symbol_buffer;
    delete[] freq_buffer;
    delete[] output_data;

    return retval;
}

