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



dxnode* min_dxnode(dxnode* buffer, uchar _len){
    ushort len = _len ? _len : 0x100; //
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
uchar min_exnode(exnode* buffer, exnode*& minptr, uchar _len){
    ushort len = _len ? _len : 0x100; //
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

    /*if(_head_exnode == _tail_exnode){
        minptr = _min_exnode;
        return 0x1;
    }*/


    uchar retval = 0x1;


    while(_head_exnode != _tail_exnode){
        /*std::cout << "_head_exnode=" << _head_exnode << std::endl;
        std::cout << "_tail_exnode=" << _tail_exnode << std::endl;


        std::cout<<"min_exnode loop"<<std::endl;*/
        if(*_head_exnode){
            retval = 0x0;
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
    return retval;
}




dnode hman_dtree(uchar* symbol_buffer, ull* freq_buffer, const uchar& _n){

    /*
        0x44    *symbol_buffer
        0x88    *(symbol_buffer+1)
        0x66    *(symbol_buffer+2)

        0x1     *freq_buffer
        0x2     *(freq_buffer+1)
        0x3     *(freq_buffer+2)

        0x3     n

    */

    ushort n = _n ? _n : 0x100; //

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

    if(n==0x1){
        dnode root;
        root.symbol = NULL;
        root.left = *dxnode_buffer;
        root.freq = (*dxnode_buffer)->freq;
        root.right = NULL;
        return root;

    }
    /*else if(n<0x1){
        dnode root;
        root.symbol = NULL;
        root.left = NULL;
        root.freq = 0x0;
        root.right = NULL;
        return root;
    }*/

    while(true){
        ///<debug print>
        /*std::cout << "<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3" << std::endl;
        for(int i=0; i<n; i++){
            print_dxnode(*(dxnode_buffer+i));
        }
        std::cout << "#################################" << std::endl;*/
        ///</debug print>
        dxnode couple = new dnode;
        dxnode* min1 = min_dxnode(dxnode_buffer, _n);
        couple->symbol = NULL;
        couple->left = *min1;
        couple->freq = (*min1)->freq;

        //std::cout << "inloop" << std::endl;
        //(*min1)->up = couple; //check

        *min1 = NULL; //check
        dxnode* min2 = min_dxnode(dxnode_buffer, _n);
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

void hman_etree(uchar* symbol_buffer, ull* freq_buffer, const uchar& _n, exnode* leaf_buffer){
    ushort n = _n ? _n : 0x100; //

    uchar* symbol_head = symbol_buffer;
    uchar* symbol_tail = symbol_head+n;
    ull* freq_head = freq_buffer;

    exnode* exnode_buffer = new exnode[n];

    exnode* exnode_head = exnode_buffer;

    while(symbol_head != symbol_tail){
        leaf_buffer[*symbol_head] = new enode;
        leaf_buffer[*symbol_head]->symbol = symbol_head;
        leaf_buffer[*symbol_head]->freq = *freq_head++;
        leaf_buffer[*symbol_head]->up = NULL; //
        leaf_buffer[*symbol_head]->polarity = NODEDIR::LEFT; //

        *exnode_head = leaf_buffer[*symbol_head];

        exnode_head++;
        symbol_head++;
    }

    //std::cout << "Before DEBUG LOOP" << std::endl;

    if(n==0x0){
        return;
    }
    else if(n==0x1){
        exnode root = new enode;
        root->symbol = NULL;
        (*exnode_buffer)->up = root;
        root->freq = (*exnode_buffer)->freq;
        root->up = NULL;
        (*exnode_buffer)->polarity = NODEDIR::LEFT;
        return;

    }

    uchar stay = 0x1;

    while(stay){ //DEBUG THIS LOOP
        //std::cout << "DEBUG LOOP ITERATION START" << std::endl;
        ///<debugging loop>
        /*for(int i=0; i<n; i++){
            print_exnode(*(exnode_buffer+i));
        }*/
        ///</debugging loop>

        exnode couple = new enode;

        exnode* min1;
        //std::cout << "Bef min" << std::endl;
        if(min_exnode(exnode_buffer, min1, _n)){
            (*min1)->up = NULL;
            //std::cout << "LAAST" << std::endl;
            break;
        }
        //std::cout << "Aft min" << std::endl;

        couple->symbol = NULL;

        (*min1)->polarity = NODEDIR::LEFT;
        couple->freq = (*min1)->freq;
        (*min1)->up = couple; //check

        *min1 = NULL; //check

        exnode* min2;
        /*stay = !*/min_exnode(exnode_buffer, min2, _n);
        /*if(!stay){
            //(*min2)->up = NULL;
            std::cout << "!stay" << std::endl;
        }*/

        (*min2)->polarity = NODEDIR::RIGHT;
        couple->freq += (*min2)->freq;

        (*min2)->up = couple; //check
        *min2 = couple;

        //(*min2)->up = NULL;
        /*if(!stay){
            break;
        }*/
    }

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
