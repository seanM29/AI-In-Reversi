import java.awt.*;
import java.security.PublicKey;
import java.util.HashSet;
import java.util.Set;

/**
 * Created by sean on 2017/4/11.
 */
public class Rule {
    static final int WhitePlayer = 1;
    static final int BlackPlayer = 2;
    static final int White = 1;
    static final int Black = 2;

    int counter=0;
    public int[][] board = new int[8][8];
    Set feasible_location_playerB = new HashSet();
    Set feasible_location_playerW = new HashSet();
    int[][] Move = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

    public Rule() {
    }

    //得到用户可以落子的地方
    public Set get_loaction(int playerType) {
        if (playerType == WhitePlayer) {
            return feasible_location_playerW;
        } else {
            return feasible_location_playerB;
        }
    }

    public Set get_location_playerB() {
        return feasible_location_playerB;
    }

    public Set get_location_playerW() {
        return feasible_location_playerW;
    }

    //检查是否有可以落子的地方
    boolean check_feasible_location(int player) {
        if (player == BlackPlayer) {
            if (feasible_location_playerB.isEmpty()) {
                return false;
            } else {
                return true;
            }
        } else {
            if (feasible_location_playerW.isEmpty()) {
                return false;
            } else {
                return true;
            }
        }
    }


    //重置获得用户可以落子的地方
    void reset_location() {
        feasible_location_playerB.clear();
        feasible_location_playerW.clear();
        counter++;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                for (int k = 0; k < 8; k++) {
                    if (board[i][j] == 0 && Is_feasible(i, j, Move[k]) == Black) {
                        int[] a = {i, j};
                        feasible_location_playerB.add(a);

                    }
                    if (board[i][j] == 0 && Is_feasible(i, j, Move[k]) == White) {

                        int[] a = {i, j};
                        feasible_location_playerW.add(a);

                    }
                }
            }
        }
        /*if (feasible_location_playerB.size() <= 0) {
            for (int j = 0; j < 8; j++) {
                for (int k = 0; k < 8; k++) {
                    System.out.print(board[j][k]);
                }
                System.out.print("\n");
            }
            System.out.print("\n");
            //System.out.print(counter);
        }*/
    }

    //检查该位置是否为可以落子的地方，针对当前的移动
    int Is_feasible(int x, int y, int[] M) {
        int cx = x + M[0];
        int cy = y + M[1];
        int state = 0;

        //提前剪枝
        if (cx >= 8 || cy >= 8 || cx < 0 || cy < 0 || board[cx][cy] == 0) {
            return 0;
        } else if (board[cx][cy] == White) {
            state = White;
        } else {
            state = Black;
        }


        cx = x + M[0];
        cy = y + M[1];
        //进行判断
        while ((0 <= cx) && (cx < 8) && (0 < cy) && (cy < 8)) {
            //System.out.println("cx:"+M[0]+"cy:"+M[1]);
            if (board[cx][cy] == state) {
                cx = cx + M[0];
                cy = cy + M[1];
            } else if (board[cx][cy] == 0) {
                return 0;
            } else {
                return board[cx][cy];
            }
        }
        return 0;
    }

    public void setBoard(int[][] b) {
        board = b;
        reset_location();
    }

    //更新棋盘
    public void update(int[] postion, int state) {
        board[postion[0]][postion[1]] = state;
        for (int i = 0; i < 8; i++) {
            if (whether_turn(postion, Move[i])) {
                int cx = postion[0] + Move[i][0];
                int cy = postion[1] + Move[i][1];
                //System.out.println("cx:"+cx+"cy:"+cy+"state:"+board[cx][cy]);
                while (board[cx][cy] != state) {
                    board[cx][cy] = state;
                    cx = cx + Move[i][0];
                    cy = cy + Move[i][1];
                    // System.out.println("cx:"+cx+"cy:"+cy+"state:"+board[cx][cy]);
                }
            }
        }
        reset_location();
    }

    //判断当前移动是否可以翻转
    private boolean whether_turn(int[] postion, int[] M) {

        int state = board[postion[0]][postion[1]];
        int cx = postion[0] + M[0];
        int cy = postion[1] + M[1];

        if (cx >= 8 || cy >= 8 || cx < 0 || cy < 0 || board[cx][cy] == state || board[cx][cy] == 0) {
            return false;
        }
        while ((0 <= cx) && (cx < 8) && (0 < cy) && (cy < 8)) {
            if (board[cx][cy] == state) {
                return true;
            }
            cx = cx + M[0];
            cy = cy + M[1];
        }
        return false;
    }

    //检查游戏是否结束
    public boolean checkTerminal() {
        if ((!check_feasible_location(WhitePlayer)) && (!check_feasible_location(BlackPlayer))) {
            return true;
        } else {
            return false;
        }
    }
}
