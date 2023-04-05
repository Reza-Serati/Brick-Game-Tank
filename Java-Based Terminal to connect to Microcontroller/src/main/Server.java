package main;

import java.net.*;
import java.io.*;
import java.util.*;


public class Server {
	public static void main(String[] args) {
     
		Socket s = null;
        ObjectOutputStream out = null;  
        System.out.println("Connecting to Server ...");
        try {
            s = new Socket("localhost", 81);
            out = new ObjectOutputStream (s.getOutputStream());
            
//            ArrayList<String> list = new ArrayList<String>();
//		    for (int i = 0; i < 10 ; i++){
//		        list.add("String: "+i);
//		    }
            
		    out.writeUTF("Salam");
            out.flush();
            
        System.out.println("ArrayList sent to Server");
        } catch ( Exception e) {
            e.printStackTrace();
        }
        finally{
            if (out!= null){
                try{
                    out.close();
                }
                catch (Exception ex){
                	ex.printStackTrace();
                }
            }
            if (s != null){
                try{
                    s.close();
                }
                catch (Exception ex){
                	ex.printStackTrace();
                }
            }
        }
    }
}
