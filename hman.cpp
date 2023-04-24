#include <cstddef>
#include "hman.h"


struct enode;
struct dnode;

typedef unsigned char uchar;
typedef unsigned long long ull;
typedef dnode* dxnode; //for decoding
typedef enode* exnode; //for encoding



dxnode* min_dxnode(dxnode* buffer, uchar len){
    dxnode* _min_dxnode;
    dxnode* _head_dxnode = buffer;
    dxnode* _tail_dxnode = buffer+len;

    while(_head_dxnode != _tail_dxnode){
        if(_head_dxnode){
            _min_dxnode = _head_dxnode;
            break;
        }
        _head_dxnode++;
    }

    if(!_min_dxnode) return NULL;

    while(_head_dxnode != _tail_dxnode){
        if(*_head_dxnode){
            if((*_head_dxnode)->freq < (*_min_dxnode)->freq){
                _min_dxnode = _head_dxnode;
            }
        }
        _head_dxnode++;
    }
    return _min_dxnode;
}

exnode* min_exnode(exnode* buffer, uchar len){
    exnode* _min_exnode;
    exnode* _head_exnode = buffer;
    exnode* _tail_exnode = buffer+len;

    while(_head_exnode != _tail_exnode){
        if(_head_exnode){
            _min_exnode = _head_exnode;
            break;
        }
        _head_exnode++;
    }

    if(!_min_exnode) return NULL;

    while(_head_exnode != _tail_exnode){
        if(*_head_exnode){
            if((*_head_exnode)->freq < (*_min_exnode)->freq){
                _min_exnode = _head_exnode;
            }
        }
        _head_exnode++;
    }
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
        (*dxnode_head++)->freq = *freq_buffer++;
    }

    while(true){
        dxnode couple = new dnode;
        dxnode* min1 = min_dxnode(dxnode_buffer, n);
        couple->symbol = NULL;
        couple->left = *min1;
        couple->freq = (*min1)->freq;
        //(*min1)->up = couple; //check

        *min1 = NULL; //check
        dxnode* min2 = min_dxnode(dxnode_buffer, n);
        if(min2){
            couple->right = *min2;
            couple->freq += (*min2)->freq;
            *min2 = couple;
            //(*min2)->up = couple; //check
            *min2 = NULL; //check
        }
        else return *(couple->left);
    }
    //*min_xnode(xnode_buffer, n) = NULL;
    //*min_xnode(xnode_buffer, n) = NULL;
}

void hman_etree(uchar* symbol_buffer, ull* freq_buffer, const uchar& n, exnode* leaf_buffer){

    exnode* exnode_buffer = new exnode[n];
    exnode* exnode_head = exnode_buffer;
    exnode* exnode_tail = exnode_buffer+n;

    uchar* symbol_head = symbol_buffer;
    ull* freq_head = freq_buffer;

    while(exnode_head != exnode_tail){
        *exnode_head = new enode;
        (*exnode_head)->symbol = symbol_head++;
        (*exnode_head++)->freq = *freq_buffer++;
    }

    exnode* leaf_head = leaf_buffer;
    exnode* leaf_tail = leaf_buffer+0x100; // !
    exnode_head = exnode_buffer;
    freq_head = freq_buffer;
    while(leaf_head != leaf_tail){
        *leaf_head++ = *freq_head++ ? *exnode_head++ : NULL; // check?
    }


    while(true){
        exnode couple = new enode;
        exnode* min1 = min_exnode(exnode_buffer, n);
        couple->symbol = NULL;
        //couple->left = *min1;
        (*min1)->polarity = NODEDIR::LEFT;
        couple->freq = (*min1)->freq;
        (*min1)->up = couple; //check

        *min1 = NULL; //check
        exnode* min2 = min_exnode(exnode_buffer, n);
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
            couple->up = NULL;
            break;
        }
    }
    //*min_xnode(xnode_buffer, n) = NULL;
    //*min_xnode(xnode_buffer, n) = NULL;
}

