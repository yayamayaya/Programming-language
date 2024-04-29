#ifndef CODEGEN
#define CODEGEN

#define _PUSH_NUM(num)          fprintf(asm_file, "push %lf\n", (double)num)
#define _PUSH_REG(reg)          fprintf(asm_file, "push %s\n", reg)
#define _PUSH_REL(addr)         fprintf(asm_file, "push [cx+%d]\n", addr)
#define _POP_REL(addr)          fprintf(asm_file, "pop [cx+%d]\n\n", addr)
#define _POP_REG(reg)           fprintf(asm_file, "pop %s\n", reg)
#define _LABEL(type, name)      fprintf(asm_file, "\n%c%p:\n", type, name)
#define _FUNC(name)             fprintf(asm_file, "%s:\n\n", name)
#define _CALL_FUNC(name)        fprintf(asm_file, "call %s\n\n", name)
#define _JE_LABEL(type, name)   fprintf(asm_file, "je %c%p\n\n", type, name)
#define _JMP_LABEL(type, name)  fprintf(asm_file, "jmp %c%p\n\n", type, name)
#define _RET()                  fprintf(asm_file, "ret\n\n")
#define _MOV_RAM(arg1, arg2)    fprintf(asm_file, "mov %s, %s\n", arg1, arg2)
#define _FREE_LCL_MEM(addr)     fprintf(asm_file, "mov, [cx+%d], 0\n", addr)
#define _IN()                   fprintf(asm_file, "in\n")

#define _CALL_MAIN()            fprintf(asm_file, "push cx\n");                                 \
                                fprintf(asm_file, "call Elbasy\n");                             \
                                fprintf(asm_file, "out\n");                                     \
                                fprintf(asm_file, "halt\n\n\n");                                


#endif