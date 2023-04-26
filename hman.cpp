#include <cstddef>
#include "hman.h"
#include <iostream> //debug
#include "tests.h" //debug


struct enode;
struct dnode;

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long long ull;
typedef dnode* dxnode; //for decoding
typedef enode* exnode; //for encoding


///<debug functions>

/*void print_dxnode(dxnode node){
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
}*/

///</debug functions>



dxnode* min_dxnode(dxnode* buffer, uchar len){
    ///<debug print arg>
    /*std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    for(int i=0; i<len; i++){
        print_dxnode(*(buffer+i));
    }
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;*/
    ///</debug print arg>


    dxnode* _min_dxnode = NULL;
    dxnode* _head_dxnode = buffer;
    dxnode* _tail_dxnode = buffer+len;

    while(_head_dxnode != _tail_dxnode){
        if(*_head_dxnode){
            _min_dxnode = _head_dxnode;
            _head_dxnode++;
            break;
        }
        _head_dxnode++;
    }

    if(!_min_dxnode) return NULL;

    //std::cout << ((*_min_dxnode)->freq) << std::endl;

    //std::cout<< "Inmin alive" << std::endl;

    while(_head_dxnode != _tail_dxnode){
        if(*_head_dxnode){
            //std::cout << "Pre-critical" << std::endl;
            //std::cout << ((*_head_dxnode)->freq) << std::endl;
            //std::cout << *((*_min_dxnode)->symbol) << std::endl;


            if(((*_head_dxnode)->freq) < ((*_min_dxnode)->freq)){
                //std::cout << "Critical" << std::endl;
                _min_dxnode = _head_dxnode;
            }
        }
        _head_dxnode++;
    }

    //std::cout << "Inmin bef ret" << std::endl;


    return _min_dxnode;
}
// return 0x1 if it is the only non-null exnode
uchar min_exnode(exnode* buffer, exnode*& minptr, uchar len){
    ///<debug print arg>
    /*std::cout << "=+=+=++=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=" << std::endl;
    for(int i=0; i<len; i++){
        print_exnode(*(buffer+i));
    }
    std::cout << "=+=+=++=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=" << std::endl;*/
    ///</debug print arg>
    exnode* _min_exnode = NULL;
    exnode* _head_exnode = buffer;
    exnode* _tail_exnode = buffer+len;

    /*std::cout << "n2=";
    printbp(&len, 8);*/

    while(_head_exnode != _tail_exnode){
        if(*_head_exnode){
            _min_exnode = _head_exnode;
            _head_exnode++;
            break;
        }
        _head_exnode++;
    }

    if(!_min_exnode){
        minptr = NULL;
        return 0x0;
    }

    /*std::cout << "n3=";
    printbp(&len, 8);*/

    if(_head_exnode == _tail_exnode){
        minptr = _min_exnode;
        return 0x1;
    }

    while(_head_exnode != _tail_exnode){
        /*std::cout << "_head_exnode=" << _head_exnode << std::endl;
        std::cout << "_tail_exnode=" << _tail_exnode << std::endl;


        std::cout<<"min_exnode loop"<<std::endl;*/
        if(*_head_exnode){
            /*std::cout << "min_exnode inloop cond 1" << std::endl;
            std::cout << ((*_head_exnode)->freq) << std::endl;
            std::cout << ((*_min_exnode)->freq) << std::endl;*/

            if(((*_head_exnode)->freq) < ((*_min_exnode)->freq)){
                //std::cout << "min_exnode inloop cond 2" << std::endl;
                _min_exnode = _head_exnode;
            }
        }
        //std::cout << "BEF_INC: " << ((*_head_exnode)->freq) << std::endl;
        _head_exnode++;
        //std::cout << "AFT_INC: " << ((*_head_exnode)->freq) << std::endl;

    }
    //std::cout<<"bef min_exnode exit"<<std::endl;
    minptr = _min_exnode;
    return 0x0;
}




