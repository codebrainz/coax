cflags  := $(CPPFLAGS) -Isrc -Iinclude $(CFLAGS) -std=gnu17 -fPIC -Wall -Wextra
ldflags := $(LDFLAGS)

ifdef RELEASE
	cflags += -DNDEBUG -O3 -s -flto
else
	cflags += -g -O0
endif

sources := $(wildcard lib/*.c)
headers := $(wildcard lib/*.h) $(wildcard include/coax/*.h)

all: coax

clean:
	$(RM) coax libcoax.so

coax: libcoax.so src/main.c
	$(CC) $(strip $(cflags) -o $@ src/main.c $(ldflags) -L. -lcoax)

libcoax.so: $(sources) $(headers)
	$(CC) $(strip $(cflags) -DCX_INSIDE_COMPILATION=1 -shared -o $@ $(sources) $(ldflags))

.PHONY: all clean
