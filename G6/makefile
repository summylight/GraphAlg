.PHONY: NMSRW6  SSRW6  WRW6  clean
CC := g++
CFLAGS := -g
objects =  NMSRW6 SSRW6 WRW6
all : $(objects)
$(objects): %:%.cpp
	$(CC)  $(CFLAGS) $< -o ../$@ -ligraph
clean:
		-rm  *.o
