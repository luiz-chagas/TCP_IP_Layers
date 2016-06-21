/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package networklayer;

/**
 *
 * @author rapha
 */
public class Route {
    
    private String mask;
    private String destiny;

    public Route(String mask, String destiny) {
        this.mask = mask;
        this.destiny = destiny;
    }

    public String getMask() {
        return mask;
    }

    public void setMask(String mask) {
        this.mask = mask;
    }

    public String getDestiny() {
        return destiny;
    }

    public void setDestiny(String destiny) {
        this.destiny = destiny;
    }
    
    
}
