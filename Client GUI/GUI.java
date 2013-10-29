import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.GridLayout;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;
import java.io.InputStream;
import java.io.PrintWriter;
import java.net.MalformedURLException;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.Semaphore;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.SwingWorker;
import javax.swing.border.EmptyBorder;


public class GUI extends JFrame implements ActionListener{

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	static Socket gameSocket = null;
	static Semaphore WorkerSemaphore = new Semaphore(1);
	JPanel optionPanel,buttonPanel,scorePanel, content, board;
	static JLabel whiteScore, blackScore;

	JButton undo, newGame;
	
	static GameSquare[][] gameGrid;
	
	static ImageIcon white_icon;

	static ImageIcon black_icon;
	
	static GUI game = null;
	
	public void createButtonGrid(int width, int length){
		content=new JPanel();
		board = new JPanel();
		board.setLayout(new GridLayout(width,length));
		content.setLayout(new BorderLayout());
		gameGrid = new GameSquare[width][length];
		for(int y=0; y<length; y++){
			for(int x=0; x<width; x++){
				gameGrid[x][y]=new GameSquare(x,y);
				gameGrid[x][y].addActionListener(this);
				gameGrid[x][y].setBackground(Color.WHITE);
			    gameGrid[x][y].setForeground(Color.BLACK);
			    if(x==3 && y==3){
			    	gameGrid[x][y].setIcon(white_icon);
			    }
			    else if(x==3 && y==4){
			    	gameGrid[x][y].setIcon(black_icon);
			    }
			    else if(x==4 && y==3){
			    	gameGrid[x][y].setIcon(black_icon);
			    }
			    else if(x==4 && y==4){
			    	gameGrid[x][y].setIcon(white_icon);
			    }
			    board.add(gameGrid[x][y]);
			}
		}
		content.add(board, BorderLayout.CENTER);
		content.setVisible(true);
		setContentPane(content);
	}

