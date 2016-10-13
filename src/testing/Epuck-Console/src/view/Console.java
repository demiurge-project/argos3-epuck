package view;

import java.awt.Dimension;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;

import javax.swing.BorderFactory;
import javax.swing.DefaultListModel;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.ListSelectionModel;

import model.CellRenderer;

public class Console extends JPanel {
	private static final long serialVersionUID = 1L;

	private JList list;
	private DefaultListModel listModel;
	
	public Console() {
		this.setPreferredSize(new Dimension(250,150));
		this.setMinimumSize(new Dimension(250,150));
		init();
		this.setBorder(BorderFactory.createTitledBorder("Messages"));
	}
	
	private void init() {
		this.listModel = new DefaultListModel();
		this.list = new JList(this.listModel);
		list.setCellRenderer(new CellRenderer());
		list.setSelectionMode(ListSelectionModel.SINGLE_INTERVAL_SELECTION);
		list.setLayoutOrientation(JList.VERTICAL);
		list.setVisibleRowCount(-1);
		JScrollPane listScroller = new JScrollPane(list);
		listScroller.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
		listScroller.setPreferredSize(new Dimension(240, 118));
		this.add(listScroller);
	}
	
	public void setMessage(String msg) {
		DateFormat dateFormat = new SimpleDateFormat("HH:mm:ss");
		Date date = new Date();
		listModel.addElement("["+dateFormat.format(date)+"] " + msg);
		//if(button)
		//this.list.ensureIndexIsVisible(this.listModel.getSize()-1);
		//if(this.listModel.getSize==Integer.MAX_VALUE)
		//clearAll();
	}
	
	public void clearAll() {
		
	}
}
