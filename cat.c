#include "io.h"
#include "test_utils.h"

// Usage: ./cat [-s SIZE] [-o OUTFILE] [FILE]
//    Copies the input FILE to OUTFILE one character at a time.

int main(int argc, char* argv[]) {
	// parse arguments
	io_arguments args=io_arguments_parse(argc, argv, "s:o:i:");

	io_test_begin();
	io_file* inf = io_open_check(args.input_file, O_RDONLY);
	io_file* outf = io_open_check(args.output_file,
			O_WRONLY | O_CREAT | O_TRUNC);

	while (args.input_size > 0) {

		int ch = io_readc(inf);
		if (ch == EOF) {
			break;
		}
		io_writec(outf, ch);
		--args.input_size;
	}

	io_close(inf);
	io_close(outf);
	io_test_end();
}