dnode hman_dtree(uchar* symbol_buffer, ull* freq_buffer, const uchar& n){

    /*
        0x44    *symbol_buffer
        0x88    *(symbol_buffer+1)
        0x66    *(symbol_buffer+2)

        0x1     *freq_buffer
        0x2     *(freq_buffer+1)
        0x3     *(freq_buffer+2)

        0x3     n

    */

    dxnode* dxnode_buffer = new dxnode[n];
    dxnode* dxnode_head = dxnode_buffer;
    dxnode* dxnode_tail = dxnode_buffer+n;

    uchar* symbol_head = symbol_buffer;
    ull* freq_head = freq_buffer;

    while(dxnode_head != dxnode_tail){
        *dxnode_head = new dnode;
        (*dxnode_head)->left = NULL;
        (*dxnode_head)->right = NULL;
        (*dxnode_head)->symbol = symbol_head++;
        (*dxnode_head)->freq = *freq_head++;
        dxnode_head++;
    }

    /*
        0x44    *((*dxnode_buffer)->symbol)
        0x1     (*dxnode_buffer)->freq
        NULL    (*dxnode_buffer)->left
        NULL    (*dxnode_buffer)->right

        0x88    *((*(dxnode_buffer+1))->symbol)
        0x2     (*(dxnode_buffer+1))->freq
        NULL    (*(dxnode_buffer+1))->left
        NULL    (*(dxnode_buffer+1))->right

        0x66    *((*(dxnode_buffer+2))->symbol)
        0x3     (*(dxnode_buffer+2))->freq
        NULL    (*(dxnode_buffer+2))->left
        NULL    (*(dxnode_buffer+2))->right

    */

    //std::cout << "betw loops" << std::endl;

    while(true){
        ///<debug print>
        /*std::cout << "<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3" << std::endl;
        for(int i=0; i<n; i++){
            print_dxnode(*(dxnode_buffer+i));
        }
        std::cout << "#################################" << std::endl;*/
        ///</debug print>
        dxnode couple = new dnode;
        dxnode* min1 = min_dxnode(dxnode_buffer, n);
        couple->symbol = NULL;
        couple->left = *min1;
        couple->freq = (*min1)->freq;

        //std::cout << "inloop" << std::endl;
        //(*min1)->up = couple; //check

        *min1 = NULL; //check
        dxnode* min2 = min_dxnode(dxnode_buffer, n);
        if(min2){
            couple->right = *min2;
            couple->freq += (*min2)->freq;
            *min2 = couple;
            //(*min2)->up = couple; //check
            //*min2 = NULL; //check
        }
        else return *(couple->left);
    }

    /*
        0x44    *((*min1)->symbol)
        0x1     (*min1)->freq
        NULL    (*min1)->left
        NULL    (*min1)->right

        NULL    couple->symbol
        0x44    *(couple->left->symbol)
        0x1     couple->left->freq
        0x1     couple->freq

        NULL    *dxnode_buffer

        0x88    *((*(dxnode_buffer+1))->symbol)
        0x2     (*(dxnode_buffer+1))->freq
        NULL    (*(dxnode_buffer+1))->left
        NULL    (*(dxnode_buffer+1))->right

        0x66    *((*(dxnode_buffer+2))->symbol)
        0x3     (*(dxnode_buffer+2))->freq
        NULL    (*(dxnode_buffer+2))->left
        NULL    (*(dxnode_buffer+2))->right


        0x88    *((*min2)->symbol)
        0x2     (*min2)->freq
        NULL    (*min2)->left
        NULL    (*min2)->right

        0x88    *(couple->right->symbol)
        0x2     couple->right->freq
        0x3     couple->freq


        NULL    *dxnode_buffer

        NULL    *((*(dxnode_buffer+1))->symbol)
        0x3     (*(dxnode_buffer+1))->freq

        0x66    *((*(dxnode_buffer+2))->symbol)
        0x3     (*(dxnode_buffer+2))->freq
        NULL    (*(dxnode_buffer+2))->left
        NULL    (*(dxnode_buffer+2))->right


        ------------------------------------------------

        NULL   (*min1)->symbol
        0x3    (*min1)->freq

        NULL    couple->symbol
        NULL    couple->left->symbol
        0x3     couple->left->freq
        0x3     couple->freq


        NULL    *dxnode_buffer

        NULL    *(dxnode_buffer+1)

        0x66    *((*(dxnode_buffer+2))->symbol)
        0x3     (*(dxnode_buffer+2))->freq
        NULL    (*(dxnode_buffer+2))->left
        NULL    (*(dxnode_buffer+2))->right

        0x66    *((*min2)->symbol)
        0x3     (*min2)->freq
        NULL    (*min2)->left
        NULL    (*min2)->right

        0x66    *(couple->right->symbol)
        0x3     couple->right->freq
        NULL    couple->right->left
        NULL    couple->right->right

        0x6     couple->freq



        NULL    *dxnode_buffer

        NULL    *(dxnode_buffer+1)

        NULL    ((*(dxnode_buffer+2))->symbol
        0x6     (*(dxnode_buffer+2))->freq

        -----------------------------------------------------

        NULL    (*min1)->symbol
        0x6     (*min1)->freq

        NULL    couple->symbol
        NULL    couple->left->symbol
        0x6     couple->left->freq


        NULL    *dxnode_buffer

        NULL    *(dxnode_buffer+1)

        NULL    *(dxnode_buffer+2)




    */

    //*min_xnode(xnode_buffer, n) = NULL;
    //*min_xnode(xnode_buffer, n) = NULL;
}

