package model;

public class Key {
	
	private boolean up,left,down,right;
	
	public Key() {
		this.up=false;
		this.left=false;
		this.down=false;
		this.right=false;
	}

	public void setUp(boolean b) {
		this.up=b;
	}
	
	public boolean getUp() {
		return this.up;
	}
	
	public void setLeft(boolean b) {
		this.left=b;
	}

	public boolean getLeft() {
		return this.left;
	}
	
	public void setDown(boolean b) {
		this.down=b;
	}

	public boolean getDown() {
		return this.down;
	}

	public void setRight(boolean b) {
		this.right=b;
	}
	
	public boolean getRight() {
		return this.right;
	}
}
