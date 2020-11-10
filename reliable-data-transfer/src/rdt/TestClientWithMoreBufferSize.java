package rdt;

public class TestClientWithMoreBufferSize {
    public static void main(String[] args) {
        if (args.length != 3) {
            System.out.println("Required arguments: dst_hostname dst_port local_port");
            return;
        }
        String hostname = args[0];
        int dst_port = Integer.parseInt(args[1]);
        int local_port = Integer.parseInt(args[2]);

        RDT rdt = new RDT(hostname, dst_port, local_port, 3, 3);
        RDT.setLossRate(0.4);

        byte[] data = new byte[10];
        for (int i=0; i<10; i++)
            data[i] = 0;
        rdt.send(data, 10);
        for (int i=0; i<10; i++)
            data[i] = 1;
        rdt.send(data, 10);

        for (int i=0; i<10; i++)
            data[i] = 2;
        rdt.send(data, 10);
        for (int i=0; i<10; i++)
            data[i] = 3;
        rdt.send(data, 10);

        for (int i=0; i<10; i++)
            data[i] = 4;
        rdt.send(data, 10);


        System.out.println(System.currentTimeMillis() + ":Client has sent all data " );
        System.out.flush();

        rdt.close();
        System.out.println("Client is done " );
    }
}