	public GUI(int width, int length) throws MalformedURLException, IOException{
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		white_icon= new ImageIcon(Toolkit.getDefaultToolkit().getImage(getClass().getResource("/resources/white_circle.png")));
		black_icon= new ImageIcon(Toolkit.getDefaultToolkit().getImage(getClass().getResource("/resources/black_circle.png")));
		createButtonGrid(width,length);
		scorePanel = new JPanel ();
		optionPanel= new JPanel();
		buttonPanel=new JPanel();
		scorePanel.setLayout(new BorderLayout());
		optionPanel.setLayout(new BorderLayout());
		buttonPanel.setLayout(new BorderLayout());
		whiteScore = new JLabel("White Score: 2", JLabel.CENTER);
		blackScore = new JLabel("Black Score: 2", JLabel.CENTER);
		scorePanel.add(whiteScore,BorderLayout.NORTH);
		scorePanel.add(blackScore,BorderLayout.SOUTH);
		scorePanel.setBorder(new EmptyBorder(0,25,100,25));
		optionPanel.add(scorePanel,BorderLayout.SOUTH);
		undo= new JButton("Undo");
		undo.addActionListener(this);
		newGame = new JButton("New Game");
		newGame.addActionListener(this);
		newGame.setVisible(false);
		buttonPanel.add(undo,BorderLayout.SOUTH);
		buttonPanel.add(newGame, BorderLayout.NORTH);
		buttonPanel.setBorder(new EmptyBorder(100,0,0,0));
		optionPanel.add(buttonPanel, BorderLayout.NORTH);
		content.add(optionPanel, BorderLayout.EAST);
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
		String s = null;
		try {
			s = SocketRead(gameSocket);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		System.out.println(s);
		try {
			game = new GUI(8,8);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		WorkerThread my_worker = new WorkerThread("HUMAN-AI\0\n");
		my_worker.execute();
		my_worker = new WorkerThread("HARD\0\n");
		my_worker.execute();
		my_worker= new WorkerThread("WHITE\0\n");
		my_worker.execute();
		game.setSize(1000,1000);
		game.setVisible(true);
	}

	@Override
	public void actionPerformed(ActionEvent e) {
		//System.out.println(((GameSquare) e.getSource()).getBoardLocation());
		if(e.getSource().equals(undo)){
			WorkerThread my_worker = new WorkerThread("UNDO\0\n");
			my_worker.execute();
			my_worker = new WorkerThread("DISPLAY\0\n");
			my_worker.execute();
		}
		else if(e.getSource().equals(newGame)){
			try {
				gameSocket.close();
			} catch (IOException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}
			try {
				game= new GUI(8,8);
			} catch (IOException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}
			try {
				gameSocket= new Socket("sun.cse.tamu.edu",50000);
			} catch (IOException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}
			WorkerThread my_worker = new WorkerThread("HUMAN-AI\0\n");
			my_worker.execute();
			my_worker = new WorkerThread("HARD\0\n");
			my_worker.execute();
			my_worker= new WorkerThread("WHITE\0\n");
			my_worker.execute();
			my_worker= new WorkerThread("DISPLAY\0\n");
			my_worker.execute();
		}
		else{
			WorkerThread my_worker = new WorkerThread(((GameSquare) e.getSource()).getBoardLocation() + "\0\n");
			my_worker.execute();
		}
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
		sock.setSoTimeout(5000);
		in.read(data, 0, 512);
		String ret = new String(data, "ASCII");
		return ret;
	}
	
	static class GameSquare extends JButton{
		/**
		 * 
		 */
		private static final long serialVersionUID = 2L;
		ImageIcon white_icon = new ImageIcon("white_circle.jpg");
		ImageIcon black_icon= new ImageIcon("black_circle.gif");
		int internal_column;
		int internal_row;
		
		GameSquare(int x, int y){
			internal_column=x+1;
			internal_row=y+1;
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
			String s = new String("");
			try {
				WorkerSemaphore.acquire();
				try {
					SocketWrite(gameSocket, data);
					s=SocketRead(gameSocket);
				} finally {
					WorkerSemaphore.release();
				}
			} catch (Exception e3) {
				e3.printStackTrace();
			}
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
			System.out.println("'"+data+"'");
			if(data.length()>4 && data!="DISPLAY\0\n"){
				if(s.substring(0,2).equals("OK")){
					System.out.println("Command Executed successfully");
				}
				else if(s.substring(0,7).equals("ILLEGAL")){
					System.out.println("Failed to execute command: " + data);
				}
			}
			else{
				if(s.substring(0,7).equals("ILLEGAL")){
					System.out.println("Illegal Move Detected");
				}
				else{
					System.out.println(s);
					int y=0;
					int whiteScoreIndex=s.indexOf("White Score: ");
					int blackScoreIndex=s.indexOf("Black Score: ");
					String whiteStringScore=s.substring(whiteScoreIndex+13,whiteScoreIndex+15);
					String blackStringScore=s.substring(blackScoreIndex+13,blackScoreIndex+15);
					if(whiteStringScore.toCharArray()[1]==' '){
						whiteStringScore=whiteStringScore.substring(0,1);
					}
					if(blackStringScore.toCharArray()[1]==' ' || blackStringScore.toCharArray()[1]=='\n'){
						blackStringScore=blackStringScore.substring(0,1);
					}
					int whiteIntScore=Integer.parseInt(whiteStringScore);
					int blackIntScore=Integer.parseInt(blackStringScore);
					whiteScore.setText("White Score: " + whiteIntScore);
					blackScore.setText("Black Score: " + blackIntScore);
					String[] board = s.split(";");
					for(String temp : board){
						if(y>1 && y <10){
							int x=0;
							String[] line=temp.split("\\|");
							for(String temp2 : line){
								//System.out.println("'"+temp2+"'");
								if(temp2.equals("@")){
									gameGrid[x-1][y-2].setIcon(black_icon);
									//System.out.println("Detected a black piece");
								}
								else if(temp2.equals("O")){
									gameGrid[x-1][y-2].setIcon(white_icon);
									//System.out.println("Detected a white piece");
								}
								else if(temp2.equals("_")){
									gameGrid[x-1][y-2].setIcon(null);
									//System.out.println("Detected an empty piece");
								}
								else{
									//System.out.println("String matched no cases!");
								}
								x++;
							}
							//System.out.println();
						}
						y++;
					}
					if(whiteIntScore+blackIntScore==64){
						if(whiteIntScore>blackIntScore){
							JOptionPane.showMessageDialog(null, "Game Over: Congratulations, you won!");
						}
						else if(whiteIntScore==blackIntScore){
							JOptionPane.showMessageDialog(null, "Game Over: You Tied!");
						}
						else{
							JOptionPane.showMessageDialog(null, "Game Over: You Lost!");
						}
					}
				}
			}
		}
	}
}
