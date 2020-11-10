/**
 * Name: TRUNGLAM NGUYEN
 * Email: tln3@sfu.ca
 */
package rdt;

public class TestClientWithLongData {
    public static void main(String[] args) {
        if (args.length != 3) {
            System.out.println("Required arguments: dst_hostname dst_port local_port");
            return;
        }
        String hostname = args[0];
        int dst_port = Integer.parseInt(args[1]);
        int local_port = Integer.parseInt(args[2]);

        RDT rdt = new RDT(hostname, dst_port, local_port, 1, 3);
        RDT.setLossRate(0.4);

        byte[] data = new byte[300];
        for (int i=0; i<300; i++)
            data[i] = 0;
        rdt.send(data, 300);
        for (int i=0; i<300; i++)
            data[i] = 1;
        rdt.send(data, 300);

        System.out.println(System.currentTimeMillis() + ":Client has sent all data " );
        System.out.flush();

        rdt.close();
        System.out.println("Client is done " );
    }
}
