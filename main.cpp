#include <iostream>
#include <cstring>
#include <cstdlib>
#include "filenet.h"
#include "code_fio.h"
#include "tests.h"
#include <windows.h>

using namespace std;

void usage(char* argv0){
    std::cout << "Usage:" << std::endl;
    std::cout << "Transmit file" << std::endl;
    std::cout << "    " << argv0 << " -t source_path local_ip_addr local_port" << std::endl;
    std::cout << "Receive file from host" << std::endl;
    std::cout << "    " << argv0 << " -r dest_path remote_ip_addr remote_port" << std::endl;
    std::cout << "Huffman-encode file with symbol-frequency map output in separate file" << std::endl;
    std::cout << "    " << argv0 << " -e source_path dest_path symfreq_path" << std::endl;
    std::cout << "Huffman-encode file using a known symbol-frequency map" << std::endl;
    std::cout << "    " << argv0 << " -er source_path dest_path symfreq_path" << std::endl;
    std::cout << "Huffman-decode file using a known symbol-frequency map" << std::endl;
    std::cout << "    " << argv0 << " -d source_path dest_path symfreq_path" << std::endl;
    std::cout << "Huffman-encode to a single file, without isolating symbol-frequency map" << std::endl;
    std::cout << "    " << argv0 << " -e source_path dest_path" << std::endl;
    std::cout << "Huffman-decode a file with unknown symbol-frequency map" << std::endl;
    std::cout << "    " << argv0 << " -d source_path dest_path" << std::endl;
    std::cout << std::endl;
}

int main(int argc, char** argv)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); //green

    if(argc<2){ usage(*argv); return 0; }

    if(!strcmp("-t", argv[1])){
        if(argc != 5){ usage(*argv); return 0; }
        cout << "Transmiter mode" << endl;
        transmit_file(argv[2], argv[3], atoi(argv[4]));
    }
    else if(!strcmp("-r", argv[1])){
        if(argc != 5){ usage(*argv); return 0; }
        cout << "Receiver mode" << endl;
        receive_file(argv[2], argv[3], atoi(argv[4]));  std::cout << "After receiver mode" <<std::endl;
    }
    else if(!strcmp("-e", argv[1])){
        if(argc==4){
            std::cout << "Huffman-encode to a single file, without isolating symbol-frequency map" << std::endl;
            encode_file_compact(argv[2], argv[3]);
        }
        else if(argc==5){
            std::cout << "Huffman-encode file with symbol-frequency map output in separate file" << std::endl;
            encode_file_distinct(argv[2], argv[3], argv[4]);
        }
        else { usage(*argv); return 0; }
    }
    else if(!strcmp("-er", argv[1])) {
        if(argc != 5) { usage(*argv); return 0; }
        encode_file_distinct_recycle(argv[2], argv[3], argv[4]);
    }
    else if(!strcmp("-d", argv[1])){
        if(argc==4){
            std::cout << "Huffman-decode a file with unknown symbol-frequency map" << std::endl;
            decode_file_compact(argv[2], argv[3]);
        }
        else if(argc==5){
            std::cout << "Huffman-decode file using a known symbol-frequency map" << std::endl;
            decode_file_distinct(argv[2], argv[3], argv[4]);
        }
        else {usage(*argv); return 0; }
    }
    else {
        usage(*argv); return 0;
    }
    return 0;
}
