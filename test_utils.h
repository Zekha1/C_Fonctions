#ifndef TEST_UTILS_H
#define TEST_UTILS_H

typedef struct io_arguments {                                          
	size_t input_size;          // `-s` option: input size. Default SIZE_MAX
	size_t block_size;          // `-b` option: block size. Default 0  
	size_t stride;              // `-t` option: stride. Default 1024   
	unsigned char  lines;                 // `-l` option: read by lines. Default 0 
	const char* output_file;    // `-o` option: output file. Default NULL
	const char* input_file;     // input file. Default NULL            
	const char* program_name;   // name of program                     
	const char* opts;           // options string                      
} io_arguments;      

io_arguments io_arguments_parse(int argc, char** argv, const char* opts);

void io_test_begin();
void io_test_end();




#endif
