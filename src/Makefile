PROJ_NAME=ted
ALUNO=Jose Henrique Goncalves Rodrigues

CC=gcc
CFLAGS= -ggdb -O0 -std=c99 -fstack-protector-all -Werror=implicit-function-declaration -Wall -Wextra
LIBS=-lm

OBJETOS= main.o geo.o qry.o vis.o figure.o list.o tree.o geom.o svg.o

$(PROJ_NAME): $(OBJETOS)
	$(CC) -o $(PROJ_NAME) $(OBJETOS) $(LIBS)

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

main.o: main.c geo.h qry.h list.h
geo.o: geo.c geo.h figure.h list.h
qry.o: qry.c qry.h vis.h svg.h figure.h list.h
vis.o: vis.c vis.h tree.h figure.h list.h svg.h geom.h
figure.o: figure.c figure.h
list.o: list.c list.h
tree.o: tree.c tree.h
geom.o: geom.c geom.h
svg.o: svg.c svg.h figure.h list.h

clean:
	rm -f *.o $(PROJ_NAME)
