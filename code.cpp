typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long long ull;

#include "hman.h"
#include <cstring>
#include "tests.h" //debug
#include <iostream> //debug
#include "bt_print.h" //debug
#include <windows.h> //debug



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


void symfreq_catalogue(uchar* data_buffer, const ull& data_len, uchar* symbol_buffer, ull* freq_buffer, uchar& uniq_symbol_cnt){//here debug + ip
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
        //++*(nullfreq+*dhead++);    // a tak pozwala

        *(nullfreq+*dhead) = *(nullfreq+*dhead) + 1;
        dhead++;

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

    std::cout << "<Uniq symbol cnt> " << (int)uniq_symbol_cnt << "</Uniq symbol cnt>" << std::endl;

}

//returns dest shift
// content_len - in bits!
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

        //if(content_len-(0x8-dest_offset)-n) n++; // :)
        if((content_len-(0x8-dest_offset))%8) n++;

        //std::cout << "n=" << (int)n << std::endl;

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

            //*dest = content >> (dest_offset+content_len-16-8*i); break;

            if((dest_offset+content_len) > ((0x2+i)<<0x3)  /* dest_offset+content_len > 16+8*i */){
                //std::cout << "I condition" << std::endl;
                *dest++ = content >> (dest_offset+content_len-(((0x2+i))<<0x3));
                //*dest = content >> (dest_offset+content_len-16-8*i);
                //dest++;
            }
            else {
                //std::cout << "II condition" << std::endl;
                *dest++ = content << ((((0x2+i))<<0x3)-dest_offset-content_len);
                //*dest = content << (16+8*i-dest_offset-content_len);
                //dest++;
            }
            dest_shift++;

        }





    }

    dest_offset = (dest_offset+content_len)%0x8;
    if(dest_offset) {dest--; dest_shift--; }

    // (content_len - (8-dest_offset))%8






    return dest_shift;
}



/*void encode(uchar* input_data, uchar* output_data, const size_t& len){

}*/

void encodec(uchar* input_data, const size_t& input_len, uchar* symbol_buffer, ull* freq_buffer, const uchar& n, uchar* output_data, size_t& output_len){

    /*std::cout << "<input_data>" << std::endl;
    printbp(input_data, input_len*8);
    std::cout << "</input_data>" << std::endl;
    std::cout << "<input_len>" << input_len << "</input_len>" << std::endl;
    std::cout << "<n>" << (int)n << "</n>" << std::endl;
    std::cout << "<symbol_buffer>" << std::endl;
    printbp(symbol_buffer, n*8);
    std::cout << "</symbol_buffer>" << std::endl;*/

    exnode* leaf_buffer = new exnode[0x100];

    hman_etree(symbol_buffer, freq_buffer, n, leaf_buffer);

    //printBT()

    uchar* input_head = input_data;
    uchar* input_tail = input_data+input_len;

    memset(output_data, 0, input_len); // ! - not 0x100

    uchar* output_head = output_data;
    uchar output_offset = 0x0;


    exnode relay;
    uint code = 0x0; // 32b or maybe 64b ? (if changed, in next lines refactoring neccessary)
    uchar h = 0x0;
    output_len = 0x1;

    while(input_head != input_tail){
        /*getchar();
        std::cout << "<input_index>" << (int)(input_head-input_data) << "</input_index>" << std::endl;*/
        relay = *(leaf_buffer+*input_head);
        code = 0x0;
        h = 0x0;

        while(relay->up){

            code |= (relay->polarity) << h++;
            relay = relay->up;
        }

        /*code |= (relay->polarity) << h++;
        while(relay = relay->up){
            code |= (relay->polarity) << h++;
        }*/

        /*std::cout << "<symbol>0x" << std::hex << (int)*input_head << std::dec << "(" << *input_head << ")</symbol>" << std::endl;
        std::cout << "<code h='" << (int)h << "'>" << std::endl;
        printu(code);
        std::cout << "</code>" << std::endl;*/


        /* Tu jest blad bo code moze byc teoretycznie na 32 bitach */
        //code <<= 0x20-h;

        ////uchar inject(uchar*& dest, uint content, uchar content_len, uchar dest_offset)
        ////size_t inject(uchar*& dest, const uint& content, const uchar& content_len, const uchar& dest_offset_in, uchar& dest_offset_out)
        //size_t inject(uchar*& dest, const uint& content, const uchar& content_len, uchar& dest_offset)

        /*std::cout << "<output_head>";
        printf("%p", output_head);
        std::cout << "</output_head>" << std::endl;*/
        output_len += inject(output_head, code, h, output_offset);

        /*std::cout << "<output_data>" << std::endl;
        //printbp(output_data, 8*(input_len+0x1));
        std::cout << "</output_data>" << std::endl;

        std::cout << "<output_offset>" << (int)output_offset << "</output_offset>" << std::endl;*/



        input_head++;

    }
    /*if(output_offset){
        output_len++;
    }*/
    //if(!output_offset) output_len--;


    ///<append output offset byte>
    if(output_offset){
        *(output_data+output_len) = output_offset;
        output_len++;
    }
    else *(output_data+output_len-0x1) = 0x8;

    ///</append output offset byte>


    /*std::cout << "<output>" << std::endl;
    printbp(output_data, 8*output_len);
    std::cout << "</output>" << std::endl;*/

    //std::cout << "end encodec" << std::endl;
}

