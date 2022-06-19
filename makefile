COMPILER = g++
CFLAGS += -g -Wall
INCPATH += -I ./header

TARGET = PacketAnalyzer
MAINOBJ = main.o
OBJECTS += \
	OperatingManager.o \
	EthernetManager.o \
	IP_Manager.o \
	UDP_Manager.o \
	TCP_Manager.o \
	ApplicationManager.o \
	DNS_Manager.o \
	FTP_Manager.o \
	Telnet_Manager.o \
	HTTP_Manager.o

SUBDIRS = source 

.PHONY: all clean

all: $(TARGET)

bin:
	mkdir -p bin

$(TARGET): $(OBJECTS) $(MAINOBJ)
	$(COMPILER) -o $@ $^ $(INCPATH) $(CFLAGS)

$(OBJECTS):
	for subdir in $(SUBDIRS); do\
		cd $$subdir;\
		make;\
		mv *.o ../;\
		cd ..;\
	done
.c.o: $(MAINOBJ)
	$(COMPILER) -c $^ $(INCPATH) $(CFLAGS)
clean:
	rm $(TARGET)
	rm *.o