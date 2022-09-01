
all:read write


read:mmap_read.c
	gcc $< -o $@

write:mmap_write.c
	gcc $< -o $@

.PHONY:clean
clean:
	rm -rf read write