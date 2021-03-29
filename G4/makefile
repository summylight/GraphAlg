.PHONY: NMSRW4  SSRW4  WRW4  clean
CC := g++
CFLAGS := -g
objects =  NMSRW4 SSRW4 WRW4
all : $(objects)
$(objects): %:%.cpp
	$(CC)  $(CFLAGS) $< -o ../$@ -ligraph
clean:
		-rm  *.o
