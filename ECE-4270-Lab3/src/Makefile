CC=gcc
CFLAGS=
SOURCES	:= $(wildcard *.c)
OBJS := $(SOURCES:%.c=%.o)

TARGET := riscv-pipeline

.PHONY : all
all : $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@  

.PHONY	: clean
clean	:
	rm -rf $(TARGET) $(OBJS)