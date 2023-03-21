CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99
LDFLAGS = 

ifeq ($(OS),Windows_NT)
    $(error Windows is not a supported platform)
endif

TARGET = ipkcp
SRCS = ipkcp.c

all: $(TARGET)

$(TARGET): $(SRCS)
    $(CC) $(CFLAGS) $(SRCS) $(LDFLAGS) -o $(TARGET)

clean:
    rm -f $(TARGET) *.o
