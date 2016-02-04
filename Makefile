CC = gcc
OBJS = main.o ftm_serial.o ftm_time.o ftm_debug.o ftm_list.o ftm_queue.o simclist.o ftm_serial_io.o ftm_mem.o ftm_lora.o ftm_lora_wan.o
TARGET = lora
LDFLAGS = -levent -lconfig -lpthread

.SUFFIXES : .c .o

all : $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

clean :
	rm -f $(OBJS) $(TARGET)
