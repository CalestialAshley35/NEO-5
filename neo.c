#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>

#define RAM_SIZE 8192  // 8KB for 13-bit CPU
#define MAX_FILES 100
#define MAX_FILENAME 100
#define MAX_CONTENT 4096

// Memory and Registers
uint16_t RAM[RAM_SIZE]; // 13-bit addressable memory
uint16_t REG[8];        // 8 General-purpose 13-bit registers
uint16_t PC = 0;        // Program Counter
uint16_t IR = 0;        // Instruction Register

// Function Prototypes
void execute_instruction(uint16_t instruction);
void load_program(const char *filename);
void shell();

// File system simulation
void create_file(const char *filename);
void view_file(const char *filename);
void delete_file(const char *filename);
void list_files();
void change_directory(const char *dirname);

// Math command
void process_math_command(const char *expression);

// Utility functions
void print_registers();
void print_ram();

int main() {
    printf("Welcome to the Advanced 13-bit CPU Emulator with Math Support!\n");
    shell();
    return 0;
}

// CPU Instruction Set Execution
void execute_instruction(uint16_t instruction) {
    uint16_t opcode = (instruction & 0xF000) >> 12; // Top 4 bits for opcode
    uint16_t reg1 = (instruction & 0x0F00) >> 8;    // Next 4 bits for register 1
    uint16_t reg2 = (instruction & 0x00F0) >> 4;    // Next 4 bits for register 2
    uint16_t value = (instruction & 0x00FF);        // Immediate value or address

    switch (opcode) {
        case 0x1: // LOAD: Load immediate value into a register
            REG[reg1] = value;
            break;
        case 0x2: // ADD: Add two registers and store in reg1
            REG[reg1] = REG[reg1] + REG[reg2];
            break;
        case 0x3: // STORE: Store register value into RAM
            RAM[value] = REG[reg1];
            break;
        case 0x4: // JUMP: Jump to memory address
            PC = value;
            break;
        case 0xF: // HALT: Stop execution
            printf("HALT: CPU Stopped.\n");
            exit(0);
            break;
        default:
            printf("Unknown instruction: 0x%04X\n", instruction);
    }
    PC++;
}

// Load Assembly Program into RAM
void load_program(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Unable to open program file '%s'.\n", filename);
        return;
    }

    uint16_t address = 0;
    uint16_t instruction;
    while (fscanf(file, "%hx", &instruction) != EOF) {
        RAM[address++] = instruction;
    }
    fclose(file);
    printf("Program '%s' loaded successfully.\n", filename);
}

// Math Command Processor
void process_math_command(const char *expression) {
    char op;
    int num1, num2, result;

    if (sscanf(expression, "%d %c %d", &num1, &op, &num2) == 3) {
        switch (op) {
            case '+':
                result = num1 + num2;
                break;
            case '-':
                result = num1 - num2;
                break;
            case '*':
                result = num1 * num2;
                break;
            case '/':
                if (num2 == 0) {
                    printf("Error: Division by zero.\n");
                    return;
                }
                result = num1 / num2;
                break;
            case '%':
                if (num2 == 0) {
                    printf("Error: Division by zero.\n");
                    return;
                }
                result = num1 % num2;
                break;
            default:
                printf("Error: Unsupported operator '%c'.\n", op);
                return;
        }
        printf("Result: %d\n", result);
    } else {
        printf("Error: Invalid expression.\n");
    }
}

// Interactive Shell
void shell() {
    char command[256];
    char arg1[128], arg2[128];

    while (1) {
        printf("CPU$ ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0; // Remove newline

        if (sscanf(command, "run %s", arg1) == 1) {
            load_program(arg1);
            while (RAM[PC] != 0xFFFF) { // 0xFFFF as End of Program
                IR = RAM[PC];
                execute_instruction(IR);
            }
        } else if (sscanf(command, "create %s", arg1) == 1) {
            create_file(arg1);
        } else if (sscanf(command, "view %s", arg1) == 1) {
            view_file(arg1);
        } else if (sscanf(command, "delete %s", arg1) == 1) {
            delete_file(arg1);
        } else if (strcmp(command, "list") == 0) {
            list_files();
        } else if (sscanf(command, "cd %s", arg1) == 1) {
            change_directory(arg1);
        } else if (strncmp(command, "math", 4) == 0) {
            process_math_command(command + 5);
        } else if (strcmp(command, "exit") == 0) {
            break;
        } else if (strcmp(command, "print regs") == 0) {
            print_registers();
        } else if (strcmp(command, "print ram") == 0) {
            print_ram();
        } else {
            printf("Unknown command: '%s'\n", command);
        }
    }
}

// File System Commands
void create_file(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error: Could not create file '%s'.\n", filename);
        return;
    }
    printf("Enter content (end with CTRL+D):\n");
    char buffer[MAX_CONTENT];
    while (fgets(buffer, sizeof(buffer), stdin)) {
        fputs(buffer, file);
    }
    fclose(file);
    printf("File '%s' created successfully.\n", filename);
}

void view_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file '%s'.\n", filename);
        return;
    }
    char buffer[MAX_CONTENT];
    printf("Content of '%s':\n", filename);
    while (fgets(buffer, sizeof(buffer), file)) {
        printf("%s", buffer);
    }
    fclose(file);
}

void delete_file(const char *filename) {
    if (remove(filename) == 0) {
        printf("File '%s' deleted successfully.\n", filename);
    } else {
        printf("Error: Could not delete file '%s'.\n", filename);
    }
}

void list_files() {
    DIR *dir = opendir(".");
    if (!dir) {
        printf("Error: Could not open current directory.\n");
        return;
    }
    struct dirent *entry;
    printf("Files in current directory:\n");
    while ((entry = readdir(dir)) != NULL) {
        printf("- %s\n", entry->d_name);
    }
    closedir(dir);
}

void change_directory(const char *dirname) {
    if (chdir(dirname) == 0) {
        printf("Directory changed to '%s'.\n", dirname);
    } else {
        printf("Error: Could not change to directory '%s'.\n", dirname);
    }
}

// Utility Functions
void print_registers() {
    printf("Registers:\n");
    for (int i = 0; i < 8; i++) {
        printf("R%d: 0x%04X\n", i, REG[i]);
    }
}

void print_ram() {
    printf("RAM Dump (First 16 Bytes):\n");
    for (int i = 0; i < 16; i++) {
        printf("0x%04X ", RAM[i]);
        if ((i + 1) % 8 == 0) printf("\n");
    }
}
