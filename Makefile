all : TruncateAudio wavFileTestbed

wavFileTestbed :
	gcc -o wavFileTestbed wavFileTestbed.c -lm

TruncateAudio :
	gcc -o TruncateAudio TruncateAudio.c -lm


clean :
	-rm -f TruncateAudio wavFileTestbed TruncateAudio.o wavFileTestbed.o