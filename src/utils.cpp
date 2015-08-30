
static vector<vector<double> > multiply(const vector<vector<double> > &A, const vector<vector<double> > &B) {
	if (A.size() == 0 || B.size() == 0  || A[0].size() != B.size()) {
		return vector<vector<double> >();
	}

	int rows = A.size();
	int cols = B[0].size();
	int dim = B.size();
	vector<vector<double> > resultado(rows, vector<double>(cols, 0));

	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			for (int k = 0; k < dim; ++k) {
				resultado[i][j] = A[i][dim] * B[dim][j];
			}
		}
	}
	return resultado;
}
