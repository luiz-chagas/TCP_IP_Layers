
package networklayer;

public class RouteTable {
    
    private Route routes[];
    private int numRoutes;
    
    public void createTable(){
        
    }

    public Route[] getRoutes() {
        return routes;
    }

    public void setRoutes(int numRoutes, Route[] routes) {
        this.numRoutes = numRoutes;
        this.routes = routes;
    }

    public int getNumRoutes() {
        return numRoutes;
    }
}
