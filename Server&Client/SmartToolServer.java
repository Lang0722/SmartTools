import javax.xml.crypto.Data;
import java.net.*;
import java.io.*;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.*;

public class SmartToolServer extends Thread {
    private Socket socket = null;
    boolean status;
    PrintWriter out = null;
    BufferedReader in = null;
    BufferedReader in_file = null;
    String inputLine, outputLine;


    public SmartToolServer(Socket socket) {
        super("SmartToolServer");
        this.socket = socket;
    }

    public void run() {
        try {
            // set the input output stream for the connection
            out = new PrintWriter(socket.getOutputStream(), true);
            in = new BufferedReader(
                    new InputStreamReader(
                            socket.getInputStream()));

            status = true;
            while (status) {
                if ((inputLine = in.readLine()) != null)
                    processInput(inputLine);
                if (inputLine.equals("Bye"))
                    break;
            }

            out.close();
            in.close();
            socket.close();
        } catch (IOException | InterruptedException e) {
            e.printStackTrace();
        }

    }

    private void processInput(String inputLine) throws IOException, InterruptedException {
        List<String> temp_list = new ArrayList<String>();
        List<String> ldr_list = new ArrayList<String>();
        List<String> time = new ArrayList<String>();

        in_file = new BufferedReader(new FileReader("data.txt"));
        if (inputLine.equals("1") || inputLine.equals("2")) {
            int lineNum = 1;
            String prestr;
            while ((prestr = in_file.readLine()) != null){
                lineNum++;
            }

            in_file.close();
            in_file = new BufferedReader(new FileReader("data.txt"));

            for (int i =0; i<lineNum-8;i++){
                in_file.readLine();
            }

            for (int i = 0; i < 6; i++) {
                String str = in_file.readLine();
                if (str != null) {
                    String[] temp = str.split("[;]");
                    System.out.println(str);
                    time.add(temp[0]);
                    temp_list.add(temp[1]);
                    ldr_list.add(temp[2]);
                }else{
                    temp_list.add("0");
                    ldr_list.add("0");
                    time.add("0");
                }
            }
            out.println(temp_list);
            out.println(ldr_list);
            out.println(time);
            out.println("done");
            return;
        }

        in_file.close();

    }
}
