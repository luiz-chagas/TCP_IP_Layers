/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package networklayer;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Scanner;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.regex.Pattern;

/**
 *
 * @author rapha
 */
public class NetworkLayer {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {

        /*
        try {
            ServerSocket servidor = new ServerSocket(3000);
            Socket cliente = servidor.accept();
            System.out.println("Nova conexão com o cliente "
                    + cliente.getInetAddress().getHostAddress()
            );

            Scanner s = new Scanner(cliente.getInputStream());
            
            while (s.hasNextLine()) {
                System.out.println(s.nextLine());
            }

            s.close();
            servidor.close();
            cliente.close();
*/
            RouteTable route = new RouteTable();
            //String routes[][]={{"255.255.253","255.255.254"},{"255.255.253","255.255.254"},{"255.255.253","255.255.254"}};
            //route.createTable(routes);
            //System.out.println(route.getRoutes()[2][1]);
            System.out.println(route.compareNetmask("127.57.76.251"));
        //} catch (IOException ex) {
            //Logger.getLogger(NetworkLayer.class.getName()).log(Level.SEVERE, null, ex);
       // }

    }

}