void decodec(uchar* input_data, const size_t& input_len, uchar* symbol_buffer, ull* freq_buffer, const uchar& n, uchar* output_data, size_t& output_len){

    /*std::cout << "DECODEC INPUT" << std::endl;
    std::cout << "Input: " << std::endl;
    printbp(input_data, 8*input_len);
    std::cout << "Symbol buffer: " << std::endl;
    printbp(symbol_buffer, 8*n);
    std::cout << "Freq buffer: " << std::endl;
    for(int i=0; i<n; i++){
        printu(*(freq_buffer+i));
    }
    std::cout << "n: " << std::endl;
    std::cout << (int)n << std::endl;*/


    dnode root = hman_dtree(symbol_buffer, freq_buffer, n);

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8); //blue
    printBT(&root, nullptr);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); //green


    /*inside_checker(*input_data == 0x18, "");
    inside_checker(input_len == 0x1, "");
    inside_checker(*symbol_buffer == 0x44, "");
    inside_checker(*(symbol_buffer+1) == 0x88, "");
    inside_checker(*(symbol_buffer+2) == 0x66, "");
    inside_checker(*freq_buffer == 0x1, "");
    inside_checker(*(freq_buffer+1) == 0x2, "");
    inside_checker(*(freq_buffer+2) == 0x3, "");
    inside_checker(n == 0x3, "");*/



    /*inside_checker(root.left->freq == 0x3, "root.left->freq == 0x3");
    inside_checker(root.left->symbol == NULL, "root.left->symbol == NULL");
    inside_checker(root.left->left->freq == 0x1, "root.left->left->freq == 0x1");
    inside_checker(*(root.left->left->symbol) == 0x44, "*(root.left->left->symbol) == 0x44");
    inside_checker(root.left->right->freq == 0x2, "root.left->right->freq == 0x2");
    inside_checker(*(root.left->right->symbol) == 0x88, "*(root.left->right->symbol) == 0x88");

    inside_checker(root.right->freq == 0x3, "root.right->freq == 0x3");
    inside_checker(*(root.right->symbol) == 0x66, "*(root.right->symbol) == 0x66");*/


    uchar* input_head = input_data;
    uchar input_offset = 0x0;
    uchar* input_tail = input_data+input_len;

    dnode* dnode_head = &root; //readonly head

    uchar* output_head = output_data;
    output_len = 0x0;

    /*uchar* freq_head = freq_buffer;
    uchar* freq_tail = freq_buffer+n;
    while(freq_head<freq_tail){
        output
    }*/

    uchar breakout;

    //std::cout << "Before Decodec loop" << std::endl;

    /*if(input_head == input_tail){
        return;
    }*/


    // last byte is output_offset of encodec
    if(input_len < 0x2){
        return;
    }

    while(input_head != input_tail-1){
        /*std::cout << "INPUT_INDEX=" << (int)(input_head-input_data) << std::endl;
        std::cout << "Decoding..." << std::endl;*/
        //std::cout << "Decodec iter" << std::endl;
        breakout = 0x0;

        uchar max_offset = (input_head == (input_tail-2)) ? *(input_tail-1) : 0x8; // last byte is output_offset of encodec

        //std::cout << (int)max_offset << std::endl;

        while(input_offset < max_offset){

            //std::cout << "INPUT_OFFSET=" << (int)input_offset << std::endl;
            if((*input_head>>(0x7-input_offset))&0x1){
                //Sleep(10); //system("cls");
                //std::cout << "RIGHT" << std::endl;
                //printBT(&root, dnode_head->right);
                if(dnode_head->right->symbol){
                    //std::cout << "SYMBOL [" << *(dnode_head->right->symbol) << "]" << std::endl;
                    *output_head = *(dnode_head->right->symbol);
                    output_len++;
                    output_head++;
                    breakout = 0x1;
                    dnode_head = &root;
                    //getchar();
                }
                else dnode_head = dnode_head->right;


            }
            else {
                //Sleep(10); //system("cls");
                //std::cout << "LEFT" << std::endl;
                //printBT(&root, dnode_head->left);
                if(dnode_head->left->symbol){
                    //std::cout << "SYMBOL [" << *(dnode_head->left->symbol) << "]" << std::endl;
                    *output_head = *(dnode_head->left->symbol);
                    output_len++;
                    output_head++;
                    breakout = 0x1;
                    dnode_head = &root;
                    //getchar();
                }
                else dnode_head = dnode_head->left;


            }
            input_offset++;
            if(breakout){
                break;
            }
        }
        if(/*(!breakout) || */input_offset==max_offset){
            input_offset = 0x0;
            input_head++;
        }

    }

    /*std::cout << "[DECODED]OUTPUT_LEN" << std::endl;
    std::cout << (int)output_len << std::endl;
    std::cout << "OUTPUT" << std::endl;
    printbp(output_data, 8*output_len);*/
}
