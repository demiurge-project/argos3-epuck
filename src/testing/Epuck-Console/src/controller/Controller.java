package controller;
import java.awt.Color;

import model.ServerThread;
import view.MainPanel;
import view.Window;

/**
 * Controller created by the server thread.
 * Only the controller and the server thread are communicating.
 * @author Brian Delhaisse.
 */
public class Controller {
	
	private ServerThread st;
	private MainPanel view;
	private String msgToSend="NULL";
	private Window window;
	
	public Controller(ServerThread st, Window window, int id) {
		this.st = st;
		this.window = window;
		
		//Creating the view.
		this.view = new MainPanel(this);
		this.window.addTabbedPanel(this.view, id);
	}
	
	/**
	 * Parse the message received.
	 * @param msg - msg to parse
	 */
	public void parse(String msg) {
		int index=0, index1=0, endIndex=0;
		boolean cont=true;
		
		//Message
		if((index=msg.indexOf("Msg=[")) != -1) {
			index+=5;
			String s = msg.substring(index, msg.indexOf(']', index));
			this.view.setMessage(s);
		}
		
		msg = msg.replaceAll(",", ".");

		//Proximity sensors
		if((index=msg.indexOf("P=[")) != -1) {
			index += 3;
			double proximity[] = new double[8];
			for(int i=0; i<proximity.length; i++) {
				if(i<(proximity.length-1))
					index1 = msg.indexOf(';', index);
				else
					index1 = msg.indexOf(']', index);
				proximity[i] = Double.parseDouble(msg.substring(index, index1));
				index = index1+1;
			}
			this.view.setProximitySensors(proximity);
		}
		
		//Light sensors
		if((index=msg.indexOf("L=[")) != -1) {
			index +=3;
			double light[] = new double[8];
			for(int i=0; i<light.length; i++) {
				if(i<(light.length-1))
					index1 = msg.indexOf(';', index);
				else
					index1 = msg.indexOf(']', index);
				light[i] = Double.parseDouble(msg.substring(index, index1));
				index = index1+1;
			}
			this.view.setLightSensors(light);
		}
		
		//Ground sensors
		if((index=msg.indexOf("G=[")) != -1) {
			index += 3;
			double ground[] = new double[3];
			for(int i=0; i<ground.length; i++) {
				if(i<(ground.length-1)) {
					index1 = msg.indexOf(';', index);
					ground[i] = Double.parseDouble(msg.substring(index, index1));
					index = index1+1;
				} else
					ground[i] = Double.parseDouble(msg.substring(index, msg.indexOf(']', index)));
			}
			this.view.setGroundSensors(ground);
		}
		
		//RAB sensors
		if((index=msg.indexOf("RAB=[")) != -1) {
			index +=5;
			String angleRangeData[] = new String[6];
			endIndex = msg.indexOf(']', index);
			while(cont) {
				for(int i=2; i<angleRangeData.length; i++) {
					index1 = msg.indexOf('/', index);
					angleRangeData[i] = msg.substring(index, index1);
					index = index1+1;
				}
				for(int i=0; i<2; i++) {
					if(i==0)
						index1 = msg.indexOf('/', index);
					else {
						index1 = msg.indexOf(';', index);
						if((index1 > endIndex) || (index1==-1)) {
							index1 = msg.indexOf(']', index);
							cont = false;
						}
					}
					angleRangeData[i] = msg.substring(index, index1);
					index = index1+1;
				}
				this.view.setRABSensors(angleRangeData);
			}
			cont = true;
		}
		
		//IRcom sensors
		if((index=msg.indexOf("IR=[")) != -1) {
			index +=4;
			int dataAndSensorId[] = new int[2];
			double rangeAndAngle[] = new double[2];
			endIndex = msg.indexOf(']', index);
			while(cont) {
				for(int i=0; i<dataAndSensorId.length; i++) {
					index1 = msg.indexOf('/', index);
					dataAndSensorId[i] = Integer.parseInt(msg.substring(index, index1));
					index = index1+1;
				}
				for(int i=0; i<rangeAndAngle.length; i++) {
					if(i==0)
						index1 = msg.indexOf('/', index);
					else {
						index1 = msg.indexOf(';', index);
						if((index1 > endIndex) || (index1==-1)) {
							index1 = msg.indexOf(']', index);
							cont = false;
						}
					}
					rangeAndAngle[i] = Double.parseDouble(msg.substring(index, index1));
					index = index1+1;
				}
				this.view.setIRSensors(dataAndSensorId, rangeAndAngle);
			}
		}
	}
	
