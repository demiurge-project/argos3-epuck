import model.Server;


/**
 * Main class. Console for Epuck.
 * @author Brian Delhaisse.
 */
public class Main {

	public static void main(String[] args) {
		//Window w = new Window();
		//new Controller(null, w, 1);
		//new Controller(null, w, 2);
		
		if(args.length>0) {
			if(args[0].matches("[0-9]+"))
				new Server(Integer.parseInt(args[0]));
			else
				System.out.println("The port must be a number.");
		} else {
			new Server();
		}
	}
}
