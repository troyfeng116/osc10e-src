import java.io.*;
import java.net.*;

public class EchoServer {
    private static final int PORT = 7979;

    public static void main(String[] args) {
        try {
            ServerSocket myServerSocket = new ServerSocket(PORT);
            System.out.printf("[EchoServer] listening on port %d\n", myServerSocket.getLocalPort());

            while (true) {
                Socket myClientSocket = myServerSocket.accept();
                System.out.println("[EchoServer] accepted connection from client");

                BufferedReader myClientInputReader = new BufferedReader(
                        new InputStreamReader(myClientSocket.getInputStream()));
                PrintWriter myPout = new PrintWriter(myClientSocket.getOutputStream(), true);

                String myLine;
                while ((myLine = myClientInputReader.readLine()) != null) {
                    myPout.println(myLine);
                }

                System.out.println("[EchoServer] closing connection to client");
                myClientSocket.close();
            }
        } catch (IOException myIoException) {
            System.err.println(myIoException);
        }
    }
}