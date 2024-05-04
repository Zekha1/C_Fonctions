#ifndef IO_HH
#define IO_HH
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>

//struct io_file;
typedef struct io_file io_file;

io_file* io_fdopen(int fd, int mode);
io_file* io_open_check(const char* filename, int mode);
int io_close(io_file* f);

off_t io_filesize(io_file* f);

int io_seek(io_file* f, off_t pos);

int io_readc(io_file* f);
int io_writec(io_file* f, int ch);

ssize_t io_read(io_file* f, char* buf, size_t sz);
ssize_t io_write(io_file* f, const char* buf, size_t sz);

int io_flush(io_file* f);

#endif
