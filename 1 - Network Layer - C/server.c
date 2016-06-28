#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
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

int main(int argc, char **argv){
	int server_socket;
	int client_socket;
    int network_socket;
	socklen_t sock_len;
	ssize_t len;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
    struct sockaddr_in network_addr;
	int fd;
	int frame_size;
	FILE *received_frame;
	int port_number;
    char *ip;
	char buffer[512];
	char filename[18];
	int i = 0;
    struct Frame frame1;

	// if(argc != 2){
	// 	fprintf(stderr, "Usage: server\n");
	// 	exit(EXIT_FAILURE);
	// }

    //ip = argv[1];
	//port_number = atoi(argv[2]);
    ip = "127.0.0.1";
    port_number = 13000;

	/* Create server socket */
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == -1){
		fprintf(stderr, "Error creating socket --> %s", strerror(errno));
		exit(EXIT_FAILURE);
	}else{
		fprintf(stdout, "Server socket created successfully.\n");
	}

    /*Create network socket*/
    network_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (network_socket == -1){
		fprintf(stderr, "Error creating socket --> %s", strerror(errno));
		exit(EXIT_FAILURE);
	}else{
		fprintf(stdout, "Network socket created successfully.\n");
	}

	/* Zeroing server_addr struct */
	memset(&server_addr, 0, sizeof(server_addr));
    memset(&network_addr, 0, sizeof(network_addr));

	/* Construct server_addr struct */
	server_addr.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &(server_addr.sin_addr));
	server_addr.sin_port = htons(port_number);

    /* Construct network_addr struct */
	network_addr.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &(network_addr.sin_addr));
	network_addr.sin_port = htons(12000);

	/* Bind */
	if ((bind(server_socket, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))) == -1){
		fprintf(stderr, "Error on bind --> %s", strerror(errno));
		exit(EXIT_FAILURE);
	}else{
		fprintf(stdout, "Bind successful.\n");
	}
	/* Listening to incoming connections */
	if ((listen(server_socket, 5)) == -1){
		fprintf(stderr, "Error on listen --> %s", strerror(errno));
		exit(EXIT_FAILURE);
	}else{
		fprintf(stdout, "Server is now listening on port %d.\n",port_number);
	}

	sock_len = sizeof(struct sockaddr_in);
	/* Accepting incoming clients */
	while(1){
		client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &sock_len);
		if (client_socket == -1){
			fprintf(stderr, "Error on accept --> %s", strerror(errno));
			exit(EXIT_FAILURE);
		}
		fprintf(stdout, "Accept client --> %s\n", inet_ntoa(client_addr.sin_addr));

		// Receiving frame size
		int n = read(client_socket, buffer, sizeof(buffer));
		printf("Read %d bytes from client\n",n);
		frame_size = atoi(buffer);
		fprintf(stdout, "TMQ Negotiated --> %d bytes\n", frame_size);

		while(1){
			bzero(buffer, frame_size);

			// Receiving frame
			len = read(client_socket, buffer, frame_size);
			if((int)len <= 0) break;

			sprintf(filename, "ReceivedFrame%05d",i++);
			received_frame = fopen(filename, "w");
			if (received_frame == NULL){
				fprintf(stderr, "Failed to create file --> %s\n", strerror(errno));
				exit(EXIT_FAILURE);
			}
			fwrite(buffer, sizeof(char), len, received_frame);
			fprintf(stdout, "Server received %d bytes.\n", (int) len);

            memcpy(frame1.data, &buffer[14], 494 );

            fclose(received_frame);

            //Send frame1->data to Network layer
            if (connect(network_socket, (struct sockaddr *)&network_addr, sizeof(struct sockaddr)) == -1) {
        		fprintf(stderr, "Error on connect --> %s\n", strerror(errno));
        		exit(EXIT_FAILURE);
        	}else{
        		fprintf(stdout, "Connection to network established on port %d.\n", 12000);
        	}
            // Send message
            sprintf(buffer, "%s", frame1.data);
        	len = send(network_socket, buffer, 494, 0);
        	if (len < 0) {
        		fprintf(stderr, "Error on connecting to the network --> %s", strerror(errno));
        		exit(EXIT_FAILURE);
        	}else{
        		fprintf(stdout, "Sent %ld bytes to the network\n", len);
        	}

            bzero(buffer, frame_size);
            sleep(3);
            // Receiving message
			len = read(network_socket, buffer, frame_size);
			if((int)len <= 0) break;
			fprintf(stdout, "Server received %d bytes from network.\n", (int) len);

            // Send message
            printf("%s", buffer);
        	len = send(client_socket, buffer, 512, 0);
        	if (len < 0) {
        		fprintf(stderr, "Error on connecting to the network --> %s", strerror(errno));
        		exit(EXIT_FAILURE);
        	}else{
        		fprintf(stdout, "Sent %ld bytes to the physical\n", len);
        	}
            close(network_socket);
            break;
		}
		close(client_socket);
	}
	close(server_socket);

	return 0;
}
