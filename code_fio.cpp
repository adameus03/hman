/*
    1B number of symbols    =n
    nB  symbol buffer
    8*nB  freq buffer
*/

#include "code.h"
#include <cstring>
#include <fstream>

typedef unsigned char uchar;
typedef unsigned short ushort;


/*void buffstore_symreq(uchar* symbol_buffer, ull* freq_buffer, const uchar& uniq_symbol_cnt, uchar* dbuff){

}*/

void store_symfreq(const char* dest_path, uchar* symbol_buffer, ull* freq_buffer, const uchar& _uniq_symbol_cnt){
    ushort uniq_symbol_cnt = _uniq_symbol_cnt ? _uniq_symbol_cnt : 0x100;
    /*uchar* symbol_buffer = new uchar[0x100];
    ull* freq_buffer = new ull[0x100];
    uchar uniq_symbol_cnt;
    symfreq_catalogue(data_buffer, data_len, symbol_buffer, freq_buffer, uniq_symbol_cnt);*/

    ushort dbuff_size = 0x9*uniq_symbol_cnt+0x1;
    uchar* dbuff = new uchar[dbuff_size];

    uchar* dbuff_head = dbuff;
    //uchar* dbuff_tail = dbuff+dbuff_size;

    *dbuff_head++ = uniq_symbol_cnt;
    memcpy(dbuff_head, symbol_buffer, uniq_symbol_cnt);
    dbuff_head += uniq_symbol_cnt;

    ull* freq_head = freq_buffer;
    ull* freq_tail = freq_buffer+uniq_symbol_cnt;
    while(freq_head != freq_tail){
        *dbuff_head++ = (uchar)(*freq_head>>0x38);
        *dbuff_head++ = (uchar)((*freq_head>>0x30) & 0xff);
        *dbuff_head++ = (uchar)((*freq_head>>0x28) & 0xff);
        *dbuff_head++ = (uchar)((*freq_head>>0x20) & 0xff);
        *dbuff_head++ = (uchar)((*freq_head>>0x18) & 0xff);
        *dbuff_head++ = (uchar)((*freq_head>>0x10) & 0xff);
        *dbuff_head++ = (uchar)((*freq_head>>0x8) & 0xff);
        *dbuff_head++ = (uchar)(*freq_head++ & 0xff);
    }

    std::ofstream f_out(dest_path, std::ofstream::trunc|std::ofstream::binary);
    std::filebuf* f_out_pbuf = f_out.rdbuf();
    f_out_pbuf->sputn((char*)dbuff, dbuff_size);
    f_out.close();
    delete[] dbuff;

}

void store_symfreq(const char* dest_path, uchar* data_buffer, const size_t& data_len){
    uchar* symbol_buffer = new uchar[0x100];
    ull* freq_buffer = new ull[0x100];
    uchar uniq_symbol_cnt;
    symfreq_catalogue(data_buffer, data_len, symbol_buffer, freq_buffer, uniq_symbol_cnt);
    store_symfreq(dest_path, symbol_buffer, freq_buffer, uniq_symbol_cnt);
}

void load_symfreq(const char* source_path, uchar* symbol_buffer, ull* freq_buffer, uchar& n){

    std::ifstream f_in(source_path, std::ifstream::binary);
    std::filebuf* f_in_pbuf = f_in.rdbuf();
    size_t sbuff_size = f_in_pbuf->pubseekoff(0, f_in.end, f_in.in);
    f_in_pbuf->pubseekpos(0, f_in.in);
    uchar* sbuff = new uchar[sbuff_size];
    f_in_pbuf->sgetn((char*)sbuff, sbuff_size);
    f_in.close();

    /*std::cout << "<SOURCE_PATH>" << std::endl;
    std::cout << source_path << std::endl;
    std::cout << "</SOURCE_PATH>" << std::endl;

    std::cout << "<SBUFF>" << std::endl;
    printbp(sbuff, sbuff_size*8);
    std::cout << "</SBUFF>" << std::endl;*/

    uchar* sbuff_head = sbuff;
    n = *sbuff_head++;

    ushort ns = n ? n : 0x100; //

    memcpy(symbol_buffer, sbuff_head, ns);
    sbuff_head += ns;

    ull* freq_head = freq_buffer;
    ull* freq_tail = freq_buffer+ns;
    while(freq_head != freq_tail){
        *freq_head++ = (ull)*(sbuff_head+7) | ((ull)*(sbuff_head+6)<<0x8) | ((ull)*(sbuff_head+5)<<0x10) | ((ull)*(sbuff_head+4)<<0x16) | ((ull)*(sbuff_head+3)<<0x20) | ((ull)*(sbuff_head+2)<<0x28) | ((ull)*(sbuff_head+1)<<0x30) | ((ull)*sbuff_head<<0x38);
        sbuff_head += 0x8;
    }


    /*std::cout << "<N>" << std::endl;
    printbp(&n, 8);
    std::cout << "</N>" << std::endl;*/


    delete[] sbuff;
}

