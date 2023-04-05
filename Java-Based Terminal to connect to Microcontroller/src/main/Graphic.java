package main;

import java.awt.Color;
import java.awt.Font;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.util.ArrayList;
import java.util.concurrent.TimeUnit;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSlider;
import javax.swing.JTextField;
import javax.swing.WindowConstants;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

import javafx.scene.media.Media;
import javafx.scene.media.MediaPlayer;
import jssc.SerialPort;
import jssc.SerialPortEvent;
import jssc.SerialPortEventListener;
import jssc.SerialPortException;;

public class Graphic  extends JPanel{
	
	private static final long serialVersionUID = 1L;
	private JFrame frame = new JFrame("Uart Connection"); 

	private JButton reset_btn;
	private static JButton play_pause_btn;
	private JButton left_right_btn;
	
	public static JTextField nameField;
	public static JButton send_btn;
	private JPanel main_panel;
	private JLabel high_score_label, name_label, time__label, score_label;
	private JLabel showScore, showName, lock_label;
	static JLabel level_label, light_percent_label;
	private JLabel temp_level_label;
	static JLabel progressive_score_label, heart_label, state_label;
	private JLabel[][] list = new JLabel[5][3];
	public ArrayList<Obj> obj_list = new ArrayList<>(); 
	public static int score;
	public static String name = new String("");
	static Media hit = new Media(new File("finish.mp3").toURI().toString());
	static MediaPlayer mediaPlayer = new MediaPlayer(hit);
	static int tmp_lvl = 0;
	static JButton[][] keypad = new JButton[4][3];
	static Obj obj = new Obj("", -1, -1, -1, -1, -1, -1, -1);
	static JSlider slider;
	JPanel key_panel ;
	public Graphic(){
        
		reset_btn = new JButton("Reset");
		play_pause_btn = new JButton("Start");
		left_right_btn = new JButton("Left/Right");
		send_btn = new JButton("Send");
		nameField = new JTextField(" ");
		high_score_label = new JLabel("High Scores");
		name_label = new JLabel("Names");
		time__label = new JLabel("Time");
		score_label = new JLabel("Scores");
		level_label = new JLabel("Level : ");
		progressive_score_label = new JLabel("Score : ");
		showName = new JLabel("");
		showScore = new JLabel("");
		temp_level_label = new JLabel("Temp Lvl :");
		heart_label = new JLabel("Heart : ");
		light_percent_label = new JLabel("Light : ");
		lock_label = new JLabel("Lock");
		state_label = new JLabel("Stoped");
		showName.setVisible(false);
		showScore.setVisible(false);
		
		
		
		for(int i=0; i<5; i++)
			obj_list.add(new Obj("name_" + (i+1), -1, -1, -1, -1, -1, -1, -1));
		

		for(int i=0; i<5; i++)
			for(int j=0; j<3; j++){
				if(j == 0){
					list[i][j] = new JLabel(obj_list.get(i).name);
					list[i][j].setFont(new Font("Verdana", Font.BOLD, 15));
					list[i][j].setBounds(95, 390 + (i * 40), 100, 40);;
					this.add(list[i][j]);
				}
				else if(j == 1){
					list[i][j] = new JLabel("0");
					list[i][j].setFont(new Font("Verdana", Font.BOLD, 15));
					list[i][j].setBounds(280, 390 + (i * 40), 100, 40);;
					this.add(list[i][j]);
				}
				else if(j == 2){
					list[i][j] = new JLabel(obj_list.get(i).hr + ":" + obj_list.get(i).min + ":" + 
							obj_list.get(i).sec + "    " +  obj_list.get(i).year + "/" + 
							obj_list.get(i).month + "/" + obj_list.get(i).hr);
					list[i][j].setFont(new Font("Verdana", Font.BOLD, 15));
					list[i][j].setBounds(370, 390 + (i * 40), 200, 40);;
					this.add(list[i][j]);
				
				}
			}
		
		Thread thread = new Thread(new Runnable(){
			@Override
			public void run(){
				try {
					API.serialPort.addEventListener(new PortReader(), SerialPort.MASK_RXCHAR);
				} catch (SerialPortException e) {
					
					e.printStackTrace();
				}
			}
		});
		
		thread.start();
		
		reset_btn.addActionListener(new ActionListener(){
        	@Override
        	public void actionPerformed(ActionEvent event) {
        		try {
        			System.out.println("ads");
        			API.buffer = new byte[16];
        			API.buffer[0] = 'r';
        			API.buffer[1] = 'e';
        			API.buffer[2] = 's';
//        			Graphic.hit = new Media(new File("car_move.mp3").toURI().toString());
//                	Graphic.mediaPlayer = new MediaPlayer(Graphic.hit);
//                	Graphic.mediaPlayer.play();
        			
       			 	API.serialPort.writeBytes(API.buffer);
       	        }
       	        catch (SerialPortException ex) {
       	            System.out.println(ex);
       	        }
        	}
        });
		play_pause_btn.addActionListener(new ActionListener(){
        	@Override
        	public void actionPerformed(ActionEvent event){
        		for(int i=0; i<4; i++)
                    for(int j=0; j<3; j++) 
                    	keypad[i][j].setEnabled(false);
                    	
                    
        		try {
        			API.buffer = new byte[16];
        			if(play_pause_btn.getText().equals("Pause")){
        				API.buffer[0] = 'p';
            			API.buffer[1] = 'a';
            			API.buffer[2] = 's';
            			play_pause_btn.setText("Play");
            			state_label.setText("Paused");
        			}
        			else if(play_pause_btn.getText().equals("Play")){
	        			API.buffer[0] = 'p';
	        			API.buffer[1] = 'l';
	        			API.buffer[2] = 'y';
	        			play_pause_btn.setText("Pause");
	        			state_label.setText("Playing");
        			}
        			else if(play_pause_btn.getText().equals("Start")){
           			 	
	        			API.buffer[0] = 's';
	        			API.buffer[1] = 't';
	        			API.buffer[2] = 'r';
	        			play_pause_btn.setText("Pause");
	        			state_label.setText("Playing");
        			}
        			else if(play_pause_btn.getText().equals("New Start")){
        				System.out.println("asd");
	        			API.buffer[0] = 'n';
	        			API.buffer[1] = 's';
	        			API.buffer[2] = 't';
	        			play_pause_btn.setText("Start");
	        			state_label.setText("Stoped");
        			}
       			 	API.serialPort.writeBytes(API.buffer);
       			 	
       	        }
       	        catch (SerialPortException ex) {
       	            System.out.println(ex);
       	        }
        	}
        });
		left_right_btn.addActionListener(new ActionListener(){
        	@Override
        	public void actionPerformed(ActionEvent event){
        		try {
        			API.buffer = new byte[16];
        			API.buffer[0] = 'l';
        			API.buffer[1] = 'r';
        			API.buffer[2] = 'b';
       			 	API.serialPort.writeBytes(API.buffer);
       			 	Graphic.hit = new Media(new File("car_l_r.mp3").toURI().toString());
       			 	Graphic.mediaPlayer = new MediaPlayer(Graphic.hit);
       			 	Graphic.mediaPlayer.play();
       	        }
       	        catch (SerialPortException ex) {
       	            System.out.println(ex);
       	        }
        	}
        });
		send_btn.addActionListener(new ActionListener(){
        	@Override
        	public void actionPerformed(ActionEvent event){
        		showName.setVisible(true);
        		showScore.setVisible(true);
        		showName.setText(nameField.getText());
        		showScore.setText(score + "");
            	nameField.setEnabled(false);
            	send_btn.setEnabled(false);
            	obj.name = nameField.getText();
            	edit_high_score(obj);
        		
        	}
        });
		
		
		ActionListener keypad_listener = new ActionListener() {
			
            @Override
            public void actionPerformed(ActionEvent e) {
//                if (e.getSource() instanceof JButton) {
                    String text = ((JButton) e.getSource()).getText();
                    if(text.equals("Go")){
//                    	System.out.println(tmp_lvl);
                    	API.buffer = new byte[16];
                    	if(tmp_lvl > 0 && tmp_lvl < 11){
                			API.buffer[0] = 's';
                			API.buffer[1] = 'f';
                			API.buffer[2] = 'l';// set finit level
                			API.buffer[3] = (byte) tmp_lvl ;	
                			
               			 	try {
								API.serialPort.writeBytes(API.buffer);
								API.serialPort.writeBytes(API.buffer);
							} catch (SerialPortException e1) {
								e1.printStackTrace();
							}
                    	}
                    	else if(tmp_lvl == 11){
                			API.buffer[0] = 's';
                			API.buffer[1] = 'i';
                			API.buffer[2] = 'l';// set infinite level
                			API.buffer[3] = (byte) tmp_lvl ;
                			
               			 	try {
								API.serialPort.writeBytes(API.buffer);
								API.serialPort.writeBytes(API.buffer);
							} catch (SerialPortException e1) {
								e1.printStackTrace();
							}
                    	}
                    	else {
                    		tmp_lvl = 0;
                        	temp_level_label.setText("Invalid");
                    	}
                    		
                    		
                    }
                    else if(text.equals("Cls")){
                    	tmp_lvl = 0;
                    	temp_level_label.setText("Temp Lvl : " + tmp_lvl);
                    }
                    else{
                    	tmp_lvl *= 10;
                    	tmp_lvl += Integer.parseInt(text);
                    	temp_level_label.setText("Temp Lvl : " + tmp_lvl);
//                    	System.out.println(text);
                    }
//                }
            }
        };
		
		
		for(int i=0; i<4; i++)
            for(int j=0; j<3; j++) {
            	keypad[i][j] = new JButton((3*i+j)+1 + "");
            	keypad[i][j].addActionListener(keypad_listener);
            	(keypad[i][j]).setBackground(Color.black);
            	(keypad[i][j]).setForeground(Color.white);
            	(keypad[i][j]).setFont(new Font("Verdana", Font.BOLD, 15));
            }
		keypad[3][0].setText("Cls");
		keypad[3][1].setText("0");
		keypad[3][2].setText("Go");
		key_panel = new JPanel();
		key_panel.setLayout(new GridLayout(4, 3));
        for (int i = 0; i < 4; i++) 
            for (int j = 0; j < 3; j++) 
            	key_panel.add(keypad[i][j]);
            
        

		
		slider = new JSlider(JSlider.HORIZONTAL, 0, 10, 0); 
        slider.setBackground(Color.white);
        slider.setMinorTickSpacing(1);  
        slider.setMajorTickSpacing(10); 
        slider.setPaintTicks(true);  
        slider.setPaintLabels(true); 
        slider.addChangeListener(new ChangeListener() {
            public void stateChanged(ChangeEvent event) {
                    int value = slider.getValue();            
                    API.buffer = new byte[16];
        			API.buffer[0] = 's';
        			API.buffer[1] = 'p';
        			API.buffer[2] = 'd';
        			API.buffer[3] = (byte)(value);
       			 	try {
						API.serialPort.writeBytes(API.buffer);
						
					} catch (SerialPortException e) {
						e.printStackTrace();
					}
            }
          });
		
		main_panel = new JPanel();
		 
		main_panel.setBackground(Color.BLACK);
		this.setBackground(Color.white);
	    main_panel.setLayout(null);
	    this.setLayout(null);
	    
	    key_panel.setBounds(340, 85, 180, 180);
	    reset_btn.setBounds(60, 25, 110, 35);
	    play_pause_btn.setBounds(180, 25, 140, 35);
	    left_right_btn.setBounds(80, 80, 200, 35);
	    left_right_btn.setFont(new Font("Verdana", Font.BOLD, 15));
	    play_pause_btn.setFont(new Font("Verdana", Font.BOLD, 15));
	    reset_btn.setFont(new Font("Verdana", Font.BOLD, 15));
	   
	    slider.setBounds(330, 25, 200, 50);
	    

	    nameField.setFont(new Font("Verdana", Font.BOLD, 15));
	    nameField.setBounds(55, 140, 150, 35);
	    send_btn.setFont(new Font("Verdana", Font.BOLD, 15));
	    send_btn.setBounds(205, 140, 100, 35);
	    nameField.setEnabled(false);
	    send_btn.setEnabled(false);
	    
//	    showName.setBounds(160, 135, 100, 35);
//	    showScore.setBounds(270, 135, 100, 35);
//	    showName.setFont(new Font("Verdana", Font.BOLD, 15));
//	    showScore.setFont(new Font("Verdana", Font.BOLD, 15));
//	    showName.setForeground(Color.blue);
//	    showScore.setForeground(Color.red);
	    
	    temp_level_label.setFont(new Font("Verdana", Font.BOLD, 15));
	    temp_level_label.setBounds(375, 275, 200, 35);
	    
	    level_label.setFont(new Font("Verdana", Font.BOLD, 15));
	    level_label.setBounds(70, 190, 150, 35);
	    progressive_score_label.setFont(new Font("Verdana", Font.BOLD, 15));
	    progressive_score_label.setBounds(200, 190, 150, 35);
	    heart_label.setFont(new Font("Verdana", Font.BOLD, 15));
	    heart_label.setBounds(70, 230, 200, 35);
	    light_percent_label.setFont(new Font("Verdana", Font.BOLD, 15));
	    light_percent_label.setBounds(200, 230, 200, 35);
	    lock_label.setFont(new Font("Verdana", Font.BOLD, 15));
	    lock_label.setBounds(70, 270, 200, 35);
	    state_label.setFont(new Font("Verdana", Font.BOLD, 15));
	    state_label.setBounds(200, 270, 200, 35);
	    
	    
	    high_score_label.setBounds(245, 310, 100, 40);
	    high_score_label.setFont(new Font("Verdana", Font.BOLD, 15));
	    name_label.setBounds(100, 355, 100, 40);
	    name_label.setFont(new Font("Verdana", Font.BOLD, 15));
	    score_label.setBounds(260, 355, 100, 40);
	    score_label.setFont(new Font("Verdana", Font.BOLD, 15));
	    time__label.setBounds(430, 355, 100, 40);
	    time__label.setFont(new Font("Verdana", Font.BOLD, 15));
	    
	 
	    
	    show_graphic();
	}
	public void edit_high_score(Obj obj){
		try{
		for(int i=0; i<obj_list.size(); i++ )
			if(obj_list.get(i).score < obj.score){
				obj_list.add(i, obj);
				break;
			}
		}catch(Exception e){e.printStackTrace();}
		for(int i=0; i<5; i++)
			for(int j=0; j<3; j++){
				if(j == 0)
					list[i][j].setText(obj_list.get(i).name);

				
				else if(j == 1)
					list[i][j].setText(obj_list.get(i).score + "");

				else if(j == 2)
					list[i][j].setText(obj_list.get(i).hr + ":" + obj_list.get(i).min + ":" + 
							obj_list.get(i).sec + "    " +  obj_list.get(i).year + "/" + 
							obj_list.get(i).month + "/" + obj_list.get(i).hr);
			}
		
		show_graphic();
	}
	public void show_graphic(){
		this.add(key_panel);
        this.setBounds(11, 10, 570, 650);
        this.add(progressive_score_label);
        this.add(level_label);
        this.add(heart_label);
        this.add(light_percent_label);
        this.add(slider);
	    this.add(reset_btn);
	    this.add(left_right_btn);
	    this.add(play_pause_btn);
	    this.add(nameField);
	    this.add(send_btn);
	    this.add(showName);
	    this.add(showScore);
	    this.add(high_score_label);
	    this.add(temp_level_label);
	    this.add(name_label);
	    this.add(score_label);
	    this.add(time__label);
	    this.add(state_label);
	    this.add(lock_label);
	    main_panel.add(this);
	    
	    frame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
	    frame.add(main_panel);
        frame.setSize(600, 700);
        frame.setLocationRelativeTo(null);
        frame.setResizable(false);
        frame.getContentPane();
        frame.setVisible(true);
	}
	
	
	private static class PortReader implements SerialPortEventListener {
		Obj obj = new Obj("", -1, -1, -1, -1, -1, -1, -1);
		String receivedData = new String("");
		boolean game_over_arrived = false, time_arrived = false, date_arrived = false;
		boolean level_arrived = false, score_arrived = false, heart_arrived = false;
		boolean light_arrived = false, state_arrived = false;
		

