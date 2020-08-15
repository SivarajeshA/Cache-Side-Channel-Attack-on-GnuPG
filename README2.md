# Programming Assignment 1
## CS665: Secure Memory Systems, 2019-20, Semester-I

## Directory Structure
PA1 code base contains seven directory, Makefile and other supported files required to complete the PA1 and described as follows:

**1.** The **lib** directory contains the standard functions and data structure that can be used in any task given for the PA1. This directory contains two files, namely **util.h** and **util.c**. All commonly used functions and data structures are declared in the **util.h**, and the corresponding definition is provided in the **util.c**. If you need additional functions and data structure that can be referenced in any task for PA1, utilize **util.h** and **util.c** for declaration and definition for the same.

**2.** The **report** directory contains a blank **README** file. Before submitting the assignment, populate it with necessary information like references used, help taken from others etc. for completing the PA1. Apart from the README file, you need to submit a **report in PDF** that you can place within the same directory..

**3. Task-1a, Task-1b, and Task-3a** contain the base code for **sender** and **receiver** in files **sender.c** and **receiver.c**, respectively for each task. You need to use the same files for implementing the **sender** and **receiver** functionality required to create a **FLUSH+RELOAD** based covert channel between them to share the classified information.

**4. Task-2a** directory contains the base code for implementing the **spy** process, which needs to observe the use of RSA algorithm implemented in the **GnuPG** library with the help of a **side-channel** attack based on **FLUSH+RELOAD** attack.

**5. Examples** directory contains two example programs. One is demonstrating the use of library function for mapping a file into memory, and the other one is to convert a string into binary and vice-versa.

**5. Makefile** is used for the compilation of the PA1 code base.

**6.** The **share_mem.txt** file is for establishing a shared memory region between two processes.  For creating a covert channel between **sender** and the **receiver**, a shared memory region needed on which we can create a **FLUSH+RELOAD** based **Cache Covert Channel** as required for the **Task-1a, Task-1b, and Task-3a**.

**7. sample.txt** is for **Task-1b** where you need to send the file contents from **sender** to **receiver** over the covert channel. This file can be utilized to analyze the working of your created covert channel.

**8.** The **red_heart.jpg** file is an image file that needs to be transferred from **sender** to **receiver** over the **covert channel** for **Task-3a**.

**9.** The **CS665_2019_PA1.pdf** file desribes the **Programming Assignemnt 1 (PA1)** and the task needs to be done for completing it.

**10.** The **Cache_Side_Channel_on_GnuPG.pdf** file describes the procedure for getting the addresses of interest for mounting **FLUSH+RELOAD** based **Cache Side-Channel** attack on the **RSA** Algorithm implementation in **GnuPG** library (for **Task-2a**).


## To Build System
Use **make** command to compile the assignment. After successful compilation, all the binaries are generated in the root directory of the PA1 code base.

**1. Compile code for all the Task:** To compile for all the task, use the **make** command from the root directory of the PA1 code base.
> **make**

**2. Compile Individual Task code:** Individual task code can be compiled using the **make** command followed by the Task number in lower case. For example, if you want to build the code base for Task-1b, the syntax is as follows:
> **make task-1b**

Similarly, other tasks i.e., task-1a, task-2a and task-3a code can be compiled individually.

**3. Clean the build:**
> **make clean**

## Generated Binary Code
After the compilation of an individual task code or entire assignment code, binary files are placed in the root directory of the PA1 code base. These files are as follows:

**1. Task-1a:** Its **sender** and **receiver** binary files are represented as **t1a-sender** and **t1a-receiver**, respectively.

**2. Task-1b:** Its **sender** and **receiver** binary files are represented as **t2b-sender** and **t2b-receiver**, respectively.

**3. Task-2a:** Its **spy** binary file is represented as **t2a-spy**.

**4. Task-3a (Challenge Task):** Its **sender** and **receiver** binary files are represented as **t3a-sender** and **t3a-receiver**, respectively.

## Prepare Submission Tar File
To prepare the submission tar file, **make** utility can be utilized for the same. But before preparing it, first, **prepare your report in PDF** and place it inside the report directory. Apart from the report file, you need to update the **README** file present in the report directory as per the requirement. The syntax for creating the submission tar file is as follows:
> **make prepare-submit group=GroupNo**

Where GroupNo is 1, 2, 3 .... as assgined to your group.

## Pre-requisites (To solve Task-2a)

1. Install **lib32-glibc** and **gcc-multilib** (require to build GnuPG for 32bit Architecture)
2. Download GnuPG Version 1.4.13 from [https://gnupg.org/ftp/gcrypt/gnupg/gnupg-1.4.13.tar.gz](https://gnupg.org/ftp/gcrypt/gnupg/gnupg-1.4.13.tar.gz)
3. Configure GnuPG Build system for 32bit Architecture with debugging Symbols
> **./configure --build=i686-pc-linux-gnu "CFLAGS=-m32 -g" "CXXFLAGS=-m32 -g" "LDFLAGS=-m32 -g"**

4. Build GnuPG
> **make**
