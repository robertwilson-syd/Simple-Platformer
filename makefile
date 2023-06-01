CC = gcc
CFLAGS = -Wall -Werror -fsanitize=address
TARGET = platformer

all: $(TARGET)
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c
$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c
clean:
	rm $(TARGET)

