#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sched.h>

#ifndef UTIL_H_
#define UTIL_H_

#ifndef HIDEMINMAX
#define MAX(X,Y) (((X) > (Y)) ? (X) : (Y))
#define MIN(X,Y) (((X) < (Y)) ? (X) : (Y))
#endif


#define ADDR_PTR void*
#define CYCLES uint64_t

#define CACHE_BLOCK_SIZE	64
#define MAX_BUFFER_LEN	1024

// Arrays used for calibration
size_t array[5*1024];

size_t hit_histogram[600];
size_t miss_histogram[600];


typedef struct map_handle_s {
  int fd;
  size_t range;
  ADDR_PTR mapping;
} map_handle_t;

CYCLES measure_one_block_access_time(ADDR_PTR addr);
CYCLES rdtscp(void);
void clflush(ADDR_PTR addr);
void maccess(ADDR_PTR addr);
void* map_file(const char* filename, map_handle_t** handle);
void unmap_file(map_handle_t* handle);
char *string_to_binary(char *s);
char *binary_to_string(char *data);

int LCS(char *str1, char *str2,int n1, int n2);
int max(int, int);
// Methods used for calibration
int getThreshold();
uint64_t rdtsc_nofence();
uint64_t rdtsc();
uint64_t rdtsc_begin();
uint64_t rdtsc_end();
void flush(void* p);
void prefetch(void* p);
void longnop();

#endif
