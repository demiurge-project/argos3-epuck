package view;

import java.awt.Dimension;

import javax.swing.JFrame;
import javax.swing.JTabbedPane;
import javax.swing.WindowConstants;

public class Window extends JFrame {
	private static final long serialVersionUID = 1L;
	
	private JTabbedPane tabbedPanel;

	/**
	 * Create the main window.
	 */
	public Window() {
		this.setTitle("Epuck - Console");
		this.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
		this.setMinimumSize(new Dimension(750,600));
		this.setResizable(false);
		this.setLocationRelativeTo(null);
		this.setVisible(true);
		this.tabbedPanel = new JTabbedPane();
		//addTabbedPanel(new MainPanel(null),0);
	}
	
	/**
	 * Add a tabbed panel to the main window.
	 * @param panel - panel to add to the window.
	 * @param id - id of the epuck.
	 */
	public synchronized void addTabbedPanel(MainPanel mainPanel, int id) {
		String EpuckId = Integer.toString(id);
		this.tabbedPanel.addTab("Epuck "+EpuckId, null, mainPanel, "Epuck "+EpuckId);
		this.getContentPane().removeAll();
		this.getContentPane().add(this.tabbedPanel);
		this.pack();
	}
	
	/**
	 * Remove a tabbed panel from the window.
	 * @param view
	 */
	public synchronized void removeTabbedPanel(MainPanel view) {
		this.tabbedPanel.remove(view);
	}
	
}
