//
// Created by Rufelle on 18/01/2023.
//
#include <iostream>
#ifndef CHAINC_MEMORY_UNIT_H
#define CHAINC_MEMORY_UNIT_H
#include "math.h"
#include <array>
#include <cstring>
#include <unistd.h>
#include "../extern/preprocessor.h"
Preprocessor preprocessor;

namespace chain{
    void InternalCompilerError (){
        using std::endl;
        std::cerr << "ERR 023C InternalCompilerError [Compiler]: Internal compiler error instance occurred. This is not a bug on the user's end. Please contact me in Github at Xue64 to report this bug" << std::endl;
        exit(023);
    }
}



class Memory
{

    static const size_t mem_cap = pow(2, 16);
private:
    char ** heap;
    bool isDeleted = false;
    std::string START = "0000000000000000";
    // registers
    char * ac;
    char * acx;
    char * acy;
    char * r;
    char * rx;
    char * ry;
    size_t ar;
    size_t pc;
    bool flag;
    char * pc_char;
    char * ar_char;

    void set(char * arr){
        for (int i=0; i<8; i++){
            arr[i] = '0';
        }
        arr[8] = '\0';
    }

    char * regalloc(){
        auto * a = new char[8];
        if (a == NULL){
            regalloc();
        } return a;
    }

public:
    Memory(){



        r = regalloc();
        ac = regalloc();
        pc_char = regalloc();
        ar_char = regalloc();

        pc = 0;
        ar = 0;


        heap = new char*[mem_cap];
        for (int i=0; i<mem_cap; i++){
            heap[i] = new char[9];
            set(heap[i]);
        }

        set(r);
        set(ac);
        set(pc_char);
        set(ar_char);
        flag = false;

        acx = ac;
        acy = ac + 8;
        rx = r;
        ry = r + 8;


    }

    auto memory(){
        return heap;
    }

    void display(){
        for (int i=0; i<mem_cap; i++){
            std::cout << i << " > ";
            for (int j=0; j<=7; j++){
                std::cout << heap[i][j] << " ";
            } std::cout << std::endl;
        }
    }

    void display(size_t a, size_t b){
        for (int i=a; i<=b; i++){
            std::cout << i << " > ";
            for (int j=0; j<=7; j++){
                std::cout << heap[i][j] << " ";
            } std::cout << std::endl;
        }
    }

    ~Memory(){
        if (isDeleted){
            return;
        }
        for (int i=0; i<mem_cap; i++){
            delete heap[i];
        } delete heap;
    }
public:
    auto& AR(){
        return ar;
    }

    auto& PC(){
        return pc;
    }

    auto HEAP (){
        return heap;
    }


    auto write (size_t size, const std::string & cpy){
      int degree = 0;
        for (int i=0; i<size; i++){
           auto temp  = cpy[i];
           if (i%8 == 0){
               if (i!=0) degree++;
           }
           heap[pc + degree][i - (degree * 8)] = temp;
           if (temp != '1' && temp != '0'){
                chain::InternalCompilerError();
           }
        }


    }

    auto writeAt (size_t address, const std::string & cpy){
        if (address >= 65536) return;
        for (auto temp : cpy){
            if (temp != '1' && temp != '0'){
                chain::InternalCompilerError();
            }
        }
        memcpy(this->HEAP()[address], cpy.c_str(), 8);
    }



    auto inc (){
        pc++;
        ar++;
        if (pc > MEM_CAP()-100 || ar > MEM_CAP()-100) memory_full();
    }

    void set_start(const std::string & str){
        START = str;
        pc = binToDec(START);
        ar = binToDec(START);
    }

    std::string get_start(){
        return START;
    }

private:
    void memory_full(){
        std::cerr << "ERR 05 [OutOfMemoryError]: Heap is full. Cannot add instructions to memory.\n";
        exit(05);
    }
public:

    auto inc (size_t amount){
        pc += amount;
        ar += amount;

        if (preprocessor.isDebugging()) {
            std::cout << "PC NOW AT " << pc << std::endl;
        }
        if (pc > MEM_CAP()-100 || ar > MEM_CAP()-100) memory_full();
    }

    void set (size_t address){
        ar = address;
        pc = address;

    }

    int binToDec(std::string binary) {
        int n = binary.length();

        int decimal = 0;
        for (int i = 0; i < n; i++) {
            if (binary[i] == '1') {
                decimal += pow(2, n-1-i);
            }
        }
        return decimal;
    }

    std::string getBitString (size_t address, size_t size){
        std::string result;
        for (int i=0; i<size; i++){
            result.push_back(heap[address][i]);
        }
        return result;
    }



    void destroy (){
        isDeleted = true;
        for (int i=0; i<MEM_CAP(); i++){
            free(HEAP()[i]);
        }
        free(r);
        free(ac);
    }



    static size_t MEM_CAP(){return mem_cap;}
};



#endif //CHAINC_MEMORY_UNIT_H
