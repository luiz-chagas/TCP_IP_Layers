#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <sys/ioctl.h>
#include <sys/time.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>

#include <asm/types.h>

#include <math.h>
#include <signal.h>

#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/if_arp.h>

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

void prepareFrame(struct Frame *frame, char *message, char* src_mac, char* dst_mac){
    strcpy(frame->source, src_mac);
    strcpy(frame->destination, dst_mac);
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

void getMacFromIp(char* ip, char* dst_mac) {
	unsigned char buffer1[100], exp[255];

	snprintf(buffer1, sizeof(buffer1), "arp -a %s", ip);
	FILE *in = popen(buffer1, "r");
	fgets(exp, 255, in);
	fclose(in);

	unsigned char* tk = strtok(exp, " ");
	int n = 0;
	while (n == 0) {
		sscanf(tk, "%2x:%2x:%2x:%2x:%2x:%2x", &n);
		tk = strtok(NULL, " ");
	}

	if(strcmp("in", tk) != 0)
		sscanf(tk, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &dst_mac[0], &dst_mac[1], &dst_mac[2], &dst_mac[3], &dst_mac[4], &dst_mac[5]);
	else
		exit(1);
}

int main(int argc, char **argv) {
        ssize_t len;
        char buffer[512];
        int frame_size = 512;
        char *message;
        char *ip;
        int port_number;
        int client_socket;
        struct sockaddr_in remote_addr;
        struct Frame frame1;
        struct ifreq ifr;
		int ifindex = 0;     /*Ethernet Interface index*/
		int i;
		unsigned char src_mac[6], dst_mac[6];

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
        if (client_socket == -1) {
            fprintf(stderr, "Error creating socket --> %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }else{
            fprintf(stdout, "Socket created successfully.\n");
        }

        /*retrieve ethernet interface index*/
		strncpy(ifr.ifr_name, "eth0", IFNAMSIZ);
		if (ioctl(client_socket, SIOCGIFINDEX, &ifr) == -1) {
		    perror("SIOCGIFINDEX");
		    exit(1);
		}
		ifindex = ifr.ifr_ifindex;

		/*retrieve corresponding MAC*/
		if (ioctl(client_socket, SIOCGIFHWADDR, &ifr) == -1) {
		    perror("SIOCGIFHWADDR");
		    exit(1);
		}
		for (i = 0; i < 6; i++) {
		    src_mac[i] = ifr.ifr_hwaddr.sa_data[i];
		}
		printf("Successfully got our MAC address: %02X:%02X:%02X:%02X:%02X:%02X\n",
		    src_mac[0],src_mac[1],src_mac[2],src_mac[3],src_mac[4],src_mac[5]);

		getMacFromIp(ip, &dst_mac);
		printf("Successfully got destination's MAC address: %02X:%02X:%02X:%02X:%02X:%02X\n",
		    dst_mac[0],dst_mac[1],dst_mac[2],dst_mac[3],dst_mac[4],dst_mac[5]);


        /* Connect to the server */
        if (connect(client_socket, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr)) == -1) {
            fprintf(stderr, "Error on connect --> %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }else{
            fprintf(stdout, "Connection to server established on port %d.\n", port_number);
        }

        /* Sending frame size */
        sprintf(buffer, "%d", frame_size);
        len = send(client_socket, buffer, sizeof(buffer), 0);
        if (len < 0) {
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
        prepareFrame(&frame1, message, src_mac, dst_mac);
        //Send Frame
        sendFrame(&frame1, client_socket, frame_size);

        close(client_socket);

        return 0;
}
