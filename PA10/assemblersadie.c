#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdarg.h>

#include "assembler.h"
#include "field.h"
#include "lc3.h"
#include "symbol.h"
#include "tokens.h"
#include "util.h"

/** Global variable containing the head of the linked list of structures */
static line_info_t* infoHead;

/** Global variable containing the tail of the linked list of structures */
static line_info_t* infoTail;

/** Global variable containing information about the current line */
static line_info_t* currInfo;

void asm_init_line_info (line_info_t* info) {
  if (info) {
    info->next        = NULL;
    info->lineNum     = srcLineNum;
    info->address     = currAddr;
    info->machineCode = 0;
    info->opcode      = OP_INVALID;
    info->form        = 0;
    info->reg1        = -1;
    info->reg2        = -1;
    info->reg3        = -1;
    info->immediate   = 0;
    info->reference   = NULL;
  }
}

void asm_print_line_info (line_info_t* info) {
  if (info) {
    printf("%3d: address: x%04x machineCode:%04x op: %s form: %d reg1:%3d, reg2:%3d reg3:%3d imm: %d ref: %s\n",
           info->lineNum, info->address, info->machineCode,
           lc3_get_opcode_name(info->opcode), info->form, info->reg1,
           info->reg2, info->reg3, info->immediate, info->reference);
  }
 }

/* based on code from http://www.eskimo.com/~scs/cclass/int/sx11c.html */
void asm_error (char* msg, ...) {
   numErrors++;
   va_list argp;
   fprintf(stderr, "ERROR %3d: ", srcLineNum);
   va_start(argp, msg);
   vfprintf(stderr, msg, argp);
   va_end(argp);
   fprintf(stderr, "\n");
}

void asm_init (void) {
  infoHead = infoTail = currInfo = NULL; 
  tokens_init();
  lc3_sym_tab = symbol_init(0); 
}

/** performs the first pass of the assembler */
void asm_pass_one (char* asm_file_name, char* sym_file_name) {


    // static declaration of char array
    char line[MAX_LINE_LENGTH];

    // open the file and report error if needed
    FILE* fp = open_read_or_error(asm_file_name);

    if(fp!=NULL){

    	// declare a variable for the token
    	//char* token = NULL;

    	// open the file
    	while(!feof(fp)){

	    // read the lines one at a time using fgets
	    fgets(line, sizeof(line), fp);

	    srcLineNum++;

	    // convert the line to a list of tokens
	    char* token = tokenize_line(line);
	    //print_tokens();

	    // if there are any tokens on the line
	    if(token != NULL){

	    	// allocate a new line_info_t, initialize it
	    	line_info_t* temp = malloc(sizeof(line_info_t));
	    	asm_init_line_info(temp);

	    	// store in currInfo
		currInfo = temp;


	    	// call check_line_syntax to set fields in currInfo
	    	check_line_syntax(token);

	        if(currInfo->opcode==OP_END)
	    	    break;

	    	// add currInfo to linked list
	    	if(infoHead == NULL){
		    infoHead = currInfo;
		    infoTail = currInfo;
	    	}	
	    	else{
	    	    infoTail -> next = currInfo;
	    	    infoTail = currInfo;
	    	} 

	        update_address();
		free(temp);
	    }


	    // open sym file
	    FILE* sym_file = open_write_or_error(sym_file_name);

	    if(sym_file!=NULL){
	        lc3_write_sym_table(sym_file);
	    }

    	}

    	fclose(fp);

    }
}

/** generates the object file */
void asm_pass_two (char* obj_file_name) {

    FILE *fp;
    fp = fopen(obj_file_name,"w");

    // start at the beginning of the linked list
    currInfo = infoHead;   

    // loop over linked list
    while(currInfo != NULL){
	
	srcLineNum = currInfo->lineNum;
	if(currInfo->opcode != OP_INVALID){

	    int opcode = currInfo->opcode;
	    if(opcode==OP_END)
	    	return;	
	    else if(opcode==OP_BLKW){
	    	for(int i = 0; i < currInfo->immediate; i++){
		    lc3_write_LC3_word(fp,currInfo->machineCode);
	    	}
	    }

	    else{

	    	// set the prototype
	    	LC3_inst_t* info = lc3_get_inst_info(opcode);
	    	currInfo->machineCode = info->forms[currInfo->form].prototype;

		// check for BR
		if(currInfo->opcode==OP_BR){
		    currInfo->machineCode = setField(currInfo->machineCode,11,9,currInfo->reg1);
		}
	    	// set the opcode
	    	//currInfo->machineCode = setField(currInfo->machineCode, 15, 12, opcode);
		
	    	// get list of operands corresponding to this opcode
	   	LC3_inst_t* listOfOperands = lc3_get_inst_info(opcode);
			
	    	// get the list of operands
	    	operands_t operands = listOfOperands->forms[currInfo->form].operands;

	    	// call encode operand for opcodes
 	    	for (operand_t op = FMT_R1; op <= FMT_STR; op <<= 1) {

    	             // check if operands is the same as op
    	            if(op & operands)
		    	encode_operand(op);
 	        }

	        lc3_write_LC3_word(fp,currInfo->machineCode);
	    }
	}
	    //asm_print_line_info(currInfo);
	    currInfo = currInfo->next;
    }

    fclose(fp);
}

