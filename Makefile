CC=gcc
CFLAGS=-c -lm

all: wavFileTestbed TruncateAudio

wavFileTestbed : wavFileTestbed.o
	$(CC) -o wavFileTestbed wavFileTestbed.o -lm

TruncateAudio : TruncateAudio.o
	$(CC) -o TruncateAudio TruncateAudio.o -lm

wavFileTestbed.o : wavFileTestbed.c
	$(CC) $(CFLAGS) wavFileTestbed.c

TruncateAudio.o : TruncateAudio.c
	$(CC) $(CFLAGS) TruncateAudio.c


clean :
	-rm -f TruncateAudio wavFileTestbed TruncateAudio.o wavFileTestbed.o
