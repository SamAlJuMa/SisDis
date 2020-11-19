import java.rmi.Naming;

public class ClienteRMI {
    static private int N = 4, A[][] = new int[N][N], B[][] = new int[N][N], C[][] = new int[N][N];

    static int[][] parte_matriz(int[][] A, int inicio) {
        int[][] M = new int[N / 2][N];
        for (int i = 0; i < N / 2; i++)
            for (int j = 0; j < N; j++)
                M[i][j] = A[i + inicio][j];
        return M;
    }

    static void acomoda_matriz(int[][] C, int[][] A, int renglon, int columna) {
        for (int i = 0; i < N / 2; i++)
            for (int j = 0; j < N / 2; j++)
                C[i + renglon][j + columna] = A[i][j];
    }

    static long chechsum(int matriz[][]) {
        long resultado = 0;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                resultado += matriz[i][j];
            }
        }
        return resultado;
    }

    static void imprimeMatriz(int matriz[][], int filas, int columnas) {
        for (int i = 0; i < filas; i++) {
            for (int j = 0; j < columnas; j++) {
                System.out.printf("%10d", matriz[i][j]);
            }
            System.out.println("");
        }
        System.out.println("");
    }

    public static void main(String args[]) throws Exception {
        // Inicializa las matrices
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++) {
                A[i][j] = 2 * i - j;
                B[i][j] = 2 * i + j;
                C[i][j] = 0;
            }

        // transpone la matriz B, la matriz traspuesta queda en B
        int B_original[][] = B;
        for (int i = 0; i < N; i++)
            for (int j = 0; j < i; j++) {
                int x = B[i][j];
                B[i][j] = B[j][i];
                B[j][i] = x;
            }

        // partir matrices
        int A1[][] = parte_matriz(A, 0);
        int A2[][] = parte_matriz(A, N / 2);
        int B1[][] = parte_matriz(B, 0);
        int B2[][] = parte_matriz(B, N / 2);

        // en este caso el objeto remoto se llama "prueba", notar que se utiliza el
        // puerto default 1099
        String url_nodo0 = "rmi://13.90.30.29/matrices";
        String url_nodo1 = "rmi://23.96.41.138/matrices";
        String url_nodo2 = "rmi://23.96.101.195/matrices";
        String url_nodo3 = "rmi://23.96.111.120/matrices";

        // obtiene una referencia que "apunta" al objeto remoto asociado a la URL
        InterfaceRMI ref_nodo0 = (InterfaceRMI) Naming.lookup(url_nodo0);
        InterfaceRMI ref_nodo1 = (InterfaceRMI) Naming.lookup(url_nodo1);
        InterfaceRMI ref_nodo2 = (InterfaceRMI) Naming.lookup(url_nodo2);
        InterfaceRMI ref_nodo3 = (InterfaceRMI) Naming.lookup(url_nodo3);

        int C1[][] = ref_nodo0.multiplica_matrices(A1, B1);
        int C2[][] = ref_nodo0.multiplica_matrices(A1, B2);
        int C3[][] = ref_nodo0.multiplica_matrices(A2, B1);
        int C4[][] = ref_nodo0.multiplica_matrices(A2, B2);

        acomoda_matriz(C, C1, 0, 0);
        acomoda_matriz(C, C2, 0, N / 2);
        acomoda_matriz(C, C3, N / 2, 0);
        acomoda_matriz(C, C4, N / 2, N / 2);

        if (N == 4) {
            imprimeMatriz(A, N, N);
            imprimeMatriz(B_original, N, N);
            imprimeMatriz(C, N, N);
        }
        System.out.println("Checksum de la matriz C:" + chechsum(C));

    }
}