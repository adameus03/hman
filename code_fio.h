#ifndef CODE_FIO_H_INCLUDED
#define CODE_FIO_H_INCLUDED

typedef unsigned char uchar;
typedef unsigned long long ull;

void store_symfreq(const char* dest_path, uchar* symbol_buffer, ull* freq_buffer, const uchar& uniq_symbol_cnt);

void store_symfreq(const char* dest_path, uchar* data_buffer, const size_t& data_len);

void load_symfreq(const char* source_path, uchar* symbol_buffer, ull* freq_buffer, uchar& n);

void store_data(const char* dest_path, uchar* data, const size_t& len);

void encode_file_distinct(const char* source_path, const char* dest_path, const char* symfreq_path);

void encode_file_distinct_recycle(const char* source_path, const char* dest_path, const char* symfreq_path);

void decode_file_distinct(const char* source_path, const char* dest_path, const char* symfreq_path);

void encode_file_compact(const char* source_path, const char* dest_path);

void decode_file_compact(const char* source_path, const char* dest_path);



#endif // CODE_FIO_H_INCLUDED
