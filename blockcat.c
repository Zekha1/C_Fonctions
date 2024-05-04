#include "io.h"
#include "test_utils.h"

// Usage: ./blockcat [-b BLOCKSIZE] [-o OUTFILE] [FILE]
//    Copies the input FILE to standard output in blocks.
//    Default BLOCKSIZE is 4096.

int main(int argc, char* argv[]) {
    // Parse arguments
	io_arguments args=io_arguments_parse(argc, argv, "b:o:i:");

    size_t block_size = args.block_size ? args.block_size : 4096;

    // Allocate buffer, open files
    char* buf =  (char*)malloc(block_size);

    io_test_begin();
    io_file* inf = io_open_check(args.input_file, O_RDONLY);
    io_file* outf = io_open_check(args.output_file,
                                      O_WRONLY | O_CREAT | O_TRUNC);

    // Copy file data
    while (1) {
        ssize_t amount = io_read(inf, buf, block_size);
        if (amount <= 0) {
            break;
        }
        io_write(outf, buf, amount);
    }

    io_close(inf);
    io_close(outf);
    io_test_end();
    free(buf);
}
