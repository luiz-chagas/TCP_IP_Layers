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

int main(int argc, char **argv)
{
        int server_socket;
        int client_socket;
        socklen_t sock_len;
        ssize_t len;
        struct sockaddr_in server_addr;
        struct sockaddr_in client_addr;
        int fd;
        //int sent_bytes = 0;
        int frame_size;
        FILE *received_frame;
        //struct stat file_stat;
        //int offset;
        int remain_data;
        //char *file_to_send;
        int port_number;
        char buffer[512];

        if(argc != 3){
          fprintf(stderr, "Usage: server path-to-folder port_number\n");
          exit(EXIT_FAILURE);
        }

        //file_to_send = argv[1];

        port_number = atoi(argv[2]);

        /* Create server socket */
        server_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (server_socket == -1)
        {
            fprintf(stderr, "Error creating socket --> %s", strerror(errno));
            exit(EXIT_FAILURE);
        }else{
            fprintf(stdout, "Socket created successfully.\n");
        }

        /* Zeroing server_addr struct */
        memset(&server_addr, 0, sizeof(server_addr));
        /* Construct server_addr struct */
        server_addr.sin_family = AF_INET;
        inet_pton(AF_INET, "127.0.0.1", &(server_addr.sin_addr));
        server_addr.sin_port = htons(port_number);

        /* Bind */
        if ((bind(server_socket, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))) == -1)
        {
            fprintf(stderr, "Error on bind --> %s", strerror(errno));
            exit(EXIT_FAILURE);
        }else{
            fprintf(stdout, "Bind successful.\n");
        }

        /* Listening to incoming connections */
        if ((listen(server_socket, 5)) == -1)
        {
            fprintf(stderr, "Error on listen --> %s", strerror(errno));
            exit(EXIT_FAILURE);
        }else{
            fprintf(stdout, "Server is now listening.\n");
        }

        // fd = open(file_to_send, O_RDONLY);
        // if (fd == -1)
        // {
        //         fprintf(stderr, "Error opening file --> %s", strerror(errno));
        //         exit(EXIT_FAILURE);
        // }

        // /* Get file stats */
        // if (fstat(fd, &file_stat) < 0)
        // {
        //         fprintf(stderr, "Error fstat --> %s", strerror(errno));
        //         exit(EXIT_FAILURE);
        // }

        //fprintf(stdout, "File Size: \n%d bytes\n", (int) file_stat.st_size);

        sock_len = sizeof(struct sockaddr_in);
        /* Accepting incoming clients */
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &sock_len);
        if (client_socket == -1)
        {
                fprintf(stderr, "Error on accept --> %s", strerror(errno));
                exit(EXIT_FAILURE);
        }
        fprintf(stdout, "Accept client --> %s\n", inet_ntoa(client_addr.sin_addr));

        //sprintf(file_size, "%d", (int) file_stat.st_size);

        // /* Sending file size */
        // len = send(client_socket, file_size, sizeof(file_size), 0);
        // if (len < 0)
        // {
        //       fprintf(stderr, "Error on sending greetings --> %s", strerror(errno));
        //       exit(EXIT_FAILURE);
        // }

        //fprintf(stdout, "Server sent %d bytes for the size\n", (int) len);

        //offset = 0;
        //remain_data = file_stat.st_size;

        // /* Sending file data */
        // while (remain_data > 0)
        // {
        //         sent_bytes = sendfile(client_socket, fd, NULL, 256);
        //         remain_data -= sent_bytes;
        //         fprintf(stdout, "1. Server sent %d bytes from file's data, offset is now : %d and remaining data = %d\n", sent_bytes, offset, remain_data);
        // }

        // Receiving frame size
        int n = read(client_socket, buffer, 64);
        printf("Read %d bytes for size\n",n);
        frame_size = atoi(buffer);
        fprintf(stdout, "Frame size : %d\n", frame_size);

        // Receiving frame
        received_frame = fopen("ReceivedFrame", "w");
        if (received_frame == NULL)
        {
            fprintf(stderr, "Failed to open file foo --> %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        remain_data = frame_size;

        bzero(buffer, frame_size);

        while (remain_data > 0)
        {
            len = read(client_socket, buffer, frame_size);
            fwrite(buffer, sizeof(char), len, received_frame);
            remain_data -= len;
            fprintf(stdout, "Received %d bytes and we expect to receive : %d bytes\n", (int) len, remain_data);
        }
        fclose(received_frame);

        close(client_socket);
        close(server_socket);

        return 0;
}
