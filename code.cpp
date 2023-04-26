typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long long ull;

#include "hman.h"
#include <cstring>
#include "tests.h" //debug
#include <iostream> //debug



/**
    @brief
        Symbol-frequency map function
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


void symfreq_catalogue(uchar* data_buffer, const ull& data_len, uchar* symbol_buffer, ull* freq_buffer, uchar& uniq_symbol_cnt){
    uchar* dhead = data_buffer;
    uchar* dtail = data_buffer+data_len;
    ull* fhead = freq_buffer;
    //ull* ftail = freq_buffer+0x100;

    ull* nullfreq = new ull[0x100];
    ull* nullfreq_head = nullfreq;
    ull* nullfreq_tail = nullfreq+0x100;


    while(nullfreq_head != nullfreq_tail){
        *nullfreq_head++ = 0x0;
    }


    //printbp(data_buffer, 40);

    while(dhead != dtail){

        // *(freq_buffer+*dhead++)++; // MinGW g++ tak nie pozwala
        ++*(nullfreq+*dhead++);    // a tak pozwala

        /*ull* f = nullfreq_buffer + *dhead;
        (*freq)++;
        dhead++;*/

    }

    /*for(int i=0; i<256; i++){
        std::cout << std::hex << i <<std::dec << "    ";
        printu(*(nullfreq+i));
    }*/

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

    //printbp(symbol_buffer, 40);

    delete[] nullfreq;

}


/*void encode(uchar* input_data, uchar* output_data, const size_t& len){

}*/

void encodec(uchar* input_data, const size_t& input_len, uchar* symbol_buffer, ull* freq_buffer, const uchar& n, uchar* output_data, size_t& output_len){
    std::cout << "Start encodec" << std::endl;
    exnode* leaf_buffer = new exnode[0x100];
    std::cout << "Bef ETREE call" << std::endl;



    hman_etree(symbol_buffer, freq_buffer, n, leaf_buffer);
    std::cout << "Aft ETREE call" << std::endl;

    uchar* input_head = input_data;
    uchar* input_tail = input_data+input_len;

    std::cout << "bef memset" << std::endl;
    memset(output_data, 0, input_len); // ! - not 0x100
    std::cout << "aft memset" << std::endl;

    uchar* output_head = output_data;
    uchar output_offset = 0x0;

    exnode relay;
    uint code = 0x0; // 32b or maybe 64b ? (if changed, in next lines refactoring neccessary)
    uchar h = 0x0;
    output_len = 0x1;

    inside_checker(*input_data == 0x44, "*input_data == 0x44");
    inside_checker(*(input_data+1) == 0x88, "*(input_data+1) == 0x88");
    inside_checker(*(input_data+2) == 0x66, "*(input_data+2) == 0x66");

    while(input_head != input_tail){
        relay = *(leaf_buffer+*input_head);
        code = 0x0;
        h = 0x0;
        std::cout << "relayup start" << std::endl;
        if(input_head == input_data){
            inside_checker(relay->up->up->up == NULL, "relay->up->up->up == NULL for leaf 0x44");
        }
        else if(input_head == input_data+1){
            inside_checker(relay->up->up->up == NULL, "relay->up->up->up == NULL for leaf 0x88");
        }
        else if(input_head == input_data+2){
            inside_checker(relay->up->up == NULL, "relay->up->up == NULL for leaf 0x66");
        }
        //conditional_inside_checker(input_head == input_data+1, relay->up->up->up == NULL, "relay->up->up->up == NULL for leaf 0x88");
        //conditional_inside_checker(input_head == input_data+2, relay->up->up == NULL, "relay->up->up == NULL for leaf 0x66");

        while(relay->up){
            //print_exnode(relay);
            //inside_checker(relay->up->up == NULL, "relay->up->up == NULL");

            code |= (relay->polarity) << h++;
            relay = relay->up;
            //std::cout << "(" << (int)h << ")" << std::endl;
        }

        expected_conditional_inside_checker(input_head == input_data, h==0x2, "2", std::to_string(h).c_str(), "Length of code for 0x44");
        expected_conditional_inside_checker(input_head == input_data+1, h==0x2, "2", std::to_string(h).c_str(), "Length of code for 0x88");
        expected_conditional_inside_checker(input_head == input_data+2, h==0x1, "1", std::to_string(h).c_str(), "Length of code for 0x66");

        printu(code);

        std::cout << "relayup end" << std::endl;

        /*code << 0x20-h;
        *output_head |= code >> (0x18+output_offset);*/


        code <<= 0x20-h;

        std::cout << "maploop start" << std::endl;
        while(h>=0x8-output_offset){
            std::cout << "OVERFLOWING" << std::endl;
            h -= 0x8-output_offset;
            *output_head++ |= (code >> (0x18+output_offset));
            output_len++;
            output_offset = 0x0;
            code <<= 0x8-output_offset;
        }



        std::cout << "maploop end" << std::endl;


        *output_head |= (code >> (0x18+output_offset));
        std::cout << "xdc" << std::endl;
        output_offset += h;


        /*h -= 0x8-output_offset;
        code << 0x8-output_offset;
        while(h!=0x0){
            *(++output_head) |= code >> 0x18;
            if(h>0x8){

            }
        }*/
        std::cout << output_len << std::endl;
        printbp(output_data, output_len*8);

        input_head++;

    }
    /*if(output_offset){
        output_len++;
    }*/
    if(output_offset==0) output_len--;
    std::cout << "end encodec" << std::endl;
}

void decodec(uchar* input_data, const size_t& input_len, uchar* symbol_buffer, ull* freq_buffer, const uchar& n, uchar* output_data, size_t& output_len){
    dnode root = hman_dtree(symbol_buffer, freq_buffer, n);

    uchar* input_head = input_data;
    uchar input_offset = 0x0;
    uchar* input_tail = input_data+input_len;

    dnode* dnode_head = &root; //readonly head

    uchar* output_head = output_data;
    output_len = 0x0;

    uchar breakout;

    while(input_head != input_tail){
        breakout = 0x0;
        while(input_offset < 0x8){
            if((*input_head>>(0x7-input_offset))&0x1){
                if(dnode_head->left->symbol){
                    *output_head++ = *(dnode_head->left->symbol);
                    output_len++;
                    breakout = 0x1;
                }
                else dnode_head = dnode_head->left;
            }
            else {
                if(dnode_head->right->symbol){
                    *output_head++ = *(dnode_head->right->symbol);
                    output_len++;
                    breakout = 0x1;
                }
                else dnode_head = dnode_head->right;
            }
            input_offset++;
            if(breakout){
                break;
            }
        }
        if((!breakout) || input_offset==0x8){
            input_offset = 0x0;
        }
    }
}
