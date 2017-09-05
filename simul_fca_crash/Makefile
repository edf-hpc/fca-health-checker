EXE_NAME = fca_crash
OBJS = fca_crash.o
CC = gcc
INC = -I. -I/opt/mellanox/fca/include
LIBS = -L/opt/mellanox/fca/lib -L/opt/mellanox/fca/lib64 -lfca

all: $(EXE_NAME)
.c.o:
	$(CC) $< -c $(INC)

$(EXE_NAME): $(OBJS)
	$(CC) -o $@ $(OBJS) $(INC) $(LIBS)

clean:
	rm -f *.o $(EXE_NAME)
