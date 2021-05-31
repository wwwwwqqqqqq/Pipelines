//
//  siavm.c
//  Assignment4
//
//  Created by 秦玮蔚 on 2020/6/12.
//  Copyright © 2020 Qin Weiwei. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

//const int MEMORY = 1024;
//unsigned char memory[1024];
/*
unsigned char *memory;
int registers[16];
unsigned int PC = 0;

int OP1,OP2;
int result;
unsigned char *currentInstruction;
*/

bool ifFlush;
bool ifFetch = false;
bool ifDecode = false;
bool ifExecute = false;
bool ifStore = true;

//struct a processor where all 4 components (fetch, decode, execute and store) are running at the same time
struct processor {
    unsigned char *memory;
    int registers[16];
    int PC;
    
    int decodeOP1,decodeOP2;
    int executeOP1,executeOP2;
    int storeOP1,storeOP2;

    int executeResult,storeResult;
    int currentFetch,currentDecode,currentExecute,currentStore;
    unsigned char currentInstructionFetch[4];
    unsigned char currentInstructionDecode[4];
    unsigned char currentInstructionExecute[4];
    unsigned char currentInstructionStore[4];
}processor;

//add
void ADD() {
    printf("add");
    processor.executeResult = (int)processor.registers[processor.executeOP1] + (int)processor.registers[processor.executeOP2 >> 4];
}

//and
void AND() {
    printf("and");
    processor.executeResult = (int)processor.registers[processor.executeOP1] & (int)processor.registers[processor.executeOP2 >> 4];
}

//divide
void DIVIDE() {
    printf("divide");
    processor.executeResult = (int)processor.registers[processor.executeOP1] / (int)processor.registers[processor.executeOP2 >> 4];
}

//multiply
void MULTIPLY() {
    printf("multiply");
    processor.executeResult = (int)processor.registers[processor.executeOP1] * (int)processor.registers[processor.executeOP2 >> 4];
}

//substract
void SUBSTRACT() {
    printf("substract");
    processor.executeResult = (int)processor.registers[processor.executeOP1] - (int)processor.registers[processor.executeOP2 >> 4];
}

//or
void OR() {
    printf("or");
    processor.executeResult = (int)processor.registers[processor.executeOP1] | (int)processor.registers[processor.executeOP2 >> 4];
}

//add immediate
void ADDIMMEDIATE() {
    printf("add immediate");
    processor.executeResult = (int)processor.registers[processor.executeOP1] + processor.executeOP2;
}

//brach if less
void BRACHIFLESS() {
    printf("brach if less");
    if((int)processor.registers[processor.executeOP1 / 16] < (int)processor.registers[processor.executeOP1 % 16]) {
            processor.executeResult = processor.executeOP2;
            processor.executeResult = processor.executeResult * 2;
        }else {
            processor.executeResult = 1;
        }
}

//brach if equal
void BRACHIFEQUAL() {
    printf("brach if equal");
    if((int)processor.registers[processor.executeOP1 / 16] == (int)processor.registers[processor.executeOP1 % 16]) {
            processor.executeResult = processor.executeOP2;
            processor.executeResult = processor.executeResult * 2;
        }else {
            processor.executeResult = 1;
        }
}

//jump
void JUMP() {
    printf("jump");
    processor.executeResult = processor.executeOP1;
    processor.executeResult = processor.executeResult << 16;
    processor.executeResult |= processor.executeOP2;
    processor.executeResult = processor.executeResult * 2;
}

//call
void CALL() {
    printf("call");
    processor.executeResult = processor.executeOP1;
    processor.executeResult = processor.executeResult << 16;
    processor.executeResult |= processor.executeOP2;
    processor.executeResult = processor.executeResult * 2;
    processor.registers[15] = (int)processor.registers[15] - 4;
    int i = 0;
    for(int j = 3;j >= 0;j --) {
        processor.memory[(int)processor.registers[15] + i] = (processor.PC >> (j * 8));
        i ++;
    }
    i = 0;
}

