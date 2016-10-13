package view;

import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;

import javax.swing.BorderFactory;
import javax.swing.DefaultListModel;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.ListSelectionModel;

import model.CellRenderer;

public class MsgPanel extends JPanel implements ActionListener {
	private static final long serialVersionUID = 1L;

	private JList list;
	private DefaultListModel listModel;
	private JButton clearButton;
	private JCheckBox autoScrollButton;
	
	public MsgPanel() {
		super(new GridBagLayout());
		this.setPreferredSize(new Dimension(270,200));
		this.setMinimumSize(new Dimension(270,200));
		init();
		this.setBorder(BorderFactory.createTitledBorder("Messages"));
	}
	
	private void init() {
		GridBagConstraints c = new GridBagConstraints();
		
		this.listModel = new DefaultListModel();
		this.list = new JList(this.listModel);
		list.setCellRenderer(new CellRenderer());
		list.setSelectionMode(ListSelectionModel.SINGLE_INTERVAL_SELECTION);
		list.setLayoutOrientation(JList.VERTICAL);
		list.setVisibleRowCount(-1);
		JScrollPane listScroller = new JScrollPane(list);
		listScroller.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
		listScroller.setPreferredSize(new Dimension(250, 140));
		c.gridwidth=2;
		c.gridx=0;
		c.gridy=0;
		//JPanel panel = new JPanel();
		//panel.add(listScroller);
		//panel.setBorder(BorderFactory.createTitledBorder("Messages"));
		//this.add(panel,c);
		this.add(listScroller,c);
		
		this.autoScrollButton = new JCheckBox("auto-scroll");
		this.autoScrollButton.setSelected(true);
		c.gridwidth=1;
		c.gridx=0;
		c.gridy = 1;
		this.add(this.autoScrollButton,c);
		
		this.clearButton = new JButton("clear");
		this.clearButton.addActionListener(this);
		c.gridx = 1;
		c.anchor = GridBagConstraints.SOUTHEAST;
		this.add(this.clearButton,c);
	}
	
	public void setMessage(String msg) {
		DateFormat dateFormat = new SimpleDateFormat("HH:mm:ss");
		Date date = new Date();
		listModel.addElement("["+dateFormat.format(date)+"] " + msg);
		if(this.autoScrollButton.isSelected())
			this.list.ensureIndexIsVisible(this.listModel.getSize()-1);
		if(this.listModel.getSize()==Integer.MAX_VALUE)
			clearAll();
	}

	@Override
	public void actionPerformed(ActionEvent e) {
		if(e.getActionCommand().equals("clear")) {
			this.listModel.removeAllElements();
		}
	}
	
	public void clearAll() {
		
	}
}
