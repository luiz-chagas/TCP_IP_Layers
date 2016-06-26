/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package networklayer;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.util.logging.Level;
import java.util.logging.Logger;

public class NetworkLayer {

    public static final int receivePort = 3000;
    public static final int sendPort = 3001;
    public static final int portNumber = 3002;
    public static final String localHost = "127.0.0.1";
    public static final String physicalLayer = "";
    
    
    public static void main(String[] args) {
        try {
            while (true) {
                //Abre a tabela de Rotas
                RouteTable route = new RouteTable();
                //Ouve porta
                DatagramSocket ds = new DatagramSocket(receivePort);
                byte[] msg = new byte[4024]; //Buffer para o dado
                //recebe e fechar o socket
                DatagramPacket pkg = new DatagramPacket(msg, msg.length);
                ds.receive(pkg);
                ds.close();
                
                String gateway = route.compareNetmask(new String(pkg.getData()));
                if(gateway.equals("RouteNotFound")){
                    System.out.println(gateway + " - Pacote Descartado");
                }
                else if(InetAddress.getLocalHost().getHostAddress().equals(gateway)){
                    InetAddress addr = InetAddress.getByName(localHost);
                    msg = pkg.getData();
                    pkg = new DatagramPacket(msg,msg.length, addr,sendPort);
                    ds.send(pkg);
                }
                else{
                    Runtime.getRuntime().exec(physicalLayer+" "+msg+" "+gateway+" "+portNumber);
                }

            }
        } catch (IOException eConection) {
            Logger.getLogger(NetworkLayer.class.getName()).log(Level.SEVERE, null, eConection);
            System.err.printf("Arquivo de Rotas Vazio\n",
                    eConection.getMessage());
        }

    }

}
