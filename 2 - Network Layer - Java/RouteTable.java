package networklayer;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.regex.Pattern;

public class RouteTable {

    private static final String FILE = "routesTable.txt";
    private String routes[][];// IpDestino - NetMask - Gateway
    private int numRoutes;

    public RouteTable() {
        int i=0;
        
        try {         
            FileReader file = new FileReader("src/networklayer/"+FILE);
            BufferedReader readFile = new BufferedReader(file);
            String line = readFile.readLine();  
            numRoutes = Integer.parseInt(line);
            routes = new String [numRoutes][3];
            line = readFile.readLine();
            while (line != null) {
                routes[i][0] = line;
                line = readFile.readLine();
                routes[i][1] = line;
                line = readFile.readLine();
                routes[i][2] = line;
                line = readFile.readLine();                
                i++;
            }
            file.close();
        } catch (IOException fileNotFound) {
            System.err.printf("Rotas não foram previamente definidas\n",
                    fileNotFound.getMessage());
        } catch(NumberFormatException emptyFile){
            System.err.printf("Arquivo de Rotas Vazio\n",
                    emptyFile.getMessage());
        }

    }

    public void createTable(String[][] routes) {
        this.routes = routes;
        numRoutes = routes.length;
        try {
            FileWriter file = new FileWriter("src/networklayer/"+FILE);
            PrintWriter writeFile = new PrintWriter(file);
            writeFile.println(routes.length);
            for(int i=0;i<routes.length;i++){
                writeFile.println(routes[i][0]);
                writeFile.println(routes[i][1]);
                writeFile.println(routes[i][2]);
            }
            file.close();
        } catch (IOException ex) {
            Logger.getLogger(RouteTable.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    public String compareNetmask(String ipDest){
        int i=0,j;
        String[] aux1;
        int[] aux2 = new int [4];
        aux1 = ipDest.split(Pattern.quote("."));;
        for(j=0; j<aux2.length;j++){
            aux2[j] = Integer.parseInt(aux1[j]);
        }
        while(i<routes.length){
            aux1 = routes[i][1].split(Pattern.quote("."));;
            for(j=0; j<aux2.length;j++){
                aux2[j] = (aux2[j] & (Integer.parseInt(aux1[j])));
            }
            if(routes[i][0].equals(aux2[0]+"."+aux2[1]+"."+aux2[2]+"."+aux2[3])){
                return routes[i][2];
            }
            i++;
        }
        return "RouteNotFound";
    }
    
    private static boolean andIpMask(int ip, int mask){
        return ((ip & mask) == ip);
    }

    public String[][] getRoutes() {
        return routes;
    }

    public int getNumRoutes() {
        return numRoutes;
    }
}
