/*
 * MyTest.cpp
 *
 *  Created on: Feb 15, 2016
 *      Author: wing
 */

#include<iostream>

#include "MyLdpc.h"
using namespace std;

void test2() {
	const int z = 24;
	const int ldpcN = z * 24;
	const int ldpcK = ldpcN / 6 * 5;
	const int ldpcM = ldpcN - ldpcK;
	const enum rate_type rate = rate_5_6;

	Coder coder(ldpcK, ldpcN, rate);

	const int srcLength = 10;
	char * srcCode = (char*) malloc(srcLength);
	char * priorCode = (char*) malloc(coder.getPriorCodeLength(srcLength));
	int * priorCodeInt = (int*) malloc(
			sizeof(int) * 8 * coder.getPriorCodeLength(srcLength));
	float * postCode = (float*) malloc(coder.getPostCodeLength(srcLength));
	char * newSrcCode = (char*) malloc(srcLength);

	for (int i = 0; i < srcLength; i++) {
		srcCode[i] = 'a' + i % 26;
	}

	coder.forEncoder();
	coder.forDecoder(1);
	coder.encode(srcCode, priorCode, srcLength);
	using namespace Eigen;
	typedef Triplet<DataType> T;
	std::vector<T> tripletList;
	SparseMatrix<int> smN(ldpcN,1);

	cout << "Right code = " << endl;
	for (int i = 0; i < coder.getPriorCodeLength(srcLength); ++i) {
		char tmp = priorCode[i];
		for (int j = 0; j < 8; ++j) {
			if (tmp & (1 << (7-j))) {
				priorCodeInt[i * 8 + j] = 1;
				tripletList.push_back(T(i * 8 + j, 0, 1));
				cout << "1 ";
			} else {
				priorCodeInt[i * 8 + j] = 0;
				cout << "0 ";
			}
		}
	}
	cout << endl;
	//cout<<coder.checkMatrix<<endl;
	smN.setFromTriplets(tripletList.begin(),tripletList.end());
	LOGA(coder.checkMatrix.cols());
	LOGA(smN.rows());

	coder.test(priorCode, postCode, coder.getPostCodeLength(srcLength), 0.1);
	coder.decode(postCode, newSrcCode, srcLength);
	cout<<"srcCode=";
	for (int i = 0; i < srcLength; i++) {
		cout << srcCode[i];
	}
	cout << endl;
	cout<<"newSrcCode=";
	for (int i = 0; i < srcLength; i++) {
		cout << newSrcCode[i];
	}
	cout << endl;
}
void test3() {
	using namespace Eigen;
	SparseMatrix<int> a(2, 2);
	a.insert(0, 0) = 1;
	a.insert(0, 1) = 1;
	a.insert(1, 1) = 0;
	a.insert(1, 0) = 0;

	a.makeCompressed();
	cout << a.nonZeros() << endl;
	for (int k = 0; k < a.outerSize(); ++k)
		for (SparseMatrix<int>::InnerIterator it(a, k); it; ++it) {
			cout << "k=" << k << endl;
			cout << it.value() << " " << it.row() << " " << it.col() << endl;

		}
}

int main() {
	test2();
}

