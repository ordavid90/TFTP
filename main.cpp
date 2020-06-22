
#include "TFTP.h"

int main(int argc, char* argv[]) {
    const int WAIT_FOR_PACKET_TIMEOUT = 3;
    const int NUMBER_OF_FAILURES = 7;
    const int MAX_BLOCK_LEN = 512;
    int sockfd, newsockfd, portn, client_addr_len;
    ssize_t bytes_recieved = 0;
    data_t data_msg = {0};
    socklen_t addr_len = 0;
    int write_file, lastWriteSize;
    char data[MAX_DATA_SIZE];
    short int op_code = *(short int *) data;
    char *data_p;
    char file_name[255];
    ack_t msg = {0};
    char mode[255];


    ack_t ack_msg = {0};
    struct timeval time_val;
    time_val.tv_sec = WAIT_FOR_PACKET_TIMEOUT;
    time_val.tv_usec = 0;
    int line_len = 2;
    int sock_ready = 0;
    char buffer[512] = {0};
    fd_set read_fds;
    struct sockaddr_in my_addr = {0}, client_addr = {0};
    bool return_wait_new_msg = false;

    if (argc < 2) {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("ERROR opening socket");
    }

    portn = atoi(argv[1]);
    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = INADDR_ANY;
    my_addr.sin_port = htons(portn);

    if (bind(sockfd, (struct sockaddr *) &my_addr, sizeof(my_addr)) < 0) {
        error("ERROR on binding");
    }
    if (listen(sockfd, line_len) < 0) {
        error("ERROR on listening");
    }

    while (true) {
        addr_len = sizeof(struct sockaddr_in);
        FD_ZERO(&read_fds);
        FD_SET(sockfd, &read_fds);
        ssize_t size_req = recvfrom(sockfd,data,MAX_DATA_SIZE,0,(struct sockaddr *) &client_addr, &addr_len);
        if (size_req<0)
        {
            close(sockfd);
            printf("TTFTP_ERROR:ERROR on recvfrom()");
            return -1;
        }
        op_code = ntohs(op_code);
        data_p = data + 2;
        strcpy(file_name, data_p);
        data_p += strlen(data_p) + 1;
        strcpy(mode, data_p);


        do { // todo: return ACK0 massage to the sender
            client_addr_len = sizeof(client_addr);
            if (newsockfd = accept(sockfd, (struct sockaddr *) &client_addr, client_addr_len) < 0) {
                perror("ERROR using accept()");
                return -1;
            }
            client_addr_len = sizeof(client_addr);
                    do {
                                do {
                                    sock_ready = select(sockfd + 1, &read_fds, NULL, NULL, &time_val);
                                    if (sock_ready < 0) {
                                        close(newsockfd);
                                        perror("ERROR using select()");
                                        return -1;
                                    }

                                    if (sock_ready > 0)// TODO: if there was something at the socket and
                                        // we are here not because of a timeout
                                    {
                                        // TODO: Read the DATA packet from the socket (at
                                        // least we hope this is a DATA packet)
                                        bytes_recieved = recvfrom(sockfd, data, MAX_BLOCK_LEN, 0,
                                                                  (struct sockaddr *) &client_addr,
                                                                  &addr_len) < 0;
                                        if (bytes_recieved) {
                                            close(sockfd);
                                            close(write_file);
                                            perror("ERROR using recvfrom");
                                            return -1;
                                        }
                                        data_msg = *((data_t *) data);

                                    }

                                    if (sock_ready == 0) // TODO: Time out expired while waiting for data
                                        //		  to appear at the socket
                                    {
                                        //TODO: Send another ACK for the last packet
                                        timeoutExpiredCount++;
                                        msg.op_code = htons(ACK_OP);
                                        msg.block_num = htons(0);
                                        if (sendto(sockfd, &msg, sizeof(msg), 0,  (struct sockaddr *) &client_addr,sizeof(struct sockaddr_in)) < 0) {
                                            perror("ERROR using sendto");
                                            return -1;
                                        }
                                    }

                                    if (timeoutExpiredCount >= NUMBER_OF_FAILURES) {
                                        perror("FATAL ERROR - timeout expired");
                                        return -1;
                                    }


                                } while (false); // TODO: Continue while some socket was ready
                        // but recvfrom somehow failed to read the data

                        // we receive something that is not data
                        if (ntohs(data_msg.op_code) != DATA_OP) {
                            printf("ERRORFLOW: OP not match to DATA_OP\n");
                            close(sockfd);
                            close(write_file);
                            printf("RECVFAIL\n");
                            return_wait_new_msg = true;
                            break;
                        }

                        if (ntohs(data_msg.block_num) !=
                            htons(ack_msg.block_num) + 1) // TODO: The incoming block number is not what we have
                            // expected, i.e. this is a DATA pkt but the block number
                            // in DATA was wrong (not last ACK’s block number + 1)
                        {
                            rintf("ERRORFLOW: unmatch block num,excepted %d but arrived %d\n", htons(ack_msg.block_num + 1),
                                  ntohs(data_msg.block_num));
                            close(sockfd);
                            close(write_file);
                            printf("RECVFAIL\n");
                            return_wait_new_msg = true;
                            break;
                        }
                    } while (false);
            if (return_wait_new_msg) break;
            printf("IN:DATA,%d,%d\n", ntohs(data_msg.block_num), (int) bytes_recieved - 4);
            timeoutExpiredCount = 0;
            lastWriteSize = write(write_file, &data_msg.data, bytes_recieved - 4); // write next bulk of data
            printf("WRITING:%d\n", lastWriteSize);
            ack_msg.op_code = htons(ACK_OP);
            ack_msg.block_num = htons(htons(ack_msg.block_num) + 1);
            if (sendto(socket_fd, &ack_msg, sizeof(ack_t), 0, (struct sockaddr *) &client_addr,
                       sizeof(struct sockaddr_in)) < 0) {
                close(sockfd);
                close(write_file);
                printf("ERROR couldnt send ACK %d/n", ack_msg.block_num);
                exit(1);
            }

            printf("OUT:ACK, %d\n", htons(ack_msg.block_num));
            if (bytes_recieved < MAX_DATA_SIZE) {
                communication_opend = false;
            } else {
                communication_opend = true;
            }


            timeoutExpiredCount = 0;
            lastWriteSize = fwrite(...); // write next bulk of data
            // TODO: send ACK packet to the client
        } while (FALSE) // Have blocks left to be read from client (not end of transmission)

    }

}
