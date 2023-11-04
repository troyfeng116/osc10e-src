import java.io.*;
import java.net.*;

public class EchoClient {
    private static final String HOST = "127.0.0.1";
    private static final int PORT = 7979;

    public static void main(String[] args) {
        try {
            Socket mySocket = new Socket(HOST, PORT);

            BufferedReader myStdinInputReader = new BufferedReader(new InputStreamReader(System.in));
            PrintWriter myPout = new PrintWriter(mySocket.getOutputStream(), true);
            BufferedReader myServerInputReader = new BufferedReader(new InputStreamReader(mySocket.getInputStream()));

            String myLine;
            while (true) {
                System.out.printf("Enter line to echo: ");
                myLine = myStdinInputReader.readLine();
                if (myLine == null) {
                    break;
                }

                myPout.println(myLine);

                System.out.printf("echo: %s\n", myServerInputReader.readLine());
            }

            mySocket.close();
        } catch (IOException myIoException) {
            System.err.println(myIoException);
        }
    }
}