#include <bits/stdc++.h>

using namespace std;

// DEFINES
#define inf 0x1FFFFFFF
#define lng long long
#define ulng unsigned long long
#define ui unsigned int;
#define for_range(val, first, bound) for (__typeof(first) val = (first); val < (bound); ++val)
#define for_iter(it,c) for(__typeof((c).begin()) it=(c).begin();it!=(c).end();++it)
#define for_riter(it,c) for(__typeof((c).rbegin()) it=(c).rbegin();it!=(c).rend();++it)
#define max_array(val, array, size) for (int aux_def_index = 0; aux_def_index < size; ++aux_def_index) { val <?= array[aux_def_index] }
#define min_array(val, array, size) for (int aux_def_index = 0; aux_def_index < size; ++aux_def_index) { val >?= array[aux_def_index] }
#define present(container, val) ((container).find(val) != (container).end())

// TYPEDEF
typedef vector<double> vd;
typedef vector< vd > vvd;
typedef pair<int,int> ii;

vd eliminacion_gaussiana(int n, vvd& A) {
    // encuentro matriz triangular inferior:
    for_range(i, 0, n-1) {
        double pivote = A[i][i];
        for_range(j, i+1, n) {
            double coef = A[j][i] / pivote; // sabemos de antemano que el pivote no es cero.
            // modifico la fila j usando la fila i
            for_range(k, i, n+1) {
                A[j][k] -= coef * A[i][k];
            }
        }
    }

    // resuelvo el sistema triangular que queda
    vd X(n,0);
    for(int i = n-1; i >= 0; --i) {
        double b = A[i][n];
        for_range(j, i+1, n) {
            b -= A[i][j] * X[j];
        }
        X[i] = b / A[i][i];
    }
    return X;
}

int main() {
    // defino la matriz a resolver
    int n;
    cout << "n: ";
    cin >> n;

    vvd A(n, vd(n+1, 0));

    for_range(i, 0, n) {
        for_range(j, 0, n) {
            cout << "A[" << i+1 << "][" << j+1 << "]: ";
            int x, y, v;
            cin >> v;
            A[i][j] = v;
        }
        cout << "b[" << i+1 << "]: ";
        int v;
        cin >> v;
        A[i][n] = v;
    }

    // resuelvo
    vd X = eliminacion_gaussiana(n, A);

    cout << "Matriz triangulada: "<< endl;
    for_range(i, 0, n) {
        for_range(j, 0, n+1) {
            cout << A[i][j] << " ";
        }
        cout << endl;
    }

    cout << "Solucion al sistema: "<< endl;
    for_range(i, 0, n) {
        cout << X[i] << " ";
    }
    cout << endl;

	return 0;
}
