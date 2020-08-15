#include "util.h"
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

/*
 * Loads from virtual address addr and measure the access time
 */
extern inline __attribute__((always_inline))
CYCLES measure_one_block_access_time(ADDR_PTR addr) {

  CYCLES cycles;

  asm volatile(
    " mfence           \n\t"
    " lfence           \n\t"
    " lfence            \n\t"
    " mov %%rax, %%rdi \n\t"
    " mov (%1), %%r8   \n\t"
    " lfence           \n\t"
    " rdtsc            \n\t"
    " sub %%rdi, %%rax \n\t"
    : "=a"(cycles) /*output*/
    : "c"(addr)
    : "r8", "rdi");

  return cycles;
}

/*
 * Returns Time Stamp Counter
 */
extern inline __attribute__((always_inline))
CYCLES rdtscp(void) {

  CYCLES cycles;
  asm volatile (
    " rdtscp  \n\t"
    : "=a" (cycles));

	return cycles;
}

/*
 * Flushes the cache block accessed by a virtual address out of the cache
 */
extern inline __attribute__((always_inline))
void clflush(ADDR_PTR addr) {

  asm volatile (
    "mfence       \n\t"
    "clflush (%0) \n\t"
    :
    :"r"(addr));
}


/*
 * Loads from virtual address addr
 */
void maccess(ADDR_PTR addr) {

  asm volatile (
    " movq (%0), %%rax  \n\t"
    :
    : "c"(addr)
    : "rax");
}

/*
 * Map a file in shared memory space
 */
void* map_file(const char* filename, map_handle_t** handle){

  if (filename == NULL || handle == NULL) {
    return NULL;
  }

  *handle = calloc(1, sizeof(map_handle_t));
  if (*handle == NULL) {
    return NULL;
  }

  (*handle)->fd = open(filename, O_RDONLY);
  if ((*handle)->fd == -1) {
    return NULL;
  }

  struct stat filestat;
  if (fstat((*handle)->fd, &filestat) == -1) {
    close((*handle)->fd);
    return NULL;
  }

  (*handle)->range = filestat.st_size;

  (*handle)->mapping = mmap(0, (*handle)->range, PROT_READ, MAP_SHARED, (*handle)->fd, 0);

  return (*handle)->mapping;
}

/*
 * unmap a file mapped in memory
 */
void unmap_file(map_handle_t* handle) {

  if (handle == NULL) {
    return;
  }

  munmap(handle->mapping, handle->range);
  close(handle->fd);

  free(handle);
}

/*
* Convert a given ASCII string to a binary string.
*/
char *string_to_binary(char *s) {

  if (s == NULL) return 0; /* no input string */

  size_t len = strlen(s);

  // Each char is one byte (8 bits) and + 1 at the end for null terminator
  char *binary = malloc(len * 8 + 1);
  binary[0] = '\0';

  for (size_t i = 0; i < len; ++i) {
    char ch = s[i];
    for (int j = 7; j >= 0; --j) {
        if (ch & (1 << j)) {
            strcat(binary, "1");
        } else {
            strcat(binary, "0");
        }
    }
  }

  return binary;
}

/*
 * Convert a binary string to a ASCII string
 * Accept a binary string in data. The length of binary string must be multiple of 8.
 * Basically this function coverting a 8bit data stream into a character.
 */
char *binary_to_string(char *data) {

  size_t msg_len = strlen(data) / 8;

  char *msg = malloc(msg_len+1);

  for (int i = 0; i < msg_len; i++) {
      char tmp[8];
      int k = 0;

      for (int j = i * 8; j < ((i + 1) * 8); j++) {
          tmp[k++] = data[j];
      }

      char tm = strtol(tmp, 0, 2);
      msg[i] = tm;
  }

  msg[msg_len] = '\0';
  return msg;
}

// Reference: Below code is taken from github repo
// https://github.com/Anish-Saxena/Flush-Reload
size_t onlyreload(void* addr)
{
  size_t time = rdtsc();
  maccess(addr);
  size_t delta = rdtsc() - time;
  return delta;
}

size_t flushandreload(void* addr)
{
  size_t time = rdtsc();
  maccess(addr);
  size_t delta = rdtsc() - time;
  clflush(addr);
  return delta;
}

int getThreshold()
{
  memset(array,-1,5*1024*sizeof(size_t));
  maccess(array + 2*1024);
  //Doubt why need sched_yield
  sched_yield();
  for (int i = 0; i < 12*1024*1024; ++i)
  {
    size_t d = onlyreload(array+2*1024);
    hit_histogram[MIN(599,d)]++;
  }
  clflush(array+1024);
  for (int i = 0; i < 12*1024*1024; ++i)
  {
    size_t d = flushandreload(array+2*1024);
    miss_histogram[MIN(599,d)]++;
  }
  size_t hit_max = 0;
  size_t hit_max_i = 0;
  size_t miss_min_i = 0;
  for (size_t i = 0; i < 600; ++i)
  {
    // printf("%3zu: %10zu %10zu\n",i,hit_histogram[i],miss_histogram[i]);
    if (hit_max < hit_histogram[i])
    {
      hit_max = hit_histogram[i];
      hit_max_i = i;
    }
    if (miss_histogram[i] > 3 && miss_min_i == 0)
      miss_min_i = i;
  }
  size_t min = -1UL;
  size_t min_i = 0;
  for (int i = hit_max_i; i < miss_min_i; ++i)
  {
    if (min > (hit_histogram[i] + miss_histogram[i]))
    {
      min = hit_histogram[i] + miss_histogram[i];
      min_i = i;
    }
  }
  return min_i;
}

uint64_t rdtsc_nofence() {
  uint64_t a, d;
  asm volatile ("rdtsc" : "=a" (a), "=d" (d));
  a = (d<<32) | a;
  return a;
}

uint64_t rdtsc() {
  uint64_t a, d;
  asm volatile ("mfence");
  asm volatile ("rdtsc" : "=a" (a), "=d" (d));
  a = (d<<32) | a;
  asm volatile ("mfence");
  return a;
}

uint64_t rdtsc_begin() {
  uint64_t a, d;
  asm volatile ("mfence\n\t"
    "CPUID\n\t"
    "RDTSCP\n\t"
    "mov %%rdx, %0\n\t"
    "mov %%rax, %1\n\t"
    "mfence\n\t"
    : "=r" (d), "=r" (a)
    :
    : "%rax", "%rbx", "%rcx", "%rdx");
  a = (d<<32) | a;
  return a;
}

uint64_t rdtsc_end() {
  uint64_t a, d;
  asm volatile("mfence\n\t"
    "RDTSCP\n\t"
    "mov %%rdx, %0\n\t"
    "mov %%rax, %1\n\t"
    "CPUID\n\t"
    "mfence\n\t"
    : "=r" (d), "=r" (a)
    :
    : "%rax", "%rbx", "%rcx", "%rdx");
  a = (d<<32) | a;
  return a;
}
