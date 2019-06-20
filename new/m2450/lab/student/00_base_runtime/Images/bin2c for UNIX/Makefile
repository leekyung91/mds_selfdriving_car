CC = cc
CFLAGS  = -g -Wall -Werror
TARGET = bin2c

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	$(RM) $(TARGET)