void store_data(const char* dest_path, uchar* data, const size_t& len){
    std::ofstream f_out(dest_path, std::ofstream::trunc|std::ofstream::binary);
    std::filebuf* f_out_pbuf = f_out.rdbuf();
    f_out_pbuf->sputn((char*)data, len);
    f_out.close();
}

void encode_file_distinct(const char* source_path, const char* dest_path, const char* symfreq_path){ // hman -e source_path dest_path symfreq_path
    std::ifstream f_in(source_path, std::ifstream::binary);
    std::filebuf* f_in_pbuf = f_in.rdbuf();
    size_t sbuff_size = f_in_pbuf->pubseekoff(0, f_in.end, f_in.in);
    f_in_pbuf->pubseekpos(0, f_in.in);
    uchar* sbuff = new uchar[sbuff_size];
    f_in_pbuf->sgetn((char*)sbuff, sbuff_size);
    f_in.close();


    uchar* symbol_buffer = new uchar[0x100];
    ull* freq_buffer = new ull[0x100];
    uchar uniq_symbol_cnt;

    symfreq_catalogue(sbuff, sbuff_size, symbol_buffer, freq_buffer, uniq_symbol_cnt);
    store_symfreq(symfreq_path, symbol_buffer, freq_buffer, uniq_symbol_cnt);


    uchar* output = new uchar[sbuff_size];
    size_t output_len;

    encodec(sbuff, sbuff_size, symbol_buffer, freq_buffer, uniq_symbol_cnt, output, output_len);
    store_data(dest_path, output, output_len);
    delete[] sbuff;
    delete[] output;
    delete[] symbol_buffer;
    delete[] freq_buffer;
}

void encode_file_distinct_recycle(const char* source_path, const char* dest_path, const char* symfreq_path){ // hman -er source_path dest_path symfreq_path
    std::ifstream f_in(source_path, std::ifstream::binary);
    std::filebuf* f_in_pbuf = f_in.rdbuf();
    size_t sbuff_size = f_in_pbuf->pubseekoff(0, f_in.end, f_in.in);
    f_in_pbuf->pubseekpos(0, f_in.in);
    uchar* sbuff = new uchar[sbuff_size];
    f_in_pbuf->sgetn((char*)sbuff, sbuff_size);
    f_in.close();

    uchar* symbol_buffer = new uchar[0x100];
    ull* freq_buffer = new ull[0x100];
    uchar n;
    load_symfreq(symfreq_path, symbol_buffer, freq_buffer, n);

    uchar* output = new uchar[sbuff_size];
    size_t output_len;

    encodec(sbuff, sbuff_size, symbol_buffer, freq_buffer, n, output, output_len);
    store_data(dest_path, output, output_len);
    delete[] sbuff;
    delete[] output;
    delete[] symbol_buffer;
    delete[] freq_buffer;
}

void decode_file_distinct(const char* source_path, const char* dest_path, const char* symfreq_path){ // hman -d source_path dest_path symfreq_path
    std::ifstream f_in(source_path, std::ifstream::binary);
    std::filebuf* f_in_pbuf = f_in.rdbuf();
    size_t sbuff_size = f_in_pbuf->pubseekoff(0, f_in.end, f_in.in);
    f_in_pbuf->pubseekpos(0, f_in.in);
    uchar* sbuff = new uchar[sbuff_size];
    f_in_pbuf->sgetn((char*)sbuff, sbuff_size);
    f_in.close();

    uchar* symbol_buffer = new uchar[0x100];
    ull* freq_buffer = new ull[0x100];
    uchar n;
    load_symfreq(symfreq_path, symbol_buffer, freq_buffer, n);


    ushort ns = n?n:0x100;
    size_t output_len = 0x0;
    ull* freq_buffer_head = freq_buffer;
    ull* freq_buffer_tail = freq_buffer+ns;
    while(freq_buffer_head != freq_buffer_tail){
        output_len += *freq_buffer_head++;
    }



    uchar* output = new uchar[/*sbuff_size*/output_len];
    //size_t output_len;

    /*std::cout << "<SBUFF_SIZE>" << std::endl;
    std::cout << sbuff_size << std::endl;
    std::cout<< "</SBUFF_SIZE>" << std::endl;
    std::cout << "<N>" << std::endl;
    printbp(&n, 8);
    std::cout << "</N>" << std::endl;*/

    decodec(sbuff, sbuff_size, symbol_buffer, freq_buffer, n, output, output_len);
    store_data(dest_path, output, output_len);
    delete[] sbuff;
    delete[] output;
    delete[] symbol_buffer;
    delete[] freq_buffer;
}

