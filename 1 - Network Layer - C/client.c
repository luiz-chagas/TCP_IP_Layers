#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>

struct Frame{
    // HEADER
    unsigned char source[6]; // source's MAC address
    unsigned char destination[6]; // destination's MAC address
    unsigned char type[2];// type of protocol - 0x0800 for IPv4
    // DATA
    unsigned char data[494]; // 494 bytes limit for data
    // FOOTER
    unsigned char checksum[4];
};

void prepareFrame(struct Frame *frame, char *message){
    strcpy(frame->source, "\x01\x02\x03\x04\x05\x06");
    strcpy(frame->destination, "\x11\x12\x13\x14\x15\x16");
    strcpy(frame->type, "\x08\x00");
    sprintf(frame->data, "%-494s", message);
    strcpy(frame->checksum, "\x00\x20\x20\x3a");
}

void sendFrame(struct Frame *frame, int socket, ssize_t frame_size){
    ssize_t sent_bytes;
    int remain_data = (int) frame_size;
    while (remain_data > 0)
    {
         sent_bytes = send(socket, frame, frame_size, 0);
         remain_data -= sent_bytes;
         fprintf(stdout, "Client sent %d bytes to the server, remaining data = %d\n", (int) sent_bytes, remain_data);
    }
}

int main(int argc, char **argv)
{
        ssize_t len;
        char buffer[512];
        int frame_size = 512;
        char *message;
        char *ip;
        int port_number;
        int client_socket;
        struct sockaddr_in remote_addr;
        struct Frame frame1;

        if(argc != 4){
          fprintf(stderr, "Usage: client message ip port\n");
          exit(EXIT_FAILURE);
        }

        message = argv[1];
        ip = argv[2];
        port_number = atoi(argv[3]);

        /* Zeroing remote_addr struct */
        memset(&remote_addr, 0, sizeof(remote_addr));

        /* Construct remote_addr struct */
        remote_addr.sin_family = AF_INET;
        inet_pton(AF_INET, "127.0.0.1", &(remote_addr.sin_addr));
        remote_addr.sin_port = htons(port_number);

        /* Create client socket */
        client_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (client_socket == -1)
        {
            fprintf(stderr, "Error creating socket --> %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }else{
            fprintf(stdout, "Socket created successfully.\n");
        }

        /* Connect to the server */
        if (connect(client_socket, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr)) == -1)
        {
            fprintf(stderr, "Error on connect --> %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }else{
            fprintf(stdout, "Connection to server established on port %d.\n", port_number);
        }

        /* Sending frame size */
        sprintf(buffer, "%d", frame_size);
        len = send(client_socket, buffer, sizeof(buffer), 0);
        if (len < 0)
        {
            fprintf(stderr, "Error on sending greetings --> %s", strerror(errno));
            close(client_socket);
            exit(EXIT_FAILURE);
        }else{
            fprintf(stdout, "Sent %ld bytes to the server\n", len);
            fprintf(stdout, "TMQ negotiated --> %d bytes\n", frame_size);
        }

        //I'm not going to deal with messages over 512 bytes
        // if(strlen(message)>512){
        //     int i = 0;
        //     while(i+512 < strlen(message)){
        //         //Prepare Frame
        //         prepareFrame(&frame1, message);
        //         //Send Frame
        //         sendFrame(&frame1, client_socket, frame_size);
        //         i += 512;
        //     }
        // }else{
        //     //Prepare Frame
        //     prepareFrame(&frame1, message);
        //     //Send Frame
        //     sendFrame(&frame1, client_socket, frame_size);
        // }
        //Prepare Frame
        prepareFrame(&frame1, message);
        //Send Frame
        sendFrame(&frame1, client_socket, frame_size);

        close(client_socket);

        return 0;
}
