package view;

import java.awt.Color;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;

import javax.swing.BorderFactory;
import javax.swing.JPanel;
import javax.swing.JTabbedPane;

import controller.Controller;

public class MainPanel extends JPanel {
	private static final long serialVersionUID = 1L;
	
	private InstrumentsPanel ip;
	private RABPanel rbp;
	private GroundSensorsPanel gsp;
	private ProximityAndLightSensorsPanel plp;
	private LedsPanel lp;
	private JTabbedPane trgb;
	private MsgPanel msg;
	private JPanel mainPanel;
	private Controller controller;
	
	public MainPanel(Controller c) {
		this.controller = c;
		init();
	}
	
	private void init() {
		this.mainPanel = new JPanel(new GridBagLayout());
		GridBagConstraints c = new GridBagConstraints();
		
		this.lp = new LedsPanel(this);
		c.gridx = 0;
		c.gridy = 0;
		this.mainPanel.add(this.lp);
		
		//TODO: to remove
		/*for(int i=0; i<8; i++) {
			this.controller.setLedsActuator(i, 0);
		}*/
		
		this.trgb = new JTabbedPane();
		for(int id=0; id<3; id++) {
			RGBPanel rgb = new RGBPanel(id, this);
			this.trgb.addTab("RGB "+(id+1), null, rgb, "RGB Led Sensor "+(id+1));
		}
		c.gridx = 1;
		this.mainPanel.add(this.trgb,c);
		
		this.rbp = new RABPanel(this);
		c.gridx = 2;
		c.insets = new Insets(10,8,0,0);
		this.mainPanel.add(this.rbp,c);
		
		c.insets = new Insets(-20,-8,0,0);
		
		JPanel panel = new JPanel(new GridBagLayout());
		GridBagConstraints ct = new GridBagConstraints();
		ct.gridx = 0;
		ct.gridy = 0;
		this.gsp = new GroundSensorsPanel();
		this.gsp.setValue(0, 0, 0);
		panel.add(this.gsp,ct);
		ct.gridy = 1;
		ct.insets = new Insets(5,0,0,0);
		this.msg = new MsgPanel();
		panel.add(this.msg,ct);
		
		c.gridx = 0;
		c.gridy = 1;
		this.mainPanel.add(panel,c);
		
		this.plp = new ProximityAndLightSensorsPanel();
		c.gridx = 1;
		this.mainPanel.add(this.plp,c);
		
		this.ip = new InstrumentsPanel(this);
		c.gridx = 2;
		c.anchor = GridBagConstraints.SOUTHEAST;
		this.mainPanel.add(this.ip,c);
		
		this.mainPanel.setBorder(BorderFactory.createEmptyBorder(0,5,5,5));

		this.add(this.mainPanel);
	}

	public void setGroundSensors(double[] ground) {
		if(ground.length == 3) { 
			this.gsp.setValue(ground[0], ground[1], ground[2]);
		}
	}

	public void setProximitySensors(double[] proximity) {
		this.plp.setProximitySensors(proximity);
	}

	public void setLightSensors(double[] light) {
		this.plp.setLightSensors(light);
	}

	public void setRABSensors(String[] angleRangeData) {
		this.rbp.addMessage(angleRangeData);
	}

	public void setIRSensors(int[] dataAndSensorId, double[] rangeAndAngle) {
		
	}

	public void setMessage(String s) {
		this.msg.setMessage(s);
	}
	
	public void setWheelsActuator(double vl, double vr) {
		this.controller.setWheelsActuator(vl,vr);
	}
	
	public void setWheelsActuator(boolean enabled) {
		this.controller.setWheelsActuator(enabled);
	}

	public void setRGBActuator(int id, Color color) {
		this.plp.setRGB(id, color);
		this.controller.setRGBActuator(id, color);
	}
	
	public void setRGBActuator(int id, boolean enabled) {
		this.controller.setRGBActuator(id, enabled);
	}
	
	public void setLedsActuator(int id, int on) { //off=0, on=1, null=2
		this.plp.setLed(id, on%2);
		this.controller.setLedsActuator(id, on);
	}
	
	public void setRABActuator(String[] s) {
		this.controller.setRABActuator(s);
	}
	
	public void setRABActuator(boolean enabled) {
		this.controller.setRABActuator(enabled);
	}
}
