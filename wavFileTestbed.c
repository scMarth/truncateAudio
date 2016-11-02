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


      //printf("1-4: %c%c%c%c\n", ch1, ch2, ch3, ch4);
      //printf("5-8: %c%c%c%c\n", ch5, ch6, ch7, ch8);

      //printf("Checked header.\n");

      fseek(file, 44, SEEK_SET); // Set the cursor to the first sample
      long sum=0;
      long numSamples=0;
      double mean; //average
      int number = 0;
      long minimum=0;
      long maximum=0;
      double variance;
      double std_dev;
      double dist_squared = 0;

      long all_pos_sum=0;
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

         //Process Number Here
         //to_buffer(number);
         //printf("%d\n", number);
         numSamples++;
         sum += number;
         all_pos_sum += abs(number);

         if (number > maximum) maximum = number;
         if (number < minimum) minimum = number;


      }


      rectified_avg = (double)all_pos_sum / (double)numSamples;

      mean = ((double)sum)/((double)numSamples);
      printf("sum is %ld\n", sum);
      printf("numSamples is %ld\n", numSamples);
      printf("Mean is %f\n", mean);
      printf("Rectified sum is %ld\n", all_pos_sum);
      printf("Rectified average is %f\n", rectified_avg);

      fseek(file, 44, SEEK_SET); // Set the cursor to the first sample
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

         //Process Number Here
         //to_buffer(number);
         rectified_dist_squared += pow(( (double)abs(number)  - mean), 2);
         dist_squared += pow(((double)number- mean), 2);
      }
      
      rectified_variance = rectified_dist_squared / (double)numSamples;
      rectified_std_dev = sqrt(rectified_variance);

      variance = dist_squared / (double)numSamples;
      std_dev = sqrt(variance);
      printf("dist_squared is %f\n", dist_squared);
      printf("variance is %f\n", variance);
      printf("standard deviation is %f\n", std_dev);

      printf("rectified variance is %f\n", rectified_variance);
      printf("rectified standard deviation is %f\n", rectified_std_dev);

      double min_dev = fabs((double) minimum / std_dev);
      double max_dev = fabs((double) maximum / std_dev);

      printf("maximum is %ld, which is %f standard deviations above the mean\n", maximum, max_dev);
      printf("minimum is %ld, which is %f standard deviations below the mean\n", minimum, min_dev);

   }
   return 0;
}
