#include <vector>
#include <iostream>

using namespace std;

class Utils {
	public:
		static vector<vector<double> > multiply(const vector<vector<double> > &A, const vector<vector<double> > &B) {
			if (A.size() == 0 || B.size() == 0  || A[0].size() != B.size()) {
				cout << "No coinciden los tamaños para multiplicar las matrices" << endl;
				cout << "A: " << A.size() << "x" << A[0].size() << endl; // 900 x 900
				cout << "B: " << B.size() << "x" << B[0].size() << endl; // 1 x 900
				return vector<vector<double> >();
			}

			int rows = A.size();
			int cols = B[0].size();
			int dim = B.size();
			vector<vector<double> > resultado(rows, vector<double>(cols, 0));

			for (int i = 0; i < rows; ++i) {
				for (int j = 0; j < cols; ++j) {
					for (int k = 0; k < dim; ++k) {
						resultado[i][j] += A[i][k] * B[k][j];
					}
				}
			}
			return resultado;
		}
};
