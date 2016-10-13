package view;
import java.awt.Dimension;

import javax.swing.BorderFactory;
import javax.swing.JPanel;
import javax.swing.JTextField;


public class GroundSensorsPanel extends JPanel {

	private static final long serialVersionUID = 1L;
	private JTextField sensor1, sensor2, sensor3;

	public GroundSensorsPanel() {
		//super(new GridBagLayout());
		this.sensor1=new JTextField(5);
		this.sensor2=new JTextField(5);
		this.sensor3=new JTextField(5);
		
		this.sensor1.setEditable(false);
		this.sensor2.setEditable(false);
		this.sensor3.setEditable(false);
		
		this.add(sensor1);
		this.add(sensor2);
		this.add(sensor3);
		
		this.setMinimumSize(new Dimension(250,60));
		
		this.setBorder(BorderFactory.createTitledBorder("Ground Sensors"));
	}
	
	public void setValue(double s1, double s2, double s3) {
		this.sensor1.setText(String.valueOf(s1));
		this.sensor2.setText(String.valueOf(s2));
		this.sensor3.setText(String.valueOf(s3));
	}
}
