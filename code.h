#ifndef CODE_H_INCLUDED
#define CODE_H_INCLUDED

typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long long ull;

void symfreq_catalogue(uchar* data_buffer, const ull& data_len, uchar* symbol_buffer, ull* freq_buffer, uchar& uniq_symbol_cnt);

void encodec(uchar* input_data, const ull& input_len, uchar* symbol_buffer, ull* freq_buffer, const uchar& n, uchar* output_data, ull& output_len);

void decodec(uchar* input_data, const ull& input_len, uchar* symbol_buffer, ull* freq_buffer, const uchar& n, uchar* output_data, ull& output_len);



#endif // CODE_H_INCLUDED
