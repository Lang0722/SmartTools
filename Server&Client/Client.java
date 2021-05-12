import java.io.*;
import java.net.*;
import java.text.DecimalFormat;
import java.util.Collections;
import java.util.List;

public class Client {
    private Socket socket = null;
    private PrintWriter out = null;
    private BufferedReader in = null;
    BufferedReader stdIn;
    String fromServer;
    String fromUser;
    String[] tempData;
    String[] ldrData;
    String[] time;

    public void sendRequest() throws IOException {
        try {
            socket = new Socket("localhost", 8600);
            // Chain a writing stream
            out = new PrintWriter(socket.getOutputStream(), true);
            // Chain a reading stream
            in = new BufferedReader(
                    new InputStreamReader(
                            socket.getInputStream()));

        } catch (UnknownHostException e) {
            System.err.println("Don know this host");
            System.exit(-1);
        } catch (IOException e) {
            System.err.println("Can not connect this socket");
            System.exit(-1);
        }

        stdIn = new BufferedReader(
                new InputStreamReader(System.in));

        // keep process the input from user and server
        while (true) {
            processInput();
        }
    }

    private void processInput() throws IOException {
        // print the instructions
        System.out.println("please input");
        System.out.println("1. Graph");
        System.out.println("2. Data");

        //wait user input
        fromUser = stdIn.readLine();
        System.out.println("Client: " + fromUser);
        out.println(fromUser);

        int flag = 0;

        String temp_list = null;
        String ldr_list = null;
        String time_list = null;
        // wait the server's message and store data to the string
        while (!(fromServer = in.readLine()).equals("done")) {
            if (flag == 0) {
                temp_list = fromServer;
            } else if (flag == 1) {
                ldr_list = fromServer;
            } else if (flag == 2) {
                time_list = fromServer;
            }
            flag++;
        }

        // format the server's message
        assert temp_list != null;
        temp_list = temp_list.replaceAll("[\\[\\]\\s*]", "");
        assert ldr_list != null;
        ldr_list = ldr_list.replaceAll("[\\[\\]\\s*]", "");
        assert time_list != null;
        time_list = time_list.replaceAll("[\\[\\]]", "");

        tempData = temp_list.split(",");
        ldrData = ldr_list.split(",");
        time = time_list.split(",");

        double[] tempDataNum = new double[tempData.length];
        double[] ldrDataNum = new double[ldrData.length];

        for (int i = 0; i < tempData.length; i++) {
            tempDataNum[i] = Integer.parseInt(tempData[i]) * 0.01;
            ldrDataNum[i] = Integer.parseInt(ldrData[i]);
        }

        if (fromUser.equals("1")) { // show the temp and ldr's graph
            System.out.println("Temperature:");
            System.out.print(" ");
            for (int i = 0; i < tempDataNum.length; i++) {
                System.out.print(time[i]);
                System.out.print("  ");

                double percent = tempDataNum[i] / 50;
                for (int j = 0; j < percent * 20; j++) {
                    System.out.print("*");
                }
                System.out.println();
            }

            System.out.println("LDR:");
            System.out.print(" ");
            for (int i = 0; i < ldrData.length; i++) {
                System.out.print(time[i]);
                System.out.print("  ");

                double percent = ldrDataNum[i] /100;
                for (int j = 0; j < percent * 20; j++) {
                    System.out.print("*");
                }
                System.out.println();
            }
        } else if (fromUser.equals("2")) {   // show the temp and ldr's data
            System.out.println(" ");
            for (int i = 0; i < tempDataNum.length; i++) {
                System.out.print(time[i]);
                System.out.print("  ");

                DecimalFormat df = new DecimalFormat("0.00");
                System.out.print("T = " + df.format(tempDataNum[i]) + " C ");

                System.out.println("LDR = " + ldrData[i]+"%");
            }
        }

    }

    public static void main(String[] args) throws Exception {
//        Integer[] integers = new Integer[]{1, 2, -5, 15, 1, 6, 7, 8, 7, 10,15};
//        GraphDisplay.echo(integers);
        Client client = new Client();
        client.sendRequest();
    }
}
