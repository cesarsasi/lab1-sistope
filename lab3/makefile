salida_out = lab3


salida_headers = Declaracion.h
salida_source  = $(salida_headers:.h=.c) lab3.c
salida_objects = $(salida_source:.c=.o)

CC     = gcc
CFLAGS = -Wall

depends = .depends

build : $(salida_out) 

$(salida_out) : $(salida_objects)
	$(CC) $(CFLAGS) -o $@ $^ -lm -lpthread

$(objects) :
	$(CC) $(CFLAGS) -c -o $@ $*.c

$(depends) : $(salida_source) $(salida_headers)
	@$(CC) -MM $(salida_source) > $@


clean :
	$(RM) $(salida_out) $(salida_objects) $(zipfile) $(depends)

.PHONY : build zip clean

sinclude $(depends)