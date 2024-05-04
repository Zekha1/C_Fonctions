#include "io.h"
#include "test_utils.h"

// Usage: ./reordercat [-b BLOCKSIZE] [-r RANDOMSEED] [-s SIZE]
//                       [-o OUTFILE] [FILE]
//    Copies the input FILE to OUTFILE in blocks. The blocks are
//    transferred in random order, but the resulting output file
//    should be the same as the input. Default BLOCKSIZE is 4096.

int main(int argc, char* argv[]) {
    // Parse arguments
    srandom(83419);
    io_arguments args=io_arguments_parse(argc, argv, "b:r:s:o:i:");
    size_t block_size = args.block_size ? args.block_size : 4096;

    // Allocate buffer, open files, measure file sizes
    char* buf = (char*)malloc(block_size);

    io_test_begin();
    io_file* inf = io_open_check(args.input_file, O_RDONLY);

    if ((ssize_t) args.input_size < 0) {
        args.input_size = io_filesize(inf);
    }
    if ((ssize_t) args.input_size < 0) {
        fprintf(stderr, "reordercat: can't get size of input file\n");
        exit(1);
    }
    if (io_seek(inf, 0) < 0) {
        fprintf(stderr, "reordercat: input file is not seekable\n");
        exit(1);
    }

    io_file* outf = io_open_check(args.output_file,
                                      O_WRONLY | O_CREAT | O_TRUNC);
    if (io_seek(outf, 0) < 0) {
        fprintf(stderr, "reordercat: output file is not seekable\n");
        exit(1);
    }

    // Calculate random permutation of file's blocks
    size_t nblocks = args.input_size / block_size;
    if (nblocks > (30 << 20)) {
        fprintf(stderr, "reordercat: file too large\n");
        exit(1);
    } else if (nblocks * block_size != args.input_size) {
        fprintf(stderr, "reordercat: input file size not a multiple of block size\n");
        exit(1);
    }

    size_t* blockpos = (size_t*)malloc(nblocks*sizeof(size_t));
    for (size_t i = 0; i < nblocks; ++i) {
        blockpos[i] = i;
    }

    // Copy file data
    while (nblocks != 0) {
        // Choose block to read
        size_t index = random() % nblocks;
        size_t pos = blockpos[index] * block_size;
        blockpos[index] = blockpos[nblocks - 1];
        --nblocks;

        // Transfer that block
        io_seek(inf, pos);
        ssize_t amount = io_read(inf, buf, block_size);
        if (amount <= 0) {
            break;
        }
        io_seek(outf, pos);
        io_write(outf, buf, amount);
    }

    io_close(inf);
    io_close(outf);
    io_test_end();
    free(buf);
    free(blockpos);
}
