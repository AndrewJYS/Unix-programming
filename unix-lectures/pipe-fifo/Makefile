
all:read write


read:fifo_read.c
	gcc $< -o $@

write:fifo_write.c
	gcc $< -o $@

.PHONY:clean
clean:
	rm -rf read write