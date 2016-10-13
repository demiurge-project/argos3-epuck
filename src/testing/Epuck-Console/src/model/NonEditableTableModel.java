package model;

import javax.swing.table.DefaultTableModel;

public class NonEditableTableModel extends DefaultTableModel {
	private static final long serialVersionUID = 1L;
	
	public NonEditableTableModel(Object[] objects) {
		super(objects, 0);
	}
	
	public boolean isCellEditable(int row, int column) {
		return false;
	}
}
