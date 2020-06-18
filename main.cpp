#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/netinet/in.h>

int main(int argc, char* argv[]) {
    int sockfd, newsockfd, portn;
    int line_len = 2;
    char buffer[512] = {0};
    struct sockaddr_in my_addr = {0};

    if (argc < 2) {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        error("ERROR opening socket");
    }

    portn = atoi(argv[1]);
    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = INADDR_ANY;
    my_addr.sin_port = htos(portn);

    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(my_addr)) < 0){
        error("ERROR on binding");
    }
    if (listen(sockfd, line_len) < 0){
        error("ERROR on listening")
    }
    newsockfd = accept(sockfd, (struct sockaddr *)&my_addr,  )


}
    const int WAIT_FOR_PACKET_TIMEOUT = 3; const int NUMBER_OF_FAILURES = 7;
    do
    {
        do
        {
            do
            {
                // TODO: Wait WAIT_FOR_PACKET_TIMEOUT to see if something appears
                //		 for us at the socket (we are waiting for DATA)

                if ()// TODO: if there was something at the socket and
                    // we are here not because of a timeout
                {
                    // TODO: Read the DATA packet from the socket (at
                    // least we hope this is a DATA packet)
                }

                if (...) // TODO: Time out expired while waiting for data
                //		  to appear at the socket
                {
                    //TODO: Send another ACK for the last packet
                    timeoutExpiredCount++;
                }

                if (timeoutExpiredCount>= NUMBER_OF_FAILURES)
                {
                    // FATAL ERROR BAIL OUT
                }


            }while (...) // TODO: Continue while some socket was ready
            // but recvfrom somehow failed to read the data

            if (...) // TODO: We got something else but DATA
            {
                // FATAL ERROR BAIL OUT
            }

            if (...) // TODO: The incoming block number is not what we have
            // expected, i.e. this is a DATA pkt but the block number
            // in DATA was wrong (not last ACK’s block number + 1)
            {
                // FATAL ERROR BAIL OUT
            }
        }while (FALSE);
        timeoutExpiredCount = 0;
        lastWriteSize = fwrite(...); // write next bulk of data
        // TODO: send ACK packet to the client
}
