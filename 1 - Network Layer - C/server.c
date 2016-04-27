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
        int frame_size;
        FILE *received_frame;
        int remain_data;
        int port_number;
        char buffer[512];
        char filename[18];
        int i = 0;

        if(argc != 2){
          fprintf(stderr, "Usage: server port_number\n");
          exit(EXIT_FAILURE);
        }

        port_number = atoi(argv[1]);

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
            fprintf(stdout, "Server is now listening on port %d.\n",port_number);
        }

        sock_len = sizeof(struct sockaddr_in);
        /* Accepting incoming clients */
        while(1){
            client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &sock_len);
            if (client_socket == -1)
            {
                    fprintf(stderr, "Error on accept --> %s", strerror(errno));
                    exit(EXIT_FAILURE);
            }
            fprintf(stdout, "Accept client --> %s\n", inet_ntoa(client_addr.sin_addr));

            // Receiving frame size
            int n = read(client_socket, buffer, sizeof(buffer));
            printf("Read %d bytes from client\n",n);
            frame_size = atoi(buffer);
            fprintf(stdout, "TMQ Negotiated --> %d bytes\n", frame_size);

            // Receiving frame
            sprintf(filename, "ReceivedFrame%05d",i++);
            received_frame = fopen(filename, "w");
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
                fprintf(stdout, "Server received %d bytes.\n", (int) len);
            }
            fclose(received_frame);
            close(client_socket);
        }
        close(server_socket);

        return 0;
}
