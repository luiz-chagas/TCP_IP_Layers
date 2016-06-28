import java.io.*;
import java.net.*;

public class Client {
    public static void main(String argv[]) throws Exception {
    	ServerSocket socketRecv = new ServerSocket(12001);

        while(true) {
        	//socket que recebe da camada de transporte
            Socket connectionSocket = socketRecv.accept();
            BufferedReader inFromTransport = new BufferedReader(new InputStreamReader(connectionSocket.getInputStream()));
            DataOutputStream outToTransport = new DataOutputStream(connectionSocket.getOutputStream());
            //recebe da camada de aplicação
            String msg = inFromTransport.readLine();

			//socket que envia pra física
		    Socket socketSend = new Socket("localhost", 13001);
		    DataOutputStream outToPhysical = new DataOutputStream(socketSend.getOutputStream());
		    BufferedReader inFromPhysical = new BufferedReader(new InputStreamReader(socketSend.getInputStream()));
			//envia pra física
		    outToPhysical.writeBytes(msg + '\n');

		    //recebe resposta
		    String answ = inFromPhysical.readLine();
        System.out.println(answ);
        	//repassa resposta
            outToTransport.writeBytes(answ + '\n');
        }
    }
}
