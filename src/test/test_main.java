package test;

import melco.test.cantool.CanTool;

public class test_main {
    public static void main(String[] args) {
        CanTool canTool = new CanTool();
        if (canTool.init("slcan0") > 0) {
            System.out.println("CAN device initialized successfully");
        }

//        if (canTool.sendCommand("122#00010000") > 0) {
//            System.out.println("CAN command sent successfully");
//        }

        if (canTool.sendCommand("122#04020000") > 0) {
            System.out.println("CAN command sent successfully");
        }

    }
}
