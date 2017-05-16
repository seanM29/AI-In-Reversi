import java.util.HashSet;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.Set;

/**
 * Created by sean on 2017/4/14.
 */
public class Node {
    double Reward=0;
    double markValue=0;
    int VisitTime=0;
    Set location =new HashSet();
    Set feasible_location =new HashSet();       //剩下可以落子的地方 没被访问过的
    boolean terminal=false;
    String ID;
    Node Parent=null;
    LinkedList<Node> childlist = new LinkedList<Node>();
    int maxChildernSize=-1;
    int Player=-1;
    int []positon=null;

    public Node() {
        ID="test wtf";
    }


    public void setMarkValue(double t){
        markValue=t;
    }


   public Node(Node p,String id,boolean t,Set nextlocation,int player,int []pos){
        Parent=p;
        ID =id;
       terminal=t;
       location=nextlocation;

       maxChildernSize=nextlocation.size();
       Player=player;
       positon=pos;

       Iterator<int[]> it = nextlocation.iterator();
       while (it.hasNext()) {
           int[] postion = it.next();
           feasible_location.add(postion);
       }
    }

    //初始化生成结点
    public Node(boolean t,Set nextlocation,int player) {
        terminal=t;
        location=nextlocation;
        Iterator<int[]> it = nextlocation.iterator();
        while (it.hasNext()) {
            int[] postion = it.next();
            feasible_location.add(postion);
        }
        Player=player;
    }

    public Node(Node p,String id,boolean t,Set nextlocation,int player) {
        Parent=p;
        ID =id;
        terminal=t;
        location=nextlocation;
        maxChildernSize=nextlocation.size();
        Player=player;
        Iterator<int[]> it = nextlocation.iterator();
        while (it.hasNext()) {
            int[] postion = it.next();
            feasible_location.add(postion);
        }
    }

    //增加子节点
    public boolean AddChildren(Node c){
        if(childlist.add(c)) {
            return true;
        }
        else{
            return false;
        }
    }


    //terminal return true; else return false
    public boolean getTerminal(){
        return terminal;
    }

    public boolean Whther_FullyExpand() {
        if(childlist==null||childlist.size()<maxChildernSize){
            return false;
        }
        else {
            return true;
        }

    }

    public Set getLocation(){
        return location;
    }
    public Set get_Feasible_Location(){
        return feasible_location;
    }

    public LinkedList<Node> getChildren() {
        return  childlist;
    }

    public int getVisitTime() {
        return VisitTime;
    }

    public double getReward(){
        return Reward;
    }

    public int getPlayer(){

        return Player;
    }

    public void update_Reward_VisitTime(int winner){
       if(winner==Player){
           Reward-=1;
       }
       else{
           Reward+=1;
       }
        VisitTime++;
    }

    public Node getParent() {
        return Parent;
    }

    public String getID() {
        return ID;
    }

    public Node getChildren(String nodeID) {
        for(int i=0;i<childlist.size();i++){
            if(childlist.get(i).getID()==nodeID){
                return childlist.get(i);
            }
        }
        return null;
    }

    public int[] getPos(){
        return positon;
    }

    public double getMrakvalue() {
        return markValue;
    }

    //检查该位置动作是否已经做过了,已经做过返回true，
   /* public boolean checkPos(int[] postion) {
        int []child=null;

        if(childlist==null){
            return false;
        }

        for(int i=0;i<childlist.size();i++){
            child=childlist.get(i).getPos();
            if((postion[0]==child[0])&&(postion[1]==child[1])){
                return true;
            }
        }
        return false;
    }*/
}
