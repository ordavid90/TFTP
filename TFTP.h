//
// Created by yonatan on 07/01/2020.
//

#ifndef TFTP_TFTP_H
#define TFTP_TFTP_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>



#define MAX_DATA_SIZE 516
#define READ_OP 1
#define WRITE_OP 2
#define DATA_OP 3
#define ACK_OP 4
#define ERROR_OP 5

struct wrq{
    short int op_code;
    char* file_name;
    char* mode;
} __attribute__((packed));

struct data{
    short int op_code;
    short int block_num;
    char data[512];
}__attribute__((packed));

struct ack{
    short int op_code;
    short int block_num;
}__attribute__((packed));

struct err{
    short int op_code;
    short int error_num;
    char* err_msg;
}__attribute__((packed));

typedef struct wrq wrq_t;
typedef struct data data_t;
typedef struct ack ack_t;
typedef struct err err_t;
typedef struct timeval timeVal_t;

void error(char* error_msg);




#endif //TFTP_TFTP_H