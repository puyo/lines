
TARGET = lines
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

CC = g++
CXXFLAGS = `allegro-config --cflags`
LDFLAGS = `allegro-config --libs`

all: $(TARGET)

$(TARGET): $(OBJS)
