/***********************************************************************
* compile with: gcc -o TruncateAudio TruncateAudio.c -lm
*
***********************************************************************/



#include <stdio.h>
#include <stdlib.h>
#include "math.h"

printFileFormatError(){
   perror("Error: Input file is not a valid .WAV file.\n");
}

int main(int argc, char **argv) {
   int Nsamples;
   int i, j, number;
   char ch1, ch2;


   // For each argument, try to open it as a file and read it
   for(i=1; i<argc; i++){
      char *filename = argv[i];
      FILE *file = fopen(filename, "r"); // read mode
      if (file == NULL){
         perror("Error while opening the file.\n");
         exit(1);
      }

      // Check the header
      {
         char ch1, ch2, ch3, ch4, ch5, ch6, ch7, ch8;
         ch1 = fgetc(file);
         ch2 = fgetc(file);
         ch3 = fgetc(file);
         ch4 = fgetc(file);

         fseek(file,8,SEEK_SET);
         ch5 = fgetc(file);
         ch6 = fgetc(file);
         ch7 = fgetc(file);
         ch8 = fgetc(file);

         if (!(ch1 == 'R' && ch2 == 'I' && ch3 == 'F' && ch4 == 'F' &&
             ch5 == 'W' && ch6 == 'A' && ch7 == 'V' && ch8 == 'E')){
            printFileFormatError();
         }
      }


      FILE *outfile = fopen("output.wav", "wb");
      if (outfile == NULL){
         perror("Error while opening the output file.\n");
         exit(1);
      }

      // copy the header to the output file
      int i;
      for (i=0; i<=43; i++){
         fseek(file, i, SEEK_SET);
         fputc(fgetc(file), outfile);
      }



      fseek(file, 44, SEEK_SET); // Set the cursor to the first sample
      unsigned long long sum=0;
      unsigned long long numSamples=0;
      double mean; //average
      int number = 0;
      unsigned long long minimum=0;
      unsigned long long maximum=0;
      double variance;
      double std_dev;
      double dist_squared = 0;

      unsigned long long all_pos_sum=0;
      double rectified_avg;
      double rectified_dist_squared = 0;
      double rectified_variance;
      double rectified_std_dev;

      for (;;){
         ch1 = fgetc(file); // Attempt to read one byte
         if (feof(file)) break;
         ch2 = fgetc(file); // Attempt to read another byte
         if (feof(file)) break;
         number = 0;
 
       // Construct number from 16-bit chunk
         for (j=0; j<=7; j++){
            if ((1 << j) & ch2) number += (1 << (j+8));
            if ((1 << j) & ch1) number += (1 << j);
         }

         if (number >= 32768) number -= (2*32768);


         if (number >= 9000){
            number = 9000;
         }else if (number <= -9000){
            number = -9000;
         }


         //printf("%d\n", number);


         ch1=0x00;
         ch2=0x00;
         for (j=0; j<=7; j++){
            if ((1 << (j+8)) & number) ch2 += (1 << j);
            if ((1 << j) & number) ch1 += (1 << j);
         }
         fprintf(outfile, "%c%c", ch1, ch2);

      }
      fclose(outfile);
      fclose(file);
   }



   return 0;
}