//load
void LOAD() {
    printf("load");
    processor.executeResult = 0;
    for(int i = 0;i < 4;i ++) {
        processor.executeResult |= processor.memory[((int)processor.registers[(processor.executeOP1 % 16)] + (2 * processor.executeOP2)) + i];
        if(i < 3) {
            processor.executeResult = processor.executeResult << 8;
        }
    }
}

//store
void STORE() {
    printf("store");
    processor.executeResult = (int)processor.registers[processor.executeOP1 / 16];
}

//flush the pipe
void flush() {
    for(int i = 0;i < 4;i ++) {
        processor.currentInstructionDecode[1] = 0;
        processor.currentInstructionExecute[1] = 1;
        processor.currentInstructionStore[1] = 2;
    }
    processor.decodeOP1 = 0;
    processor.decodeOP2 = 0;
    processor.executeOP1 = 0;
    processor.executeOP2 = 0;
    processor.storeOP1 = 0;
    processor.storeOP2 = 0;
    
    processor.executeResult = 0;
    processor.storeResult = 0;
    
    processor.currentDecode = 0;
    processor.currentExecute = 0;
    processor.currentStore = 0;
    
    ifFlush = true;
    ifFetch = false;
    ifDecode = false;
    ifExecute = false;
    ifStore = true;
}

void fetch() {
    /*
    currentInstruction = calloc(4,sizeof(unsigned char));

    for(int i = 0;i < 2;i ++) {
      currentInstruction[i] = memory[PC + i];
    }
    if(currentInstruction[0] == 0){
      free(currentInstruction);
      currentInstruction = NULL;
    }
     */
    if(ifStore != true) {
        return;
    }
    processor.currentInstructionFetch[0] = processor.memory[processor.PC];
    processor.currentFetch = (int)processor.currentInstructionFetch[0];
    processor.currentFetch = processor.currentFetch / 16;
    
    /*
    switch(Op) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
            currentInstruction[1] = memory[PC + 1];
            PC += 2;
        case 10:
        case 11:
        case 12:
        case 13:
            currentInstruction[1] = memory[PC + 1];
            currentInstruction[2] = memory[PC + 2];
            currentInstruction[3] = memory[PC + 3];
            PC += 4;
        case 14:
        case 15:
            currentInstruction[1] = memory[PC + 1];
            PC += 2;
    }
     */
    
    if(processor.currentFetch >= 14 || processor.currentFetch <= 9) {
        processor.currentInstructionFetch[1] = processor.memory[processor.PC + 1];
        processor.PC += 2;
    }
    
    if(processor.currentFetch == 10 || processor.currentFetch == 11 || processor.currentFetch == 12 || processor.currentFetch == 13) {
        processor.currentInstructionFetch[1] = processor.memory[processor.PC + 1];
        processor.currentInstructionFetch[2] = processor.memory[processor.PC + 2];
        processor.currentInstructionFetch[3] = processor.memory[processor.PC + 3];
        processor.PC += 4;
    }
    //send the current fetch to pipe
    for(int i = 0;i < 4;i ++) {
        processor.currentInstructionDecode[i] = processor.currentInstructionFetch[i];
    }
    processor.currentDecode = processor.currentFetch;
    ifFlush = false;
    ifFetch = true;
    ifStore = false;
}

