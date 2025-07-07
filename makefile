OUTDIR := build
BIN := $(OUTDIR)/tvm
SRCDIR := src
SRC := $(wildcard $(SRCDIR)/*.c)
OBJ := $(patsubst $(SRCDIR)/%.c, $(OUTDIR)/%.o, $(SRC))
FLAGS := -Iincludes/

all: $(BIN)

$(BIN): main.o
	gcc -o $@ $(OUTDIR)/main.o $(OBJ) $(FLAGS)

main.o: main.c $(OBJ)
	gcc -c main.c -o $(OUTDIR)/main.o $(FLAGS)

$(OUTDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OUTDIR)
	gcc -c $< -o $@ $(FLAGS)

