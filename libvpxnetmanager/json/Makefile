####################################################
# Generic makefile - 万能Makefile
# for compiling and linking C++ projects on Linux 
# Author: George Foot  Modified:Jackie Lee
#################################################### 
### Customising
#
# Adjust the following if necessary; EXECUTABLE is the target
# executable's filename, and LIBS is a list of libraries to link in
# (e.g. alleg, stdcx, iostr, etc). You can override these on make's
# command line of course, if you prefer to do it that way.
#    
#
EXECUTABLE := ./libjson.a      # 可执行文件名       
LIBDIR:=        # 静态库目录   
LIBS :=              # 库文件名      
INCLUDES:=  ./  #../../Include/port2linux  ../../libchen/include  ../include       # 头文件目录  
SRCDIR:=              # 除了当前目录外，其他的源代码文件目录dd
SRCFILES :=      
#    
# # Now alter any implicit rules' variables if you like, e.g.:   

# debug | release
ver = release
ifeq ($(ver), debug)
CFLAGS = -O0 -g3 -ggdb -gstabs+ -fsanitize=address -fno-omit-frame-pointer
LASAN = -fsanitize=address
else ifeq ($(ver), asan)
CFLAGS = -O0 -fsanitize=address -fno-omit-frame-pointer
LASAN = -fsanitize=address
else
CFLAGS = -O3 
LASAN = 
endif

CC:=g++
WARNINGS := -Wno-deprecated -Wno-sign-compare -Wno-unused-but-set-variable -Wno-unused-variable -Wno-comment -Wno-unused-local-typedefs -Wno-write-strings -Wno-parentheses -Wno-reorder
#CFLAGS := -D TEST -D STDCXX_98_HEADERS -Wall -O3 -std=c++0x -fno-strict-aliasing $(WARNINGS)       
CFLAGS += -Wall  -pthread -fno-strict-aliasing $(WARNINGS)  
CPPFLAGS := $(CFLAGS) 
CPPFLAGS += $(addprefix -I,$(INCLUDES)) -fvisibility=hidden
CPPFLAGS += -MMD
#
# # The next bit checks to see whether rm is in your djgpp bin
# # directory; if not it uses del instead, but this can cause (harmless)
# # `File not found' error messages. If you are not using DOS at all,
# # set the variable to something which will unquestioningly remove
# # files.
#

RM-F := rm -f 


# # You shouldn't need to change anything below this point.
#
SRCS := $(wildcard *.cpp) $(wildcard $(addsuffix /*.cpp, $(SRCDIR))) $(SRCFILES)
OBJS := $(patsubst %.cpp,%.o,$(SRCS))
DEPS := $(patsubst %.o,%.d,$(OBJS))
MISSING_DEPS := $(filter-out $(wildcard $(DEPS)),$(DEPS))
MISSING_DEPS_SOURCES := $(wildcard $(patsubst %.d,%.cpp,$(MISSING_DEPS)))


.PHONY : all deps objs clean clear rebuild info

all: $(EXECUTABLE)

deps : $(DEPS)

objs : $(OBJS)

clean :
	@$(RM-F) *.o
	@$(RM-F) *.d
clear: clean
	@$(RM-F) $(EXECUTABLE)

rebuild: clean all 
ifneq ($(MISSING_DEPS),)
$(MISSING_DEPS) :  
	@$(RM-F) $(patsubst %.d,%.o,$@)
endif
-include $(DEPS)
$(EXECUTABLE) : $(OBJS) 
	ar	rcs	$(EXECUTABLE) $(OBJS)    
	$(CC) -o $(EXECUTABLE) -shared -fPIC -rdynamic -fsanitize=address -ldl $(OBJS) $(addprefix -L,$(LIBDIR)) $(addprefix -l,$(LIBS))
#	$(CC) -o $(EXECUTABLE) -rdynamic -ldl $(OBJS) $(addprefix -L,$(LIBDIR)) $(addprefix -l,$(LIBS))
#	$(CC) -o $(EXECUTABLE) -rdynamic -ldl $(OBJS) $(addprefix -L,$(LIBDIR)) $(addprefix -l,$(LIBS))      
#	$(CC) -o $(EXECUTABLE) -shared -fPIC $(OBJS) $(addprefix -L,$(LIBDIR)) $(addprefix -l,$(LIBS))

info:
	@echo $(SRCS)
	@echo $(OBJS)
	@echo $(DEPS)
	@echo $(MISSING_DEPS)
	@echo $(MISSING_DEPS_SOURCES)