void hman_etree(uchar* symbol_buffer, ull* freq_buffer, const uchar& n, exnode* leaf_buffer){

    //printbp(symbol_buffer, 8*n);

    /*
        0x44    *symbol_buffer
        0x88    *(symbol_buffer+1)
        0x66    *(symbol_buffer+2)

        0x1     *freq_buffer
        0x2     *(freq_buffer+1)
        0x3     *(freq_buffer+2)

        0x3     n

    */

    exnode* exnode_buffer = new exnode[n];
    exnode* exnode_head = exnode_buffer;
    exnode* exnode_tail = exnode_buffer+n;

    uchar* symbol_head = symbol_buffer;
    uchar* symbol_tail = symbol_buffer+n;

    ull* freq_head = freq_buffer;


    //uchar c=0x0; //debug
    while(exnode_head != exnode_tail){
        *exnode_head = new enode;
        (*exnode_head)->symbol = symbol_head++;
        (*exnode_head)->freq = *freq_head++;
        exnode_head++;
        //c++; //debug
    }

    /*

        0x44    *((*exnode_buffer)->symbol)
        0x1     (*exnode_buffer)->freq

        0x88    *((*(exnode_buffer+1))->symbol)
        0x2     (*(exnode_buffer+1))->freq

        0x66    *((*(exnode_buffer+2))->symbol)
        0x3     (*(exnode_buffer+2))->freq

    */

    ///<old debug>
    /*if((*exnode_buffer)->symbol == symbol_buffer && (*(exnode_buffer+1))->symbol == symbol_buffer+1 && (*(exnode_buffer+2))->symbol == symbol_buffer+2) std::cout << "!!!!ITS OKAY" << std::endl;
    else std::cout << "!!!!ITS NOT OKAY" << std::endl;*/
    ///</old debug>

    /*for(uchar i=0x0; i<n; i++){
        printu(*(freq_buffer+i));
    }*/

    exnode* leaf_head = leaf_buffer;
    exnode* leaf_tail = leaf_buffer+0x100; // !
    //exnode* leaf_tail = leaf_buffer+0x100; // !
    //exnode* leaf_tail = leaf_buffer+n;
    exnode_head = exnode_buffer;
    //freq_head = freq_buffer;
    symbol_head = symbol_buffer;

    /*while(leaf_head != leaf_tail){
        // *leaf_head = (*freq_head) ? (*exnode_head) : NULL; // check?

        if(*freq_head){
            *leaf_head = *exnode_head;
            exnode_head++;
            freq_head++;
        }
        else {
            *leaf_head = NULL;
        }

        leaf_head++;
    }*/
    /*for(uint i=0; i<256u; i++){
        if((*symbol_head) == (uchar)i){
            *leaf_head = *exnode_head;
            symbol_head++;
            exnode_head++;
        }
        else {
            *leaf_head = NULL;
        }
        leaf_head++;
    }*/

    while(leaf_head<leaf_tail){
        *leaf_head++ = NULL;
    }
    leaf_head = leaf_buffer;
    while(symbol_head<symbol_tail){
        *(leaf_head + *symbol_head) = *exnode_head;
        symbol_head++;
        exnode_head++;
    }



    // Is symbol really necessary in enodes ? I.e. it's obvius that *(leaf_buffer[symb])->symb == symb

    /*
        NULL    leaf_buffer[0x0 ... 0x43]

        0x44    *(leaf_buffer[0x44]->symbol)
        0x1     leaf_buffer[0x44]->freq

        NULL    leaf_buffer[0x45 ... 0x65]

        0x66    *(leaf_buffer[0x66]->symbol)
        0x3     leaf_buffer[0x66]->freq

        NULL    leaf_buffer[0x67 ... 0x87]

        0x88    *(leaf_buffer[0x88]->symbol)
        0x2     leaf_buffer[0x88]->freq

        NULL    leaf_buffer[0x89 ... 0xff]

    */

    /*std::cout<<"BEGIN SYMBOL PRINTAGE" << std::endl;
    printbp(symbol_buffer, 8*n);
    std::cout<<"END SYMBOL PRINTAGE" << std::endl;

    std::cout << "START symbol test" << std::endl;
    ushort j=0x0;
    for(ushort i=0x0; i<0x100; i++){
        if(*(symbol_buffer+j) == (uchar)i) { std::cout << "HEY!" << std::endl; j++; }
        //std::cout << std::hex << i << std::dec << "    " << ((*(leaf_buffer+i)!=NULL)?"not NULL":"NULL") << std::endl;
    }
    std::cout << "END symbol test" << std::endl;*/


    ///<old debug>
    /*std::cout << "START PRINT LEAF_BUFFER" << std::endl;
    for(uint i=0; i<256u; i++){
        //if(*(leaf_buffer+i)) std::cout << "HERE" << std::endl;
        std::cout << std::hex << i << std::dec << "    " << ((*(leaf_buffer+i)!=NULL)?"not NULL":"NULL") << std::endl;
    }
    std::cout << "END PRINT LEAF_BUFFER" << std::endl;*/

    //return;

    /*std::cout << "ebetw loops" << std::endl;

    printbp(symbol_buffer, 24);


    for(uchar i=0x0; i<n; i++){
        printu(*(freq_buffer+i));
    }



    std::cout << "n=";
    printbp(&n, 8);*/

    ///</old debug>

    //return;
    ///<old debug>
    /*uchar c = 0x0; //debug;*/
    ///</old debug>

    while(true){ //DEBUG THIS LOOP

        ///<debug print>
        /*std::cout << "<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3" << (int)c << std::endl;
        for(int i=0; i<n; i++){
            print_exnode(*(exnode_buffer+i));
        }
        std::cout << "#################################" << (int)c << std::endl;*/
        ///</debug print>

        ///<old debug>
        /*std::cout << "S T A R T : ";
        printbp(&c, 8);

        std::cout << "bef new couple" << std::endl;*/
        ///</old debug>

        exnode couple = new enode;

        ///<old debug>
        /*std::cout << "aft new couple" << std::endl;
        std::cout << "bef emin" << std::endl;
        std::cout << "n1=";
        printbp(&n, 8);*/
        ///</old debug>
        //return;
        //exnode* min1 = min_exnode(exnode_buffer, n);
        exnode* min1;
        if(min_exnode(exnode_buffer, min1, n)){
            (*min1)->up = NULL;
            break;
        }

        ///<old debug>
        /* std::cout << "bef esymbonull" << std::endl; */
        ///</old debug>

        couple->symbol = NULL;

        ///<old debug>
        /* std::cout << "aft esymbonull" << std::endl; */
        ///</old debug>

        //couple->left = *min1;
        (*min1)->polarity = NODEDIR::LEFT;
        couple->freq = (*min1)->freq;
        (*min1)->up = couple; //check

        ///<old debug>
        // std::cout << "einloop" << std::endl;
        ///</old debug>



        *min1 = NULL; //check
        ///<debug>
        //std::cout << "nullmin" << std::endl;
        ///</debug>

        exnode* min2;
        min_exnode(exnode_buffer, min2, n);
        //return;
        /*if(min2){
            //couple->right = *min2;
            (*min2)->polarity = NODEDIR::RIGHT;
            couple->freq += (*min2)->freq;

            (*min2)->up = couple; //check
            *min2 = couple;

            //(*min2)->up = NULL;


        }
        //else return *(couple->left);
        else {
            //return;
            //couple->up = NULL;
            break;
        }*/

        (*min2)->polarity = NODEDIR::RIGHT;
        couple->freq += (*min2)->freq;

        (*min2)->up = couple; //check
        *min2 = couple;


        ///<old debug>
        /*std::cout << "F I N I S H : ";
        printbp(&c, 8);*/
        ///</old debug>

        ///<old debug>
        /* c++; //debug */
        ///</old debug>
        //return;
    }
    //*min_xnode(xnode_buffer, n) = NULL;
    //*min_xnode(xnode_buffer, n) = NULL;

    ///<debug>
    /*std::cout << "START PRINT ___FINAL___ LEAF_BUFFER" << std::endl;
    for(uint i=0; i<256u; i++){
        //if(*(leaf_buffer+i)) std::cout << "HERE" << std::endl;
        std::cout << std::hex << i << std::dec << "    " << ((*(leaf_buffer+i)!=NULL)?"not NULL":"NULL") << std::endl;
    }
    std::cout << "END PRINT ___FINAL___ LEAF_BUFFER" << std::endl;*/
    ///</debug>


}

