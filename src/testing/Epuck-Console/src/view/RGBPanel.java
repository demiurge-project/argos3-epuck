package view;

import java.awt.Color;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JCheckBox;
import javax.swing.JColorChooser;
import javax.swing.JPanel;
import javax.swing.colorchooser.AbstractColorChooserPanel;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

public class RGBPanel extends JPanel implements ChangeListener, ActionListener {
	private static final long serialVersionUID = 1L;
	
	private JColorChooser cc;
	private MainPanel mainPanel;
	private int id;
	JCheckBox checkBox;
	
	public RGBPanel(int id, MainPanel mainPanel) {
		super(new GridBagLayout());
		this.id = id;
		this.mainPanel = mainPanel;
		init();
	}
	
	private void init() {
		GridBagConstraints c = new GridBagConstraints();
		//Set up color chooser for setting text color
		this.cc = new JColorChooser();

		//remove the preview panel
		this.cc.setPreviewPanel(new JPanel());

		//Remove all the Color Chooser Panels except the RGB panel.
		AbstractColorChooserPanel[] panel = this.cc.getChooserPanels();
		for (AbstractColorChooserPanel accp : panel) {
			if (!accp.getDisplayName().equals("RGB")) { //Swatches, HSV
				this.cc.removeChooserPanel(accp);
			}
		}

		this.cc.getSelectionModel().addChangeListener(this);
		c.gridx = 0;
		c.gridy = 0;
		this.add(this.cc,c);
		
		this.checkBox = new JCheckBox();
		c.gridy = 1;
		c.anchor = GridBagConstraints.LINE_END;
		this.add(this.checkBox,c);
		this.checkBox.addActionListener(this);
	}
	
	public void stateChanged(ChangeEvent e) {
        Color color = cc.getColor();
        this.checkBox.setSelected(true);
        this.mainPanel.setRGBActuator(this.id, color);
    }

	@Override
	public void actionPerformed(ActionEvent e) {
		this.mainPanel.setRGBActuator(id, this.checkBox.isSelected());
	}

}
