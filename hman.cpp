#include <cstddef>
#include "hman.h"


struct node;

typedef unsigned char uchar;
typedef unsigned long long ull;
typedef node* xnode;

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
    ull* ftail = freq_buffer+0x100;

    while(fhead != ftail){
        *fhead++ = 0x0;
    }
    while(dhead != dtail){

        // *(freq_buffer+*dhead++)++; // MinGW g++ tak nie pozwala
        ++*(freq_buffer+*dhead++);    // a tak pozwala

    }
    uchar* shead = symbol_buffer;
    uniq_symbol_cnt=0x0;
    fhead = freq_buffer;
    while(fhead != ftail){
        if(*fhead++){
            *shead++ = uniq_symbol_cnt++;
        }
    }

}

xnode* min_xnode(xnode* buffer, uchar len){
    xnode* _min_xnode;
    xnode* _head_xnode = buffer;
    xnode* _tail_xnode = buffer+len;

    while(_head_xnode != _tail_xnode){
        if(_head_xnode){
            _min_xnode = _head_xnode;
            break;
        }
        _head_xnode++;
    }

    if(!_min_xnode) return NULL;

    while(_head_xnode != _tail_xnode){
        if(*_head_xnode){
            if((*_head_xnode)->freq < (*_min_xnode)->freq){
                _min_xnode = _head_xnode;
            }
        }
        _head_xnode++;
    }
    return _min_xnode;
}

node hman_tree(uchar* symbol_buffer, ull* freq_buffer, uchar n){

    xnode* xnode_buffer = new xnode[n];
    xnode* xnode_head = xnode_buffer;
    xnode* xnode_tail = xnode_buffer+n;

    uchar* symbol_head = symbol_buffer;
    ull* freq_head = freq_buffer;

    while(xnode_head != xnode_tail){
        *xnode_head = new node;
        (*xnode_head)->left = NULL;
        (*xnode_head)->right = NULL;
        (*xnode_head)->symbol = symbol_head++;
        (*xnode_head++)->freq = *freq_buffer++;
    }

    while(true){
        xnode couple = new node;
        xnode* min1 = min_xnode(xnode_buffer, n);
        couple->symbol = NULL;
        couple->left = *min1;
        couple->freq = (*min1)->freq;
        //*(min1)->up = couple; //check

        *min1 = NULL; //check
        xnode* min2 = min_xnode(xnode_buffer, n);
        if(min2){
            couple->right = *min2;
            couple->freq += (*min2)->freq;
            *min2 = couple;
            //*(min2)->up = couple; //check
            *min2 = NULL; //check
        }
        else return *(couple->left);
    }
    //*min_xnode(xnode_buffer, n) = NULL;
    //*min_xnode(xnode_buffer, n) = NULL;
}

//void get_codes()

