package view;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.RenderingHints;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.geom.AffineTransform;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.JCheckBox;
import javax.swing.JPanel;

import model.Key;


public class JoystickPanel extends JPanel implements MouseMotionListener, MouseListener, KeyListener, ActionListener {
	private static final long serialVersionUID = 1L;
	
	//private int SCALE=1;
	private int xOffset=20, yOffset=20, radius1=50, radius2=30, dist=3*radius1/10;
	private int xCenter=xOffset+radius1, yCenter=xOffset+radius1;
	private int xPos=xCenter,yPos=yCenter, offset=xOffset+dist;
	private int v=10, rMax=radius1-dist;
	private double vr=0, vl=0;
	private MainPanel mainPanel;
	private JCheckBox cb;
	private Key key = new Key();

	public JoystickPanel(MainPanel mainPanel) {
		super(new GridBagLayout());
		this.mainPanel = mainPanel;
		this.addMouseMotionListener(this);
		this.addMouseListener(this);
		this.addKeyListener(this);
		this.setMinimumSize(new Dimension(140,140));
		this.setPreferredSize(new Dimension(140,140));
		init();
	}
	
	private void init() {
		GridBagConstraints c = new GridBagConstraints();
		this.cb = new JCheckBox();
		
		this.cb.setSelected(false);
		//this.cb.setSelected(true);
		//this.mainPanel.setWheelsActuator(0,0);
		
		this.cb.addActionListener(this);
		c.insets = new Insets(2*radius1+yOffset,0,0,2*radius1+xOffset);
		this.add(this.cb,c);
	}
	
	public void addNotify() {
        super.addNotify();
        requestFocus();
    }
	
	public void paintComponent(Graphics g) {
		Graphics2D g2 = (Graphics2D) g;
		g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
		
		//Background of the panel
		g2.setColor(new Color(237,237,237));
		g2.fillRect(0, 0, this.getWidth(), this.getHeight());
		
		//Background of the Joystick
		g2.setColor(Color.gray);
		try {
			int db=10;
			BufferedImage background = ImageIO.read(new File("images/joystick_background.png"));
			g2.drawImage(background, xOffset-db, yOffset-db, 2*(radius1+db), 2*(radius1+db), null);
		} catch (IOException e) {
			g2.fillOval(xOffset, yOffset, 2*radius1, 2*radius1);
			//e.printStackTrace();
		}
		
		AffineTransform old = g2.getTransform();
		
		//stick of the Joystick		
		g2.setColor(new Color(10,10,10));
		g2.translate(xCenter, yCenter);
		if(Math.abs(xPos-xCenter)>1 || Math.abs(yPos-yCenter)>1) {
			double d = Math.sqrt(Math.pow((xPos-xCenter),2)+Math.pow(yPos-yCenter,2));
			if(yPos>yCenter)
				g2.rotate(Math.acos((xPos-xCenter)/d));
			else
				g2.rotate(-Math.acos((xPos-xCenter)/d));
		}
		g2.translate(-(radius1-offset+xOffset)/4, -radius2/2);
		g2.fillOval(0, 0, radius1-offset+xOffset, radius2);
		
		g2.setTransform(old);
		
		//Joystick
		g2.setColor(Color.black);
		try {
			BufferedImage joystick = ImageIO.read(new File("images/joystick.png"));
			g2.drawImage(joystick, xPos-radius2, yPos-radius2, 2*radius2, 2*radius2, null);
		} catch (IOException e) {
			g2.fillOval(xPos-radius2, yPos-radius2, 2*radius2, 2*radius2);
			//e.printStackTrace();
		}
	}

	@Override
	public void mouseDragged(MouseEvent e) {
		this.cb.setSelected(true);
		int x=e.getX();
		int y=e.getY();
		setJoystickPosition(x,y);
	}
	
	@Override
	public void mouseReleased(MouseEvent arg0) {
		setJoystickPosition(xCenter,yCenter);
	}
	
