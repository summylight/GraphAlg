.PHONY: NMSRW4  SSRW4  WRW4  clean
CC := g++
CFLAGS := -w -g -Os -std=c++20
objects =  NMSRW4 SSRW4 WRW4
all : $(objects)
$(objects): %:%.cpp
	$(CC)  $(CFLAGS) $< -o ../$@ -ligraph
clean:
		-rm  *.o