	    @Override
	    public void serialEvent(SerialPortEvent event) {
//	    	System.out.println("Received response: " + receivedData);
	        if(event.isRXCHAR() && event.getEventValue() > 0) {
	            try {
	                receivedData += API.serialPort.readString(event.getEventValue());
	                
	                if(receivedData.contentEquals("go")){
	                	System.out.println("Received response: " + receivedData);
	                	game_over_arrived = true;
	                	Graphic.nameField.setEnabled(true);
	                	Graphic.send_btn.setEnabled(true);
	                	Graphic.state_label.setText("Game Over");
	                	Graphic.play_pause_btn.setText("New Start");
	                	obj = new Obj("", -1, -1, -1, -1, -1, -1, -1);
	                	
	                	
	                	obj.name = name;
	                	
	                	receivedData = new String();
	                }
	                if(receivedData.length() == 5 && game_over_arrived){
	                	Graphic.score = Integer.parseInt(receivedData.trim());
//	                	System.out.println("Received response: " + Graphic.score);
	                	game_over_arrived = false;
	                	obj.score = Graphic.score;
	                	receivedData = new String();
	                }
	                if(receivedData.contentEquals("ti")){
	                	time_arrived = true;
	                	receivedData = new String();
	                }
	                if(receivedData.length() == 6 && time_arrived){
	                	obj.sec =  Integer.parseInt((receivedData.substring(0, 2)).trim());
	                	obj.min =  Integer.parseInt((receivedData.substring(2, 4)).trim());
	                	obj.hr =  Integer.parseInt((receivedData.substring(4, 6)).trim());

	                	time_arrived = false;
	                	receivedData = new String();
	                }
	                if(receivedData.contentEquals("da")){
	                	date_arrived = true;
	                	receivedData = new String();
	                }
	                if(receivedData.length() == 6 && date_arrived){
	                	obj.day =  Integer.parseInt((receivedData.substring(0, 2)).trim());
	                	obj.month =  Integer.parseInt((receivedData.substring(2, 4)).trim());
	                	obj.year =  Integer.parseInt((receivedData.substring(4, 6)).trim());
	                	
	                	date_arrived = false;
	                	receivedData = new String();
	                	Graphic.obj = this.obj;
	                	
	                	System.out.println(obj.name + " " + obj.score + " " + obj.hr + " " + obj.min + " " + obj.sec + " " + obj.day + " " + obj.month + " " + obj.year);
	                	Graphic.hit = new Media(new File("finish.mp3").toURI().toString());
	                	Graphic.mediaPlayer = new MediaPlayer(Graphic.hit);
	                	Graphic.mediaPlayer.play();
	                	for(int i=0; i<4; i++)
	                        for(int j=0; j<3; j++) 
	                        	keypad[i][j].setEnabled(true);

	                }
	                
	                if(receivedData.contentEquals("ul")){
//	                	System.out.println("Received response: " + receivedData);
	                	level_arrived = true;
	                	receivedData = new String();
	                }
	                if(receivedData.length() == 2 && level_arrived){
//	                	System.out.println("Received response: " + receivedData);
	                	int level = Integer.parseInt(receivedData.trim());
	                	Graphic.level_label.setText("Level : " + level);
	                	level_arrived = false;
	                	receivedData = new String();
	                }
	                if(receivedData.contentEquals("us")){
//	                	System.out.println("Received response: " + receivedData);
	                	score_arrived = true;
	                	receivedData = new String();
	                }
	                if(receivedData.length() == 5 && score_arrived){
//	                	System.out.println("Received response: " + receivedData);
	                	int score = Integer.parseInt(receivedData.trim());
	                	Graphic.progressive_score_label.setText("Score : " + score);
	                	score_arrived = false;
	                	receivedData = new String();
	                }
	                if(receivedData.contentEquals("uh")){
//	                	System.out.println("Received response: " + receivedData);
	                	heart_arrived = true;
	                	receivedData = new String();
	                }
	                if(receivedData.length() == 1 && heart_arrived){
//	                	System.out.println("Received response: " + receivedData);
	                	int heart = Integer.parseInt(receivedData.trim());
	                	Graphic.heart_label.setText("Heart : " + heart);
	                	heart_arrived = false;
	                	receivedData = new String();
	                }
	                if(receivedData.contentEquals("lp")){//light percent
//	                	System.out.println("Received response: " + receivedData);
	                	light_arrived = true;
	                	receivedData = new String();
	                }
	                if(receivedData.length() == 3 && light_arrived){
//	                	System.out.println("Received response: " + receivedData);
	                	int light = Integer.parseInt(receivedData.trim());
	                	Graphic.light_percent_label.setText("Light : " + light);
	                	light_arrived = false;
	                	receivedData = new String();
	                }
	                if(receivedData.contentEquals("ss")){//send state
//	                	System.out.println("Received response: " + receivedData);
	                	
	                	Graphic.hit = new Media(new File("lvl_finish.mp3").toURI().toString());
	                	Graphic.mediaPlayer = new MediaPlayer(Graphic.hit);
	                	Graphic.mediaPlayer.play();
	                	
	                	Graphic.slider.setValue(0);
	                	API.buffer = new byte[16];
	        			API.buffer[0] = 's';
	        			API.buffer[1] = 'p';
	        			API.buffer[2] = 'd';
	        			API.buffer[3] = (byte)(Graphic.slider.getValue());
	       			 	try {
							API.serialPort.writeBytes(API.buffer);
						} catch (SerialPortException e) {}
	       			 	
	       			 	Graphic.state_label.setText("Stoped");
	       			 	Graphic.play_pause_btn.setText("Start");
	       			 	
	                	receivedData = new String();
	                }	
	                if(receivedData.contentEquals("ps")){//play sound
//	                	System.out.println("Received response: " + receivedData);
	                	receivedData = new String();
	                	Graphic.hit = new Media(new File("car_move.mp3").toURI().toString());
	                	Graphic.mediaPlayer = new MediaPlayer(Graphic.hit);
	                	Graphic.mediaPlayer.play();
	                }
	                
	            }
	            catch (SerialPortException ex) {
	                System.out.println("Error in receiving string from COM-port: " + ex);
	            }
	        }
	    }

	}
	

}


