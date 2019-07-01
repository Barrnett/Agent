#Makefile
CC = g++
CFLAGS = -fPIC -I. -I./include/sigar -I./include -Wall
LFLAGS = -fPIC -shared 
#LFLAGS = -fPIC -ldl 

RELEASE =1
BITS =64

ifeq ($(RELEASE),0)
    # debug
    CFLAGS += -g
else
    # release
    CFLAGS += -static -O2 -DNDEBUG
    LFLAGS += 
endif

ifeq ($(BITS),64)
    CFLAGS += -m64 -D__x86_64__ -I./include/mysql/ia64-64bit -I./include/mysql
    LFLAGS += -m64 -L./lib/ia64-64bit -lmysqlcppconn -lmysql  -ljson
else
    CFLAGS += -m32 -D__x86_32__ -I./include/mysql/x86-32bit -I./include/mysql
    LFLAGS += -m32 -L./lib/x86-32bit  -lmysqlcppconn -lmysql -ljson
endif


TARGETS = libvpxnetmanager.so
#TARGETS = main
#OBJS = LoaderUtil.o monitor.o sigar_info.o mysql_info.o
OBJS = LoaderUtil.o monitor.o  mysql_info.o

$(TARGETS): $(OBJS)
	$(CC) -o $(TARGETS) $(LFLAGS) $(OBJS)

sigar_info.o: sigar_info.cpp sigar_info.h
	$(CC) $(CFLAGS) -c sigar_info.cpp

LoaderUtil.o: LoaderUtil.cpp LoaderUtil.h
	$(CC) $(CFLAGS) -c LoaderUtil.cpp

monitor.o: monitor.cpp monitor.h sigar_info.h
	$(CC) $(CFLAGS) -c monitor.cpp

mysql_info.o: mysql_info.cpp mysql_info.h
	$(CC) $(CFLAGS) -c mysql_info.cpp

.PHONY : clean
clean:
	rm -f $(OBJS) $(TARGETS)