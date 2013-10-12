PROG:=roombaLocalTest
ROOMBALIBDIR := ..
ROOMBACONFIG := --enable-short-read --enable-nonblocking
CFLAGS += -Wall -Werror -ansi -pedantic -I$(ROOMBALIBDIR)/includes -D_BSD_SOURCE
CFLAGS += -g
#CFLAGS += -pg
#CFLAGS += -fprofile-arcs -ftest-coverage
LOADLIBS := -Llib -lroomba

all:lib/Makefile $(PROG) 


$(PROG): roombaLocalTest.o unit.o roombaTest.o roombaCommandTest.o roombaStreamTest.o roombaRepeatTest.o roombaExtraTest.o
	make -C lib all
	$(CC) $(CFLAGS) -o $@  $^ $(LOADLIBS)
lib/Makefile:
	-@mkdir lib
	@cd lib;\
	if test -f $(ROOMBALIBDIR)/configure; then \
		$(ROOMBALIBDIR)/configure $(ROOMBACONFIG);\
	elif test -f ../$(ROOMBALIBDIR)/configure; then \
		../$(ROOMBALIBDIR)/configure $(ROOMBACONFIG); \
	else \
		echo "Error: Roomba Library Configure"; \
		exit 1; \
	fi
clean:
	-rm $(PROG) *.o
	-rm *.gcov *.gcda *.gcno
	-rm -R lib
test: $(PROG)
	./$(PROG)
