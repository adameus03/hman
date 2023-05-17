typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long long ull;

#include "hman.h"
#include <cstring>
//#include "tests.h" //debug
#include <iostream> //verbose
#include "bt_print.h" //verbose
#include <windows.h> //verbose



/**
    @brief
        Symbol-frequency map function
        Used to catalogue data by measuring symbol-frequency stats
    @param data_buffer
        Input data to catalogue
    @param data_len
        Length of the input data buffer
    @param symbol_buffer
        Pre-allocated 256B output buffer for unique symbols occuring in the data_buffer.
        The symbols will be stored at the beginning of the buffer
    @param freq_buffer
        Pre-allocated 256*8B output buffer for storing the number of occurences of the unique symbols
        in the input data buffer
    @param uniq_symbol_cnt
        Number of unique symbols detected in the input data buffer. Each symbol has size of 1B.

*/


void symfreq_catalogue(uchar* data_buffer, const ull& data_len, uchar* symbol_buffer, ull* freq_buffer, uchar& uniq_symbol_cnt){//here debug + ip
    uchar* dhead = data_buffer;
    uchar* dtail = data_buffer+data_len;
    ull* fhead = freq_buffer;

    ull* nullfreq = new ull[0x100];
    ull* nullfreq_head = nullfreq;
    ull* nullfreq_tail = nullfreq+0x100;


    while(nullfreq_head != nullfreq_tail){
        *nullfreq_head++ = 0x0;
    }

    while(dhead != dtail){

        *(nullfreq+*dhead) = *(nullfreq+*dhead) + 1;
        dhead++;
    }

    uchar* shead = symbol_buffer;
    uniq_symbol_cnt=0x0;
    uchar s = 0x0;
    nullfreq_head = nullfreq;

    while(nullfreq_head != nullfreq_tail){
        if(*nullfreq_head){
            *shead++ = s;
            uniq_symbol_cnt++;
            *fhead++ = *nullfreq_head;
        }
        s++;
        nullfreq_head++;
    }

    delete[] nullfreq;

    std::cout << "<Uniq symbol cnt> " << (int)uniq_symbol_cnt << "</Uniq symbol cnt>" << std::endl;

}

//returns dest shift
// content_len - in bits!

/**
    @brief
        Bit injection function
    @param dest
        Input: Pointer reference of injection destination byte
        Output: first non-injected bit's byte pointer reference
    @param content
        The data sequence to inject
    @param conten_len
        Injection length
    @param dest_offset
        Input: Offset of injection
        Output: Offset of first non-injected bit
*/

size_t inject(uchar*& dest, const uint& content, const uchar& content_len, uchar& dest_offset){
    // przesun content o 32-content_len w lewo
    // przesun content o 24+dest_offset w prawo

    // wynikowo: przesun w prawo o 24+dest_offset-32+content_len = dest_offset+content_len-8 w prawo

    //dest_offset+content_len-0x8

    size_t dest_shift = 0x0;

    if(dest_offset+content_len>0x8){
        *dest |= (uchar)(content >> (dest_offset+content_len-0x8));
    }
    else {
        *dest |= (uchar)(content << (0x8-dest_offset-content_len));
    }
    dest++; dest_shift++;


    if(content_len>(0x8-dest_offset)){//
        uchar n = (content_len-(0x8-dest_offset)) >> 0x3;

        if((content_len-(0x8-dest_offset))%8) n++;

        for(uchar i=0x0; i<n; i++){
            // w lewo o 32-content_len
            // w lewo o 8-dest_offset
            // w lewo o 8*i
            // w prawo o 24
                                    /// 24-32+content_len-8+dest_offset-8*i = content_len+dest_offset-16-8*i
            /*
                 x  x  x  x  x  x  x  x    x  x  x  x  c0 c1 c2   c3 c4 c5 c6 c7 c8 c9 c10   c11 c12 c13 c14 c15 c16 c17 c18     u
                 32-19=13
                 c0 c1 c2 c3 c4 c5 c6 c7   c8 c9 c10 c11 c12 c13 c14 c15   c16 c17 c18 0 0 0 0 0    u<<=13
                 c4 c5 c6 c7 c8 c9 c10 c11    c12 c13 c14 c15 c16 c17 c18 0     00000000 00000000   u<<=4



            */

            // wynikowo: w prawo o 24-32-8+dest_offset+content_len-8*i
            // czyli: w prawo o dest_offset+content_len-16-8*i
            // czyli:  w prawo o dest_offset+content_len-2*(8+i)

            if((dest_offset+content_len) > ((0x2+i)<<0x3)){
                *dest++ = content >> (dest_offset+content_len-(((0x2+i))<<0x3));
            }
            else {
                *dest++ = content << ((((0x2+i))<<0x3)-dest_offset-content_len);
            }
            dest_shift++;
        }
    }

    dest_offset = (dest_offset+content_len)%0x8;
    if(dest_offset) {dest--; dest_shift--; }

    return dest_shift;
}

