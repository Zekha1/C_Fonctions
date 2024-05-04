#include "io.h"
#include "test_utils.h"
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <errno.h>

// profile.c
//    The profile functions measure how much time and memory are used
//    by your code. The io_profile_end() function prints a simple
//    report to standard error. The io_parse_arguments() function
//    parses common arguments into a structure.

static double begin;

void io_test_begin() {
	struct timespec tv;
	int r=clock_gettime(CLOCK_REALTIME, &tv);
    assert(r >= 0);
	begin = tv.tv_sec + tv.tv_nsec * 1.0e-9;
}

void io_test_end() {
    struct timespec tv;
	double end;
	int r=clock_gettime(CLOCK_REALTIME, &tv);
    assert(r >= 0);
	end = tv.tv_sec + tv.tv_nsec * 1.0e-9;

    char buf[256];
    int len = sprintf(buf, "%lf\n",end-begin);
      int fdtest = open("./.testutils",O_WRONLY|O_CREAT|O_TRUNC,0644);                
    // Print the report to file descriptor 100 if it's available. Our
    // `check.pl` test harness uses this file descriptor.
    //off_t off = lseek(100, 0, SEEK_CUR);
    //int fd = (off != (off_t) -1 || errno == ESPIPE ? 100 : STDOUT_FILENO);
    //if (fd == STDOUT_FILENO) {
      //  fflush(stdout);
    //}
//    ssize_t nwritten = write(STDOUT_FILENO, buf, len);
    ssize_t nwritten = write(fdtest, buf, len);
	close(fdtest);
    assert(nwritten == len);
}


io_arguments io_arguments_parse(int argc, char** argv,const char * opts) {
	io_arguments io_args={0};

	/* default values */

	io_args.input_size = -1;
	io_args.block_size = 0;
	io_args.stride = 1024;
	io_args.lines = 0;
	io_args.output_file = io_args.input_file = NULL;
	io_args.opts = opts;
	io_args.program_name = argv[0];

	int arg;
	char* endptr;
	while ((arg = getopt(argc, argv, opts)) != -1) {
		switch (arg) {
			case 's':
				io_args.input_size = (size_t) strtoul(optarg, &endptr, 0);
				if (endptr == optarg || *endptr) {
					goto usage;
				}
				break;
			case 'b':
				io_args.block_size = (size_t) strtoul(optarg, &endptr, 0);
				if (io_args.block_size == 0 || endptr == optarg || *endptr) {
					goto usage;
				}
				break;
			case 'r': {
						  unsigned long seed = strtoul(optarg, &endptr, 0);
						  if (endptr == optarg || *endptr) {
							  goto usage;
						  }
						  srandom(seed);
						  break;
					  }
			case 'o':
					  io_args.output_file=optarg;
					  break;
			default:
					  goto  usage;
		}
	}
	if (optind > argc) goto usage;
	if (optind != argc)
		io_args.input_file = argv[optind];
	return io_args;

usage:
	fprintf(stderr, "Usage: %s", io_args.program_name);
	if (strchr(opts, 's')) {
		fprintf(stderr, " [-s SIZE]");
	}
	if (strchr(opts, 'b')) {
		fprintf(stderr, " [-b BLOCKSIZE]");
	}
		if (strchr(opts, 'r')) {
		fprintf(stderr, " [-r seed]");
	}

	if (strchr(opts, 'o')) {
		fprintf(stderr, " [-o OUTFILE]");
	}
	fprintf(stderr, " [FILE]\n");

	exit(1);
}