	private void createMsgToSend(String identity, String msg) {
		int index=0;
		if(this.msgToSend.contains("NULL")) {
			this.msgToSend = msg;
		} else {
			if((index=this.msgToSend.indexOf(identity+"=[")) != -1) {
				int index1 = this.msgToSend.indexOf(']', index) + 1;
				replace(index,index1,msg);
			} else {
				this.msgToSend += msg;
			}
		}
		send();
	}
	
	private void replace(int index, int index1, String msg) {
		if(index==0) {
			if(index1==this.msgToSend.length())
				this.msgToSend = msg;
			else
				this.msgToSend = this.msgToSend.substring(index1)+msg;
		} else {
			if(index1==this.msgToSend.length())
				this.msgToSend = this.msgToSend.substring(0,index)+msg;
			else
				this.msgToSend = this.msgToSend.substring(0,index)+msg+this.msgToSend.substring(index1,this.msgToSend.length());
		}
	}
	
	private void remove(String identity) {
		if(!this.msgToSend.contains("NULL")) {
			int index=0;
			if((index=this.msgToSend.indexOf(identity+"=[")) != -1) {
				int index1 = this.msgToSend.indexOf(']', index) + 1;
				if(index==0) {
					if(index1==this.msgToSend.length())
						this.msgToSend = "NULL";
					else
						this.msgToSend = this.msgToSend.substring(index1);
				} else {
					if(index1==this.msgToSend.length())
						this.msgToSend = this.msgToSend.substring(0,index);
					else
						this.msgToSend = this.msgToSend.substring(0,index)+this.msgToSend.substring(index1,this.msgToSend.length());
				}
			}
			send();
		}
	}
	
	private void send() {
		if(this.st != null)
			this.st.msgTosend(this.msgToSend);
	}
	
	public void setWheelsActuator(double vl, double vr) {
		String identity = "W";
		String msg = identity+"=["+Double.toString(vl)+";"+Double.toString(vr)+"]";
		createMsgToSend(identity, msg);
	}
	
	public void setWheelsActuator(boolean enabled) {
		if(!enabled) {
			String identity = "W";
			remove(identity);
		}
	}

	/**
	 * Set the RGB led to a specific color.
	 * @param id - id of the led.
	 * @param color - color of the led.
	 */
	public void setRGBActuator(int id, Color color) {
		String identity = "RGB"+String.valueOf(id);
		String msg = identity+"=["+String.valueOf(color.getRed())+";"
				+String.valueOf(color.getGreen())+";"+String.valueOf(color.getBlue())+"]";
		createMsgToSend(identity, msg);
	}
	
	/**
	 * Let the epuck decides what to do with its RGB leds.
	 * @param id - id of the led.
	 * @param enabled - if false, then it's the epuck which decides to set its RGB leds to a specific color.
	 */
	public void setRGBActuator(int id, boolean enabled) {
		if(!enabled) {
			String identity = "RGB"+String.valueOf(id);
			remove(identity);
		}
	}
	
	/**
	 * Switch on/off the basic leds.
	 * @param id - id of the led.
	 * @param on - on=1, off=0, null=2. if null, then it's the epuck which decides to switch on/off the leds.
	 */
	public void setLedsActuator(int id, int on) { //off=0, on=1, null=2
		on = on%3;
		String identity = "L"+String.valueOf(id);
		if(on<2) {
			String msg = identity+"=["+String.valueOf(on)+"]";
			createMsgToSend(identity,msg);
		} else {
			remove(identity);
		}
	}
	
	public void setRABActuator(String[] s) {
		String identity = "RAB";
		String msg = identity+"=["+s[0]+";"+s[1]+";"+s[2]+";"+s[3]+"]";
		createMsgToSend(identity, msg);
	}
	
	public void setRABActuator(boolean enabled) {
		if(!enabled) {
			String identity = "RAB";
			remove(identity);
		}
	}
	
	public void removeTabFromWindow() {
		this.window.removeTabbedPanel(this.view);
	}
	
}
