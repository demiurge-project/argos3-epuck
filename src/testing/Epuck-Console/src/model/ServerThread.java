package model;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

import view.Window;
import controller.Controller;


public class ServerThread extends Thread {
	
	private Socket socket=null;
	private Controller controller=null;
	private Window window=null;
	private int id;
	private String msg = "NULL";
	
	public ServerThread(Socket socket, Window window) {
		this.socket = socket;
		this.window = window;
		System.out.println(this.socket);
	}

	public void run() {
		try {
			PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
			BufferedReader in = new BufferedReader(
					new InputStreamReader(socket.getInputStream()));
			
			String inputLine;
			if((inputLine = in.readLine()) != null) {
				if(inputLine.matches("[0-9]+")) {
					System.out.println("Id received by the client: "+inputLine);
					this.id = Integer.parseInt(inputLine);
					this.controller = new Controller(this, this.window, this.id);
					
					while ((inputLine = in.readLine()) != null) {
						//System.out.println("Message received by the client: "+inputLine);
						this.controller.parse(inputLine);
						out.println(this.msg);
						if(inputLine.equals("end"))
							break;
					}
				} else {
					System.out.println("The first message should contain only the epuck's id (integer number).");
				}
			}
			this.socket.close();
			System.out.println("End");
			this.controller.removeTabFromWindow();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	public void msgTosend(String msg) {
		this.msg = msg;
	}
}
