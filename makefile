OUTDIR := build
BIN := $(OUTDIR)/nova
SRCDIR := src
SRC := $(wildcard $(SRCDIR)/*.c)
OBJ := $(patsubst $(SRCDIR)/%.c, $(OUTDIR)/%.o, $(SRC))
FLAGS := -Iincludes/
TESTS := $(wildcard tests/*.c)
TESTS_OUT := $(patsubst tests/%.c, $(OUTDIR)/%.test, $(TESTS))
all: $(BIN)

$(BIN): main.o
	gcc -o $@ $(OUTDIR)/main.o $(OBJ) $(FLAGS)

main.o: main.c $(OBJ)
	gcc -c main.c -o $(OUTDIR)/main.o $(FLAGS)

$(OUTDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OUTDIR)
	gcc -c $< -o $@ $(FLAGS)

test: $(BIN) $(TESTS_OUT)
	@echo "Done Testing $^"

$(OUTDIR)/%.test: tests/%.c
	gcc $< -o $@ $(FLAGS) $(OBJ)
	@echo "\033[33m---- Running $< ----\033[0m"
	@./$@
	./$@ > $@.output
	@echo "\033[33m---- Done ($<) ----\033[0m"

.PHONY: test
