package main;

import java.io.File;
import java.io.IOException;
import java.util.concurrent.CountDownLatch;
import javax.swing.SwingUtilities;

import javafx.embed.swing.JFXPanel;
import javafx.scene.media.Media;
import javafx.scene.media.MediaPlayer;
import jssc.SerialPort;
import jssc.SerialPortException;


public class API {
	static SerialPort serialPort = new SerialPort("COM10");
	static byte[] buffer = new byte[16] ;
	
    public static void initToolkit() throws InterruptedException{
        final CountDownLatch latch = new CountDownLatch(1);
        SwingUtilities.invokeLater(() -> {
            new JFXPanel();
            latch.countDown();
        });
    }
	 void buildGUI() {
		 try {
			initToolkit();
		} catch (InterruptedException e) {}
		
		 
	        SwingUtilities.invokeLater( () -> {
	        	new Graphic();
	        } );
	    }
	    
	    public static void main( String[] a ) throws IOException {
			
   			try {
				serialPort.openPort();
			} catch (SerialPortException e1) {
				e1.printStackTrace();
			}
   			try {
				serialPort.setParams(SerialPort.BAUDRATE_38400, 
				                         SerialPort.DATABITS_8,
				                         SerialPort.STOPBITS_1,
				                         SerialPort.PARITY_NONE);
			} catch (SerialPortException e) {
				e.printStackTrace();
			}
	        (new API()).buildGUI();
	    }

}
