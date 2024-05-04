#include "io.h"
#include "test_utils.h"
// Usage: ./randblockcat [-b MAXBLOCKSIZE] [-r RANDOMSEED] [FILE]
//    Copies the input FILE to standard output in blocks. Each block has a
//    random size between 1 and MAXBLOCKSIZE (which defaults to 4096).

int main(int argc, char* argv[]) {
    // Parse arguments
    srandom(83419);
    io_arguments args=io_arguments_parse(argc, argv, "b:r:o:i:");
    size_t max_blocksize = args.block_size ? args.block_size : 4096;

    // Allocate buffer, open files
    char* buf = (char*)malloc(max_blocksize);

    io_test_begin();
    io_file* inf = io_open_check(args.input_file, O_RDONLY);
    io_file* outf = io_open_check(args.output_file,
                                      O_WRONLY | O_CREAT | O_TRUNC);

    // Copy file data
    while (1) {
        size_t m = (random() % max_blocksize) + 1;
        ssize_t amount = io_read(inf, buf, m);
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
