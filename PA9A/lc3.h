#ifndef __LC3_H__
#define __LC3_H__

/*
 * "Copyright (c) 2012 by Fritz Sieker."
 * based on ideas/code by Steven S. Lumetta
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

/** @file lc3.h
 *  @brief definitions of the LC3 instruction set architecture (ISA) (do not modify)
 *  @details This defines the details of the LC3 instruction set architecture
 *  (ISA). It is a separate file so that it can be shared by both an assembler
 *  and a simulator.
 * <p>
 * @author Fritz Sieker
 */

#include <stdio.h>

/** A handy way of declaring global variables in a header file */
#ifndef LC3_VAR
#define LC3_VAR extern
#endif

#include "symbol.h" 

/** prototype for handy function to duplicate a string */
char* strdup(char *);

/** LC3 words are 16 bits */
#define LC3_WORD unsigned short

/** The LC3 defines a memory accessed by a 16 bit address */
#define LC3_MEM_SIZE 65536

/** The LC3 contains 8 general purpose register, named R0..R7 */
#define LC3_NUM_REGS 8  

/** Return address stored in R7 */
#define RETURN_ADDR_REG   7

/** Global flag defining whether using .obj file or .hex file */
LC3_VAR int inHex;

/** Global variable holding symbol table */
LC3_VAR sym_table_t* lc3_sym_tab;

/** The LC3 opcodes and pseudo-ops. The codes of OP_BR .. OP_TRAP corresponds
 *  exactly to the numeric values assigned to the 16 LC3 instructions. The
 *  codes assigned to the pseudo-ops is arbitrary. PCi is the incremented PC
 */
typedef enum opcode {
  OP_INVALID = -1, /**< -1: invalid opcode                                    */
  OP_BR,           /**<  0: PC = PCi + PCoffset9 if condition is met          */
  OP_ADD,          /**<  1: DR = SR1 + SR2 or DR = SR1 + imm5                 */
  OP_LD,           /**<  2: DR = mem[PCi + PCoffset9]                         */
  OP_ST,           /**<  3: mem[PCi + PCoffset9] = SR                         */
  OP_JSR_JSRR,     /**<  4: R7 = PCi and (PC = SR or PC = PCi + PCoffest9)    */
  OP_AND,          /**<  5: DR = SR1 & SR2                                    */
  OP_LDR,          /**<  6: DR = mem[BaseR + offset6]                         */
  OP_STR,          /**<  7: mem[BaseR + offset6] = SR                         */
  OP_RTI,          /**<  8: PC = R7, exit supervisor mode                     */
  OP_NOT,          /**<  9: DR = ~SR1                                         */
  OP_LDI,          /**< 10: DR = mem[mem[PCi + PCoffset9]]                    */
  OP_STI,          /**< 11: mem[mem[PCi + offset9]] = SR                      */
  OP_JMP_RET,      /**< 12: PC = R7 (RET) or PC = Rx (JMP Rx)                 */
  OP_RESERVED,     /**< 13: Currently not used                                */
  OP_LEA,          /**< 14: DR = PCi + PCoffset9                              */
  OP_TRAP,         /**< 15: R7 = PCi, PC = mem[mem[trap]], enter sys mode     */
  OP_ORIG,         /**< 16: memory location where code is loaded              */
  OP_END,          /**< 17: end of propgram - only comments may follow        */
  OP_BLKW,         /**< 18: allocate N words of storage initialized with 0    */
  OP_FILL,         /**< 19: allocate 1 word of storage initialed with operand */
  OP_STRINGZ,      /**< 20: allocate N+1 words of storage initialized with
                            string and null terminator (1 char per word)      */
  OP_GETC,         /**< 21: Read character from keyboard, no echo (trap x20)  */
  OP_OUT,          /**< 22: Write one character                   (trap x21)  */
  OP_PUTS,         /**< 23: Write null terminated string          (trap x22)  */
  OP_IN,           /**< 24: Print prompt and read/echo character  (trap x23)  */
  OP_PUTSP,        /**< 25: Write packed null terminated string   (trap x24)  */
  OP_HALT,         /**< 26: Halt execution                        (trap x25)  */
  OP_GETS,         /**< 27: Read string from kbd, store in memory (trap x26)  */
  OP_ZERO,         /**< 28: DR = 0 (AND DR,DR,#0)                             */
  OP_COPY,         /**< 29: DR = SR1 (ADD DR,SR1,#0)                          */
  OP_NEG,          /**< 30: DR = -DR (NOT DR,DR and ADD DR,DR,#1)             */
  NUM_OPCODES      /**< 31: Initialized by compiler                           */
} opcode_t;

