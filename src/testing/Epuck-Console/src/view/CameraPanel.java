package view;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.BorderFactory;
import javax.swing.JPanel;


public class CameraPanel extends JPanel {
	private static final long serialVersionUID = 1L;

	public CameraPanel() {
		this.setMinimumSize(new Dimension(250,250));
		this.setPreferredSize(new Dimension(250,250));
		this.setBorder(BorderFactory.createTitledBorder("Camera"));
	}
	
	public void paintComponent(Graphics g) {
		int db=10;
		try {
			BufferedImage epuck = ImageIO.read(new File("images/epuck.png"));
			g.drawImage(epuck, db, db, this.getWidth()-db, this.getHeight()-db, null);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
