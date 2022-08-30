# version-5
SRC=$(wildcard ./*.c)
OBJS=$(patsubst %.c, %.o, $(SRC))

main:$(OBJS)
	gcc $^ -o $@

%.o:%.c
	@gcc -c $< -o $@

.PHONY:clean
clean:
	-rm -rf $(OBJS) main




# # version-4
# SRC=$(wildcard ./*.c)
# OBJS=$(patsubst %.c, %.o, $(SRC))

# main:$(OBJS)
# 	gcc $^ -o $@

# %.o:%.c
# 	gcc -c $< -o $@

# clean:
# 	rm -rf $(OBJS) main



# version -3
# OBJS = add.o sub.o mul.o div.o main.o

# main:$(OBJS)
# 	gcc $^ -o $@

# %.o:%.c
# 	gcc -c $< -o $@

# clean:
# 	rm -rf $(OBJS) main



# version-2
# OBJS = add.o sub.o mul.o div.o main.o

# main:$(OBJS)
# 	gcc $^ -o $@

# add.o:add.c
# 	gcc -c $< -o $@

# sub.o:sub.c
# 	gcc -c $< -o $@

# mul.o:mul.c
# 	gcc -c $< -o $@

# div.o:div.c
# 	gcc -c $< -o $@

# main.o:main.c
# 	gcc -c $< -o $@

# clean:
# 	rm -rf $(OBJS) main



# version-1
# OBJS = add.o sub.o mul.o div.o main.o

# main:$(OBJS)
# 	gcc $(OBJS) -o main

# add.o:add.c
# 	gcc -c add.c -o add.o

# sub.o:sub.c
# 	gcc -c sub.c -o sub.o

# mul.o:mul.c
# 	gcc -c mul.c -o mul.o

# div.o:div.c
# 	gcc -c div.c -o div.o

# main.o:main.c
# 	gcc -c main.c -o main.o

# clean:
# 	rm -rf $(OBJS) main
