#ifndef GNUPG_H_
#define GNUPG_H_

#define gnupg_path "/home/tushar/IITK/CS665/PA1/gnupg-1.4.13/g10/gpg"

#define base_add 0x00000000

#define square_add 0x000c5ffc		//mpih-mul.c:270 (First cache line in mpih_sqr_n())
// 0x000c63b1

#define remainder_add 0x000c55d9	//mpih-div.c:329 (Loop in default case in mpihelp_divrem())

#define multiply_add 0x000c5980	//mpih-mul.c:121 (First cache line of mul_n())

#endif