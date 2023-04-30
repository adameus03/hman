#include "hman.h"
#include <cstring>
#include <iostream>
#include <windows.h>

void printBT(const std::string& prefix, dnode* node, bool isLeft, dnode* marker)
{

    if (node != nullptr)
    {
        std::cout << prefix;
        std::cout << (isLeft ? "|--" : "L--");

        // set node color

        if(node == marker){
            if(marker->symbol == NULL)
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); //red
            else
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14); //gold?
        }

        // print the value of the node


        if(node->symbol){
            std::cout << "0x" << std::hex << (int)*(node->symbol) << std::dec << "(" /*<< *(node->symbol)*/ << ")" << std::endl;
        }
        else {
            std::cout << node->freq << std::endl;
        }

        //reset node color

        if(node==marker){
            //SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); //green
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9); //blue
        }

        // enter the next tree level - left and right branch
        printBT(prefix + (isLeft ? "|   " : "    "), node->right, true, marker);
        printBT(prefix + (isLeft ? "|   " : "    "), node->left, false, marker);
    }

    /*else {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); //red
        std::cout << "NULL";
    }*/
}

void printBT(dnode* node, dnode* marker)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); //green
    printBT("", node, false, marker);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); //green
}