void decode() {
    /*
    printf("DECODE: Read only from the array of bytes from fetch.\n");
    if(currentInstruction == NULL) {
      return;
    }
    int op = currentInstruction[0] >> 4;
    if(op == 0xA | op == 0xB | op == 0xC | op == 0xD) {
      unsigned int temp = PC + 2;
      for(int i = 2;i < 4;i ++) {
        currentInstruction[i] = memory[temp];
        temp += 1;
      }
    }
    switch(op) {
      case 0x7:
            OP1 = (currentInstruction[1] & 0x20) >> 5;
            OP2 = currentInstruction[1] & 0x1F;
            break;
      case 0x8:
            OP1 = ((currentInstruction[0] & 0x0F) << 8) | currentInstruction[1];
            break;
      case 0x9:
            OP1 = (signed char) currentInstruction[1];
            break;
      case 0xA:
      case 0xB:
            OP1 = (signed int)((((currentInstruction[1] & 0x0F) << 8) | currentInstruction[2]) << 8) | currentInstruction[3];
            break;
      case 0xC:
            OP1 = ((((((currentInstruction[0] & 0x0F) << 8) | currentInstruction[1]) << 8) | currentInstruction[2]) << 8) | currentInstruction[3];
            break;
      case 0xD:
            OP1 = currentInstruction[1];
            OP2 = (currentInstruction[2] << 8) | currentInstruction[3];
            break;
      case 0xE:
      case 0xF:
            OP1 = (signed char) currentInstruction[1] & 0x0F;
            break;
      default:
            break;
    }
     */
    if(ifFlush == true || ifFetch != true) {
        return;
    }
    for(int i = 0;i < 4;i ++) {
        if(processor.currentInstructionDecode[i] != processor.currentInstructionFetch[i]) {
            return;
        }
       }
    switch(processor.currentDecode) {
        //“3R” instructions
        case 1://add
        case 2://and
        case 3://divide
        case 4://multiply
        case 5://substract
        case 6://or
            processor.decodeOP1 = (int)processor.currentInstructionDecode[0];
            processor.decodeOP1 %= 16;
            processor.decodeOP2 = (int)processor.currentInstructionDecode[1];
        case 7://Stack Instructions
            processor.decodeOP1 = (int)processor.currentInstructionDecode[0];
            processor.decodeOP1 %= 16;
            processor.decodeOP2 = (int)processor.currentInstructionDecode[1];
            processor.decodeOP2 = processor.decodeOP2 >> 6;
        case 8://load
            processor.decodeOP1 = (int)processor.currentInstructionDecode[1];
        case 9://store
            processor.decodeOP1 = (int)processor.currentInstructionDecode[0];
            processor.decodeOP1 %= 16;
            processor.decodeOP2 = (int)processor.currentInstructionDecode[1];
            if(processor.decodeOP2 > 127) {
                processor.decodeOP2 -= 256;
            }
        case 10://Branch instructions
        case 11:
            processor.decodeOP1 = (int)processor.currentInstructionDecode[0] % 16;
            processor.decodeOP1 = processor.decodeOP1 << 4;
            processor.decodeOP1 |= (int)processor.currentInstructionDecode[1] / 16;
            if((int)processor.currentInstructionDecode[1] % 16 >= 8) {
                processor.decodeOP2 = -1048576;
                processor.decodeOP2 = processor.decodeOP2 | (((int)processor.currentInstructionDecode[1]%16)<<16);
                processor.decodeOP2 = processor.decodeOP2 | ((int)processor.currentInstructionDecode[2])<<8;
                processor.decodeOP2 = processor.decodeOP2 | (int)processor.currentInstructionDecode[3];
            }else {
                processor.decodeOP2 = (int)processor.currentInstructionDecode[1]%16;
                processor.decodeOP2 = processor.decodeOP2<<8;
                processor.decodeOP2 = processor.decodeOP2 | (int)processor.currentInstructionDecode[2];
                processor.decodeOP2 = processor.decodeOP2<<8;
                processor.decodeOP2 = processor.decodeOP2 | (int)processor.currentInstructionDecode[3];
            }
        case 12://jump
        case 13:
            processor.decodeOP1 = (int)processor.currentInstructionDecode[0];
            processor.decodeOP1 = processor.decodeOP1%16;
            processor.decodeOP1 = processor.decodeOP1<<8;
            processor.decodeOP1 = processor.decodeOP1 | (int)processor.currentInstructionDecode[1];

            //store bottom bits
            processor.decodeOP2 = (int)processor.currentInstructionDecode[2];
            processor.decodeOP2 = processor.decodeOP2<<8;
            processor.decodeOP2 = processor.decodeOP2 | (int)processor.currentInstructionDecode[3];
        case 14:
        case 15:
            processor.decodeOP1 = (int)processor.currentInstructionDecode[0];
            processor.decodeOP1 = processor.decodeOP1%16;
            processor.decodeOP1 = processor.decodeOP1<<4;

            processor.decodeOP1 = processor.decodeOP1 | ((int)processor.currentInstructionDecode[1]/16);
            processor.decodeOP2 = ((int)processor.currentInstructionDecode[1]%16);
    }
    
    /*
    if(isFetch != true) {
        return;
    }
    switch(currentOp) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            OP1Decode = (int)currentInstruction[0];
            OP1Decode %= 16;
            OP2Decode = (int)currentInstruction[1];
        case 7:
            OP1Decode = (int)currentInstruction[0];
            OP1Decode %= 16;
            OP2Decode = (int)currentInstruction[1];
            OP2Decode = OP2Decode >> 6;
        case 8:
            OP1Decode = (int)currentInstruction[1];
        case 9:
            OP1Decode = (int)currentInstruction[0];
            OP1Decode %= 16;
            OP2Decode = (int)currentInstruction[1];
            if(OP2Decode > 127) {
                OP2Decode -= 256;
            }
        case 10:
            OP1Decode = (int)currentInstruction[0] % 16;
            OP1Decode = OP1Decode << 4;
            OP1Decode |= (int)currentInstruction[1] / 16;
            if((int)currentInstruction[1] % 16 >= 8) {
     
     }
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
    }
     */
    //send current decode to pipe
    for(int i = 0;i < 4;i ++) {
         processor.currentInstructionExecute[i] = processor.currentInstructionDecode[i];
     }

    processor.currentExecute = processor.currentDecode;
    processor.executeOP1 = processor.decodeOP1;
    processor.executeOP2 = processor.decodeOP2;
    ifFetch = false;
    ifDecode = true;
    
}

