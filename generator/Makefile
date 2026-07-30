CXX = g++
CXXFLAGS = -g -Wall -Wextra -std=c++23 -Iinclude -I..
LDFLAGS = -lfl
TARGET = lab1

LOCAL_SRCS = main.cpp gen_strings.cpp generator.hpp time.cpp
PARENT_SRCS = ../classes.hpp ../my_regex.cpp ../smc_class.cpp ../parser_sm.cpp ../flex_lexer.cpp ../flex_lexer.hpp 

OBJS = $(LOCAL_SRCS:.cpp=.o)
OBJS := $(OBJS:.cc=.o)
OBJS += $(patsubst ../%.cpp,%.o,$(filter %.cpp,$(PARENT_SRCS)))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: ../%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

lex.yy.cc: lexer.l
	flex -+ -o lex.yy.cc $<

clean:
	rm -f $(OBJS) $(TARGET) lex.yy.cc

rebuild: clean all