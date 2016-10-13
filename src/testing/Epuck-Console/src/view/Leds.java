package view;

import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.ButtonGroup;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JRadioButton;

public class Leds extends JPanel implements ActionListener {
	private static final long serialVersionUID = 1L;

	private int id;
	private JLabel label;
	private JRadioButton onButton, offButton, nullButton;
	private ButtonGroup group;
	private MainPanel mainPanel;
	
	public Leds(int id, MainPanel mainPanel) {
		super(new GridBagLayout());
		this.id = id;
		this.mainPanel = mainPanel;
		this.setPreferredSize(new Dimension(220,25));
		this.setMinimumSize(new Dimension(220,25));
		init();
	}
	
	private void init() {
		//Create the label
		this.label = new JLabel("Led "+(this.id+1)+": ");
		
		//Create the radio buttons "on", "off" and "null"
		onButton = new JRadioButton("On");
		onButton.setActionCommand("On");
		offButton = new JRadioButton("Off");
		offButton.setActionCommand("Off");
		nullButton = new JRadioButton("Code");
		nullButton.setActionCommand("Null");
		
		nullButton.setSelected(true);
		//offButton.setSelected(true);
		
		//Group the radio buttons.
		group = new ButtonGroup();
		group.add(onButton);
		group.add(offButton);
		group.add(nullButton);
		
		//Add the listeners to each button.
		onButton.addActionListener(this);
		offButton.addActionListener(this);
		nullButton.addActionListener(this);
		
		//Add all the components in the panel.
		GridBagConstraints c = new GridBagConstraints();
		c.gridx=0;
		c.gridy=0;
		this.add(this.label,c);
		c.gridx=1;
		this.add(this.onButton,c);
		c.gridx=2;
		this.add(this.offButton,c);
		c.gridx=3;
		this.add(this.nullButton,c);
	}

	@Override
	public void actionPerformed(ActionEvent e) {
		if(e.getActionCommand().equals("On")) {
			this.mainPanel.setLedsActuator(id, 1);
		} else if(e.getActionCommand().equals("Off")) {
			this.mainPanel.setLedsActuator(id, 0);
		} else { //Null
			this.mainPanel.setLedsActuator(id, 2);
		}
	}
}