void execute() {
    if(ifFlush == true || ifDecode != true) {
        return;
    }
    for(int i = 0;i < 4;i ++) {
        if(processor.currentInstructionExecute[i] != processor.currentInstructionDecode[i]) {
            return;
        }
    }
    
    switch (processor.currentExecute) {
        case 0://halt
            printf("%s","halt\n");
            exit(0);
        case 1://add
            ADD();
            break;
        case 2://add
            AND();
            break;
        case 3://divide
            DIVIDE();
            break;
        case 4://multiply
            MULTIPLY();
            break;
        case 5://substract
            SUBSTRACT();
            break;
        case 6://or
            OR();
            break;
        case 7://stack
            break;
        case 8://interrupt
            break;
        case 9://add immediate
            ADDIMMEDIATE();
            break;
        case 10://brach if less
            BRACHIFLESS();
            break;
        case 11://brach if equal
            BRACHIFEQUAL();
            break;
        case 12://jump
            JUMP();
            break;
        case 13://call
            CALL();
            break;
        case 14://load
            LOAD();
            break;
        case 15://store
            STORE();
            break;
    }
    
    /*
    printf("EXECUTE: Switch statement that “does the work” and stores the work into Result.\n");
    int a;
    
    if(currentInstruction == NULL) {
      return;
    }
    int op = currentInstruction[0] >> 4;
    switch(op) {
      case 0x0:
            break;
      case 0x1:
            result = registers[currentInstruction[0] & 0x0F] + registers[currentInstruction[1] >> 4];
            break;
      case 0x2:
            result = registers[currentInstruction[0] & 0x0F] & registers[currentInstruction[1] >> 4];
            break;
      case 0x3:
            result = registers[currentInstruction[0] & 0x0F] / registers[currentInstruction[1] >> 4];
            break;
      case 0x4:
            result = registers[currentInstruction[0] & 0x0F] * registers[currentInstruction[1] >> 4];
            break;
      case 0x5:
            result = registers[currentInstruction[0] & 0x0F] - registers[currentInstruction[1] >> 4];
            break;
      case 0x6:
            result = registers[currentInstruction[0] & 0x0F] |     registers[currentInstruction[1] >> 4];
            break;
      case 0x7:
            if(OP1 == 0) {
                result = registers[currentInstruction[0] & 0x0F] << OP2;
            }else if(OP1 == 1) {
                result = registers[currentInstruction[0] & 0x0F] >> OP2;
            }
            break;
      case 0x8:
            if(OP1 == 0){
                for(int i = 0;i < 16;i ++) {
                  printf("r%d == %d\n",i,registers[i]);
                }
            }else if(OP1 == 1) {
                for(int i = 0;i < 100;i ++) {
                    if(i % 10 == 0){
                        printf("\n%08d ",i);
                    }
                    if(i % 2 == 0){
                       printf("%02x ",memory[i]);
                    }else {
                        printf("%02x ",memory[i]);
                    }
                }
            }
            break;
      case 0x9:
            result = registers[currentInstruction[0] & 0x0F] + OP1;
            break;
      case 0xA:
            if(registers[currentInstruction[0] & 0x0F] == registers[currentInstruction[1] >> 4]) {
                PC += OP1;
                break;
            }else {
                PC += 4;
                break;
            }
      case 0xB:
            if(registers[currentInstruction[0] & 0x0F] < registers[currentInstruction[1] >> 4]) {
                PC += OP1;
                break;
            }else {
                PC += 4;
                break;
            }
      case 0xC:
            PC = OP1;
            break;
      case 0xD:
            if(memory[registers[currentInstruction[0] & 0x0F] + OP1] != 0) {
                result = memory[registers[currentInstruction[0] & 0x0F] + OP1];
                PC -= OP2;
            }else {
                PC += 4;
            }
            break;
      case 0xE:
            a = registers[currentInstruction[1] >> 4] + OP1;
            result = (((((memory[a] << 8) | memory[a + 1]) << 8) | memory[a + 2]) << 8) | memory[a + 3];
            break;
      case 0xF:
            break;
      default:
            break;
    }
     */
    //send current execute to pipe
    for(int i = 0;i < 4;i ++) {
        processor.currentInstructionStore[i] = processor.currentInstructionExecute[i];
    }
    processor.currentStore = processor.currentExecute;
    processor.storeResult = processor.executeResult;
    processor.storeOP1 = processor.executeOP1;
    processor.storeOP2 = processor.executeOP2;
    ifDecode = false;
    ifExecute = true;
}

