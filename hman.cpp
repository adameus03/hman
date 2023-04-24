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



dxnode* min_dxnode(dxnode* buffer, uchar len){
    dxnode* _min_dxnode = NULL;
    dxnode* _head_dxnode = buffer;
    dxnode* _tail_dxnode = buffer+len;

    while(_head_dxnode != _tail_dxnode){
        if(*_head_dxnode){
            _min_dxnode = _head_dxnode;
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

exnode* min_exnode(exnode* buffer, uchar len){
    exnode* _min_exnode;
    exnode* _head_exnode = buffer;
    exnode* _tail_exnode = buffer+len;

    std::cout << "n2=";
    printbp(&len, 8);

    while(_head_exnode != _tail_exnode){
        if(*_head_exnode){
            _min_exnode = _head_exnode;
            break;
        }
        _head_exnode++;
    }

    if(!_min_exnode) return NULL;

    std::cout << "n3=";
    printbp(&len, 8);



    while(_head_exnode != _tail_exnode){
        std::cout << "_head_exnode=" << _head_exnode << std::endl;
        std::cout << "_tail_exnode=" << _tail_exnode << std::endl;


        std::cout<<"min_exnode loop"<<std::endl;
        if(*_head_exnode){
            std::cout << "min_exnode inloop cond 1" << std::endl;
            std::cout << ((*_head_exnode)->freq) << std::endl;
            std::cout << ((*_min_exnode)->freq) << std::endl;

            if(((*_head_exnode)->freq) < ((*_min_exnode)->freq)){
                std::cout << "min_exnode inloop cond 2" << std::endl;
                _min_exnode = _head_exnode;
            }
        }
        //std::cout << "BEF_INC: " << ((*_head_exnode)->freq) << std::endl;
        _head_exnode++;
        //std::cout << "AFT_INC: " << ((*_head_exnode)->freq) << std::endl;

    }
    std::cout<<"bef min_exnode exit"<<std::endl;
    return _min_exnode;
}




dnode hman_dtree(uchar* symbol_buffer, ull* freq_buffer, const uchar& n){

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

    std::cout << "betw loops" << std::endl;

    while(true){
        dxnode couple = new dnode;
        dxnode* min1 = min_dxnode(dxnode_buffer, n);
        couple->symbol = NULL;
        couple->left = *min1;
        couple->freq = (*min1)->freq;

        std::cout << "inloop" << std::endl;
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
    //*min_xnode(xnode_buffer, n) = NULL;
    //*min_xnode(xnode_buffer, n) = NULL;
}

void hman_etree(uchar* symbol_buffer, ull* freq_buffer, const uchar& n, exnode* leaf_buffer){
    //printbp(symbol_buffer, 8*n);

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
    if((*exnode_buffer)->symbol == symbol_buffer && (*(exnode_buffer+1))->symbol == symbol_buffer+1 && (*(exnode_buffer+2))->symbol == symbol_buffer+2) std::cout << "!!!!ITS OKAY" << std::endl;
    else std::cout << "!!!!ITS NOT OKAY" << std::endl;

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
        //*leaf_head = (*freq_head) ? (*exnode_head) : NULL; // check?

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
        *(leaf_head+*symbol_head++) = *exnode_head++;
    }

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

    std::cout << "START PRINT LEAF_BUFFER" << std::endl;
    for(uint i=0; i<256u; i++){
        //if(*(leaf_buffer+i)) std::cout << "HERE" << std::endl;
        std::cout << std::hex << i << std::dec << "    " << ((*(leaf_buffer+i)!=NULL)?"not NULL":"NULL") << std::endl;
    }
    std::cout << "END PRINT LEAF_BUFFER" << std::endl;

    //return;

    std::cout << "ebetw loops" << std::endl;

    printbp(symbol_buffer, 24);


    for(uchar i=0x0; i<n; i++){
        printu(*(freq_buffer+i));
    }



    std::cout << "n=";
    printbp(&n, 8);

    //return;
    uchar c = 0x0; //debug;
    while(true){ //DEBUG THIS LOOP
        std::cout << "S T A R T : ";
        printbp(&c, 8);

        std::cout << "bef new couple" << std::endl;
        exnode couple = new enode;
        std::cout << "aft new couple" << std::endl;
        std::cout << "bef emin" << std::endl;
        std::cout << "n1=";
        printbp(&n, 8);
        //return;
        exnode* min1 = min_exnode(exnode_buffer, n);
        std::cout << "bef esymbonull" << std::endl;

        couple->symbol = NULL;
        std::cout << "aft esymbonull" << std::endl;

        //couple->left = *min1;
        (*min1)->polarity = NODEDIR::LEFT;
        couple->freq = (*min1)->freq;
        (*min1)->up = couple; //check

        std::cout << "einloop" << std::endl;



        *min1 = NULL; //check
        exnode* min2 = min_exnode(exnode_buffer, n);
        //return;
        if(min2){
            //couple->right = *min2;
            (*min2)->polarity = NODEDIR::RIGHT;
            couple->freq += (*min2)->freq;
            *min2 = couple;
            (*min2)->up = couple; //check
            *min2 = NULL; //check
        }
        //else return *(couple->left);
        else {
            //return;
            couple->up = NULL;
            break;
        }

        std::cout << "F I N I S H : ";
        printbp(&c, 8);
        c++; //debug
        //return;
    }
    //*min_xnode(xnode_buffer, n) = NULL;
    //*min_xnode(xnode_buffer, n) = NULL;
}

