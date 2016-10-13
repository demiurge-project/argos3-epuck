package model;

import java.awt.Color;
import java.awt.Component;

import javax.swing.JList;
import javax.swing.JTextArea;
import javax.swing.ListCellRenderer;
import javax.swing.UIDefaults;

public class CellRenderer implements ListCellRenderer {

	Color color;
	private JTextArea ta;
	
	public CellRenderer() {
		UIDefaults defaults = javax.swing.UIManager.getDefaults();
		color=defaults.getColor("List.selectionBackground");
		ta = new JTextArea();
        ta.setLineWrap(true);
        ta.setWrapStyleWord(true);
	}

	@Override
	public Component getListCellRendererComponent(JList list, Object value,
			int index, boolean isSelected, boolean cellHasFocus) {
		ta.setText(value.toString());
		if(isSelected)
			ta.setBackground(color);
		else
			ta.setBackground(Color.WHITE);
		
		int width = list.getWidth();
        if (width > 0)
            ta.setSize(width, Short.MAX_VALUE);

		return ta;
	}

}