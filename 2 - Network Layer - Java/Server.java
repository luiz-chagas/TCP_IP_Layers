import java.io.*;
import java.net.*;

public class Server {
    public static void main(String args[]) throws Exception {
        ServerSocket welcomeSocket = new ServerSocket(12000);
        BufferedReader inFromTransport = null, inFromPhysical = null;
    	DataOutputStream outToTransport = null, outToPhysical = null;
		int flag = 0;

        while(true) {
        	//socket que recebe da camada física
        	if(flag == 0) {
		        Socket connectionSocket = welcomeSocket.accept();
		        inFromPhysical = new BufferedReader(new InputStreamReader(connectionSocket.getInputStream()));
		        outToPhysical = new DataOutputStream(connectionSocket.getOutputStream());
            }
            //recebe da física
            String answ = inFromPhysical.readLine();
System.out.println(answ);
			//socket que envia pro transporte
			if(flag == 0) {
				Socket socketSend = new Socket("localhost", 11000);
				outToTransport = new DataOutputStream(socketSend.getOutputStream());
				inFromTransport = new BufferedReader(new InputStreamReader(socketSend.getInputStream()));
		    }
			//envia pro transporte
		    outToTransport.writeBytes(answ + '\n');
		    
		    //recebe resposta
		    String msg = inFromTransport.readLine();
		    
		    //repassa resposta
            outToPhysical.writeBytes(msg + '\n');
            flag++;
        }
    }
}
