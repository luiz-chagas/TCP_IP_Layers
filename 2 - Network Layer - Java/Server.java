import java.io.*;
import java.net.*;

public class Server {
    public static void main(String args[]) throws Exception {
        ServerSocket welcomeSocket = new ServerSocket(12000);

        while(true) {
        	//socket que recebe da camada física
            Socket connectionSocket = welcomeSocket.accept();
            BufferedReader inFromPhysical = new BufferedReader(new InputStreamReader(connectionSocket.getInputStream()));
            DataOutputStream outToPhysical = new DataOutputStream(connectionSocket.getOutputStream());
            //recebe da física
            String answ = inFromPhysical.readLine();

			//socket que envia pro transporte
		    Socket socketSend = new Socket("localhost", 11000);
		    DataOutputStream outToTransport = new DataOutputStream(socketSend.getOutputStream());
		    BufferedReader inFromTransport = new BufferedReader(new InputStreamReader(socketSend.getInputStream()));
			//envia pro transporte
		    outToTransport.writeBytes(answ + '\n');
		    
		    //recebe resposta
		    String msg = inFromTransport.readLine();
		    System.out.println(msg);
		    //repassa resposta
            outToPhysical.writeBytes(msg + '\n');
        }
    }
}
