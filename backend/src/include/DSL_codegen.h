#ifndef CODEGEN
#define CODEGEN

#define _PR(...)                fprintf(asm_file, __VA_ARGS__)

#define _PUSH_NUM(num)          _PR("push %lf\n", (double)num)
#define _PUSH_REG(reg)          _PR("push %s\n", reg)
#define _PUSH_REL(addr)         _PR("push [cx+%d]\n", addr)
#define _PUSH_MEM(addr)         _PR("push [%d]\n", addr)
#define _POP_REL(addr)          _PR("pop [cx+%d]\n\n", addr)
#define _POP_REG(reg)           _PR("pop %s\n", reg)
#define _POP_MEM(addr)              _PR("pop [%d]\n\n", addr)
#define _LABEL(type, name)      _PR("\n%c%p:\n", type, name)
#define _FUNC(name)             _PR("%s:\n\n", name)
#define _CALL_FUNC(name)        _PR("call %s\n\n", name)
#define _JE_LABEL(type, name)   _PR("je %c%p\n\n", type, name)
#define _JMP_LABEL(type, name)  _PR("jmp %c%p\n\n", type, name)
#define _RET()                  _PR("ret\n\n")
#define _MOV(arg1, arg2)        _PR("mov %s, %s\n", arg1, arg2)
#define _FREE_LCL_MEM(addr)     _PR("mov, [cx+%d], 0\n", addr)
#define _IN()                   _PR("in\n")

#define _CALL_MAIN()            _PR("push cx\n");                                 \
                                _PR("call Elbasy\n");                             \
                                _PR("out\n");                                     \
                                _PR("halt\n\n\n");                                


#endif