import java.net.*;
import java.io.*;
import java.util.concurrent.*;

public class Server {
    public static void main(String[] args) throws IOException {

        ServerSocket server = null;
        ExecutorService service = null;
        Socket client = null;
        // creat the thread pool and server socket
        try {
            server = new ServerSocket(8600);
            System.out.println("success");
        }catch (IOException e){
            System.err.println("Can not listen on this port : 8600");
            System.exit(-1);
        }

        //creat a thread pool with volume of 15
        service = Executors.newFixedThreadPool(15);

        while (true){
            client = server.accept();
            service.submit(new SmartToolServer(client));
        }
    }
}
