
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;

import javax.swing.SwingUtilities;

public class API {
	    
	    public static void main( String[] a ) throws IOException {
	    	final int portNumber = 81;
			System.out.println("Creating server socket on port " + portNumber);
			ServerSocket serverSocket = new ServerSocket(portNumber);
			while (true) {
				System.out.println("1");
				Socket socket = serverSocket.accept();
				OutputStream os = socket.getOutputStream();
				PrintWriter pw = new PrintWriter(os, false);
				pw.println("What's your name?");

				BufferedReader br = new BufferedReader(new InputStreamReader(socket.getInputStream()));
				
				String str = br.readLine();

				if (str.equals("go")) {
					System.out.println("Go");
				} else {
					System.out.println("Don't Go");
				}

				pw.println("Hello, " + str);
				
				pw.flush();
//				socket.close();

				System.out.println("Just said hello to:" + str);
			}
			
//   			try {
//				serialPort.openPort();
//			} catch (SerialPortException e1) {
//				e1.printStackTrace();
//			}
//   			try {
//				serialPort.setParams(SerialPort.BAUDRATE_38400, 
//				                         SerialPort.DATABITS_8,
//				                         SerialPort.STOPBITS_1,
//				                         SerialPort.PARITY_NONE);
//			} catch (SerialPortException e) {
//				e.printStackTrace();
//			}
//	        (new API()).buildGUI();
	    }

}
