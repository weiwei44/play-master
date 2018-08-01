package order.mobile.indoorbuy.com.play_master.play;

public class XPlay {

    static {
        System.loadLibrary("native-lib");
    }

    public native void init(String url);
}