void store() {
    if(ifFlush == true || ifExecute != true) {
        return;
    }
    for(int i = 0;i < 4;i ++) {
        if(processor.currentInstructionStore[i] != processor.currentInstructionExecute[i]) {
            return;
        }
    }
    
    ifStore = true;
    int k = 0;
    switch (processor.currentStore) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            processor.registers[processor.storeOP2 % 16] = processor.storeResult;
        case 7:
        case 8:
            if(processor.storeOP1 == 0) {
                printf("Print registers\n");
                for(int i = 0;i < 16;i ++) {
                    printf("processor.registers[%i]: ",i);
                    printf("%02x \n",processor.registers[i]);
                }
            }else if(processor.storeOP1 == 1) {
                int a = 0;
                printf("Prit Memory");
                for(int i = 0;i < 1024;i ++) {
                    if(a % 16 == 0) {
                        printf("\n");
                    }
                    printf("%02x",processor.memory[i]);
                    if(i % 2 != 0) {
                        printf(" ");
                    }
                    a ++;
                }
                printf("\n");
            }else {
                printf("Error\n");
            }
        case 9:
            processor.registers[processor.storeOP1] = processor.storeResult;
        case 10:
        case 11:
            if(processor.storeResult != 1) {
                processor.PC = processor.PC + processor.storeResult;
                flush();
            }
        case 12:
        case 13:
            processor.PC = processor.storeResult;
            flush();
        case 14:
            processor.registers[processor.storeOP1 / 16] = processor.storeResult;
        case 15:
            for(int j = 3;j >= 0;j --) {
                processor.memory[((int)processor.registers[processor.storeOP1 % 16] + (2 * processor.storeOP2)) + k] = (processor.storeResult >> (j * 8));
                k ++;
            }
            k = 0;
    }
    
    ifExecute = false;
    ifStore = true;
    
    /*
    printf("STORE: Write result into final register or memory address. Update PC.\n");
    int a;
    int value;
    
    if(currentInstruction == NULL){
      return;
    }
    int op = currentInstruction[0] >> 4;
    switch(op) {
      case 0x0:
            PC += 2;
            break;
      case 0x1:
      case 0x2:
      case 0x3:
      case 0x4:
      case 0x5:
      case 0x6:
            registers[currentInstruction[1] & 0x0F] = result;
            PC += 2;
            break;
      case 0x7:
            registers[currentInstruction[0] & 0x0F] = result;
            PC += 2;
            break;
      case 0x8:
            PC += 2;
            break;
      case 0x9:
            registers[currentInstruction[0] & 0x0F] = result;
            PC += 2;
            break;
      case 0xA:
      case 0xB:
            break;
      case 0xC:
            break;
      case 0xD:
            registers[currentInstruction[0] & 0x0F] = result;
            break;
      case 0xE:
            registers[currentInstruction[0] & 0x0F] = result;
            PC += 2;
            break;
      case 0xF:
            value = registers[currentInstruction[0] & 0x0F];
            a = registers[currentInstruction[1] >> 4] + OP1;

            memory[a] = value >> 24;
            memory[a + 1] = (value >> 16) & 0x00FF;
            memory[a + 2] = (value >> 8) & 0x0000FF;
            memory[a + 3] = value & 0x000000FF;
            PC += 2;
            break;
      default:
            break;
    }
     */
}

