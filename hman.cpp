#include <cstddef>
#include "hman.h"
//#include <iostream> //debug
//#include "tests.h" //debug


struct enode;
struct dnode;

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long long ull;
typedef dnode* dxnode; //for decoding
typedef enode* exnode; //for encoding

dxnode* min_dxnode(dxnode* buffer, uchar _len){
    ushort len = _len ? _len : 0x100; //
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

    while(_head_dxnode != _tail_dxnode){
        if(*_head_dxnode){
            if(((*_head_dxnode)->freq) < ((*_min_dxnode)->freq)){
                _min_dxnode = _head_dxnode;
            }
        }
        _head_dxnode++;
    }
    return _min_dxnode;
}
// return 0x1 if it is the only non-null exnode
uchar min_exnode(exnode* buffer, exnode*& minptr, uchar _len){
    ushort len = _len ? _len : 0x100; //
    exnode* _min_exnode = NULL;
    exnode* _head_exnode = buffer;
    exnode* _tail_exnode = buffer+len;

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
    uchar retval = 0x1;

    while(_head_exnode != _tail_exnode){
        if(*_head_exnode){
            retval = 0x0;
            if(((*_head_exnode)->freq) < ((*_min_exnode)->freq)){
                _min_exnode = _head_exnode;
            }
        }
        _head_exnode++;
    }
    minptr = _min_exnode;
    return retval;
}




dnode hman_dtree(uchar* symbol_buffer, ull* freq_buffer, const uchar& _n){

    /*
        Example:

        0x44    *symbol_buffer
        0x88    *(symbol_buffer+1)
        0x66    *(symbol_buffer+2)

        0x1     *freq_buffer
        0x2     *(freq_buffer+1)
        0x3     *(freq_buffer+2)

        0x3     n

    */

    ushort n = _n ? _n : 0x100;

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
        Example:

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

    if(n==0x1){
        dnode root;
        root.symbol = NULL;
        root.left = *dxnode_buffer;
        root.freq = (*dxnode_buffer)->freq;
        root.right = NULL;
        return root;

    }

    while(true){
        dxnode couple = new dnode;
        dxnode* min1 = min_dxnode(dxnode_buffer, _n);
        couple->symbol = NULL;
        couple->left = *min1;
        couple->freq = (*min1)->freq;

        *min1 = NULL;
        dxnode* min2 = min_dxnode(dxnode_buffer, _n);
        if(min2){
            couple->right = *min2;
            couple->freq += (*min2)->freq;
            *min2 = couple;
        }
        else return *(couple->left);
    }
}

void hman_etree(uchar* symbol_buffer, ull* freq_buffer, const uchar& _n, exnode* leaf_buffer){
    ushort n = _n ? _n : 0x100;

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

    while(stay){
        exnode couple = new enode;
        exnode* min1;
        if(min_exnode(exnode_buffer, min1, _n)){
            (*min1)->up = NULL;
            break;
        }
        couple->symbol = NULL;
        (*min1)->polarity = NODEDIR::LEFT;
        couple->freq = (*min1)->freq;
        (*min1)->up = couple;

        *min1 = NULL;
        exnode* min2;
        min_exnode(exnode_buffer, min2, _n);

        (*min2)->polarity = NODEDIR::RIGHT;
        couple->freq += (*min2)->freq;
        (*min2)->up = couple;
        *min2 = couple;
    }

}
