#include <strings.h>
#include <stdlib.h>
#include <ctype.h>

#include "lc3.h"
#include "util.h"

/** @file util.c Implement two function in this file
 * @brief Implement two functions in this file. <b>NOTE:</b> One of the data
 * structures is missing several entries. Add the required entries to that
 * data structure.
 */

/** 
 *  A sorted list of all the opcode names the assembler might see. There are
 *  more entries than there are opcodes because some LC3 instructions have
 *  multiple forms. These include JSR/JSRR, RET/JMP and the eight BR
 *  instructions. The table also includes the various pseudo-ops. In other
 *  implementations, this might be a hash table or a BST.
 **/
static name_val_t lc3_instruction_map[] = {
  
  { ".BLKW",    OP_BLKW },
  { ".COPY",    OP_COPY },
  { ".END",     OP_END },
  { ".FILL",    OP_FILL },
  { ".NEG",     OP_NEG },
  { ".ORIG",    OP_ORIG },
  { ".STRINGZ", OP_STRINGZ },
  { ".ZERO",    OP_ZERO },
  { "ADD",      OP_ADD },
  { "ADDI",     OP_ADD },
  { "AND",      OP_AND },
  { "ANDI",     OP_AND },
  { "BR",       OP_BR },
  { "BRN",      OP_BR },
  { "BRNP",     OP_BR },
  { "BRNZ",     OP_BR },
  { "BRNZP",    OP_BR },
  { "BRP",      OP_BR },
  { "BRZ",      OP_BR },
  { "BRZP",     OP_BR },
  { "COPY", 	OP_COPY },
  { "END", 	OP_END },
  { "GETC",     OP_GETC },
  { "GETS",	OP_GETS },
  { "HALT",     OP_HALT },
  { "IN",       OP_IN   },
  { "INVALID",  OP_INVALID },
  { "JMP",      OP_JMP_RET },
  { "JSR",      OP_JSR_JSRR },
  { "JSRR",     OP_JSR_JSRR },
  { "LD",       OP_LD },
  { "LDI",      OP_LDI },
  { "LEA",      OP_LEA },
  { "NEG",	OP_NEG },
  { "NOT",      OP_NOT },
  { "NUM_OP",	NUM_OPCODES },
  { "ORIG",     OP_ORIG },
  { "OUT",      OP_OUT },
  { "PUTS",     OP_PUTS },
  { "PUTSP",    OP_PUTSP },
  { "RESERVED", OP_RESERVED },
  { "RET",      OP_JMP_RET },
  { "RTI",      OP_RTI },
  { "ST",       OP_ST },
  { "STI",      OP_STI },
  { "STR",      OP_STR },
  { "TRAP",     OP_TRAP },
  { "ZERO",	OP_ZERO }
};

/** Let compiler initialize this value */
static int numOps = sizeof(lc3_instruction_map) / sizeof(name_val_t);

/** @todo implement this function */
name_val_t* util_bin_search (name_val_t map[], int numNames, const char* name) {
  
  // put string to lowercase 

  int first = 0;
  int last = numNames-1;
  int mid = (first+last)/2;

  while(first <= last){

    // upper half
    if(strcasecmp(name, map[mid].name) > 0){
	first = mid++;
    }
    // match
    else if(strcasecmp(name, map[mid].name) == 0){
	return &map[mid];
    }
    // lower half
    else{
	last = mid--;
    }
 
    // reset mid
    mid = (first + last)/2;
  }
    return NULL; 
  
}

int util_get_opcode (const char* name) {
  name_val_t* nvp = util_bin_search(lc3_instruction_map, numOps, name); 
  return (nvp ? nvp->val : -1);
}

/** @todo implement this function */
int util_is_valid_label(const char* s) {

  int retVal = 0;

  if(isalpha(s[0]) != 0 || s[0] == '_'){
    for(int i = 1; i < sizeof(s); i++){
	if(isalnum(s[i]) != 0 || s[i] == '_')
	  retVal = 5;
    }
  }

  return retVal;
}

static name_val_t register_map[] = {
  { "R0", 0},
  { "R1", 1},
  { "R2", 2},
  { "R3", 3},
  { "R4", 4},
  { "R5", 5},
  { "R6", 6},
  { "R7", 7},
};

static int numRegisters = sizeof(register_map) / sizeof(name_val_t);

int util_get_reg (const char* regStr) {
  name_val_t* nvp = util_bin_search(register_map, numRegisters, regStr);
  return (nvp ? nvp->val : -1);
}

#define nCond 4
#define zCond 2
#define pCond 1

static name_val_t cond_code_map[] = {
  { "",    nCond | zCond | pCond },
  { "N",   nCond                 },
  { "NP",  nCond         | pCond },
  { "NZ",  nCond | zCond         },
  { "NZP", nCond | zCond | pCond },
  { "P",                   pCond },
  { "Z",           zCond         },
  { "ZP",          zCond | pCond },
};

static int numCondCodes = sizeof(cond_code_map) / sizeof(name_val_t);

int util_parse_cond (const char* ccStr) {
  name_val_t* nvp   = util_bin_search(cond_code_map, numCondCodes, ccStr);
  return (nvp ? nvp->val : -1);
}