void load(char *filename) {
    FILE *src = fopen(filename,"r");
    char line[2];
    int j = 0;
    while(fgets(line,sizeof(line),src) != NULL) {
        unsigned char temp;
        for(int i = 0;i < 1;i ++) {
            temp = line[i];
            processor.memory[j] = temp;
            j ++;
        }
    }
}

int main(int argc,char *argv[]) {
    /*
    FILE *src = fopen(argv[1],"r");
    char line[2];
    int i,size = 0;
    while (NULL != fgets(line, sizeof(line), src)) {
            unsigned char temp;
            for(i=0;i<1;i++){
            temp = line[i];
            mem[instructionSize]=temp;
            instructionSize++;
        }
    }
    fclose(src);
     */
    
    //memory = (unsigned char *) malloc(1024 * sizeof(unsigned char));
    /*
    if(argc < 2) {
        puts("ERROR: not enough element");
        free(memory);
        return -1;
    }else if(argc > 2) {
        puts("ERROR: more argument");
        free(memory);
        return -1;
    }
     */
    
    /*
    if(argc > 2) {
        puts("ERROR: more argument");
        free(memory);
        return -1;
    }
    char * filename = argv[1];
    FILE *fptr;
    fptr = fopen(filename,"rb");
    if(fptr == NULL) {
        fclose(fptr);
        puts("ERROR: can not read file");
    }
    fseek(fptr,0,SEEK_END);
    size_t fileSize = ftell(fptr);
    fseek(fptr,0,SEEK_SET);
    if(fread(memory, sizeof *memory,fileSize,fptr) != fileSize) {
        fclose(fptr);
        puts("ERROR: can not read file");
    }
    
    int halt = 0;
    while(halt != 1) {
        fetch();
        decode();
        execute();
        store();
      if(currentInstruction == NULL){
        halt = 1;
      }
    }
     
    free(memory);
     */
    processor.PC = 0;

    processor.decodeOP1 = 0;
    processor.decodeOP2 = 0;
    processor.executeOP1 = 0;
    processor.executeOP2 = 0;
    processor.storeOP1 = 0;
    processor.storeOP2 = 0;

    load(argv[1]);
    flush();
    
    while(processor.PC < 1024) {
        execute();
        store();
        decode();
        fetch();
    }
    return 0;
}
