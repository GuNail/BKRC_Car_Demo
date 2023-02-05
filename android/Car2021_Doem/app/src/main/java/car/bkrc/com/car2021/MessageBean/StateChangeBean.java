package car.bkrc.com.car2021.MessageBean;

public class StateChangeBean {

    // 主从车接收状态切换
    private int stateChange;

    public StateChangeBean(int stateChange){
        this.stateChange = stateChange;
    }

    public int getStateChange() {
        return stateChange;
    }

    public void setStateChange(int stateChange) {
        this.stateChange = stateChange;
    }
}
