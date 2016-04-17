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
    unsigned char padding[494]; // for making Frame 512 bytes long
    // FOOTER
    unsigned char checksum[4];
};

void prepareFrame(char destinationIP[16], char *message, Frame *frame){
    frame.destination = // get MAC address from IP
    frame.message = message;
}

void sendFrame(Frame *frame, int socket){
    if (send(socket, *frame, 512, 0) < 0) // returns -1 if error
    {
        fprintf(stderr, "Error on sending Frame --> %s", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char **argv)
{
        ssize_t len;
        char buffer[256];
        int file_size;
        FILE *received_file;
        int remain_data = 0;
        char *filename;
        int port_number;
        int client_socket;
        struct sockaddr_in remote_addr;

        if(argc != 3){
          fprintf(stderr, "Usage: client path-to-file port-number ip\n");
          exit(EXIT_FAILURE);
        }

        filename = argv[1];

        port_number = atoi(argv[2]);

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
        }

        /* Connect to the server */
        if (connect(client_socket, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr)) == -1)
        {
                fprintf(stderr, "Error on connect --> %s\n", strerror(errno));
                exit(EXIT_FAILURE);
        }

        /* Receiving file size */
        int n = read(client_socket, buffer, 256);
        printf("Read %d bytes for size\n",n);
        file_size = atoi(buffer);
        fprintf(stdout, "File size : %d\n", file_size);

        received_file = fopen(filename, "w");
        if (received_file == NULL)
        {
                fprintf(stderr, "Failed to open file foo --> %s\n", strerror(errno));
                exit(EXIT_FAILURE);
        }

        remain_data = file_size;

        bzero(buffer,256);

        while (remain_data > 0)
        {
                len = read(client_socket, buffer, 256);
                fwrite(buffer, sizeof(char), len, received_file);
                remain_data -= len;
                fprintf(stdout, "Received %d bytes and we hope to receive : %d bytes\n", (int) len, remain_data);
                if(len == 0){
                  close(client_socket);
                  exit(EXIT_FAILURE);
                }
        }
        fclose(received_file);

        close(client_socket);

        return 0;
}
