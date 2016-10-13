package view;

import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;

import javax.swing.JPanel;
import javax.swing.JSlider;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

public class InstrumentsPanel extends JPanel implements ChangeListener {
	private static final long serialVersionUID = 1L;

	private JoystickPanel jp;
	private JSlider speedSlider;
	private MainPanel mainPanel;
	
	public InstrumentsPanel(MainPanel mainPanel) {
		super(new GridBagLayout());
		this.mainPanel = mainPanel;
		init();
	}
	
	private void init() {
		GridBagConstraints c = new GridBagConstraints();
		
		//Create the Joystick panel
		this.jp = new JoystickPanel(this.mainPanel);
		c.gridx = 1;
		c.gridy = 0;
		this.add(this.jp,c);
		
		//Create the JSlider for the speed.
		this.speedSlider = new JSlider(JSlider.VERTICAL, 0, 10, 10);
		this.speedSlider.addChangeListener(this);
		this.speedSlider.setMajorTickSpacing(5);
		this.speedSlider.setMinorTickSpacing(2);
		this.speedSlider.setPaintTicks(true);
		this.speedSlider.setPaintLabels(true);
		c.gridx = 2;
		c.insets = new Insets(0,10,0,0);
		this.add(this.speedSlider,c);
	}

	@Override
	public void stateChanged(ChangeEvent e) {
		JSlider source = (JSlider) e.getSource();
        if (!source.getValueIsAdjusting()) {
            int v = (int) source.getValue();
            this.jp.setVelocity(v);
        }
	}
	
}
