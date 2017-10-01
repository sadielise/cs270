              .ORIG  x3000
              ADD    R3,R2,R3
              ADD    R3,R2,R3
              ADD    R3,R2,R3
              AND    R4,R5,R4
              AND    R4,R5,R4
              AND    R4,R5,R4
              NOT    R7,R4,
              TRAP   -1
              JMP    R4
              JSR    
              RET    
              RTI    
              TRAP   -1
              LDR    R2,R7,#10
              STR    R2,R7,#-8
              BRnzp  
              BRn    
              BRz    
              BRp    
              BRnz   
              BRnp   
              BRzp   
              BRnzp  
              JSRR   R0
              LD     R2,
              LDI    R3,
              LEA    R4,
              ST     R5,
              STI    R6,
              TRAP   -1
              TRAP   -1
              TRAP   -1
              TRAP   -1
              .BLKW  x0003
              ADD    R0,R4,R0
              ADD    R0,R4,R0
              ADD    R0,R4,R0
              ADD    R0,R4,R0
              ADD    R0,R4,R0
              .END                      