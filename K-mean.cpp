#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <limits>
#include <string>
#include <algorithm>
#include <random>
using namespace std;

double **distanceMatrix;

void getData(const string &fileName, vector<vector<double>> &dataList)
{
	ifstream file(fileName);
	string line = "";
	while (getline(file, line))
	{
		vector<double> vec;
		stringstream ss(line);
		string token;
		while (getline(ss, token, ','))
			vec.push_back(atof(token.c_str()));
		dataList.push_back(vec);
	}
	file.close();
	/*
	int count = 0;
	for (vector<double> &vec : dataList)
	{
		for (double &data : vec)
		{
			cout << data << " , ";
		}
		count++;
		cout << std::endl;
	}
		cout << "count=" << count;
	*/
}


void findDistance(vector<vector<double> > &dataList, vector<int> &kPoints, vector<vector<double>> &centroid) {

	for (int k : kPoints) {
		centroid.push_back(dataList.at(k));
	}
	/*
	cout << "\nInitials centroids:" << endl;
	for (int i = 0; i < centroid.size(); i++) {
		for (double k : centroid.at(i)) {
			cout << k << "\t";
		}
		cout << endl;
	}
	*/
	int flag = 0;
	do{
		vector<vector<double>> centroidCalc;     //To store sum of xi which are inside the cluster(all points nearest to this centroid)
		vector<double> centroidCount;			//To store count of points inside the cluster(All points nearest to this centroid)

		/*
			Initializing  
		*/
		for (int i = 0; i < centroid.size(); i++) {
			vector<double> temp;
			for (int j = 0; j < dataList.at(0).size(); j++) {
				temp.push_back(0.0);
			}
			centroidCalc.push_back(temp);
			centroidCount.push_back(0);
		}



		for (int j = 0; j < dataList.size(); j++) {
			double minDist = numeric_limits<double>::max();
			int centroidNo = -1;
			for (int i = 0; i < centroid.size(); i++) {
				vector<double> &tempX = centroid.at(i);
				vector<double> &tempY = dataList.at(j);
				double dist = 0.0;
				for (int k = 0; k < tempX.size(); k++) {
					dist += pow(tempX.at(k) - tempY.at(k), 2);
				}
				if (dist < minDist) {
					minDist = dist;
					centroidNo = i;
				}
			}
			centroidCount.at(centroidNo) = centroidCount.at(centroidNo) + 1;
			for (int k = 0; k < dataList.at(0).size(); k++) {
				centroidCalc.at(centroidNo).at(k) = centroidCalc.at(centroidNo).at(k) + dataList.at(j).at(k);
			}
		}
		flag = 0;
		for (int i = 0; i < centroid.size(); i++) {
			for (int j = 0; j < centroid.at(0).size(); j++) {
				double temp = static_cast<int>(((centroidCalc.at(i).at(j) / centroidCount.at(i))+0.0005)*1000)/1000.0;
				if (centroid.at(i).at(j) != temp) {
					flag = 1;
				}
				centroid.at(i).at(j) = temp;
			}
		}
		/*
		cout << "\nIntermediate centroids : " << endl;
		for (int i = 0; i < centroid.size(); i++) {
			for (double t :  centroid.at(i)) {
				cout << t << "\t";
			}
			cout << endl;
		}
		*/

	} while (flag == 1);
}



void assignCluster(vector<vector<double>> &dataList, vector<vector<double>> &centroid, vector<vector<double>> &clusters) {
	
	clusters.resize(centroid.size());

	for (int j = 0; j < dataList.size(); j++) {
		double minDist = numeric_limits<double>::max();
		int centroidNo = -1;
		for (int i = 0; i < centroid.size(); i++) {
			vector<double> &tempX = centroid.at(i);
			vector<double> &tempY = dataList.at(j);
			double dist = 0.0;
			for (int k = 0; k < tempX.size(); k++) {
				dist += pow(tempX.at(k) - tempY.at(k), 2);
			}
			if (dist < minDist) {
				minDist = dist;
				centroidNo = i;
			}
		}
		clusters.at(centroidNo).push_back(j);
	}
}


void getRandomKpoints(int size, vector<int> &kPoints, int k) {
	
	
	while (kPoints.size() < k) {
		std::random_device rd;
		int v = rd() % size;
		int flag = 0;
		for (int temp : kPoints) {
			if (v == temp) {
				flag = 1;
				break;
			}
		}
		if (flag == 0)
			kPoints.push_back(v);
	}
}



int main()
{
	vector<vector<double>> dataList;
	getData("newirish.csv", dataList);
	
	vector<int> kPoints;
	getRandomKpoints(dataList.size(),kPoints,3);

	vector<vector<double>> centroid;
	findDistance(dataList, kPoints,centroid);
	
	cout << "\nFinal centroid points:" << endl;
	for (int i = 0; i < centroid.size(); i++) {
		for (double k : centroid.at(i)) {
			cout << k << "\t";
		}
		cout << endl;
	}
	vector<vector<double>> clusters;
	assignCluster(dataList, centroid,clusters);

	for (int i = 0; i < clusters.size(); i++) {
		cout << "\nCentroid : " << i <<"\t"<<"Count : "<<clusters.at(i).size()<< endl;
		for (int points : clusters.at(i)) {
			cout << points << ", ";
		}
		cout << endl;
	}


	system("PAUSE");
	return 0;
}