import java.util.Iterator;
import java.util.LinkedList;
import java.util.Random;
import java.util.Set;

/**
 * Created by sean on 2017/4/14.
 */
public class AI {
    static final double C = 0.25;
    private int Board[][];                      //引用
    private int board[][] = new int[8][8];     //直接赋值
    private int Simulation_time = 3000;
    private Rule rule;
    private int PlayerType;
    private Score score = null;
    Node test=new Node();
    static final int WhitePlayer = 1;
    static final int BlackPlayer = 2;

    int debug = 0;
    public AI(Rule r, int[][] b, int player, Score s) {
        rule = r;
        Board = b;
        PlayerType = player;
        score = s;
    }

//游戏正式执行程序
    public int[] Play() {
        long begintime = System.currentTimeMillis();
        long endtime = System.currentTimeMillis();
        long costTime = (endtime - begintime);
        double Simulation_reward = 0;
        int simulation_time = 0;
        BackupBoard();

        Node root = new Node(null, getCurrentID(), rule.checkTerminal(), rule.get_loaction(PlayerType), PlayerType);
        //MCTS搜索执行
        while (costTime < Simulation_time) {
            int winner=-1;
            Node v = TreePolicy(root);
            winner = DefaultPolicy(v);
            BP(v, winner);
            simulation_time++;
            endtime = System.currentTimeMillis();
            costTime = (endtime - begintime);
            RestoreBoard();
           //System.out.println("Simulation time is :" + simulation_time);
        }

        System.out.println("Simulation time is :" + simulation_time);
        for(int i=0;i<root.getChildren().size();i++) {
            System.out.println("Next action reward:" + root.getChildren().get(i).getReward()+"Mark value:"+root.getChildren().get(i).getMrakvalue());
        }
        System.out.println("best action reward:" + BestChild(root, C).getReward());
        //得到AI结果
        return BestChild(root, C).getPos();
    }

    //BP更新结点的结果
    private void BP(Node v, int winner) {
        Node tmp = v;
        while (v != null) {
            v.update_Reward_VisitTime(winner);
            v = v.getParent();
        }
    }

    //随机模拟
    private int DefaultPolicy(Node v) {
        Node tmp = v;
        double reward = 0;
        int winner=-1;

        while (!tmp.getTerminal()) {
            tmp = NextNode(tmp);
        }

        return score.winner();

    }

    //随机选择一个action用来模拟
    public Node NextNode(Node tmp) {
        Random random = new Random();
        Node result = null;
        int nodePlayer = -1;

        int freeLocationSize = tmp.getLocation().size();

        //下一步是哪一种type
        if (tmp.getPlayer() == WhitePlayer) {
            nodePlayer = BlackPlayer;
        } else {
            nodePlayer = WhitePlayer;
        }
        //当前没有地方可以落子，直接换成另一方
        if(freeLocationSize<=0){

            result = new Node(rule.checkTerminal(), rule.get_loaction(nodePlayer),nodePlayer);

            return result;
        }

        int s = random.nextInt(freeLocationSize);

        //选择一个随机的action
        Iterator<int[]> it = tmp.getLocation().iterator();
        int i = 0;
        int[] postion = null;
        while (it.hasNext()) {
            if (i == s) {
                postion = it.next();
                break;
            }
            i++;
        }

        //更新棋盘
        rule.update(postion, tmp.getPlayer());


      /* if(rule.get_loaction(nodePlayer).size()<=0){
            System.out.println("stop");

            System.out.println(rule.get_loaction(nodePlayer).size());
        }*/
        result = new Node(rule.checkTerminal(), rule.get_loaction(nodePlayer),nodePlayer);
      /*  if(rule.get_loaction(nodePlayer).size()<=0){
            for(int k=0;k<8;k++){
                for(int j=0;j<8;j++){
                    System.out.print(board[k][j]);
                }
                System.out.print("\n");
            }
            System.out.println("########");
        }
*/
        return result;
    }

    /*private boolean checkID(String nodeID, Node tmp) {
        LinkedList<Node> children = tmp.getChildren();
        for (int i = 0; i < children.size(); i++) {
            if (children.get(i).getID() == nodeID) {
                return true;
            }
        }
        return false;
    }*/

    //扩展搜索game tree
    public Node TreePolicy(Node root) {
        Node tmp = root;
        debug++;
        if(tmp==null){
            System.out.println("heell");
        }
        while (!tmp.getTerminal()) {
            //已经被全部扩展了
            if (tmp.Whther_FullyExpand()) {
                tmp = BestChild(tmp, C);
                if(tmp==null){
                    System.out.println("wwll");
                }
                if(tmp==test){
                    System.out.println(tmp.getID());
                }
            } else {
                return Expand(tmp);
            }
        }
        //System.out.println("debug:"+debug);
        return tmp;

    }


    //选择出最优结点
    public Node BestChild(Node root, double c) {

        LinkedList<Node> Children = root.getChildren();
        Node result = null;
        int Rootnumber = root.getVisitTime();
        double max = -1000;
        Node children = null;

        while(Children.size()==0){
            System.out.println("fuck you");
            while(true);
        }
        //计算并得到结果
        for (int i = 0; i < Children.size(); i++) {
            children = Children.get(i);
            int childrenNumber = children.getVisitTime();
            double markValue = 1.0 * children.getReward() + c * Math.sqrt(2.0 * Math.log(Rootnumber) / childrenNumber);
            children.setMarkValue(markValue);
            if (markValue > max) {
                max = markValue;
                result = children;
            }
        }


        return result;
    }


    //随机选择一个未被访问的结点来扩展
    public Node Expand(Node tmp) {

        int nodePlayer = -1;


        if (tmp.getPlayer() == WhitePlayer) {
            nodePlayer = BlackPlayer;
        } else {
            nodePlayer = WhitePlayer;
        }
        Random random = new Random();
        int LocationSize = tmp.get_Feasible_Location().size();
        int s;
        int[] postion = null;



        //选择一个随机结点
        s = random.nextInt(LocationSize);
        Iterator<int[]> it = tmp.get_Feasible_Location().iterator();
        int i = 0;
        while (it.hasNext()) {
            if (i == s) {
                postion = it.next();
                break;
            }
            i++;
        }

        //更新数据
        rule.update(postion, tmp.getPlayer());
        String nodeID = getCurrentID();

        //返回结果更新数据
        Node result = new Node(tmp, nodeID, rule.checkTerminal(), rule.get_loaction(nodePlayer), nodePlayer, postion);
        tmp.AddChildren(result);
        tmp.get_Feasible_Location().remove(postion);

        return result;
    }


    //random get the next location
    private int[] getPostion(Set location_player) {
        Random random = new Random();
        int s = random.nextInt(location_player.size());

        Iterator<int[]> it = location_player.iterator();
        int i = 0;
        while (it.hasNext()) {
            if (i == s) {
                int[] result = it.next();
                return result;
            }
            i++;
        }

        int[] error = {-1, -1};
        return error;
    }


    //Back up the board
    private void BackupBoard() {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                board[i][j] = Board[i][j];
            }
        }
    }


    //restore the board and rule(feasible location) for the next simulation or next action
    private void RestoreBoard() {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                Board[i][j] = board[i][j];
            }
        }
        rule.reset_location();


    }


    //to make the board become the string ID
    public String getCurrentID() {
        String result = null;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                result += (Board[i][j] + "");
            }
        }
        return result;
    }


}
