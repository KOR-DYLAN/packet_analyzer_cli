COMPILER = g++
CFLAGS += -g -Wall
LDFLAGS +=
INCPATH = -I ../header

TARGETS = \
	OperatingManager.o\
	EthernetManager.o \
	IP_Manager.o \
	UDP_Manager.o \
	TCP_Manager.o \
	ApplicationManager.o \
	DNS_Manager.o \
	FTP_Manager.o \
	Telnet_Manager.o \
	HTTP_Manager.o

.PHONY: all

all: $(TARGETS)

.c.o: $(TARGETS)
	$(COMPILER) -c $^ $(INCPATH) $(CFLAGS)