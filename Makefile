LIBS = -lm
#CFLAGS   = -O3 -Wall -Werror -Wpedantic -Wno-variadic-macros -Wcast-align -Wredundant-decls -Wextra -std=c89 -pedantic -pedantic-errors -mtune=native `sdl2-config --cflags` -Iglesgae
#LDFLAGS  = -O3 `sdl2-config --libs`
CFLAGS   = -g -Wall -Werror -Wpedantic -Wno-variadic-macros -Wcast-align -Wredundant-decls -Wextra -std=c89 -pedantic -pedantic-errors -mtune=native `sdl2-config --cflags` -Iglesgae
LDFLAGS  = -g `sdl2-config --libs`

GLESGAE_OBJS = .objs/glesgae

TARGETS	 = builddir glesgae.a bin/windowtest bin/filestest bin/buttontest bin/nineslicetest bin/menutest

.PHONY: all
all: $(TARGETS)

.PHONY: tests
tests: bin/windowtest bin/filestest bin/buttontest bin/nineslicetest bin/menutest

.PHONY: builddir
builddir:
	@mkdir -p $(GLESGAE_OBJS) || true

$(GLESGAE_OBJS)/%.o: glesgae/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

glesgae.a: $(patsubst glesgae/%.c,$(GLESGAE_OBJS)/%.o,$(wildcard glesgae/*.c))
	ar -r -o $@ $^

bin/windowtest: glesgae.a tests/window.c
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS) ${LIBS} glesgae.a

bin/filestest: glesgae.a tests/file.c
	$(CC) $(CFLAGS) -o $@ $+ ${LDFLAGS} ${LIBS} glesgae.a

bin/buttontest: glesgae.a tests/button.c
	$(CC) $(CFLAGS) -o $@ $+ ${LDFLAGS} ${LIBS} glesgae.a

bin/nineslicetest: glesgae.a tests/nineslice.c
	$(CC) $(CFLAGS) -o $@ $+ ${LDFLAGS} ${LIBS} glesgae.a

bin/menutest: glesgae.a tests/menutest.c
	$(CC) $(CFLAGS) -o $@ $+ ${LDFLAGS} ${LIBS} glesgae.a

.PHONY: clean
clean:
	@rm -rf $(GLESGAE_OBJS) >/dev/null || true
	@rm -f glesgae.a
