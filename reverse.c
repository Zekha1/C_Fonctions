#include "io.h"
#include "test_utils.h"

// Usage: ./reverse [-s SIZE] [-o OUTFILE] [FILE]
//    Copies the input FILE to OUTFILE one character at a time,
//    reversing the order of characters in the input.

int main(int argc, char* argv[]) {
    // Parse arguments
    io_arguments args=io_arguments_parse(argc, argv, "s:o:i:");

    // Open files, measure file sizes
    io_test_begin();
    io_file* inf = io_open_check(args.input_file, O_RDONLY);
    io_file* outf = io_open_check(args.output_file,
                                      O_WRONLY | O_CREAT | O_TRUNC);

    if ((ssize_t) args.input_size < 0) {
        args.input_size = io_filesize(inf);
    }
    if ((ssize_t) args.input_size < 0) {
        fprintf(stderr, "reverse: can't get size of input file\n");
        exit(1);
    }
    if (io_seek(inf, 0) < 0) {
        fprintf(stderr, "reverse: input file is not seekable\n");
        exit(1);
    }

    while (args.input_size != 0) {
        --args.input_size;
        io_seek(inf, args.input_size);
        int ch = io_readc(inf);
        io_writec(outf, ch);
    }

    io_close(inf);
    io_close(outf);
    io_test_end();
}
