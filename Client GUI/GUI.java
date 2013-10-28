import java.awt.Color;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;
import java.io.InputStream;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.concurrent.ExecutionException;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.SwingWorker;


public class GUI extends JFrame implements ActionListener{

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	static Socket gameSocket = null;
	
	public void createButtonGrid(int width, int length){
		JPanel content=new JPanel();
		content.setLayout(new GridLayout(width,length));
		GameSquare[][] gameGrid = new GameSquare[width][length];
		for(int y=0; y<length; y++){
			for(int x=0; x<width; x++){
				gameGrid[x][y]=new GameSquare(x,y);
				gameGrid[x][y].addActionListener(this);
				gameGrid[x][y].setBackground(Color.GRAY);
			    gameGrid[x][y].setForeground(Color.BLACK);
			    if(x==3 && y==3){
			    	gameGrid[x][y].setSquareColor("white");
			    }
			    else if(x==3 && y==4){
			    	gameGrid[x][y].setSquareColor("black");
			    }
			    else if(x==4 && y==3){
			    	gameGrid[x][y].setSquareColor("black");
			    }
			    else if(x==4 && y==4){
			    	gameGrid[x][y].setSquareColor("white");
			    }
			    content.add(gameGrid[x][y]);
			}
		}
		content.setVisible(true);
		setContentPane(content);
	}

	public GUI(int width, int length){
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		createButtonGrid(width,length);
		pack();
	}
	
	public static void main(String[] args) {
		try {
			gameSocket = new Socket("sun.cse.tamu.edu", 50000);
		} catch (IOException e) {
			System.err.println("Failed to connect!");
			System.out.println(e.getMessage());
			System.exit(1);
		}
		GUI game = new GUI(8,8);
		WorkerThread my_worker = new WorkerThread("HUMAN-AI\n");
		my_worker.execute();
		my_worker = new WorkerThread("MEDIUM");
		my_worker.execute();
		my_worker= new WorkerThread("WHITE");
		my_worker.execute();
		game.setSize(1000,1000);
		game.setVisible(true);
		game.revalidate();
		game.repaint();
	}

	@Override
	public void actionPerformed(ActionEvent e) {
		System.out.println(((GameSquare) e.getSource()).getBoardLocation());
		((GameSquare) e.getSource()).setSquareColor("white");
	}
	
	public static int SocketWrite(Socket sock, String command)
			throws IOException { 
		PrintWriter out = new PrintWriter(sock.getOutputStream(), true);
		out.write(command);
		out.flush();
		return 0;
	}

	public static String SocketRead(Socket sock) throws IOException { 
		InputStream in = sock.getInputStream();
		byte[] data = new byte[512];
		sock.setSoTimeout(10);
		in.read(data, 0, 512);
		String ret = new String(data, "ASCII");
		return ret;
	}
	
	static class GameSquare extends JButton{
		/**
		 * 
		 */
		private static final long serialVersionUID = 2L;
		final ImageIcon white_icon = new ImageIcon("white_circle.jpg");
		final ImageIcon black_icon= new ImageIcon("black_circle.gif");
		int internal_column;
		int internal_row;
		
		GameSquare(int x, int y){
			internal_column=x+1;
			internal_row=y+1;
		}
		
		void setSquareColor(String s){
			if(s.equals("white")){
				setIcon(white_icon);
				setText("white");

			}
			else if(s.equals("black")){
				setIcon(black_icon);
				setText("black");
			}
			else{
				setIcon(null);
			}
		}
		
		String getBoardLocation(){
			String column;
			if(internal_column==1){
				column="a";
			}
			else if(internal_column==2){
				column="b";
			}
			else if(internal_column==3){
				column="c";
			}
			else if(internal_column==4){
				column="d";
			}
			else if(internal_column==5){
				column="e";
			}
			else if(internal_column==6){
				column="f";
			}
			else if(internal_column==7){
				column="g";
			}
			else{
				column="h";
			}
			return new String(column+internal_row);
		}
		
	}
	
	static class WorkerThread extends SwingWorker<String, String>{
		
		String data;
		
		WorkerThread(String send_string){
			data=send_string;
		}
		@Override
		protected String doInBackground() throws Exception {
			SocketWrite(gameSocket, data);
			String s=SocketRead(gameSocket);
			return s;
		}
		protected void done(){
			String s = null;
			try {
				s = get();
			} catch (InterruptedException | ExecutionException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			if(s=="OK\n"){
				System.out.println("Command Execute successfully");
			}
			else if(s=="ILLEGAL\n"){
				System.out.println("Failed to execute command");
			}
			else{
				System.out.println("Flip tile log here!");
			}
		}
	}

}
