#include <iostream>
#include <cstring>
#include "filenet.h"
#include "code_fio.h"

#include "tests.h"

//g++ main.cpp net.cpp -o hman -lws2_32

using namespace std;

void usage(char* argv0){
    std::cout << "Usage:" << std::endl;
    std::cout << "Transmit file" << std::endl;
    std::cout << "    " << argv0 << " -t source_path" << std::endl;
    std::cout << "Receive file" << std::endl;
    std::cout << "    " << argv0 << " -r dest_path" << std::endl;
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

    checker(test_min_dxnode(), "min_dxnode");
    checker(test_min_exnode(), "min_exnode");
    checker(test_hman_dtree(), "hman_dtree");
    checker(test_hman_etree(), "hman_etree");

    checker(test_symfreq_catalogue(), "symfreq_catalogue");
    checker(test_encodec(), "encodec");
    checker(test_decodec(), "decodec");

}

int main(int argc, char** argv)
{
    if(argc<2){ usage(*argv); return 0; }

    if(!strcmp("-t", argv[1])){
        if(argc != 3){ usage(*argv); return 0; }
        cout << "Transmiter mode" << endl;
        transmit_file(argv[2]);
    }
    else if(!strcmp("-r", argv[1])){
        if(argc != 3){ usage(*argv); return 0; }
        cout << "Receiver mode" << endl;
        receive_file(argv[2]);  std::cout << "After receiver mode" <<std::endl;
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
        else if(argc==5){ //std::cout << "BINGO" << std::endl;
            std::cout << "Huffman-decode file using a known symbol-frequency map" << std::endl;
            decode_file_distinct(argv[2], argv[3], argv[4]);
        }
        else {usage(*argv); return 0; }
    }
    else {
        usage(*argv); return 0;
    }


    std::cout << "AFTER CONDITIONS" << std::endl;

    /*cout << "Hello world!" << endl;
    if(argc==3){
        if(!strcmp(argv[1], "-r")){
            cout << "Receiver mode" << endl;
            receive_file(argv[2]);

        }
        else if(!strcmp(argv[1], "-t")){
            cout << "Transmiter mode" << endl;
            transmit_file(argv[2]);

        }
        else {
            cout << "Invalid mode" << endl;
        }
    }
    else {
        cout << "Wrong # of args" << endl;
    }*/
    return 0;
}
