/**
 * Created by sean on 2017/4/11.
 */
public class Score {
    int Score_playerW=2;
    int Score_playerB=2;
    static  final int WhitePlayer =1;
    static  final int BlackPlayer =2;
    int [][]board;

    void setBoard(int [][]b){
        board=b;
    }


//更新分数
    public void update(){
        Score_playerW=0;
        Score_playerB=0;
        for(int i=0;i<8;i++){
            for(int j=0;j<8;j++){
                if(board[i][j]==2){
                    Score_playerB++;
                }
                if(board[i][j]==1){
                    Score_playerW++;
                }
            }
        }
    }

    //得出赢家
    public int winner(){
        update();
        if(Score_playerW>Score_playerB){
            return WhitePlayer;
        }
        else if(Score_playerB>Score_playerW){
            return  BlackPlayer;
        }
        else {
            return 0;
        }
    }


}
