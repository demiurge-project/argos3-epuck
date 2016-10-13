package view;
import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;

import javax.swing.BorderFactory;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;


public class InfoPanel extends JPanel {
	private static final long serialVersionUID = 1L;

	private JTextField[] textField;
	private String[] str = {"Speed","Acceleration","Angular velocity","Angular Acceleration"};
	private JLabel[] label;
	
	public InfoPanel() {
		super(new GridBagLayout());
		init();
		this.setPreferredSize(new Dimension(220,150));
		this.setBorder(BorderFactory.createTitledBorder("Info about the motion"));
	}
	
	public void init() {
		this.textField = new JTextField[4];
		this.label = new JLabel[4];
		GridBagConstraints c = new GridBagConstraints();
		for(int i=0; i<4; i++) {
			this.textField[i] = new JTextField(4);
			this.label[i] = new JLabel(str[i]+":");
			this.textField[i].setEditable(false);
			this.label[i].setLabelFor(this.textField[i]);
			c.gridx=0;
			c.gridy=i;
			c.anchor = GridBagConstraints.LINE_START;
			this.add(this.label[i],c);
			c.gridx=1;
			//c.anchor = GridBagConstraints.LINE_END;
			c.fill = GridBagConstraints.BOTH;
			this.add(this.textField[i],c);
		}
		this.textField[0].setText("10");
		this.textField[1].setText("0");
		this.textField[2].setText("-1.3");
		this.textField[3].setText("0");
		
		
		
		/*this.speed = new JTextField(4);
		this.speedL = new JLabel("Speed: ");
		this.speedL.setLabelFor(this.speed);
		
		this.acc = new JTextField(4);
		this.accL = new JLabel("Acceleration: ");
		this.accL.setLabelFor(this.acc);
		
		this.angVel = new JTextField(4);
		this.angVelL = new JLabel("Angular velocity: ");
		this.angVelL.setLabelFor(this.angVel);
		
		this.angAcc = new JTextField(4);
		this.angAccL = new JLabel("Angular acceleration: ");
		this.angAccL.setLabelFor(this.angAcc);*/
	}
}
