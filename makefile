.PHONY: 4RW 4RW.o NMSRW5 NMSRW5.o SSRW5 SSRW.o WRW5 WRW5.o clean
CC := g++
CFLAGS := -Wall -g -Os -std=c++20
objects = 4RW NMSRW5 SSRW5 WRW5 
all : $(objects)
$(objects): %:%.cpp
	$(CC) -c $(CFLAGS) $< -o $@ -ligraph
clean:
		-rm  *.o
