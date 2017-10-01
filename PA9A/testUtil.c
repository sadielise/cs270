/*
 * testUtil.c - testDriver for the functions of util.h
 *
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
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

/** @mainpage cs270 Programming Assignment - Assembler Utilities
 *  \htmlinclude "util.html"
 */

/** @file testUtil.c
  * @brief Driver to test functions of util.c (do not modify)
  *
  * @details This is a driver program to test the functions of util.c
  * The program accepts two command line parameters and executes one of the functions
  * of util.c. To understand the usage, type the <code>testUtil</code>. with no
  * parameters. This will give a brief description of each command and its parameter.
  * <p>
  * @author: Fritz Sieker
  */

/** Print a usage statement describing how program is used */
static void help() {
  puts("Usage: testUtil <command> <parameter>");
  puts("testUtil ccode  <LC3 cond code> (e.g. np)>- prints cond code or -1");
  puts("testUtil label  <LC3 label> (e.g. HERE)>  - prints 'ok' or 'invalid'");
  puts("testUtil op  <LC3-op> (e.g. ADD)>         - prints pseudo op or -1");
  puts("testUtil reg <LC3-register> (e.g. R3)>    - prints register , -1");
}

/** Print a usage statement describing how program is used, and exits */
static void usage() {
  help();
  exit(1);
}

/** Entry point of the program
 * @param argc count of arguments, will always be at least 1
 * @param argv array of parameters to program argv[0] is the name of
 * the program, so additional parameters will begin at index 1.
 * @return 0 the Linux convention for success.
 */
int main (int argc, char* argv[]) {
  if (argc < 3)
    usage();

  char* cmd = argv[1];
  char* name = argv[2];

  if (strcmp(cmd, "ccode") == 0) {
    printf("numeric cond-code: %d\n", util_parse_cond(name));
  }
  else if (strcmp(cmd, "label") == 0) {
    printf("%s\n", (util_is_valid_label(name) ? "ok" : "invalid"));
  }
  else if (strcmp(cmd, "op") == 0) {
    printf("opcode: %d\n", util_get_opcode(name));
  }
  else if (strcmp(cmd, "reg") == 0) {
    printf("register number: %d\n", util_get_reg(name));
  }
  else {
    usage();
  }

  return 0;
}
