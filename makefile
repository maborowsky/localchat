CC=gcc
CFLAGS=-pthread

localchat: localchat.o network.o peers.o
	gcc -o localchat localchat.o network.o peers.o -pthread
