package melco.test.cantool;

public class CanTool {
    native public int init(String deviceName);
    native public int sendCommand(String canCommand);
    static {
        System.loadLibrary("cantool");
    }
}