/** @todo implement this function */
void asm_term (void) {

    currInfo = infoHead;
    while(currInfo!=NULL){
	free(currInfo->reference);
	free(currInfo);
	currInfo = currInfo->next;
    }

    symbol_term(lc3_sym_tab);
    tokens_term();
}

/** checks if a token is a label */
char* check_for_label (char* token) {

    // check for opcode
    int check = util_get_opcode(token);

    // if not an opcode, check for label
    if(check == -1){
	int check2 = util_is_valid_label(token);
	if(check2 == 0){
	    asm_error(ERR_BAD_LABEL, token);
	    return next_token();
	}
	else{
	    if(symbol_find_by_name(lc3_sym_tab,token)!=NULL)
		asm_error(ERR_DUPLICATE_LABEL,token);	
	    symbol_add(lc3_sym_tab, strdup(token), currInfo->address);
	    return next_token();
	}	  
    }

    else
	return token;
}

/** check the syntax of a source line */
void check_line_syntax (char* token) {

    // determine if the first token is a label
    char* test = check_for_label(token);

    // if the line is just a label, return
    if(test == NULL){
	return;
    }

    // else, make sure this is an opcode
    else{
	int check = util_get_opcode(test);
	if(check == -1)
	    asm_error(ERR_MISSING_OP, token);
	
	else{
	    // convert token to an opcode and store in data structure
	    int opcode = util_get_opcode(test);
	    currInfo->opcode = opcode;

	    // Case for .END
	    if(opcode==OP_END)
		return;

	    // Case for RET
	    if(strcmp(token,"RET")==0)
		return;

	    // Case for BR
	    if(opcode==OP_BR){
		int reg = util_parse_cond(token + 2);
		currInfo->reg1 = reg;
	    }
		
	    // get list of operands corresponding to this opcode
	    LC3_inst_t* listOfOperands = lc3_get_inst_info(opcode);

	    // find the form
	    int form;

	    if(listOfOperands->formBit != -1){
		if(strcmp(listOfOperands->forms[0].name, token) == 0){
		    form = 0;
		    currInfo->form = form;
		}
		else if(strcmp(listOfOperands->forms[1].name, token) == 0){
		    form = 1;
		    currInfo->form = form;
		}
	    }
	    // if there are operands for the opcode, call scan_operands
	    scan_operands((listOfOperands->forms[form]).operands);
	}
    }   
}

/** take a field from currInfo and place it in machineCode field */
void encode_operand (operand_t operand) {
  switch (operand) {
    case FMT_R1:
	currInfo->machineCode = setField(currInfo->machineCode,11,9,currInfo->reg1);
	break;
    case FMT_R2:
	currInfo->machineCode = setField(currInfo->machineCode,8,6,currInfo->reg2);
	break;
    case FMT_R3:
    case FMT_IMM5: {
	if(currInfo->reg3 != -1)
	    currInfo->machineCode = setField(currInfo->machineCode,2,0,currInfo->reg3);
	else  
	    currInfo->machineCode = setField(currInfo->machineCode,4,0,currInfo->immediate);
    }
	break;
    case FMT_IMM6:
	currInfo->machineCode = setField(currInfo->machineCode,5,0,currInfo->immediate);
	break;
    case FMT_VEC8:
	currInfo->machineCode = setField(currInfo->machineCode,7,0,currInfo->immediate);
	break;
    case FMT_PCO9:
	encode_PC_offset_or_error(9);
	break;
    case FMT_PCO11:
	encode_PC_offset_or_error(11);
	break;
    case FMT_IMM16:
	currInfo->machineCode = setField(currInfo->machineCode,15,0,currInfo->immediate);
	break;
    default:
        break;
  }

}

/** convert the reference into a PC offset */
void encode_PC_offset_or_error (int width) {
    symbol_t* symbol = symbol_find_by_name(lc3_sym_tab, currInfo->reference);
    if(symbol == NULL){
	asm_error(ERR_MISSING_LABEL, currInfo->reference);
	return;
    }
    else{
	int pcOffset = symbol->addr - currInfo->address - 1;
	int fits = fieldFits(pcOffset,width,1);
	if(fits==0){
	    asm_error(ERR_IMM_TOO_BIG, currInfo->reference);
	    return;
	}
	currInfo->machineCode = setField(currInfo->machineCode,width-1,0,pcOffset);
    }	
}

