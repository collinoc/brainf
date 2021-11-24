#ifndef BRAINF_INSTR
#define BRAINF_INSTR

#define TAPE_SIZE 1024 * 1

#define MOV_R '>'
#define MOV_L '<'
#define INC   '+'
#define DEC   '-'
#define OUT   '.'
#define IN    ','
#define JUMP  '['
#define RET   ']'

typedef struct {
    char* tape;
    char* head;
    int ip;
} Interpreter;

#endif