/**
    @brief
        Encode (Compress) data using cataloged symbol-frequency
    @param input_data
        Data buffer to encode
    @param input_len
        Input buffer bytelength
    @param symbol_buffer
        Buffer of unique bytes occurring in input_data
    @param freq_buffer
        Buffer of frequencies of unique bytes occurring in input_data
    @param n
        Length of symbol_buffer
    @param output_data
        Output buffer for compressed data
    @param output_len
        Resultant output buffer length
*/

void encodec(uchar* input_data, const size_t& input_len, uchar* symbol_buffer, ull* freq_buffer, const uchar& n, uchar* output_data, size_t& output_len){

    exnode* leaf_buffer = new exnode[0x100];

    hman_etree(symbol_buffer, freq_buffer, n, leaf_buffer);

    uchar* input_head = input_data;
    uchar* input_tail = input_data+input_len;

    memset(output_data, 0, input_len);

    uchar* output_head = output_data;
    uchar output_offset = 0x0;


    exnode relay;
    uint code = 0x0;
    uchar h = 0x0;
    output_len = 0x1;

    while(input_head != input_tail){

        relay = *(leaf_buffer+*input_head);
        code = 0x0;
        h = 0x0;

        while(relay->up){

            code |= (relay->polarity) << h++;
            relay = relay->up;
        }

        output_len += inject(output_head, code, h, output_offset);
        input_head++;

    }

    ///<append output offset byte>
    if(output_offset){
        *(output_data+output_len) = output_offset;
        output_len++;
    }
    else *(output_data+output_len-0x1) = 0x8;

    ///</append output offset byte>

}

/**
    @brief
        Decode (Decompress) data using cataloged symbol-frequency
    @param input_data
        Data buffer to decode
    @param input_len
        Input buffer bytelength
    @param symbol_buffer
        Buffer of unique bytes occurring in input_data
    @param freq_buffer
        Buffer of frequencies of unique bytes occurring in input_data
    @param n
        Length of symbol_buffer
    @param output_data
        Output buffer for decompressed data
    @param output_len
        Resultant output buffer length
*/

void decodec(uchar* input_data, const size_t& input_len, uchar* symbol_buffer, ull* freq_buffer, const uchar& n, uchar* output_data, size_t& output_len){

    dnode root = hman_dtree(symbol_buffer, freq_buffer, n);

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8); //blue
    printBT(&root, nullptr);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); //green

    uchar* input_head = input_data;
    uchar input_offset = 0x0;
    uchar* input_tail = input_data+input_len;

    dnode* dnode_head = &root; //readonly head

    uchar* output_head = output_data;
    output_len = 0x0;

    uchar breakout;

    // last byte is output_offset of encodec
    if(input_len < 0x2){
        return;
    }

    while(input_head != input_tail-0x1){

        breakout = 0x0;
        uchar max_offset = (input_head == (input_tail-2)) ? *(input_tail-1) : 0x8; // last byte is output_offset of encodec

        while(input_offset < max_offset){
            if((*input_head>>(0x7-input_offset))&0x1){
                if(dnode_head->right->symbol){
                    *output_head = *(dnode_head->right->symbol);
                    output_len++;
                    output_head++;
                    breakout = 0x1;
                    dnode_head = &root;
                }
                else dnode_head = dnode_head->right;
            }
            else {
                if(dnode_head->left->symbol){
                    *output_head = *(dnode_head->left->symbol);
                    output_len++;
                    output_head++;
                    breakout = 0x1;
                    dnode_head = &root;
                }
                else dnode_head = dnode_head->left;
            }
            input_offset++;
            if(breakout){
                break;
            }
        }
        if(input_offset==max_offset){

            input_offset = 0x0;
            input_head++;
        }
    }
}