/*
    0x44    *((*min1)->symbol)
    0x1     (*min1)->freq

    NULL    couple->symbol

    0x0     (*min1)->polarity
    0x1     couple->freq

    NULL    (*min1)->up->symbol
    0x1     (*min1)->up->freq



    0x44    *((*exnode_buffer)->symbol)
    0x1     (*exnode_buffer)->freq
    0x0     (*exnode_buffer)->polarity

    0x88    *((*(exnode_buffer+1))->symbol)
    0x2     (*(exnode_buffer+1))->freq

    0x66    *((*(exnode_buffer+2))->symbol)
    0x3     (*(exnode_buffer+2))->freq




    NULL    *exnode_buffer

    0x88    *((*(exnode_buffer+1))->symbol)
    0x2     (*(exnode_buffer+1))->freq

    0x66    *((*(exnode_buffer+2))->symbol)
    0x3     (*(exnode_buffer+2))->freq



    0x44    *(leaf_buffer[0x44]->symbol)
    0x1     leaf_buffer[0x44]->freq
    0x0     leaf_buffer[0x44]->polarity

    NULL    leaf_buffer[0x44]->up->symbol
    0x1     leaf_buffer[0x44]->up->freq



    0x88    *((*min2)->symbol)
    0x2     (*min2)->freq

    0x1     (*min2)->polarity
    0x3     couple->freq



    NULL    (*min2)->up->symbol
    0x3     (*min2)->up->freq



    NULL    *exnode_buffer

    NULL    (*(exnode_buffer+1))->symbol
    0x3     (*(exnode_buffer+1))->freq

    0x66    *((*(exnode_buffer+2))->symbol)
    0x3     (*(exnode_buffer+2))->freq

    # maybe display leaf_buffer ??
    -----------------------------------------------------------

    NULL    (*min1)->symbol
    0x3     (*min1)->freq

    NULL    couple->symbol

    0x0     (*min1)->polarity
    0x3     couple->freq

    NULL    (*min1)->up->symbol
    0x3     (*min1)->up->freq


    NULL    *exnode_buffer

    NULL    *(exnode_buffer+1)

    0x66    *((*(exnode_buffer+2))->symbol)
    0x3     (*(exnode_buffer+2))->freq



    0x66    *((*min2)->symbol)
    0x3     (*min2)->freq

    0x1     (*min2)->polarity
    0x6     couple->freq

    NULL    (*min2)->up->symbol
    0x6     (*min2)->up->freq




    NULL    *exnode_buffer

    NULL    *(exnode_buffer+1)

    NULL    (*(exnode_buffer+2))->symbol
    0x6     (*(exnode_buffer+2))->freq

    -----------------------------------------------------------

    NULL    (*min1)->symbol
    0x6     (*min1)->freq

    NULL    couple->symbol

    0x0     (*min1)->polarity
    0x6     couple->freq

    NULL    (*min1)->up->symbol
    0x6     (*min1)->up->freq




    NULL    *exnode_buffer

    NULL    *(exnode_buffer+1)

    NULL    *(exnode_buffer+2)



    NULL    min2

*/
