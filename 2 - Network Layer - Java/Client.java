import java.io.*;
import java.net.*;
import java.util.concurrent.TimeUnit;

public class Client {
    public static void main(String argv[]) throws Exception {
    	ServerSocket socketRecv = new ServerSocket(12001);
    	BufferedReader inFromTransport = null, inFromPhysical = null;
    	DataOutputStream outToTransport = null, outToPhysical = null;
		int flag = 0;
		
        while(true) {
        	//socket que recebe da camada de transporte
        	if(flag == 0) {
		        Socket connectionSocket = socketRecv.accept();
		        inFromTransport = new BufferedReader(new InputStreamReader(connectionSocket.getInputStream()));
		        outToTransport = new DataOutputStream(connectionSocket.getOutputStream());
            }
            
            //recebe da camada de aplicação
            String msg = inFromTransport.readLine();

			//socket que envia pra física
			if(flag == 0) {
				Socket socketSend = new Socket("localhost", 13001);
				outToPhysical = new DataOutputStream(socketSend.getOutputStream());
				inFromPhysical = new BufferedReader(new InputStreamReader(socketSend.getInputStream()));
		    }
		    
			//envia pra física
		    outToPhysical.writeBytes(msg + '\n');

		    //recebe resposta
		    String answ = inFromPhysical.readLine();
        
        	//repassa resposta
            outToTransport.writeBytes(answ + '\n');
            flag ++;
        }
    }
}
