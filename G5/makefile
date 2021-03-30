.PHONY: 4RW NMSRW5 SSRW5  NOBACK5 4SHOTGUN SHOTGUN5 WRW5 clean
CC := g++
CFLAGS := -g
objects = 4RW NMSRW5 SSRW5  NOBACK5 4SHOTGUN SHOTGUN5 WRW5
all : $(objects)
$(objects): %:%.cpp
	$(CC)  $(CFLAGS) $< -o ../$@ -ligraph
clean:
		-rm  *.o
