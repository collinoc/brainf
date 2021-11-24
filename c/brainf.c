#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interpreter.h"

char*   read_file(char*);
void    interpret(char*);
int     verify_head(char*, char*);
int     verify_code(char*, int);
void    do_jump(char*, Interpreter*);
void    do_ret(char*, Interpreter*);

int main(int argc, char** argv)
{
    if (argc != 2) {
        printf("Invalid argument count %d. "
               "Please supply a file path.\n", argc);
        return 1;
    }

    char* code = read_file(argv[1]);

    interpret(code);

    free(code);

    return 0;
}

void interpret(char* code)
{
    int code_len = strlen(code);

    Interpreter* interpreter = malloc(sizeof(Interpreter));
    if (interpreter == NULL) {
        printf("Allocation failed.\n");
        exit(1);
    }

    interpreter->tape = calloc(TAPE_SIZE, sizeof(char));
    if (interpreter-> tape == NULL) { 
        printf("Allocation failed.\n");
        exit(1); 
    }
    
    interpreter->head = interpreter->tape;

    for (interpreter->ip = 0; interpreter->ip < code_len; interpreter->ip++)
    {
        switch (code[interpreter->ip])
        {
            case MOV_R:
                verify_head(interpreter->tape, interpreter->head + 1);
                interpreter->head++;
                break;

            case MOV_L:
                verify_head(interpreter->tape, interpreter->head - 1);
                interpreter->head--;
                break;
            
            case INC:
                *interpreter->head += 1;
                break;

            case DEC:
                *interpreter->head -= 1;
                break;

            case OUT:
                printf("%c", (char) *interpreter->head);
                break;

            case IN:
                *interpreter->head = getchar();
                break;

            case JUMP:
                do_jump(code, interpreter);
                break;

            case RET:
                do_ret(code, interpreter);
                break;

            default: break;
        }
    }

    free(interpreter->tape);
    free(interpreter);
}

void do_jump(char* code, Interpreter* itpr)
{
    if (!*itpr->head) 
    {
        int loop_depth = 1;

        while (loop_depth) 
        {            
            itpr->ip++;
            verify_code(code, itpr->ip);

            if (code[itpr->ip] == '[')
                loop_depth++;
            else if (code[itpr->ip] == ']')
                loop_depth--;
        }
    }
}

void do_ret(char* code, Interpreter* itpr)
{
    if (*itpr->head)
    {
        int loop_depth = 1;

        while (loop_depth)
        {
            itpr->ip--;
            verify_code(code, itpr->ip);

            if (code[itpr->ip] == ']')
                loop_depth++;
            else if (code[itpr->ip] == '[')
                loop_depth--;
        }
    }
}

int verify_code(char* code, int ip)
{
    if (ip < 0 || ip >= strlen(code)) 
    {
        printf("Code out of bounds!\n");
        exit(1);
    }

    return 1;
}

int verify_head(char* tape, char* head)
{
    if (head < tape || head >= (tape + TAPE_SIZE)) 
    {
        printf("Execution out of bounds!\n");
        exit(1);
    }

    return 1;
}

char* read_file(char* filename)
{
    FILE* brainf_fl = fopen(filename, "r");
    
    fseek(brainf_fl, 0, SEEK_END);

    int end = ftell(brainf_fl);
    fseek(brainf_fl, 0, SEEK_SET);

    char* file_contents = malloc(end + 1);
    fread(file_contents, 1, end, brainf_fl);
    file_contents[end] = '\0'; // Null terminated

    fclose(brainf_fl);

    return file_contents;
}