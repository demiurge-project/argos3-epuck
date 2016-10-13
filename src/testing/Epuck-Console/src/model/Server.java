package model;
import java.io.IOException;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketException;
import java.util.Enumeration;

import view.Window;


public class Server {

	private int portNumber;
	private ServerSocket serverSocket=null;
	private Window window=null;
	
	public Server(int portnumber) {
		System.out.println("Server: initialisation");
		try {
			Enumeration e = NetworkInterface.getNetworkInterfaces();
			while(e.hasMoreElements()) {
			    NetworkInterface n = (NetworkInterface) e.nextElement();
			    Enumeration ee = n.getInetAddresses();
			    String str="";
			    while (ee.hasMoreElements()) {
			        InetAddress i = (InetAddress) ee.nextElement();
			        str = i.getHostAddress();
			        if (str.startsWith("10.0.1.")) {
			        	System.out.println("Current IP address (epuck): " + str);
			        }
			    }
			}
		} catch (SocketException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		
		this.setPortNumber(portnumber);
		this.start();
	}
	
	public Server() {
		this(1050);
	}
	
	private void setPortNumber(int portnumber) {
		//There is 2^16-1 UDP/TCP ports allocated.
		//The first 1024 port numbers are reserved for common applications (ftp,ssh,http,...)
		if(portnumber < 1024 || portnumber > 65535) {
			this.portNumber = 1050;
		} else {
			this.portNumber = portnumber;
		}
		System.out.println("Port: " + this.portNumber);
	}
	
	private void start() {
		try {
			System.out.println("Creating the server socket...");
			this.serverSocket = new ServerSocket(this.portNumber);
			System.out.println("The server socket has been created: " + 
					this.serverSocket.getLocalSocketAddress());
			if(this.serverSocket != null) {
				this.window = new Window();
				while(true) {
					Socket socket = this.serverSocket.accept();
					System.out.println("Server: client detected, proceeding its request...");
					new ServerThread(socket, this.window).start();
				}
			}
		} catch (IOException e) {
			System.out.println("Server: Exception caught when trying to listen on port "
	                + portNumber + " or listening for a connection");
			e.printStackTrace();
		}
	}
}