void encode_file_compact(const char* source_path, const char* dest_path){ // hman -e source_path dest_path
    std::ifstream f_in(source_path, std::ifstream::binary);
    std::filebuf* f_in_pbuf = f_in.rdbuf();
    size_t sbuff_size = f_in_pbuf->pubseekoff(0, f_in.end, f_in.in);
    f_in_pbuf->pubseekpos(0, f_in.in);
    uchar* sbuff = new uchar[sbuff_size];
    f_in_pbuf->sgetn((char*)sbuff, sbuff_size);
    f_in.close();

    uchar* symbol_buffer = new uchar[0x100];
    ull* freq_buffer = new ull[0x100];
    uchar uniq_symbol_cnt;
    symfreq_catalogue(sbuff, sbuff_size, symbol_buffer, freq_buffer, uniq_symbol_cnt);


    ushort uniq_symbol_cnt_s = uniq_symbol_cnt ? uniq_symbol_cnt : 0x100; //

    size_t dbuff_size = sbuff_size+0x9*uniq_symbol_cnt_s+0x1;
    uchar* dbuff = new uchar[dbuff_size];

    uchar* dbuff_head = dbuff;
    //uchar* dbuff_tail = dbuff+dbuff_size;


    *dbuff_head++ = uniq_symbol_cnt;



    memcpy(dbuff_head, symbol_buffer, uniq_symbol_cnt_s);
    dbuff_head += uniq_symbol_cnt_s;

    ull* freq_head = freq_buffer;
    ull* freq_tail = freq_buffer+uniq_symbol_cnt_s;
    while(freq_head != freq_tail){
        *dbuff_head++ = (uchar)(*freq_head>>0x38);
        *dbuff_head++ = (uchar)((*freq_head>>0x30) & 0xff);
        *dbuff_head++ = (uchar)((*freq_head>>0x28) & 0xff);
        *dbuff_head++ = (uchar)((*freq_head>>0x20) & 0xff);
        *dbuff_head++ = (uchar)((*freq_head>>0x18) & 0xff);
        *dbuff_head++ = (uchar)((*freq_head>>0x10) & 0xff);
        *dbuff_head++ = (uchar)((*freq_head>>0x8) & 0xff);
        *dbuff_head++ = (uchar)(*freq_head++ & 0xff);
    }


    size_t output_len;

    encodec(sbuff, sbuff_size, symbol_buffer, freq_buffer, uniq_symbol_cnt, dbuff_head, output_len);

    store_data(dest_path, dbuff, 0x9*uniq_symbol_cnt_s+0x1+output_len);

}

void decode_file_compact(const char* source_path, const char* dest_path){ // hman -d source_path dest_path
    std::ifstream f_in(source_path, std::ifstream::binary);
    std::filebuf* f_in_pbuf = f_in.rdbuf();
    size_t sbuff_size = f_in_pbuf->pubseekoff(0, f_in.end, f_in.in);
    f_in_pbuf->pubseekpos(0, f_in.in);
    uchar* sbuff = new uchar[sbuff_size];
    f_in_pbuf->sgetn((char*)sbuff, sbuff_size);
    f_in.close();

    uchar* symbol_buffer = new uchar[0x100];
    ull* freq_buffer = new ull[0x100];
    uchar n;

    uchar* sbuff_head = sbuff;
    n = *sbuff_head++;

    ushort ns = n ? n : 0x100; //

    std::cout << "<ns>" << ns << "</ns>" << std::endl;

    memcpy(symbol_buffer, sbuff_head, ns);
    sbuff_head += ns;

    ull* freq_head = freq_buffer;
    ull* freq_tail = freq_buffer+ns;
    while(freq_head != freq_tail){
        *freq_head++ = (ull)*(sbuff_head+7) | ((ull)*(sbuff_head+6)<<0x8) | ((ull)*(sbuff_head+5)<<0x10) | ((ull)*(sbuff_head+4)<<0x16) | ((ull)*(sbuff_head+3)<<0x20) | ((ull)*(sbuff_head+2)<<0x28) | ((ull)*(sbuff_head+1)<<0x30) | ((ull)*sbuff_head<<0x38);
        sbuff_head += 0x8;
    }



    size_t output_len = 0x0;
    ull* freq_buffer_head = freq_buffer;
    ull* freq_buffer_tail = freq_buffer+ns;
    while(freq_buffer_head != freq_buffer_tail){
        output_len += *freq_buffer_head++;
    }



    uchar* output = new uchar[/*sbuff_size*/output_len];
    //size_t output_len;

    decodec(sbuff_head, sbuff_size-0x9*ns-0x1, symbol_buffer, freq_buffer, n, output, output_len);
    store_data(dest_path, output, output_len);

    delete[] sbuff;
    delete[] output;
    delete[] symbol_buffer;
    delete[] freq_buffer;

}
