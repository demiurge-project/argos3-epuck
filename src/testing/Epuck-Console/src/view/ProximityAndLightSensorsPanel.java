package view;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;

import javax.swing.JPanel;


public class ProximityAndLightSensorsPanel extends JPanel {
	private static final long serialVersionUID = 1L;
	
	private int radius=100, xOffset=50, yOffset=30;
	private int xCenter=radius+xOffset, yCenter=radius+yOffset;
	private int db = 5;
	private int directionAngle = 0;
	private int nbOfRGBLeds = 3;
	private Color[] rgb = new Color[nbOfRGBLeds];
	private double[] proximity = null;
	private double[] light = null;
	private int angdeg[] = {17, 50, 90, 150, 210, 270, 310, 343};
	private int leds[] = {0, 0, 0, 0, 0, 0, 0, 0};
	//private double[] proximity = {0.87, 0.64, 0.49, 0.13, 0.67, 0.51, 0.34, 0.91};
	//private double[] light = {1.58, 0.39, 1.27, 0.97, 0.43, 0.73, 1.46, 1.05};

	public ProximityAndLightSensorsPanel() {
		this.setMinimumSize(new Dimension(300,250));
		this.setPreferredSize(new Dimension(300,250));
	}
	
	/**
	 * Draw the epuck, sensors, and the LEDs.
	 */
	public void paintComponent(Graphics g) {
		Graphics2D g2 = (Graphics2D) g;
		int x[], y[];
		
		//Background
		g2.setColor(new Color(237,237,237));
		//g2.setColor(Color.red);
		g2.fillRect(0, 0, this.getWidth(), this.getHeight());
		
		//save the ancient transform (just in case)
		//AffineTransform old = g2.getTransform();
				
		//Draw the Epuck.
		g2.translate(xCenter, yCenter);
		g2.rotate(Math.toRadians(this.directionAngle));
		g2.setColor(Color.black);
		g2.drawOval(-radius, -radius, 2*radius, 2*radius);
		
		//Draw the direction given by the accelerometer orientation
		x = new int[]{0, radius/5, -radius/5};
		y = new int[]{-radius/3, radius/4, radius/4};
		g2.drawPolygon(x, y, 3);
		
		//Draw the 8 infrared proximity and light sensors.
		g2.rotate(Math.toRadians(-90));
		for(int i=0; i<8; i++) {
			g2.rotate(Math.toRadians(-angdeg[i]));
			x = new int[]{radius-db, radius-2*db, radius-2*db, radius-db};
			y = new int[]{-radius/10, -radius/10, radius/10, radius/10};
			g2.fillPolygon(x, y, 4);
			g2.rotate(Math.toRadians(angdeg[i]));
		}
		g2.rotate(Math.toRadians(90));
		
		//Draw the values read by the proximity and light sensors.
		if(this.proximity!=null) {
			double ang=0, cos=0, sin=0;
			for(int i=0; i<8; i++) {
				ang = Math.toRadians(-angdeg[i]-90);
				cos = Math.cos(ang);
				sin = Math.sin(ang);
				g2.setColor(Color.blue);
				if(cos>=0 && sin>=0) {
					g2.drawString(String.valueOf(proximity[i]), (int) ((radius+db)*cos+db), 
							(int) ((radius+2*db)*sin)-2);
					g2.setColor(Color.red);
					g2.drawString(String.valueOf(light[i]), (int) ((radius+db)*cos+db), 
							(int) ((radius+2*db)*sin)+14);
				} else if(cos<0 && sin>=-0.01) {
					g2.drawString(String.valueOf(proximity[i]), (int) ((radius+db)*cos-6*db), 
							(int) ((radius+2*db)*sin)-2);
					g2.setColor(Color.red);
					g2.drawString(String.valueOf(light[i]), (int) ((radius+db)*cos-6*db), 
							(int) ((radius+2*db)*sin)+14);
				} else if(cos<0 && sin<0) {
					g2.drawString(String.valueOf(proximity[i]), (int) ((radius+db)*cos-5*db), 
							(int) ((radius+2*db)*sin)-10);
					g2.setColor(Color.red);
					g2.drawString(String.valueOf(light[i]), (int) ((radius+db)*cos-5*db), 
							(int) ((radius+2*db)*sin)+6);
				} else {
					g2.drawString(String.valueOf(proximity[i]), (int) ((radius+db)*cos), 
							(int) ((radius+2*db)*sin)-10);
					g2.setColor(Color.red);
					g2.drawString(String.valueOf(light[i]), (int) ((radius+db)*cos), 
							(int) ((radius+2*db)*sin)+6);
				}
			}
		}
		
		g2.setColor(Color.black);
		
		//Draw the 12 Range and Bearing sensors.
		for(int i=0; i<12; i++) {
			g2.fillOval(radius-db/2, -db/2, db, db);
			g2.rotate(Math.toRadians(30));
		}
		
		//Draw the 8 Leds.
		g2.rotate(Math.toRadians(-112.5));
		for(int i=0; i<8; i++) {
			if(leds[i]==1) {
				g2.setColor(Color.yellow);
				g2.fillOval(radius-db/2, -db/2, db, db);
			} else {
				g2.setColor(Color.black);
				g2.drawOval(radius-db/2, -db/2, db, db);
			}
			g2.rotate(Math.toRadians(-45));
		}
		g2.rotate(Math.toRadians(+112.5));
		
		//Draw the 3 RGB Leds.
		g2.rotate(Math.toRadians(-270));
		for(int i=0; i<3; i++) {
			g2.setColor(Color.white);
			g2.setColor(rgb[i]);
			g2.rotate(Math.toRadians(120));
			g2.fillOval(radius-4*db, -db, 2*db, 2*db);
		}
		
		//g2.setColor(Color.black);
		//value: [0,x]. If one sensor is broken: display "E" for error.
		//g2.drawString(str, x, y);
	}
	
	public void setProximitySensors(double[] proximity) {
		this.proximity = proximity;
		repaint();
	}
	
	public void setLightSensors(double[] light) {
		this.light = light;
		repaint();
	}
	
	public void setRGB(int id, Color color) {
		if(id<0)
			id=0;
		else if(id>=nbOfRGBLeds)
			id=2;
		this.rgb[id] = color;
		repaint();
	}
	
	public void setDirection(int angle) {
		this.directionAngle = angle%360;
		repaint();
	}

	public void setLed(int id, int i) {
		if(id<0)
			id=0;
		else if(id>7)
			id=7;
		leds[id] = i;
		repaint();
	}
}
