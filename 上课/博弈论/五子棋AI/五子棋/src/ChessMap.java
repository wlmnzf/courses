/*姓名：王佳欣
 * 学号：0837077
 * */
import javax.swing.*;

import java.awt.*;
import java.awt.event.*;
import java.net.URL;
@SuppressWarnings("serial")
public class ChessMap extends JFrame {
	private ImageIcon map;				//棋盘背景位图
	private ImageIcon blackchess;		//黑子位图
	private ImageIcon whitechess;		//白子位图
	private ChessPanel cp;				//棋盘
	private JPanel east;
	private JPanel west;
	private static final int FINAL_WIDTH = 450;
	private static final int FINAL_HEIGHT = 500;
	//以下为下拉菜单
	private JMenuBar menubar;			
	private JMenu[] menu={new JMenu("开始"),new JMenu("设置"),new JMenu("帮助")};
	private JMenuItem[] menuitem1={new JMenuItem("重新开始"),new JMenuItem("悔棋"),new JMenuItem("退出")};
	private JMenuItem[] menuitem2={new JMenuItem("禁手选择"),new JMenuItem("人机博弈"),new JMenuItem("人人对弈")};
	private JMenuItem[] menuitem3={new JMenuItem("规则"),new JMenuItem("关于")};
	private boolean haveai=true;		//人与人下还是人与电脑下，true与电脑下
	Mouseclicked mouseclicked=new Mouseclicked();
	MouseMoved mousemoved=new MouseMoved();
	Menuitemclicked menuclicked=new Menuitemclicked();
	
	//构造函数
	public ChessMap(){
		setTitle("五子棋 ");
		setSize(FINAL_WIDTH,FINAL_HEIGHT);
		setResizable(false);
		init();
		setLocation(Toolkit.getDefaultToolkit().getScreenSize().width / 2
				- FINAL_WIDTH / 2, Toolkit.getDefaultToolkit()
				.getScreenSize().height
				/ 2 - FINAL_HEIGHT / 2);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		cp.reset();	//新开棋局，即:置棋盘为空等
		setVisible(true);
	}
	
