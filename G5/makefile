.PHONY: 4RW NMSRW5 SSRW5  WRW5 clean
CC := g++
CFLAGS := -w -g -Os -std=c++20
objects = 4RW NMSRW5 SSRW5 WRW5
all : $(objects)
$(objects): %:%.cpp
	$(CC)  $(CFLAGS) $< -o ../$@ -ligraph
clean:
		-rm  *.o