/** make sure the next token is a comma */
void get_comma_or_error (void) {
    if(strcmp(next_token(), ",") != 0)
	asm_error(ERR_EXPECTED_COMMA, next_token());
}

/** convert token to an immediate value */
void get_immediate_or_error (char* token, int width, int isSigned) {
    int value;
    int valid = lc3_get_int(token, &value);
    if(valid == 0)
	asm_error(ERR_BAD_IMM, token);
    else{
	int fits = fieldFits(value, width, isSigned);
	if(fits == 0)
	    asm_error(ERR_IMM_TOO_BIG, token);
	else
	    currInfo->immediate = value;
    }
}
/** get label reference and store in currInfo */
void get_PC_offset_or_error (char* token) {
    int check = util_is_valid_label(token);
    if(check == 0)
	asm_error(ERR_BAD_LABEL, token);
    else
	currInfo->reference = strdup(token);
}

/** determine if string represents a valid register */
int get_reg_or_error (char *token) {
    int check = util_get_reg(token);
    if(check == -1){
	asm_error(ERR_EXPECTED_REG, token);
	return -1;
    }
    else
	return check;
}

/** opens a file for reading */
FILE *open_read_or_error (char* file_name) {

    // open the file
    FILE* fp = fopen(file_name, "r");

    // throw error if necessary
    if(fp == NULL){
	asm_error(ERR_OPEN_READ, file_name);
    }

    // return file pointer
    return fp;
}

/** opens a file for writing */
FILE *open_write_or_error (char* file_name) {
    
    // open the file
    FILE* fp = fopen(file_name, "w");

    // throw error if necessary
    if(fp == NULL){
	asm_error(ERR_OPEN_WRITE, file_name);
    }

    // return file pointer
    return fp;
}

/** converts the token to a value and stores it in currInfo */
void get_operand (operand_t operand, char* token) {
  switch (operand) {
    case FMT_R1:
	currInfo->reg1 = get_reg_or_error(token);
	break;
    case FMT_R2:
	currInfo->reg2 = get_reg_or_error(token);
	break;
    case FMT_R3:
    case FMT_IMM5: {
	int isReg = util_get_reg(token);
	if(isReg != -1){
	    currInfo->reg3 = isReg;
	    currInfo->form = 0;
	}
	else{
    	    int value;
    	    int valid = lc3_get_int(token, &value);
   	    if(valid == 0)
	        asm_error(ERR_BAD_IMM, token);
    	    else{
		int fits = fieldFits(value, 5, 1);
		if(fits == 0)
	    	    asm_error(ERR_IMM_TOO_BIG, token);
		else{
	    	    currInfo->immediate = value;
		    currInfo->form = 1;
		}
	    }
   	}
    } 
	break;
    case FMT_IMM6:
	get_immediate_or_error(token, 6, 1);	
	break;
    case FMT_VEC8:
	get_immediate_or_error(token, 8, 0);
	break;
    case FMT_PCO9:
    case FMT_PCO11:
	get_PC_offset_or_error(token);
	break;
    case FMT_IMM16:{
	if(currInfo->opcode==OP_ORIG)
	    get_immediate_or_error(token, 16, 0);
	else
	    get_immediate_or_error(token, 16, 1);
    }
	break;
    case FMT_STR:
	currInfo->reference = strdup(token);
	break;
    default:
        break;
  }
}

// can use ints instead of unsigned ints as long as v is never negative. This
// is true for the use in LC3 assembler.
// see http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetKernighan
int count_bits (int v) {

    unsigned int c;
    for(c = 0; v; c++){
	v &= v-1;
    }
  
    return c;
}

/** scan all the operands of an LC3 instruction */
void scan_operands (operands_t operands) {
  int operandCount = 0;
  int numOperands  = count_bits(operands);
  int errorCount   = numErrors;

  for (operand_t op = FMT_R1; op <= FMT_STR; op <<= 1) {
    if (errorCount != numErrors)
      return; // error, so skip processing remainder of line

    // check if operands is the same as op
    if(op & operands){
	char* t = next_token();
	get_operand(op, t);
	operandCount = operandCount + 1;
	if(operandCount < numOperands){
	    get_comma_or_error();
	}
	if(operandCount > numOperands)
	    asm_error(ERR_EXTRA_OPERAND, t);
    }
  }
}

/** update the current address */
void update_address (void) {

    if(currInfo->opcode==OP_ORIG){
	currInfo->address = currInfo->immediate;
	currAddr = currInfo->immediate;
    }
    else if(currInfo->opcode==OP_BLKW){
	currAddr += currInfo->immediate;
    }
    else if(currInfo->opcode==OP_INVALID){
	return;
    }
    else{
        currAddr++;
    }
}
