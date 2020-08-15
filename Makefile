CC = gcc
CFLAGS = -g

src = $(wildcard lib/*.c) \
			$(wildcard Task-1a/*.c) \
			$(wildcard Task-1b/*.c) \
			$(wildcard Task-2a/*.c) \
			$(wildcard Task-3a/*.c) \
			$(wildcard Examples/*.c)

obj = $(src:.c=.o)

all:	task-1a task-1b task-2a task-3a examples

task-1a:	t1a-sender t1a-receiver

t1a-sender:	Task-1a/sender.o lib/util.o
	$(CC) $^ -o $@

t1a-receiver:	Task-1a/receiver.o lib/util.o
	$(CC)  $^ -o $@


task-1b:	t1b-sender t1b-receiver

t1b-sender:	Task-1b/sender.o lib/util.o
	$(CC) $^ -o $@

t1b-receiver:	Task-1b/receiver.o lib/util.o
	$(CC)  $^ -o $@


task-2a:	Task-2a/spy.o lib/util.o
	$(CC) $^ -o $@

task-3a:	t3a-sender t3a-receiver

t3a-sender:	Task-3a/sender.o lib/util.o
	$(CC) $^ -o $@

t3a-receiver:	Task-3a/receiver.o lib/util.o
	$(CC)  $^ -o $@

examples:	file_mapping string_binary

file_mapping:	Examples/file_mapping.o lib/util.o
	$(CC) $^ -o $@

string_binary:	Examples/string_binary.o lib/util.o
	$(CC) $^ -o $@


group%-pa1.tar.gz:	clean
	tar cf - `find . -type f | grep -v '^\.*$$' | grep -v '/CVS/' | grep -v '/\.svn/' | grep -v '/\.git/' | grep -v 'group[0-9].*\.tar\.gz' | grep -v '/submit.token$$'` | gzip > $@


.PHONY: prepare-submit
prepare-submit:	group$(group)-pa1.tar.gz

.PHONY: clean
clean:
	rm -f *-sender *-receiver task-2a file_mapping string_binary $(obj)