	private void setJoystickPosition(int x, int y) {
		double d = Math.sqrt(Math.pow((x-xCenter),2)+Math.pow(y-yCenter,2));
		if(d>(radius1+xOffset-offset)) {
			this.xPos = (int) (x - ((d-(radius1+xOffset-offset))/d) * (x-xCenter));
			this.yPos = (int) (y - ((d-(radius1+yOffset-offset))/d) * (y-yCenter));
		} else {
			this.xPos=x;
			this.yPos=y;
		}
		double r = Math.sqrt(Math.pow((this.xPos-xCenter),2)+Math.pow(this.yPos-yCenter,2));
		double theta;
		if(this.xPos==this.xCenter && this.yPos==this.yCenter) {//r==0
			theta = 0;
			this.vr = 0;
			this.vl = 0;
		} else {
			if(this.xPos>=this.xCenter && this.yPos>=this.yCenter) { //1st quadrant
				theta = Math.asin((this.yPos-this.yCenter)/r);
			} else if(this.xPos<this.xCenter && this.yPos>=this.yCenter) { //2nd quadrant
				theta = Math.acos((this.xPos-this.xCenter)/r);
			} else if(this.xPos<this.xCenter && this.yPos<this.yCenter) { //3rd quadrant
				theta = - Math.asin((this.yPos-this.yCenter)/r) + Math.PI;
			} else { //4rth quadrant
				theta = - Math.acos((this.xPos-this.xCenter)/r) + 2*Math.PI;
			}
			//System.out.println("theta: "+Math.toDegrees(theta));
			if(theta < (10*Math.PI/180) && theta > (-10*Math.PI/180)) {
				this.vr = -v;
				this.vl = v;
			} else if(theta < (10*Math.PI/180+Math.PI/2) && theta > (-10*Math.PI/180+Math.PI/2)) {
				this.vr = -v;
				this.vl = -v;
			} else if(theta < (10*Math.PI/180+Math.PI) && theta > (-10*Math.PI/180+Math.PI)) {
				this.vr = v;
				this.vl = -v;
			} else if(theta < (10*Math.PI/180+3*Math.PI/2) && theta > (-10*Math.PI/180+3*Math.PI/2)) {
				this.vr = v;
				this.vl = v;
			} else {
				this.vr = - r*v/rMax * (Math.sin(theta) + Math.cos(theta));
				this.vl = - r*v/rMax * (Math.sin(theta) - Math.cos(theta));
			}
		}
		//System.out.println("("+this.vl+" , "+this.vr+")");
		this.mainPanel.setWheelsActuator(vl,vr);
		repaint();
	}
	
	@Override
	public void keyPressed(KeyEvent e) {
		this.cb.setSelected(true);
		if(e.getKeyCode()==KeyEvent.VK_UP) {
			if(this.key.getDown())
				this.key.setDown(false);
			else
				this.key.setUp(true);
		} else if(e.getKeyCode()==KeyEvent.VK_LEFT) {
			if(this.key.getRight())
				this.key.setRight(false);
			else
				this.key.setLeft(true);
		} else if(e.getKeyCode()==KeyEvent.VK_DOWN) {
			if(this.key.getUp())
				this.key.setUp(false);
			else
				this.key.setDown(true);
		} else if(e.getKeyCode()==KeyEvent.VK_RIGHT) {
			if(this.key.getLeft())
				this.key.setLeft(false);
			else
				this.key.setRight(true);
		}
		checkJoystickPos();
	}
	
	private void checkJoystickPos() {
		if(this.key.getUp() && this.key.getRight())
			this.setJoystickPosition(this.getWidth()-offset, offset);
		else if(this.key.getUp() && this.key.getLeft())
			this.setJoystickPosition(offset, offset);
		else if(this.key.getUp())
			this.setJoystickPosition(xCenter,offset);
		
		else if(this.key.getDown() && this.key.getLeft())
			this.setJoystickPosition(offset, this.getHeight()-offset);
		else if(this.key.getDown() && this.key.getRight())
			this.setJoystickPosition(this.getWidth()-offset, this.getHeight()-offset);
		else if(this.key.getDown())
			this.setJoystickPosition(xCenter,this.getHeight()-offset);
			
		else if(this.key.getLeft())
			this.setJoystickPosition(offset,yCenter);
		else if(this.key.getRight())
			this.setJoystickPosition(this.getWidth()-offset,yCenter);
		
		else
			this.setJoystickPosition(xCenter,yCenter);
	}

	@Override
	public void keyReleased(KeyEvent e) {
		if(e.getKeyCode()==KeyEvent.VK_UP) {
			this.key.setUp(false);
		} else if(e.getKeyCode()==KeyEvent.VK_LEFT) {
			this.key.setLeft(false);
		} else if(e.getKeyCode()==KeyEvent.VK_DOWN) {
			this.key.setDown(false);
		} else if(e.getKeyCode()==KeyEvent.VK_RIGHT) {
			this.key.setRight(false);
		}
		checkJoystickPos();
	}
	
	@Override
	public void mousePressed(MouseEvent e) {
		this.cb.setSelected(true);
		this.requestFocus();
		int x=e.getX(), y=e.getY();
		setJoystickPosition(x,y);
	}
	
	@Override
	public void actionPerformed(ActionEvent e) {
		if(!this.cb.isSelected())
			this.mainPanel.setWheelsActuator(false);
		else
			this.mainPanel.setWheelsActuator(0, 0);
	}
	
	public void setVelocity(int v) {
		this.v = v;
	}

	@Override
	public void keyTyped(KeyEvent e) {}
	
	@Override
	public void mouseMoved(MouseEvent e) {}

	@Override
	public void mouseClicked(MouseEvent e) {}

	@Override
	public void mouseEntered(MouseEvent e) {}

	@Override
	public void mouseExited(MouseEvent e) {}
}