	//初始化与默认值
 	public void init() 
 	{
		map=new ImageIcon(getClass().getResource("bg.jpg"));
		blackchess=new ImageIcon(getClass().getResource("blackchess.gif"));
		whitechess=new ImageIcon(getClass().getResource("whitechess.gif"));
		cp=new ChessPanel(map,blackchess,whitechess);
		menubar=new JMenuBar();
		menuitem1[0].setActionCommand("Restart");
		menuitem1[1].setActionCommand("Rollback");
		menuitem1[2].setActionCommand("Exit");
		menuitem2[0].setActionCommand("Forbid");
		menuitem2[1].setActionCommand("Robot");
		menuitem2[2].setActionCommand("Human");
		menuitem3[0].setActionCommand("Rule");
		menuitem3[1].setActionCommand("About");
		for(int i=0;i<3;i++)
			menu[0].add(menuitem1[i]);
		for(int i=0;i<3;i++)
			menu[1].add(menuitem2[i]);
		for(int i=0;i<2;i++)
			menu[2].add(menuitem3[i]);
		for(int i=0;i<3;i++)
			menubar.add(menu[i]);
		Container p = getContentPane();
		setJMenuBar(menubar);
		east = new JPanel();
		west = new JPanel();
		p.add(east, "East");
		p.add(west, "West");
		p.add(cp, "Center");
		cp.addMouseListener(mouseclicked);
		cp.addMouseMotionListener(mousemoved);
		menuitem1[0].addActionListener(menuclicked);
		menuitem1[1].addActionListener(menuclicked);
		menuitem1[2].addActionListener(menuclicked);
		menuitem2[0].addActionListener(menuclicked);
		menuitem2[1].addActionListener(menuclicked);
		menuitem2[2].addActionListener(menuclicked);
		menuitem3[0].addActionListener(menuclicked);
		menuitem3[1].addActionListener(menuclicked);
	
	}
	class Mouseclicked extends MouseAdapter		//判断鼠标左击并通知棋盘和电脑
	{
		public void mouseClicked(MouseEvent e)
		{
		  if(cp.win==false){
			    if(haveai){           //和电脑博弈
    		              Point p1=new Point();
    		              p1=cp.getPoint(e.getX(),e.getY());
    		              int x=p1.x;
    		              int y=p1.y;
                          // 如果该位置已经放置棋子
    		              System.out.println("x="+x+",y="+y);
                          if (cp.isChessOn[x][y] != 2)
                                     return;
                          // 玩家为黑棋,考虑禁手
                          if( cp.able_flag && cp.bw == 0) {//able_flag设置了禁手并且是黑棋在下
             	                 int type = cp.getType(x,y,cp.bw);
             	                 String str = null;
             	                 switch(type){
             		             case 20: 
             			               str = "黑长连禁手!请选择其它位置下棋!";
             			               break;
             		             case 21:
             			               str = "黑四四禁手!请选择其它位置下棋!";
             			               break;
             		             case 22: 
             			               str = "黑三三禁手!请选择其它位置下棋!";
             			               break;
             		             default : break;
             	                 }
             	                 if(str != null) {
             		                    JOptionPane.showMessageDialog(null,str);
             		                     return;
             	                 }
     		              }
                          boolean flag=cp.haveWin(x, y, cp.bw);
                          cp.update( x, y );
                          cp.putVoice();  //落子声音
                          // 第一步棋,需初始化设置边界值
                         if( cp.chess_num == 1){  
                      	 if(x-1>=0)
                     	          cp.x_min = x-1;
                         if(x-1<=15)
                     	          cp.x_max = x+1;
                         if(y-1>=0)
                     	          cp.y_min = y-1;
                         if(y-1<=15)
                     	          cp.y_max = y+1;
                  }
                 else 
                 	cp.resetMaxMin(x,y);
                 if (flag) {
                     cp.wined(1 - cp.bw);
                     return;
                 }
                 cp.putOne(cp.bw);
			}else{                                        //和人博弈
				Point p1=new Point();
 		        p1=cp.getPoint(e.getX(),e.getY());
 		        int x=p1.x;
 		        int y=p1.y;
                 // 如果该位置已经放置棋子
 		        System.out.println("x="+x+",y="+y);
                 if (cp.isChessOn[x][y] != 2)
                             return;
                 // 玩家为黑棋,考虑禁手
                 if( cp.able_flag && cp.bw == 0) {
          	           int type = cp.getType(x,y,cp.bw);
          	           String str = null;
          	           switch(type){
          		       case 20: 
          			       str = "黑长连禁手!请选择其它位置下棋!";
          			       break;
          		       case 21:
          			       str = "黑四四禁手!请选择其它位置下棋!";
          			       break;
          		       case 22: 
          			       str = "黑三三禁手!请选择其它位置下棋!";
          			       break;
          		       default : break;
          	           }
          	           if(str != null) {
          		               JOptionPane.showMessageDialog(null,str);
          		               return;
          	           }
  		       }
                boolean flag=cp.haveWin(x, y, cp.bw);
                cp.update( x, y );
                cp.putVoice();  //落子声音
                cp.repaint();
              // 第一步棋,需初始化设置边界值
              if( cp.chess_num == 1){  
              	if(x-1>=0)
                  	cp.x_min = x-1;
                  if(x-1<=15)
                  	cp.x_max = x+1;
                  if(y-1>=0)
                  	cp.y_min = y-1;
                  if(y-1<=15)
                  	cp.y_max = y+1;
              }
              else 
              	cp.resetMaxMin(x,y);
              if (flag) {
                  cp.wined(1 - cp.bw);
                  return;
              }
			}
    	} 
		}
	}
	class MouseMoved implements MouseMotionListener		//调试用，获得鼠标位置
	{
		public void mouseMoved(MouseEvent e)
    	{
    		cp.showMousePos(e.getPoint());
    	}
    	public void mouseDragged(MouseEvent e)
    	{}
	}
	class Menuitemclicked implements ActionListener		//菜单消息处理
	{
		public void actionPerformed(ActionEvent e) 
		{
      		JMenuItem target = (JMenuItem)e.getSource();
      		String actionCommand = target.getActionCommand();
      		if(actionCommand.equals("Restart")){ 		//重开一局
        	   cp.reset();	
        	   if(cp.sbw==cp.WHITE_ONE)
        		   cp.update(7, 7); 
        	   //player=cp.BLACK_ONE;
      		}
      		if(actionCommand.equals("Rollback")){ 		//悔棋
      			if(cp.win) {
        			JOptionPane.showMessageDialog(null,"棋局已经结束,不能悔棋!请重新开始新的棋局!");
        			return;
                }
        		// 当前轮到玩家下棋,取消两步  否则,取消一步
        		if(cp.chess_num >= 2 && cp.bw == cp.sbw){
        			cp.isChessOn[cp.pre[cp.chess_num-1][0]][cp.pre[cp.chess_num-1][1]] = 2;
        			cp.isChessOn[cp.pre[cp.chess_num-2][0]][cp.pre[cp.chess_num-2][1]] = 2;
        			cp.chess_num -= 2;
        			cp.repaint();
        		}
        		else if(cp.chess_num >= 1 && cp.bw == 1-cp.sbw){
        			cp.isChessOn[cp.pre[cp.chess_num-1][0]][cp.pre[cp.chess_num-1][1]] = 2;
        			cp.chess_num --;
       				cp.repaint();
       			}
      		}
      		else if(actionCommand.equals("Exit")){ 		//退出
        		System.exit(1);	
      		}
      		else if(actionCommand.equals("Forbid")){     //禁手选择
        		Object[] options = { "无禁手", "有禁手" };
        		int sel = JOptionPane.showOptionDialog(
          				null, "你的选择：", "禁手选择",
          				JOptionPane.DEFAULT_OPTION,
          				JOptionPane.QUESTION_MESSAGE, null,
          				options, options[0]);
          		if(sel==1){
                        cp.able_flag=true;
                        System.out.println("有禁手");
          		}else{
          			    cp.able_flag=false;
                        System.out.println("无禁手");
          		}
          	}
      		else if(actionCommand.equals("Robot")){            //人机博弈
      			haveai=true;
      			Object[] options = { "人类先手", "机器先手" };
        		int sel = JOptionPane.showOptionDialog(
          				null, "你的选择：", "先手选择",
          				JOptionPane.DEFAULT_OPTION,
          				JOptionPane.QUESTION_MESSAGE, null,
          				options, options[0]);
          		if(sel==1){       //机器先手
          			    cp.sbw=cp.WHITE_ONE;
          			    cp.update(7, 7);
          			    System.out.println("机器先手");
         			    
          		}else{             //人先手
          			    //player=cp.BLACK_ONE;
          			    cp.sbw=cp.BLACK_ONE;
          			    System.out.println("人先手");
          		}
      		}
          	else if(actionCommand.equals("Human")){ 		//人人博弈
        		haveai=false;	
        		cp.setHumanhuman(true);
      		}else if(actionCommand.equals("Rule")){          //规则
      			JOptionPane.showConfirmDialog(null,
      			"1、无禁手：" +"\n"+
				"   黑白双方依次落子，任一方先在棋盘上形成连续的五个(含五个以上)棋子的一方为胜。" +"\n"+
				"2、有禁手：（走禁手就输，禁手不能落子）" +"\n"+
				"   鉴于无禁手规则黑棋必胜，人们不断采用一些方法限制黑棋先行的优势，以平衡黑白双方的形式。" +"\n"+
				"   于是针对黑棋的各种禁手逐渐形成。" +"\n"+
				"   禁手主要分为以下几类：" +"\n"+
				"   (1)黑长连禁手：连成六个以上连续相同的棋子。" +"\n"+
				"   (2)黑三三禁手：两个以上的活三。" + "\n"+
				"   (3)黑四四禁手：两个以上的四。" + "\n"+
				"   禁手是针对黑棋而言的，白棋没有任何禁手。" ,"规则",JOptionPane.CLOSED_OPTION,JOptionPane.INFORMATION_MESSAGE);
      		}
      		else if(actionCommand.equals("About")){ 		//版权与帮助
        		JOptionPane.showConfirmDialog(null,"团队成员：\n" +
        				                                      "陈江涛   07061229\n" +
        				                                      "夏军       07061225\n" +
        				                                      "李盘龙   07061230\n" +
        				                                      "郝云焘   07061207\n","关于",JOptionPane.CLOSED_OPTION,JOptionPane.INFORMATION_MESSAGE);	
      		}
    	}

	}
  public static void main(String[] args) {
	    new ChessMap();	
  }
} 
