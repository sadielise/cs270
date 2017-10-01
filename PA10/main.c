/*
 * "Copyright (c) 2014 by Fritz Sieker."
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written
 * agreement is hereby granted, provided that the above copyright notice
 * and the following two paragraphs appear in all copies of this software.
 *
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE TO ANY PARTY FOR DIRECT,
 * INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
 * OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE AUTHOR
 * HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * THE AUTHOR SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS"
 * BASIS, AND THE AUTHOR NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT,
 * UPDATES, ENHANCEMENTS, OR MODIFICATIONS."
 */
/** @mainpage cs270 Programming Assignment - LC3 Assembler
 *  \htmlinclude "assembler.html"
 */


/** @file main.c
 *  @brief Driver for the lc3 assembler
 *  @details This file is a driver for the lc3 assembler and contains main().
 * <p>
 * @author <b>Fritz Sieker</b>
 */

#include <string.h>
#include <stdlib.h>

#define LC3AS_VAR

#include "assembler.h"

/** print usage statement for program */
static void usage (void) {
  fprintf(stderr, "Usage: lc3as [-hex] <ASM filename> or lc3as -disassemble <HEX filename>\n");
  exit (1);
}

/** Verify file has a <code>.asm</code> suffix */
static char* check_for_asm_file (char* fname) {
  int len = strlen(fname);
  
  if (len >= 5) { // mininum length name is x.asm
    char* suffix = fname + len -4;
    if (strcmp(suffix, ".asm") == 0){
      return suffix;
		}
  }

  return 0;
}

/** Verify file has a <code>.hex</code> suffix */
static char* check_for_hex_file (char* fname) {
  int len = strlen(fname);
  if (len >= 5) { // mininum length name is x.asm
    char* suffix = fname + len -4;
    if (strcmp(suffix, ".hex") == 0){

      return suffix;
		}
  }

  return 0;
}



/** The entry point of the assembler. The program is invoked using:
 *  <pre><code>
 *  mylc3as [-hex] assembly_file_name
 *  </code></pre>
 *  @param argc - count of arguments
 *  @param argv - an array of arguments
 */
int main (int argc, char* argv[]) {
	int disassembler = 0;
  char* file = argv[argc -1];

	//check if running the assembler or the disassembler
  if (argc == 3) {
    if (strcmp(argv[1], "-hex") == 0)
      inHex = 1;
		else if(strcmp(argv[1], "-disassemble")==0)
			disassembler = 1;
    else
      usage(); // this exits
  }
	//printf("debug0\n");

	//make sure a .hex file or a .asm file
	if(disassembler == 0){
  	if (! check_for_asm_file(file))
  	  usage(); // this exits
	}
	else{
		if (! check_for_hex_file(file))
  	  usage();
	}


  asm_init();

	//printf("debug1\n");

  char* obj_file = strdup(file);
  char* suffix;
	if(disassembler ==0)
   suffix= check_for_asm_file(obj_file);
	else
		suffix = check_for_hex_file(obj_file);
	//printf("suffix: %s\n", suffix);


	//if assembler make a .hex file
	if(disassembler == 0){
 	 if (inHex)
	    strcpy(suffix, ".hex");
	  else
	    strcpy(suffix, ".obj");
	}
	else{
		strcpy(suffix, ".asm");
	}

	//printf("debug2\n");

  numErrors = 0;
  srcLineNum = 0;

  char* sym_file = strdup(file);
	
	//printf("debug3\n");
	//	printf("objfile: %s\n", obj_file);

  if(disassembler ==0){
   suffix= check_for_asm_file(sym_file);
	}
	else{
		suffix = check_for_hex_file(sym_file);
	}


	//printf("debug4\n");
	//printf("suffix: %s\n", suffix);

  strcpy(suffix, ".sym");

  printf("STARTING PASS 1\n");
	if(disassembler ==0){
  	asm_pass_one(file, sym_file);
	}
 else{
		dsm_pass_one(file, sym_file);
	}
  printf("%d errors found in first pass\n", numErrors);

  if (numErrors == 0) {
    srcLineNum = 0;
    printf("STARTING PASS 2\n");
		if(disassembler == 0){
    	asm_pass_two(obj_file);
		}
		else{
			dsm_pass_two(obj_file);
		}
    printf("%d errors found in second pass\n", numErrors);
  }

  if (numErrors > 0) {
    remove(obj_file); // errors ignored
    remove(sym_file); // errors ignored
  }

  free(obj_file);
  free(sym_file);

  asm_term();

  return (numErrors != 0);
}
