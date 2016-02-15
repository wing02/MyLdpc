/*
 * MyTest.cpp
 *
 *  Created on: Feb 15, 2016
 *      Author: wing
 */

#include<iostream>
#include"MyLdpc.h"
using namespace std;
void test1() {
	Matrix<int, Dynamic, Dynamic> mat;    //=new Matrix<int, Dynamic,Dynamic>();
	mat.resize(2, 2);

	mat(1, 0) = 1;
	mat(0, 1) = 1;
	mat(1, 1) = 1;

	//Matrix<int, Dynamic, Dynamic> inv = inverse(mat);

	cout << mat << endl;
	//cout << inv << endl;

}

int main() {
	test1();
	a(1);
}

