from sys import stdin, argv

TAPE_LEN = 1024

def verify_code(code, pc):
    if  pc < 0 or pc >= len(code):
        print("Code out of bounds!")
        exit(1)

def verify_head(tape, head):
    if head < 0 or head >= len(tape):
        print("Execution out of bounds!")
        exit(1)

def interpret(code: str):
    tape = [0] * TAPE_LEN   # Tape
    head = 0                # Tape head
    pc = 0                  # Program counter

    def do_jump():
        nonlocal code, tape, head, pc

        if tape[head] == 0:
            loop_depth = 1

            while loop_depth:
                pc += 1
                verify_code(code, pc)
                
                if code[pc] == '[':
                    loop_depth += 1
                elif code[pc] == ']':
                    loop_depth -= 1

    def do_ret():
        nonlocal code, tape, head, pc

        if tape[head] != 0:
            loop_depth = 1
                
            while loop_depth:
                pc -= 1
                verify_code(code, pc)

                if code[pc] == ']':
                    loop_depth += 1
                elif code[pc] == '[':
                    loop_depth -= 1

    def eval():
        nonlocal code, tape, head, pc

        if code[pc] == '+':
            tape[head] += 1
        elif code[pc] == '-':
            tape[head] -= 1
        elif code[pc] == '>':
            head += 1
            verify_head(tape, head)
        elif code[pc] == '<':
            head -= 1
            verify_head(tape, head)
        elif code[pc] == '.':
            print(chr(tape[head]), end="")
        elif code[pc] == ',':
            tape[head] = ord(stdin.read(1))
        elif code[pc] == '[':
            do_jump()
        elif code[pc] == ']':
            do_ret()

    # Main run loop
    while True:
        if pc >= len(code):
            break

        eval()

        pc += 1

if __name__ == "__main__":
    if len(argv) != 2:
        print(f"Invalid argument count {len(argv)}. Please supply a file path.")
        exit(1)

    with open(argv[1], "r") as file:
        code = file.read()
        interpret(code)