package rdt;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;
import java.util.Timer;

public class Test {
    public static void main(String[] args) throws SocketException, InterruptedException {
        DatagramSocket socket = new DatagramSocket(3000);
        Thread test = new Thread() {
            @Override
            public void run() {
                try {
                    byte[] buf = new byte[100];
                    DatagramPacket pkt = new DatagramPacket(buf, 100);
                    socket.receive(pkt);
                } catch (IOException e) {
                    System.out.println("Closing socket");
                }
                System.out.println("Thread end");
            }

        };
        test.start();
        Thread.sleep(1000);
        socket.close();
        System.out.println("Main thread end");
    }
}
