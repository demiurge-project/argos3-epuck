package view;
import java.awt.Dimension;

import javax.swing.BorderFactory;
import javax.swing.JPanel;
import javax.swing.JProgressBar;


public class BatteryPanel extends JPanel {
	private static final long serialVersionUID = 1L;
	
	private JProgressBar progressBar;
	
	public BatteryPanel() {
		progressBar = new JProgressBar(0,100);
		progressBar.setValue(80);
		this.add(this.progressBar);
		this.setMinimumSize(new Dimension(250,65));
		this.setPreferredSize(new Dimension(250,65));
		this.setBorder(BorderFactory.createTitledBorder("Battery remaining time"));
	}
	
	public void setValue(int n) {
		if(n>100)
			this.progressBar.setValue(100);
		else if(n<0)
			this.progressBar.setValue(0);
		else
			this.progressBar.setValue(n);
	}

}
