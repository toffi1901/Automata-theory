CXX := g++
CXXFLAGS := -g -Wall -Wextra -std=c++23 -Iinclude
LDFLAGS := -lfl
TARGET := lab1


CPP_SRCS := $(wildcard *.cpp) my_regex.cpp main.cpp input.cpp flex_lexer.cpp parser_sm.cpp
CC_SRCS := lex.yy.cc
HDRS := classes.hpp AppClass.hpp parser_sm.h

OBJS := $(CPP_SRCS:.cpp=.o) $(CC_SRCS:.cc=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.cc $(HDRS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

lex.yy.cc: lexer.l
	flex -+ -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET) lex.yy.cc

rebuild: clean all

.PHONY: all clean rebuild

print-%:
	@echo '$*=$($*)'