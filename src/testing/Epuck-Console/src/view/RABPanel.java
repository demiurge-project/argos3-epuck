package view;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollBar;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.JTextField;
import javax.swing.ListSelectionModel;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;

import model.NonEditableTableModel;


public class RABPanel extends JPanel implements ListSelectionListener, ActionListener {
	private static final long serialVersionUID = 1L;

	private MainPanel mainPanel;
	private JPanel panel;
	private JLabel rabLabel;
	private JTextField rabText[];	
	private JCheckBox rabCheckBox;
	private JScrollPane scroll;
	private JButton button;
	private NonEditableTableModel tableModel;
	private JTable table;
	private ListSelectionModel listSelectionModel;
	private String msg[];
	
	public RABPanel(MainPanel mainPanel) {
		super(new GridBagLayout());
		this.mainPanel = mainPanel;
		this.setMinimumSize(new Dimension(240,280));
		this.setPreferredSize(new Dimension(240,280));
		init();
	}

	private void init() {
		GridBagConstraints c = new GridBagConstraints();
		
		//Panel
		this.panel = new JPanel(new GridBagLayout());
		GridBagConstraints ct = new GridBagConstraints();
		
		//TextField and Label
		this.rabText = new JTextField[4];
		ct.gridx = 0;
		ct.gridy = 0;
		for(int i=0; i<4; i++) {
			this.rabText[i] = new JTextField(2);
			ct.gridx=i+1;
			this.panel.add(this.rabText[i],ct);
		}
		this.rabLabel = new JLabel("RAB: ");
		this.rabLabel.setLabelFor(this.rabText[0]);
		ct.gridx = 0;
		this.panel.add(this.rabLabel,ct);
		
		//CheckBox
		this.rabCheckBox = new JCheckBox();
		this.rabCheckBox.setActionCommand("send");
		this.rabCheckBox.addActionListener(this);
		ct.gridx = 5;
		this.panel.add(this.rabCheckBox,ct);
		
		c.gridx = 0;
		c.gridwidth = 2;
		c.gridy = 0;
		this.add(this.panel,c);
		
		//Table
		this.tableModel = new NonEditableTableModel(new Object[]{"angle", "range", "d[1]", "d[2]", "d[3]", "d[4]"});
		this.table = new JTable(this.tableModel);
		this.listSelectionModel = this.table.getSelectionModel();
		this.listSelectionModel.addListSelectionListener(this);
	    this.table.setSelectionModel(this.listSelectionModel);
	    this.table.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
	    this.table.setGridColor(new Color(237,237,237));
	    
	    /*for(int i=0; i<15; i++) {
	    	this.tableModel.addRow(new Object[]{i, i+i*10, i+i*10, i+i*10, i+i*10, i+i*10});
	    }*/
		
		//ScrollPane
		this.scroll = new JScrollPane(this.table);
		this.scroll.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
		this.scroll.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
		this.scroll.setMinimumSize(new Dimension(240,220));
		this.scroll.setPreferredSize(new Dimension(240,220));
		c.gridx = 0;
		//c.gridwidth = 2;
		c.gridy = 1;
		c.anchor = GridBagConstraints.CENTER;
		this.add(this.scroll,c);
		
		//Button
		this.button = new JButton("clear");
		this.button.addActionListener(this);
		c.gridx = 0;
		c.gridy = 2;
		this.add(this.button,c);
	}

	@Override
	public void valueChanged(ListSelectionEvent e) {
		if(this.table.getRowCount()>0);
			//System.out.println(this.tableModel.getValueAt(this.table.getSelectedRow(), 0));
	}

	@Override
	public void actionPerformed(ActionEvent e) {
		boolean cont = true;
		if(e.getActionCommand().equals("clear")) {
			int rowCount = this.tableModel.getRowCount();
			for(int i=0; i<rowCount; i++) {
				this.tableModel.removeRow(0);
			}
		} else if(e.getActionCommand().equals("send") && this.rabCheckBox.isSelected()) {
			String text;
			msg = new String[4];
			for(int i=0; i<4; i++) {
				text = this.rabText[i].getText();
				if(!text.isEmpty()) {
					if(text.matches("[0-9]{0,3}")) {
						int nb = Integer.parseInt(text);
						if(nb > 255) {
							nb = Math.min(Integer.parseInt(text), 255);
							this.mainPanel.setMessage(">> A bigger value than 255 has been detected " +
									"in the Range and Bearing Panel: \"" + text + "\". \n" +
									">> The number has been set to 255.");
							this.rabText[i].setText("255");
						}
						msg[i] = String.valueOf(nb);
					} else {
						cont = false;
						this.mainPanel.setMessage(">> Error in the Range and Bearing Panel: \"" +
								text + "\" is an invalid argument. \n" +
								">> You can only send an unsigned integer which must be between 0 and 255.");
					}
				}
			}
			if(cont) {
				this.mainPanel.setRABActuator(msg);
				//this.addMessage(new String[]{"10","100","12","13","14","15"});
			}
		} else {
			this.mainPanel.setRABActuator(false);
		}
	}

	public void addMessage(String[] angleRangeData) {
		if(!this.contains(angleRangeData))
			this.tableModel.addRow(angleRangeData);
		/*System.out.println("a");
		AdjustmentListener adj = new AdjustmentListener() {  
			public void adjustmentValueChanged(AdjustmentEvent e) {  
				e.getAdjustable().setValue(e.getAdjustable().getMaximum());  
			}
		};
		scroll.getVerticalScrollBar().addAdjustmentListener(adj);*/
		/*scroll.getVerticalScrollBar().addAdjustmentListener(new AdjustmentListener() {  
			public void adjustmentValueChanged(AdjustmentEvent e) {
				e.getAdjustable().setValue(e.getAdjustable().getValue());  
			}
		});*/
		//this.scroll.setViewportView(this.table);
		//this.scroll.scrollRectToVisible(this.table.getBounds());
		//this.scroll.getVerticalScrollBar().removeAdjustmentListener(adj);
		int rowCount = this.table.getRowCount();
		if(rowCount>0)
			this.table.setRowSelectionInterval(rowCount-1, rowCount-1);
		JScrollBar vertical = scroll.getVerticalScrollBar();
		vertical.setValue(vertical.getMaximum());
	}
	
	public boolean contains(String[] angleRangeData) {
		int nbRow = this.tableModel.getRowCount();
		int length = angleRangeData.length;
		boolean bool = false;
		//System.out.println(length);
		for(int i=0; i<nbRow && length==6; i++) {
			/*System.out.println(this.tableModel.getValueAt(i,2)+" "+this.tableModel.getValueAt(i,3)+" "+
					this.tableModel.getValueAt(i,4)+" "+this.tableModel.getValueAt(i,5));*/
			//System.out.println(angleRangeData[2] +" "+ angleRangeData[3] +" "+angleRangeData[4]+" "+angleRangeData[5]);
			if(this.tableModel.getValueAt(i,2).equals(angleRangeData[2]) && this.tableModel.getValueAt(i,3).equals(angleRangeData[3])
					&& this.tableModel.getValueAt(i,4).equals(angleRangeData[4]) && this.tableModel.getValueAt(i,5).equals(angleRangeData[5])) {
				this.tableModel.setValueAt(angleRangeData[0], i, 0);
				this.tableModel.setValueAt(angleRangeData[1], i, 1);
				bool = true;
			}
		}
		return bool;
	}
}
