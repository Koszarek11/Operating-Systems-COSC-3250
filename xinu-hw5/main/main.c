/**
 * @file     main.c
 */
/* Embedded Xinu, Copyright (C) 2009, 2022.  All rights reserved. */

#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define max_buffer 1025

int eCounter(char inputArray[]);
void decode(char inputArray[], char inputArray2[], int key);
void readArray(char inputArray[], int max_size);

/**
 * Main process.  You can modify this routine to customize what Embedded Xinu
 * does when it starts up.
 */
process main(void){

    kprintf("Hello Xinu World!\n");

    kprintf("Enter your codebreaker text:\n");

    int key = 0;
    int max = 0;
    int max2nd = 0;
    int numE = 0;
    int i = 0;
    int j = 0;
    char input_array[max_buffer];
    char input_array2[max_buffer]; // array that gets manipulated
    readArray(input_array, max_buffer);

    // Block Below is Utilized to Find the 2nd Largest Number of E's
    for(i = 0; i < 27; i++)
    {
            decode(input_array,input_array2,i);
            numE = eCounter(input_array2);
            if (numE > max)
            {
                    max2nd = max;
                    max = numE;
            }
            else if (numE > max2nd && numE < max)
            {
                    max2nd = numE;
            }
    }
    for(j = 26; j >= 0; j--)
    {
            decode(input_array, input_array2, j); //Finding key of second largest number of E's
            numE = eCounter(input_array2);
            if(max == max2nd || max == 1)
            {
                    key = 0;
            }
            else if (numE == max2nd)
            {
                    key = j;
            }
    }
    decode(input_array, input_array2, key);
    kprintf("Most probable key is %d \n", key);
    kprintf("%s \n", input_array2);


    // TODO: Put your main program for codebreaker here.
    //       Replace any calls to kprintf() with kprintf().
    //       Replace any calls to kgetc() with kgetc().

    while (1);

    return 0;
}

void decode(char inputArray[], char inputArray2[], int key){ //inputArray2 gets decoded and manipulated by function

        int c = 0;
        int value = 0;
        int o = 0;
        int i = 0;  // Initiliaze all of the variables
        int skip = 0;
        while ((c = inputArray[i]) != EOF && inputArray[i] != '\0'){ //Runs until end of array

                if (c == '\n')
                {
                        skip = 1;
                }
                if (skip == 0)
                {
                        if (c >= 'A' && c <= 'Z')
                        {
                                value = c - 64; //Arranges A-Z in 1-27 Range to Encrpyt
                        }
                        else if (c >= 'a' && c <= 'z')
                        {
                                value = c - 96; //Does same thing for lowercase
                        }
                        else
                        {
                                value = 0; //Prints all other characters as whitespace
                        }

                        o = (value - o - key) % 27; //Used to Decrypt value subtracting the key and o(0)

                        if (o < 0)
                        {
                                o = o + 27; //if negative value, put back into 1-27 range
                        }
                        if (o != 0 && c != 10)
                        {
                                inputArray2[i] = o + 64; // assigning back into ASCII values for
                                                 // program to read if not whitespace
                        }
                        else
                        {
                                inputArray2[i] = 32; // prints whitespace
                        }
                        o = value; //saves output as ASCII and prints
                }
                else
                {
                        inputArray2[i] = c;
                }
                skip = 0;
                i++;
        }
        inputArray2[i] = '\0';
      }

      int eCounter(char inputArray[]){
              int currCharacter = 0;
              int i = 0;
              int count = 0;
              for(i = 0; (currCharacter = inputArray[i]) != EOF && inputArray[i] != '\0'; i++)
              {
                      if(currCharacter == 'E' || currCharacter == 'e')
                      {
                              count++;
                      }
              }
              return count;
      }
      void readArray(char inputArray[], int max_size){  //puts characters into array of max 1025


              int c = 0;
              int i = 0;
              for (i = 0; i < max_buffer && (c = kgetc()) != EOF; i++)
              {
                      inputArray[i] = c;
              }
              inputArray[i] = '\0';
      }