/** A bit field used to define the types of operands an individual
 *  LC3 instruction may have. Each value represents a different bit in
 *  the final result. When you see C code like this, it is likely that
 *  an integer value is used to represent an "array" of up to 32 boolean
 *  values. Each value is accessed with a mask that extracts the bit of
 *  interest. See the inst_format_t below.
 */
typedef enum operand {
    FMT_R1    = 0x001, /**< DR or SR                      */
    FMT_R2    = 0x002, /**< SR1 or BaseR                  */
    FMT_R3    = 0x004, /**< SR2                           */
    FMT_CC    = 0x008, /**< condition codes               */
    FMT_IMM5  = 0x010, /**< imm5                          */
    FMT_IMM6  = 0x020, /**< offset6                       */
    FMT_VEC8  = 0x040, /**< trapvect8                     */
    FMT_ASC8  = 0x080, /**< 8-bit ASCII                   */
    FMT_IMM9  = 0x100, /**< label (or address from imm9)  */
    FMT_IMM11 = 0x200, /**< label (or address from imm11) */
    FMT_IMM16 = 0x400, /**< full instruction in hex       */
    FMT_STR   = 0x800  /**< operand is string literal     */
} operand_t;

/** Define a combinatin of operands an opcode may have. For example, the
 *  the BR, LD, LDI, ST and STI instructions all have two parameters. The
 *  first is a register, the second is a nine bit offset. This form stores
 *  multiple boolean values in a single integer values by using individual
 *  bits to encode information.
 */
typedef enum operands {
  FMT_      = 0,
  FMT_RRR   = (FMT_R1 | FMT_R2 | FMT_R3),
  FMT_RRI5  = (FMT_R1 | FMT_R2 | FMT_IMM5),
  FMT_CL    = (FMT_CC | FMT_IMM9),
  FMT_R     = FMT_R2,
  FMT_I11   = FMT_IMM11,
  FMT_RL    = (FMT_R1 | FMT_IMM9),
  FMT_RRI6  = (FMT_R1 | FMT_R2 | FMT_IMM6),
  FMT_RR    = (FMT_R1 | FMT_R2),
  FMT_V     = FMT_VEC8,
  FMT_A     = FMT_ASC8,
  FMT_16    = FMT_IMM16
} operands_t;

/** This structure stores the information for one form of an instruction.
 *  Several instructions have multiple forms, but most have only one.
 */
typedef struct inst_format {
 char*      name;      /**< human readable name (e.g. "ADD")           */
 operands_t operands;  /**< operands for this operation (e.g. FMT_RRR) */
 int        prototype; /**< bits that are constant in this instruction */
} inst_format_t;

/** This structure stores the information about a single instruction.
 *  See the usage in lc3.c where the information for each LC3 instruction
 *  is defined.
 */
typedef struct LC3_inst {
  int           formBit;    /**< -1 if instruction has only one format,
                                 else bit that differentiates format.   */
  inst_format_t forms[2];   /**< syntax for both possible forms         */
} LC3_inst_t;

/** Get the information for an instruction, given its opcode This is simply
 *  an access into an array of values initialized with the information for
 *  each of the LC3's sixteen instructions and additional pseudo-ops.
 *  @param opcode - the opcode of the instruction/pseudo-op of interest
 *  @return - a pointer to the sytax information for this instruction or NULL
 */
LC3_inst_t* lc3_get_inst_info (opcode_t opcode);

/** Read an LC3 word (16 bits) from a file and return the value
 *  @param f - the object file
 *  @return the value, or -1 on EOF
 */
int lc3_read_LC3_word (FILE *f);

/** Write an LC3 word to a file in binary or hex
 *  @param f - the file to write to
 *  @param val - the value to write
 */
void lc3_write_LC3_word (FILE* f, int val);

#endif /* __LC3_H__ */

