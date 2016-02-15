//============================================================================
// Name        : MyLdpc.cpp
// Author      : wing
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================

#include"MyLdpc.h"
#include<iostream>
#include<stdlib.h>
#include<vector>
template<typename Dtype> void a(Dtype data){
	printf("a%d",data);
}
Coder::Coder(int ldpcK, int ldpcN, enum rate_type rate) :
		ldpcK(ldpcK), ldpcN(ldpcN), ldpcM(ldpcN - ldpcK), rate(rate), checkMatrix(
				ldpcK, ldpcN) {
	initCheckMatrix();
}

int Coder::initCheckMatrix() {
	//he expansion factor (z factor) is equal to n/24 for code length n;
	z = ldpcN / n_b;
	const char * hSeed;
	int seedRowLength;
	const int seedColLength = n_b;
	switch (rate) {
	case rate_1_2:
		hSeed = h_seed_1_2;
		seedRowLength = 12;
		break;
	case rate_2_3_a:
		hSeed = h_seed_2_3_a;
		seedRowLength = 8;
		break;
	case rate_2_3_b:
		hSeed = h_seed_2_3_b;
		seedRowLength = 8;
		break;
	case rate_3_4_a:
		hSeed = h_seed_3_4_a;
		seedRowLength = 6;
		break;
	case rate_3_4_b:
		hSeed = h_seed_3_4_b;
		seedRowLength = 6;
		break;
	case rate_5_6:
		hSeed = h_seed_5_6;
		seedRowLength = 4;
		break;
	}
	int permut;
	bool * tmpMatrix = (bool*) malloc(seedColLength * z * z / 8);
	typedef Triplet<bool> T;
	std::vector<T> tripletList;
	int estimation_of_entries = seedRowLength * seedColLength * z / 2;
	tripletList.reserve(estimation_of_entries);
	for (int seedRow = 0; seedRow < seedRowLength; ++seedRow) {
		memset(tmpMatrix, 0, seedColLength * z * z);
		for (int seedCol = 0; seedCol < seedColLength; ++seedCol) {
			if ((permut = hSeed[seedRow * seedColLength + seedCol]) >= 0) {
				if (rate != rate_2_3_a) {
					permut = permut * z / 96;
				} else {
					permut = permut % z;
				}
				for (int permutRow = 0; permutRow < z; ++permutRow) {
					for (int permutCol = 0; permutCol < z; ++permutCol) {
						if ((z + permutCol - permutRow) % z == permut) {
							tripletList.push_back(
									T(seedRow * z + permutRow,
											seedCol * z + permutCol, true));
						}
					}
				}

			}
		}
	}
	checkMatrix.setFromTriplets(tripletList.begin(), tripletList.end());
	return LDPC_SUCCESS;
}

int Coder::initOther() {

	//checkMatrix.topLeftCorner(ldpcM - z, ldpcK); //A
	//Matrix<bool, Dynamic, Dynamic> T;
	//T.resize(ldpcM, ldpcM);
	//T = checkMatrix.topRightCorner(ldpcM,ldpcM);//T
	//T.inverse();

	//Matrix<bool,ldpcM,ldpcM> T = checkMatrix.topRightCorner(ldpcM,ldpcM);//T
	//T.inverse();

	return LDPC_SUCCESS;
}

int Coder::encode(char * src, char * code, int srcLength) {
	return LDPC_SUCCESS;
}

int Coder::decode(float * code, char * src, int srcLength) {
	return LDPC_SUCCESS;
}

template<class Dtype>
Matrix<Dtype, Dynamic, Dynamic> inverse(
		 Matrix<Dtype, Dynamic, Dynamic>  matSrc) {
	int size = matSrc.rows();
	if (matSrc.rows() != matSrc.cols()) {
		printf("Error,row != col, no inverse");
		exit(0);
	}
	Matrix<Dtype, Dynamic, Dynamic> mat;  //=new Matrix<int, Dynamic,Dynamic>();
	Matrix<Dtype, Dynamic, Dynamic> inv;  //=new Matrix<int, Dynamic,Dynamic>();
	mat = matSrc;
	binary(mat);
	inv.resize(size, size);
	for (int r = 0; r < size; ++r) {
		for (int c = 0; c < size; ++c) {
			if (r == c)
				inv(r, c) = 1;
			else
				inv(r, c) = 0;
		}
	}
	for (int c = 0; c < size; ++c) {
		for (int r = 0; r < size; ++r) {
			if (mat(r, c) != 0) {
				Dtype tmp = mat(r, c);
				mat.col(c) += (1 / tmp - 1) * mat.col(c);
				inv.col(c) += (1 / tmp - 1) * inv.col(c);
				for (int c2 = c + 1; c2 < size; ++c2) {
					mat.col(c2) -= mat(r, c2) * mat.col(c);
					inv.col(c2) -= mat(r, c2) * inv.col(c);
				}
				break;
			}
		}
	}
	for (int r = 0; r < size; r++) {
		for (int r2 = r + 1; r2 < size; r2++) {
			Dtype tmp = mat(r2, r);
			mat.row(r2) -= tmp * mat.row(r);
			inv.row(r2) -= tmp * inv.row(r);
		}
	}
	binary(inv);
	return inv;
}

template<class Dtype>
void binary(Matrix<Dtype, Dynamic, Dynamic> & mat) {
	for (int r = 0; r < mat.rows(); ++r) {
		for (int c = 0; c < mat.cols(); ++c) {
			if (mat(r, c) < 0)
				mat(r, c) = (-mat(r, c)) % 2;
			else
				mat(r, c) = mat(r, c) % 2;
		}
	}
}

/*
#include <iostream>
#include <vector>
using namespace std;
using namespace Eigen;
int main() {
	 MatrixXd A;
	 A.inverse();
	 typedef Eigen::Triplet<double> T;
	 std::vector<T> tripletList;
	 int  estimation_of_entries=10;
	 int rows=10,cols=10;
	 tripletList.reserve(estimation_of_entries);
	 for(int i=0;i<10;++i)
	 {
	 int j=i;
	 int v_ij=1;
	 tripletList.push_back(T(i,j,v_ij));
	 }
	 SparseMatrix<double> mat(rows,cols);
	 mat.setFromTriplets(tripletList.begin(), tripletList.end());
	 cout << "Here is the matrix A:\n" << mat << endl;
	Matrix<int, Dynamic, Dynamic> mat;    //=new Matrix<int, Dynamic,Dynamic>();
	mat.resize(2, 2);

	mat(1, 0) = 1;
	mat(0, 1) = 1;
	mat(1, 1) = 1;

	Matrix<int, Dynamic, Dynamic> inv = inverse(mat);

	cout << mat << endl;
	cout << inv << endl;

}
*/
