import java.rmi.Naming;
import java.rmi.RemoteException;

public class ServidorRMI {
    private static int N;

    public static void main(String[] args) throws Exception {
        String url = "rmi://localhost/matrices";
        ClaseRMI obj = new ClaseRMI();

        // registra la instancia en el rmiregistry
        Naming.rebind(url, obj);
    }
}