import java.lang.reflect.Array;
import java.util.*;

/**
 * Created by sean on 2017/4/11.
 */
public class Board {


    static  final int WhitePlayer =1;
    static  final int BlackPlayer =2;
    static  final int White =1;
    static  final int Black =2;


    public int board[][] = new int[8][8];

    Rule r = new Rule();
    Score score = new Score();

    int player1 = 0;
    int player2 = 0;


    //初始化
    public Board(int p1, int p2) {
        player1 = p1;
        player2 = p2;

        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if ((i == 3 && j == 3) || (i == 4 && j == 4)) {
                    board[i][j] = 1;
                }
                if ((i == 3 && j == 4) || (i == 4 && j == 3)) {
                    board[i][j] = 2;
                }
            }
        }

        r.setBoard(board);


        score.setBoard(board);
    }

    public void Print() {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (j == 7) {
                    System.out.println(board[i][j]);
                } else {
                    System.out.print(board[i][j]);
                }
            }

        }
    }

    public void Action() {

        int EndFlag = 0;
        int []NextPostion = new int[2];



        int count=0;
        AI ai =new AI(r, board, BlackPlayer,score);

        PrintBoard();
        while (true) {
            count++;
            if (r.check_feasible_location(BlackPlayer)) {
                EndFlag++;

      //          Scanner scanner = new Scanner(System.in);
                //调用扫描器扫描键盘录入的数据 for debug
  //              System.out.println("请输入一个坐标：");
//                NextPostion[0]=scanner.nextInt();
    //            NextPostion[1]=scanner.nextInt();


                //获得AI落子
                NextPostion=ai.Play();
                System.out.println("x:"+NextPostion[0]+"y:"+NextPostion[1]);
                //更新棋盘和分数并且打印
                r.update(NextPostion,Black);
                score.update();
                PrintBoard();
            }
            if (r.check_feasible_location(WhitePlayer)) {

                EndFlag++;
                //用户输入
                Scanner scanner = new Scanner(System.in);
                //调用扫描器扫描键盘录入的数据
               System.out.println("请输入一个坐标：");
             //  int score = scanner.nextInt(); //定义了一个num变量接收扫描到内容。
                //NextPostion = getPostion(r.get_location_playerW());
                NextPostion[0]=scanner.nextInt();
                NextPostion[1]=scanner.nextInt();

                //更新棋盘和数据
                System.out.println("x:"+NextPostion[0]+"y:"+NextPostion[1]);
                r.update(NextPostion,White);
                score.update();

                PrintBoard();
            }
            //游戏技术
            if (EndFlag == 0) {
                break;
            }
            EndFlag = 0;
        }
        //打印结果
        PrintResult();



    }

    //打印棋盘
    private void PrintBoard() {
        for(int i=0;i<8;i++){
            for(int j=0;j<8;j++){
                System.out.print(board[i][j]);
            }
            System.out.print("\n");
        }
        System.out.println("########");
    }

    //打印结果
    private void PrintResult() {
        switch (score.winner()){
            case 0:
                System.out.println("Even");
                break;
            case 1:
                System.out.println("Player 1 wins");
                break;
            case 2:
                System.out.println("Player 2 wins");
                break;
            default:
                System.out.println("Error");
        }
    }


    //得到随机的一个落子
    private int[] getPostion(Set location_player) {
        Random random = new Random();
        int s = random.nextInt(location_player.size());

        Iterator<int []> it = location_player.iterator();
        int i=0;
        while (it.hasNext()) {
            if(i==s){
                int [] result=it.next();
                return result;
            }
            i++;
        }

        int []error={-1,-1};
        return error;
    }






}




