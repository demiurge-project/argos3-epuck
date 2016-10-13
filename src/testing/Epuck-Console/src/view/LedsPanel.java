package view;

import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;

import javax.swing.BorderFactory;
import javax.swing.JPanel;

public class LedsPanel extends JPanel {
	private static final long serialVersionUID = 1L;
	
	private int nbOfLeds=8;
	private MainPanel mainPanel;

	public LedsPanel(MainPanel mainPanel) {
		super(new GridBagLayout());
		this.mainPanel = mainPanel;
		this.setMinimumSize(new Dimension(240,240));
		this.setPreferredSize(new Dimension(240,240));
		init();
		this.setBorder(BorderFactory.createTitledBorder("Leds"));
	}
	
	private void init() {
		GridBagConstraints c = new GridBagConstraints();
		c.gridx=0;
		for(int i=0; i<nbOfLeds; i++) {
			c.gridy=i;
			this.add(new Leds(i, this.mainPanel),c);
		}
	}
}
