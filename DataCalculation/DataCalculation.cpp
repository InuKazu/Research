//#ifdef _MSC_VER
//#include <windows.h>
//#endif

#include "stdafx.h"
#include <stdio.h>
#include <math.h>
#include <vector>
#include <exception>
#include <windows.h>

#include <iostream>
#include <fstream>

#include<opencv2/opencv.hpp>
#ifdef _DEBUG
//Debugモードの場合
#pragma comment(lib,"opencv_core249d.lib")
#pragma comment(lib,"opencv_imgproc249d.lib")
#pragma comment(lib,"opencv_highgui249d.lib")
#pragma comment(lib,"opencv_objdetect249d.lib")
#pragma comment(lib,"opencv_contrib249d.lib")
#pragma comment(lib,"opencv_features2d249d.lib")
#pragma comment(lib,"opencv_flann249d.lib")
#pragma comment(lib,"opencv_gpu249d.lib")
//#pragma comment(lib,"opencv_haartraining_engined.lib")
#pragma comment(lib,"opencv_legacy249d.lib")
#pragma comment(lib,"opencv_ts249d.lib")
#pragma comment(lib,"opencv_video249d.lib")
#else
//Releaseモードの合
#pragma comment(lib,"C:\\OpenCV2.4\\lib\\opencv_core246.lib")
#pragma comment(lib,"C:\\OpenCV2.4\\lib\\opencv_imgproc246.lib")
#pragma comment(lib,"C:\\OpenCV2.4\\lib\\opencv_highgui246.lib")
#endif

using namespace std;

#define PARAM_NUM 2 //パラメータの数(次数＋1)
#define strNUM 24000 //配列の要素数2
/* 読み込み用関数 */
void cog(int pcount, int time_cog[], int time_P[], float cog_data[][3], float POLHEMUS_data[][3], int num);
void polhemus(int cogcount, int time_cog[], int time_P[], float cog_data[][3], float POLHEMUS_data[][3], int num);
//void oculus(int pcount, int time_ocu[], int time_P[], float ocu_data[][3], float POLHEMUS_data[][3], int num);

/* パラメータ算出関数 */
void PrimaryFunction(int count, float cog_data[][3], float POLHEMUS_data[][3], int num);
//void QuadraticFunction(int count, float cog_data[][3], float POLHEMUS_data[][3], int num);

void read(char *name, int num, int PorCOG);		//読み込み用関数
void write(char *name, float data[][3], int count);		//datファイル書き込み用関数
void Excel(char *name, float param[][3], float data[][3], int count, int d);		//Excel用書き込み用関数

void Excel_SUM(char *name, float data[][3]);

float POLHEMUS_data[10][strNUM][3] = { 0 };		//yaw, pitch, rollの順番で代入
float cog_data[10][strNUM][3] = { 0 };			//x, y, z座標の順番で重心代入
float Oculus_data[10][strNUM][3] = { 0 };		//yaw, pitch, rollの順番で代入

float adjst_POLHEMUS[10][strNUM][3] = { 0 };	//調整後のPOLHEMUSデータ
float adjst_COG[10][strNUM][3] = { 0 };		//調整後の重心データ
float adjst_Oculus[10][strNUM][3] = { 0 };	//調整後のOculusデータ

int pcount[10] = { 0 };		//POLHEMUSデータ数のカウンタ
int cogcount[10] = { 0 };	//重心データ数のカウンタ
int Ocucount[10] = { 0 };	//Oculusデータ数のカウンタ

int time_P[10][strNUM] = { 0 };		//POLHEMUSのデータのタイムスタンプを代入
int time_cog[10][strNUM] = { 0 };		//重心のデータのタイムスタンプを代入
int time_Ocu[10][strNUM] = { 0 };		//Ocuusのデータのタイムスタンプ代入

int Exceldatacount[10] = { 0 };		//Excelで書き込むデータ数

/* 書き込む際に配列に仮代入する */
/* ３次元目において，0:x座標，1:y座標，2:z座標のパラメータ */
//float writeparam[10][PARAM_NUM][3] = { 0 };	// １次関数用パラメータ
//float writeparam2[10][3][3][1] = { 0 };	// ２次関数用パラメータ

/* x,y,z座標一緒に考えた場合のパラメータ */
float writeparam[10][4][3] = { 0 };
/* -------------------------------------- */

//float paramcal1[11][2][1] = { 0 };		// １次関数用の平均パラメータ(11番目はパラメータの合計)
//float paramcal2[11][3][1] = { 0 };		// ２次関数用の平均パラメータ(11番目はパラメータの合計)

/* 平均パラメータの算出 */
//float aveParam1[4][3][3] = { 0 };
//float aveParam2[4][7][3] = { 0 };

/* 交差検定用平均パラメータ */
float newaveparam[10][4][3] = { 0 };	// １次関数
//float newaveparam2[10][3][3][1] = { 0 };	// ２次関数

float realdata[strNUM][3] = { 0 };
int count_sum = 0;
int count_sum_data[10] = { 0 };

int main()
{
	/* ファイルの読み込み */
	/* ----------------------POLHEMUSデータ------------------------- */
	read("POLHEMUS_data1.dat", 1, 0);
	read("POLHEMUS_data2.dat", 2, 0);
	read("POLHEMUS_data3.dat", 3, 0);
	read("POLHEMUS_data4.dat", 4, 0);
	read("POLHEMUS_data5.dat", 5, 0);
	read("POLHEMUS_data6.dat", 6, 0);
	read("POLHEMUS_data7.dat", 7, 0);
	read("POLHEMUS_data8.dat", 8, 0);
	read("POLHEMUS_data9.dat", 9, 0);
	read("POLHEMUS_data10.dat", 10, 0);


	/* ---------------------重心データ----------------------------- */
	read("COG_data1.dat", 1, 1);
	read("COG_data2.dat", 2, 1);
	read("COG_data3.dat", 3, 1);
	read("COG_data4.dat", 4, 1);
	read("COG_data5.dat", 5, 1);
	read("COG_data6.dat", 6, 1);
	read("COG_data7.dat", 7, 1);
	read("COG_data8.dat", 8, 1);
	read("COG_data9.dat", 9, 1);
	read("COG_data10.dat", 10, 1);


	/* ---------------------Oculusデータ--------------------------- */
	//read("Oculus_data1.dat", 1, 2);
	//read("Oculus_data2.dat", 2, 2);
	//read("Oculus_data3.dat", 3, 2);
	//read("Oculus_data4.dat", 4, 2);
	//read("Oculus_data5.dat", 5, 2);
	//read("Oculus_data6.dat", 6, 2);
	//read("Oculus_data7.dat", 7, 2);
	//read("Oculus_data8.dat", 8, 2);
	//read("Oculus_data9.dat", 9, 2);
	//read("Oculus_data10.dat", 10, 2);


	/* -----------------------以下、データを合わせに行く----------------------------------- */
	/* 基本的にはデータ数の少ないPOLHEMUSに合わせる */
	for (int i = 0; i < 10; i++){
		cog(pcount[i], time_cog[i], time_P[i], cog_data[i], POLHEMUS_data[i], i + 1);
		//oculus(pcount[i], time_Ocu[i], time_P[i], Oculus_data[i], POLHEMUS_data[i], i + 1);
		Exceldatacount[i] = pcount[i];
	}

	//for (int i = 0; i < 10; i++){
	//	if (pcount[i] <= cogcount[i]){
	//		cog(pcount[i], time_cog[i], time_P[i], cog_data[i], POLHEMUS_data[i], i+1);

	//		Exceldatacount[i] = pcount[i];
	//	}
	//	else {
	//		polhemus(cogcount[i], time_cog[i], time_P[i], cog_data[i], POLHEMUS_data[i], i+1);
	//		Exceldatacount[i] = cogcount[i];
	//	}
	//}

	/*--------------交差検定用パラメータの算出-----------------*/
	/* １次関数用 */
	/*---------x,y,zを一緒に考える場合に調整中-----------------*/
	/*---------PARAM_NUMを４に固定中---------------------------*/
	for (int i = 0; i < 10; i++){//全パラメータを参照
		for (int j = 0; j < 3; j++){//x,y,z座標(今回はヨー・ピッチ・ロール)
			for (int k = 0; k < 4; k++){//パラメータ数
				for (int l = 0; l < 10; l++){//足し合わせ用パラメータ
					/* 自分以外のデータの合計を算出 */
					if (i != l)
						newaveparam[i][k][j] += writeparam[l][k][j];
				}
			}
		}
	}

	/* ここで平均パラメータが求まる */
	/*---------x,y,zを一緒に考える場合に調整中-----------------*/
	/*---------PARAM_NUMを４に固定中---------------------------*/
	for (int i = 0; i < 10; i++){
		for (int j = 0; j < 3; j++){
			for (int k = 0; k < 4; k++)
				newaveparam[i][k][j] /= 9.0;
		}
	}



	///* ２次関数用 */
	//for (int i = 0; i < 10; i++){
	//	for (int j = 0; j < 3; j++){
	//		for (int k = 0; k < 3; k++){
	//			for (int l = 0; l < 10; l++){
	//				/* 自分以外のデータの合計を算出 */
	//				if (i != l)
	//					newaveparam2[i][j][k][1] += writeparam1[l][j][k][1];
	//			}
	//		}
	//	}
	//}

	///* ここで平均パラメータが求まる */
	//for (int i = 0; i < 10; i++){
	//	for (int j = 0; j < 3; j++){
	//		for (int k = 0; k < 3; k++)
	//			newaveparam2[i][j][k][1] /= 9.0;
	//	}
	//}


	///* とりあえず合計を出しておく */
	//for (int i = 0; i < 10; i++){
	//	for (int j = 0; j < 7; j++){
	//		for (int k = 0; k < 3; k++){
	//			if (j < 3)	paramcal1[10][j][k] += writeparam1[i][j][k];
	//			paramcal2[10][j][k] += writeparam2[i][j][k];
	//		}
	//	}
	//}

	///* 交差検定用平均パラメータを求める */
	//for (int i = 0; i < 10; i++){
	//	for (int j = 0; j < 7; j++){
	//		for (int k = 0; k < 3; k++){
	//			if (j < 3)	newaveparam1[i][j][k] = (paramcal1[10][j][k] - writeparam1[i][j][k]) / 9.0;
	//			newaveparam2[i][j][k] = (paramcal2[10][j][k] - writeparam2[i][j][k]) / 9.0;
	//		}
	//	}
	//}

	if (PARAM_NUM == 2){
		Excel("Excel1_1.dat", newaveparam[0], adjst_COG[0], Exceldatacount[0], 0);
		Excel("Excel1_2.dat", newaveparam[1], adjst_COG[1], Exceldatacount[1], 1);
		Excel("Excel1_3.dat", newaveparam[2], adjst_COG[2], Exceldatacount[2], 2);
		Excel("Excel1_4.dat", newaveparam[3], adjst_COG[3], Exceldatacount[3], 3);
		Excel("Excel1_5.dat", newaveparam[4], adjst_COG[4], Exceldatacount[4], 4);
		Excel("Excel1_6.dat", newaveparam[5], adjst_COG[5], Exceldatacount[5], 5);
		Excel("Excel1_7.dat", newaveparam[6], adjst_COG[6], Exceldatacount[6], 6);
		Excel("Excel1_8.dat", newaveparam[7], adjst_COG[7], Exceldatacount[7], 7);
		Excel("Excel1_9.dat", newaveparam[8], adjst_COG[8], Exceldatacount[8], 8);
		Excel("Excel1_10.dat", newaveparam[9], adjst_COG[9], Exceldatacount[9], 9);
	}

	else if (PARAM_NUM == 3){
		Excel("Excel2_1.dat", newaveparam[0], adjst_COG[0], Exceldatacount[0], 0);
		Excel("Excel2_2.dat", newaveparam[1], adjst_COG[1], Exceldatacount[1], 1);
		Excel("Excel2_3.dat", newaveparam[2], adjst_COG[2], Exceldatacount[2], 2);
		Excel("Excel2_4.dat", newaveparam[3], adjst_COG[3], Exceldatacount[3], 3);
		Excel("Excel2_5.dat", newaveparam[4], adjst_COG[4], Exceldatacount[4], 4);
		Excel("Excel2_6.dat", newaveparam[5], adjst_COG[5], Exceldatacount[5], 5);
		Excel("Excel2_7.dat", newaveparam[6], adjst_COG[6], Exceldatacount[6], 6);
		Excel("Excel2_8.dat", newaveparam[7], adjst_COG[7], Exceldatacount[7], 7);
		Excel("Excel2_9.dat", newaveparam[8], adjst_COG[8], Exceldatacount[8], 8);
		Excel("Excel2_10.dat", newaveparam[9], adjst_COG[9], Exceldatacount[9], 9);
	}

	/* 縦にデータをすべて並べたものを保存 */
	if (PARAM_NUM == 2)
		Excel_SUM("Excel1_sum.dat", realdata);
	else if (PARAM_NUM == 3)
		Excel_SUM("Excel2_sum.dat", realdata);

	return 0;
}

/* COGデータ数のほうが多かった場合 */
void cog(int pcount, int time_cog[], int time_P[], float cog_data[][3], float POLHEMUS_data[][3], int num){

	float front = 0, back = 0;		//POLHEMUSのタイムスタンプの値の前後に当たる重心側のタイムスタンプの値

	int a = 0, b = 0; //内分点

	int j = 0;	//重心側のタイムスタンプのカウンタ

	for (int i = 0; i < pcount; i++){
		back = time_cog[j];
		if (back == time_P[i]){
			adjst_COG[num - 1][i][0] = cog_data[j][0];
			adjst_COG[num - 1][i][1] = cog_data[j][1];
			adjst_COG[num - 1][i][2] = cog_data[j][2];
			j++;
			continue;
		}
		/* 最後のタイムスタンプを見るときは例外的に最後のやつを代入 */
		else if (time_cog[j + 1] == 0){
			adjst_COG[num - 1][i][0] = cog_data[j][0];
			adjst_COG[num - 1][i][1] = cog_data[j][1];
			adjst_COG[num - 1][i][2] = cog_data[j][2];
			continue;
		}
		else if (time_cog[j + 1] < time_P[i]){
			j++;
			i--;
			continue;
		}
		else if (time_cog[j + 1] == time_P[i]){
			adjst_COG[num - 1][i][0] = cog_data[j + 1][0];
			adjst_COG[num - 1][i][1] = cog_data[j + 1][1];
			adjst_COG[num - 1][i][2] = cog_data[j + 1][2];
			j++;
			continue;
		}
		else if (time_cog[j + 1] > time_P[i]){
			front = time_cog[j + 1];
			a = time_P[i] - back;
			b = front - time_P[i];
			/* 内分点から計算して代入 */
			adjst_COG[num - 1][i][0] = cog_data[j][0] + (cog_data[j + 1][0] - cog_data[j][0]) * a / (a + b);
			adjst_COG[num - 1][i][1] = cog_data[j][1] + (cog_data[j + 1][1] - cog_data[j][1]) * a / (a + b);
			adjst_COG[num - 1][i][2] = cog_data[j][2] + (cog_data[j + 1][2] - cog_data[j][2]) * a / (a + b);
			j++;
			continue;
		}
	}

	for (int i = 0; i < pcount; i++){
		for (int j = 0; j < 3; j++)
			adjst_POLHEMUS[num - 1][i][j] = POLHEMUS_data[i][j];
	}

	/* -------------------------------------データの保存--------------------------------------- */
	switch (num){
	case 1:{
		write("adjusted_POLHEMUS_data1.dat", POLHEMUS_data, pcount);
		write("adjusted_COG_data1.dat", adjst_COG[num - 1], pcount);
		break;
	}
	case 2:{
		write("adjusted_POLHEMUS_data2.dat", POLHEMUS_data, pcount);
		write("adjusted_COG_data2.dat", adjst_COG[num - 1], pcount);
		break;
	}
	case 3:{
		write("adjusted_POLHEMUS_data3.dat", POLHEMUS_data, pcount);
		write("adjusted_COG_data3.dat", adjst_COG[num - 1], pcount);
		break;
	}
	case 4:{
		write("adjusted_POLHEMUS_data4.dat", POLHEMUS_data, pcount);
		write("adjusted_COG_data4.dat", adjst_COG[num - 1], pcount);
		break;
	}
	case 5:{
		write("adjusted_POLHEMUS_data5.dat", POLHEMUS_data, pcount);
		write("adjusted_COG_data5.dat", adjst_COG[num - 1], pcount);
		break;
	}
	case 6:{
		write("adjusted_POLHEMUS_data6.dat", POLHEMUS_data, pcount);
		write("adjusted_COG_data6.dat", adjst_COG[num - 1], pcount);
		break;
	}
	case 7:{
		write("adjusted_POLHEMUS_data7.dat", POLHEMUS_data, pcount);
		write("adjusted_COG_data7.dat", adjst_COG[num - 1], pcount);
		break;
	}
	case 8:{
		write("adjusted_POLHEMUS_data8.dat", POLHEMUS_data, pcount);
		write("adjusted_COG_data8.dat", adjst_COG[num - 1], pcount);
		break;
	}
	case 9:{
		write("adjusted_POLHEMUS_data9.dat", POLHEMUS_data, pcount);
		write("adjusted_COG_data9.dat", adjst_COG[num - 1], pcount);
		break;
	}
	case 10:{
		write("adjusted_POLHEMUS_data10.dat", POLHEMUS_data, pcount);
		write("adjusted_COG_data10.dat", adjst_COG[num - 1], pcount);
		break;
	}
	default:
		break;
	}


	/* -------------------------------データからパラメータを算出------------------------------- */
	PrimaryFunction(pcount, adjst_COG[num - 1], POLHEMUS_data, num);		//１次関数の場合
	//QuadraticFunction(pcount, adjst_COG[num - 1], POLHEMUS_data, num);	//２次関数の場合	

}

/* Oculusのデータ数をPOLHEMUS側に合わせる */
void oculus(int pcount, int time_ocu[], int time_P[], float ocu_data[][3], float POLHEMUS_data[][3], int num){

	float front = 0, back = 0;		//POLHEMUSのタイムスタンプの値の前後に当たる重心側のタイムスタンプの値

	int a = 0, b = 0; //内分点

	int j = 0;	//重心側のタイムスタンプのカウンタ

	for (int i = 0; i < pcount; i++){
		back = time_ocu[j];
		if (back == time_P[i]){
			adjst_Oculus[num - 1][i][0] = ocu_data[j][0];
			adjst_Oculus[num - 1][i][1] = ocu_data[j][1];
			adjst_Oculus[num - 1][i][2] = ocu_data[j][2];
			j++;
			continue;
		}
		/* 最後のタイムスタンプを見るときは例外的に最後のやつを代入 */
		else if (time_ocu[j + 1] == 0){
			adjst_Oculus[num - 1][i][0] = ocu_data[j][0];
			adjst_Oculus[num - 1][i][1] = ocu_data[j][1];
			adjst_Oculus[num - 1][i][2] = ocu_data[j][2];
			continue;
		}
		else if (time_ocu[j + 1] < time_P[i]){
			j++;
			i--;
			continue;
		}
		else if (time_ocu[j + 1] == time_P[i]){
			adjst_Oculus[num - 1][i][0] = ocu_data[j + 1][0];
			adjst_Oculus[num - 1][i][1] = ocu_data[j + 1][1];
			adjst_Oculus[num - 1][i][2] = ocu_data[j + 1][2];
			j++;
			continue;
		}
		else if (time_ocu[j + 1] > time_P[i]){
			front = time_ocu[j + 1];
			a = time_P[i] - back;
			b = front - time_P[i];
			/* 内分点から計算して代入 */
			adjst_Oculus[num - 1][i][0] = ocu_data[j][0] + (ocu_data[j + 1][0] - ocu_data[j][0]) * a / (a + b);
			adjst_Oculus[num - 1][i][1] = ocu_data[j][1] + (ocu_data[j + 1][1] - ocu_data[j][1]) * a / (a + b);
			adjst_Oculus[num - 1][i][2] = ocu_data[j][2] + (ocu_data[j + 1][2] - ocu_data[j][2]) * a / (a + b);
			j++;
			continue;
		}
	}

	for (int i = 0; i < pcount; i++){
		for (int j = 0; j < 3; j++)
			adjst_POLHEMUS[num - 1][i][j] = POLHEMUS_data[i][j];
	}

	/* -------------------------------------データの保存--------------------------------------- */
	switch (num){
	case 1:{
		write("adjusted_POLHEMUS_data1.dat", POLHEMUS_data, pcount);
		write("adjusted_Oculus_data1.dat", adjst_Oculus[num - 1], pcount);
		break;
	}
	case 2:{
		write("adjusted_POLHEMUS_data2.dat", POLHEMUS_data, pcount);
		write("adjusted_Oculus_data2.dat", adjst_Oculus[num - 1], pcount);
		break;
	}
	case 3:{
		write("adjusted_POLHEMUS_data3.dat", POLHEMUS_data, pcount);
		write("adjusted_Oculus_data3.dat", adjst_Oculus[num - 1], pcount);
		break;
	}
	case 4:{
		write("adjusted_POLHEMUS_data4.dat", POLHEMUS_data, pcount);
		write("adjusted_Oculus_data4.dat", adjst_Oculus[num - 1], pcount);
		break;
	}
	case 5:{
		write("adjusted_POLHEMUS_data5.dat", POLHEMUS_data, pcount);
		write("adjusted_Oculus_data5.dat", adjst_Oculus[num - 1], pcount);
		break;
	}
	case 6:{
		write("adjusted_POLHEMUS_data6.dat", POLHEMUS_data, pcount);
		write("adjusted_Oculus_data6.dat", adjst_Oculus[num - 1], pcount);
		break;
	}
	case 7:{
		write("adjusted_POLHEMUS_data7.dat", POLHEMUS_data, pcount);
		write("adjusted_Oculus_data7.dat", adjst_Oculus[num - 1], pcount);
		break;
	}
	case 8:{
		write("adjusted_POLHEMUS_data8.dat", POLHEMUS_data, pcount);
		write("adjusted_Oculus_data8.dat", adjst_Oculus[num - 1], pcount);
		break;
	}
	case 9:{
		write("adjusted_POLHEMUS_data9.dat", POLHEMUS_data, pcount);
		write("adjusted_Oculus_data9.dat", adjst_Oculus[num - 1], pcount);
		break;
	}
	case 10:{
		write("adjusted_POLHEMUS_data10.dat", POLHEMUS_data, pcount);
		write("adjusted_Oculus_data10.dat", adjst_Oculus[num - 1], pcount);
		break;
	}
	default:
		break;
	}


	/* -------------------------------データからパラメータを算出------------------------------- */
	//PrimaryFunction(pcount, adjst_COG[num - 1], POLHEMUS_data, num);		//１次関数の場合
	//QuadraticFunction(pcount, adjst_COG[num - 1], POLHEMUS_data, num);	//２次関数の場合	

}

/* POLHEMUSデータ数のほうが多かった場合 */
void polhemus(int cogcount, int time_cog[], int time_P[], float cog_data[][3], float POLHEMUS_data[][3], int num){

	float front = 0, back = 0;		//POLHEMUSのタイムスタンプの値の前後に当たる重心側のタイムスタンプの値

	int a = 0, b = 0; //内分点

	int j = 0;	//重心側のタイムスタンプのカウンタ

	for (int i = 0; i < cogcount; i++){
		back = time_P[j];
		if (back == time_cog[i]){
			adjst_POLHEMUS[num - 1][i][0] = POLHEMUS_data[j][0];
			adjst_POLHEMUS[num - 1][i][1] = POLHEMUS_data[j][1];
			adjst_POLHEMUS[num - 1][i][2] = POLHEMUS_data[j][2];
			j++;
			continue;
		}
		/* 最後のタイムスタンプを見るときは例外的に最後のやつを代入 */
		else if (time_P[j + 1] == 0){
			adjst_POLHEMUS[num - 1][i][0] = POLHEMUS_data[j][0];
			adjst_POLHEMUS[num - 1][i][1] = POLHEMUS_data[j][1];
			adjst_POLHEMUS[num - 1][i][2] = POLHEMUS_data[j][2];
			continue;
		}
		else if (time_P[j + 1] < time_cog[i]){
			j++;
			i--;
			continue;
		}
		else if (time_P[j + 1] == time_cog[i]){
			adjst_POLHEMUS[num - 1][i][0] = POLHEMUS_data[j + 1][0];
			adjst_POLHEMUS[num - 1][i][1] = POLHEMUS_data[j + 1][1];
			adjst_POLHEMUS[num - 1][i][2] = POLHEMUS_data[j + 1][2];
			j++;
			continue;
		}
		else if (time_P[j + 1] > time_cog[i]){
			front = time_P[j + 1];
			a = time_cog[i] - back;
			b = front - time_cog[i];
			/* 内分点から計算して代入 */
			adjst_POLHEMUS[num - 1][i][0] = POLHEMUS_data[j][0] + (POLHEMUS_data[j + 1][0] - POLHEMUS_data[j][0]) * a / (a + b);
			adjst_POLHEMUS[num - 1][i][1] = POLHEMUS_data[j][1] + (POLHEMUS_data[j + 1][1] - POLHEMUS_data[j][1]) * a / (a + b);
			adjst_POLHEMUS[num - 1][i][2] = POLHEMUS_data[j][2] + (POLHEMUS_data[j + 1][2] - POLHEMUS_data[j][2]) * a / (a + b);
			j++;
			continue;
		}
	}

	for (int i = 0; i < cogcount; i++){
		for (int j = 0; j < 3; j++)
			adjst_COG[num - 1][i][j] = POLHEMUS_data[i][j];
	}

	/* -------------------------------------データの保存--------------------------------------- */
	switch (num){
	case 1:{
		write("adjusted_POLHEMUS_data1.dat", adjst_POLHEMUS[num - 1], cogcount);
		write("adjusted_COG_data1.dat", cog_data, cogcount);
		break;
	}
	case 2:{
		write("adjusted_POLHEMUS_data2.dat", adjst_POLHEMUS[num - 1], cogcount);
		write("adjusted_COG_data2.dat", cog_data, cogcount);
		break;
	}
	case 3:{
		write("adjusted_POLHEMUS_data3.dat", adjst_POLHEMUS[num - 1], cogcount);
		write("adjusted_COG_data3.dat", cog_data, cogcount);
		break;
	}
	case 4:{
		write("adjusted_POLHEMUS_data4.dat", adjst_POLHEMUS[num - 1], cogcount);
		write("adjusted_COG_data4.dat", cog_data, cogcount);
		break;
	}
	case 5:{
		write("adjusted_POLHEMUS_data5.dat", adjst_POLHEMUS[num - 1], cogcount);
		write("adjusted_COG_data5.dat", cog_data, cogcount);
		break;
	}
	case 6:{
		write("adjusted_POLHEMUS_data6.dat", adjst_POLHEMUS[num - 1], cogcount);
		write("adjusted_COG_data6.dat", cog_data, cogcount);
		break;
	}
	case 7:{
		write("adjusted_POLHEMUS_data7.dat", adjst_POLHEMUS[num - 1], cogcount);
		write("adjusted_COG_data7.dat", cog_data, cogcount);
		break;
	}
	case 8:{
		write("adjusted_POLHEMUS_data8.dat", adjst_POLHEMUS[num - 1], cogcount);
		write("adjusted_COG_data8.dat", cog_data, cogcount);
		break;
	}
	case 9:{
		write("adjusted_POLHEMUS_data9.dat", adjst_POLHEMUS[num - 1], cogcount);
		write("adjusted_COG_data9.dat", cog_data, cogcount);
		break;
	}
	case 10:{
		write("adjusted_POLHEMUS_data10.dat", adjst_POLHEMUS[num - 1], cogcount);
		write("adjusted_COG_data10.dat", cog_data, cogcount);
		break;
	}
	default:
		break;
	}


	/* -------------------------------データからパラメータを算出------------------------------- */
	PrimaryFunction(cogcount, cog_data, adjst_POLHEMUS[num - 1], num);		//１次関数の場合
	//QuadraticFunction(cogcount, cog_data, adjst_POLHEMUS[num - 1], num);	//２次関数の場合
}

/* 回帰分析関数 */
void PrimaryFunction(int count, float cog_data[][3], float POLHEMUS_data[][3], int num){

	cv::Mat Py(count, 1, CV_32FC1);			//POLHEMUSのデータ
	cv::Mat Pp(count, 1, CV_32FC1);			//y:ヨー，p:ピッチ，r:ロール
	cv::Mat Pr(count, 1, CV_32FC1);

	/* ---------------x,y,z座標別々で考える場合----------------------------------- */
	//cv::Mat Cx(count, PARAM_NUM, CV_32FC1);			//重心のデータ
	//cv::Mat Cy(count, PARAM_NUM, CV_32FC1);
	//cv::Mat Cz(count, PARAM_NUM, CV_32FC1);

	//cv::Mat Cxt(PARAM_NUM, count, CV_32FC1);		//重心の転置データ	
	//cv::Mat Cyt(PARAM_NUM, count, CV_32FC1);
	//cv::Mat Czt(PARAM_NUM, count, CV_32FC1);

	//cv::Mat Qx(PARAM_NUM, PARAM_NUM, CV_32FC1);		//行列CとCtをかけた結果
	//cv::Mat Qy(PARAM_NUM, PARAM_NUM, CV_32FC1);
	//cv::Mat Qz(PARAM_NUM, PARAM_NUM, CV_32FC1);

	//cv::Mat invQx(PARAM_NUM, PARAM_NUM, CV_32FC1);	//Xの逆行列
	//cv::Mat invQy(PARAM_NUM, PARAM_NUM, CV_32FC1);
	//cv::Mat invQz(PARAM_NUM, PARAM_NUM, CV_32FC1);

	//cv::Mat Ax(PARAM_NUM, 1, CV_32FC1);		//パラメータの結果(縦)
	//cv::Mat Ay(PARAM_NUM, 1, CV_32FC1);
	//cv::Mat Az(PARAM_NUM, 1, CV_32FC1);
	/* --------------------------------------------------------------------------- */




	/* ---------------x,y,z座標一緒で考える場合----------------------------------- */
	cv::Mat C(count, 4, CV_32FC1);					//重心のデータ
	cv::Mat Ct(4, count, CV_32FC1);					//重心行列の転置データ
	cv::Mat Q(4, 4, CV_32FC1);						//行列CとCtをかけた結果
	cv::Mat invQ(4, 4, CV_32FC1);					//Qの逆行列
	cv::Mat Ay(4, 1, CV_32FC1);						//パラメータ(ヨー)
	cv::Mat Ap(4, 1, CV_32FC1);						//パラメータ(ピッチ)
	cv::Mat Ar(4, 1, CV_32FC1);						//パラメータ(ロール)
	/* --------------------------------------------------------------------------- */




	//for (int i = 0; i < count; i++){

	//	Cx.at<float>(i, 0) = cog_data[i][0];
	//	Cy.at<float>(i, 0) = cog_data[i][1];
	//	Cz.at<float>(i, 0) = cog_data[i][2];

	//	for (int j = 0; j < PARAM_NUM; j++){
	//		if (j == PARAM_NUM - 1){
	//			Cx.at<float>(i, j) = 1.0;
	//			Cy.at<float>(i, j) = 1.0;
	//			Cz.at<float>(i, j) = 1.0;
	//		}
	//		Px.at<float>(i, j) = POLHEMUS_data[i][0];
	//		Py.at<float>(i, j) = POLHEMUS_data[i][1];
	//		Pz.at<float>(i, j) = POLHEMUS_data[i][2];
	//	}
	//}

	/* ------x,y,z座標別々に考えた場合--------------------------------- */
	//for (int i = 0; i < count; i++){
	//	Py.at<float>(i, 0) = POLHEMUS_data[i][0];
	//	Pp.at<float>(i, 0) = POLHEMUS_data[i][1];
	//	Pr.at<float>(i, 0) = POLHEMUS_data[i][2];

	//	for (int j = 0; j < PARAM_NUM; j++){
	//		//パラメータの数(関数の次数)によって行列の形が変化
	//		Cx.at<float>(i, j) = powf(cog_data[i][0], PARAM_NUM - j - 1);
	//		Cy.at<float>(i, j) = powf(cog_data[i][1], PARAM_NUM - j - 1);
	//		Cz.at<float>(i, j) = powf(cog_data[i][2], PARAM_NUM - j - 1);
	//	}
	//}
	/* ------------------------------------------------------------------ */

	/* --------------x,y,z座標一緒に考えた場合(改訂版)-------------------------------- */
	for (int i = 0; i < count; i++){
		Py.at<float>(i, 0) = POLHEMUS_data[i][0];
		Pp.at<float>(i, 0) = POLHEMUS_data[i][1];
		Pr.at<float>(i, 0) = POLHEMUS_data[i][2];

		C.at<float>(i, 0) = cog_data[i][0];
		C.at<float>(i, 1) = cog_data[i][1];
		C.at<float>(i, 2) = cog_data[i][2];
		C.at<float>(i, 3) = 1.0;
	}

	//パラメータの計算(最小二乗法)
	/* ---------------- yawの場合 ----------------- */
	//x座標の場合
	//Cxt = Cx.t();
	//Qx = Cxt * Cx;
	//invQx = Qx.inv();
	//Ax = invQx * Cxt * Py;

	////y座標の場合
	//Cyt = Cy.t();
	//Qy = Cyt * Cy;
	//invQy = Qy.inv();
	//Ay = invQy * Cyt * Py;

	////z座標の場合
	//Czt = Cz.t();
	//Qz = Czt * Cz;
	//invQz = Qz.inv();
	//Az = invQz * Czt * Py;
	/* ------------------------------------------- */

	/* ---------------- pitchの場合 ----------------- */
	//x座標の場合
	//Cxt = Cx.t();
	//Qx = Cxt * Cx;
	//invQx = Qx.inv();
	//Ax = invQx * Cxt * Pp;

	////y座標の場合
	//Cyt = Cy.t();
	//Qy = Cyt * Cy;
	//invQy = Qy.inv();
	//Ay = invQy * Cyt * Pp;

	////z座標の場合
	//Czt = Cz.t();
	//Qz = Czt * Cz;
	//invQz = Qz.inv();
	//Az = invQz * Czt * Pp;
	/* ------------------------------------------- */

	/* ---------------- rollの場合 ----------------- */
	//x座標の場合
	//Cxt = Cx.t();
	//Qx = Cxt * Cx;
	//invQx = Qx.inv();
	//Ax = invQx * Cxt * Pr;

	////y座標の場合
	//Cyt = Cy.t();
	//Qy = Cyt * Cy;
	//invQy = Qy.inv();
	//Ay = invQy * Cyt * Pr;

	////z座標の場合
	//Czt = Cz.t();
	//Qz = Czt * Cz;
	//invQz = Qz.inv();
	//Az = invQz * Czt * Pr;
	/* ------------------------------------------- */

	/* ---------------- 座標ごとに関係ない場合 ----------------- */
	Ct = C.t();
	Q = Ct*C;
	invQ = Q.inv();
	/* ここまではヨー・ピッチ・ロール全部一緒 */
	/* これより下は区別していく */
	Ay = invQ*Ct*Py;
	Ap = invQ*Ct*Pp;
	Ar = invQ*Ct*Pr;

	/* ----------------x,y,z座標別々で考えた場合------------------- */
	//for (int i = 0; i < PARAM_NUM; i++){
	//	writeparam[num - 1][i][0] = Ax.at<float>(i, 0);
	//	writeparam[num - 1][i][1] = Ay.at<float>(i, 0);
	//	writeparam[num - 1][i][2] = Az.at<float>(i, 0);
	//}
	/* ----------------------------------------------------------- */

	/* ----------------x,y,z座標一緒で考えた場合------------------- */
	for (int i = 0; i < 4; i++){
		writeparam[num - 1][i][0] = Ay.at<float>(i, 0);
		writeparam[num - 1][i][1] = Ap.at<float>(i, 0);
		writeparam[num - 1][i][2] = Ar.at<float>(i, 0);
	}
	/* ----------------------------------------------------------- */

	/* -----------一時的にx,y,z座標を一緒に考える--------------------- */
	/* -----------PARAM_NUMを4(固定)に変更---------------------------- */
	/* ------PARAM_NUM=2に固定して起き、1次関数用に入るようにする----- */
	if (PARAM_NUM == 2){
		switch (num){
		case 1:{
			write("Param1_1.dat", writeparam[num - 1], 4);
			break;
		}
		case 2:{
			write("Param1_2.dat", writeparam[num - 1], 4);
			break;
		}
		case 3:{
			write("Param1_3.dat", writeparam[num - 1], 4);
			break;
		}
		case 4:{
			write("Param1_4.dat", writeparam[num - 1], 4);
			break;
		}
		case 5:{
			write("Param1_5.dat", writeparam[num - 1], 4);
			break;
		}
		case 6:{
			write("Param1_6.dat", writeparam[num - 1], 4);
			break;
		}
		case 7:{
			write("Param1_7.dat", writeparam[num - 1], 4);
			break;
		}
		case 8:{
			write("Param1_8.dat", writeparam[num - 1], 4);
			break;
		}
		case 9:{
			write("Param1_9.dat", writeparam[num - 1], 4);
			break;
		}
		case 10:{
			write("Param1_10.dat", writeparam[num - 1], 4);
			break;
		}
		default:
			break;
		}
	}

	if (PARAM_NUM == 3){
		switch (num){
		case 1:{
			write("Param2_1.dat", writeparam[num - 1], PARAM_NUM);
			break;
		}
		case 2:{
			write("Param2_2.dat", writeparam[num - 1], PARAM_NUM);
			break;
		}
		case 3:{
			write("Param2_3.dat", writeparam[num - 1], PARAM_NUM);
			break;
		}
		case 4:{
			write("Param2_4.dat", writeparam[num - 1], PARAM_NUM);
			break;
		}
		case 5:{
			write("Param2_5.dat", writeparam[num - 1], PARAM_NUM);
			break;
		}
		case 6:{
			write("Param2_6.dat", writeparam[num - 1], PARAM_NUM);
			break;
		}
		case 7:{
			write("Param2_7.dat", writeparam[num - 1], PARAM_NUM);
			break;
		}
		case 8:{
			write("Param2_8.dat", writeparam[num - 1], PARAM_NUM);
			break;
		}
		case 9:{
			write("Param2_9.dat", writeparam[num - 1], PARAM_NUM);
			break;
		}
		case 10:{
			write("Param2_10.dat", writeparam[num - 1], PARAM_NUM);
			break;
		}
		default:
			break;
		}
	}
	/*--------------------------------------------------------------*/
}

/* Y = aX^2+bX+cで考えた場合 */
//void QuadraticFunction(int count, float cog_data[][3], float POLHEMUS_data[][3], int num){
//
//	cv::Mat Py(count, 1, CV_32FC1);			//POLHEMUSのデータ
//	cv::Mat Pp(count, 1, CV_32FC1);			//y:ヨー，p:ピッチ，r:ロール
//	cv::Mat Pr(count, 1, CV_32FC1);
//
//	cv::Mat Cx(count, PARAM_NUM, CV_32FC1);			//重心のデータ
//	cv::Mat Cy(count, PARAM_NUM, CV_32FC1);
//	cv::Mat Cz(count, PARAM_NUM, CV_32FC1);
//
//	cv::Mat Cxt(PARAM_NUM, count, CV_32FC1);		//重心の転置データ	
//	cv::Mat Cyt(PARAM_NUM, count, CV_32FC1);
//	cv::Mat Czt(PARAM_NUM, count, CV_32FC1);
//
//	cv::Mat Qx(PARAM_NUM, PARAM_NUM, CV_32FC1);		//行列CとCtをかけた結果
//	cv::Mat Qy(PARAM_NUM, PARAM_NUM, CV_32FC1);
//	cv::Mat Qz(PARAM_NUM, PARAM_NUM, CV_32FC1);
//
//	cv::Mat invQx(PARAM_NUM, PARAM_NUM, CV_32FC1);	//Xの逆行列
//	cv::Mat invQy(PARAM_NUM, PARAM_NUM, CV_32FC1);
//	cv::Mat invQz(PARAM_NUM, PARAM_NUM, CV_32FC1);
//
//	cv::Mat Ax(PARAM_NUM, 1, CV_32FC1);		//パラメータの結果(縦)
//	cv::Mat Ay(PARAM_NUM, 1, CV_32FC1);
//	cv::Mat Az(PARAM_NUM, 1, CV_32FC1);
//
//	//cv::Mat P(count, PARAM_NUM, CV_32FC1);	//POLHEMUSのデータ
//	//cv::Mat C(count, 7, CV_32FC1);			//重心のデータ
//	//cv::Mat Ct(7, count, CV_32FC1);		//行列Cの転置
//	//cv::Mat X(7, 7, CV_32FC1);				//行列CとCtをかけた結果
//	//cv::Mat invX(7, 7, CV_32FC1);			//Xの逆行列
//	//cv::Mat A(7, PARAM_NUM, CV_32FC1);		//パラメータの結果(縦)
//
//	//PとCに値を代入していく
//
//	for (int i = 0; i < count; i++){
//		Py.at<float>(i, 0) = cog_data[i][0];
//		Pp.at<float>(i, 0) = cog_data[i][1];
//		Pr.at<float>(i, 0) = cog_data[i][2];
//
//		for (int j = 0; j < PARAM_NUM; j++){
//			//パラメータの数(関数の次数)によって行列の形が変化
//			Cx.at<float>(i, j) = powf(POLHEMUS_data[i][0], PARAM_NUM - j - 1);
//			Cy.at<float>(i, j) = powf(POLHEMUS_data[i][1], PARAM_NUM - j - 1);
//			Cz.at<float>(i, j) = powf(POLHEMUS_data[i][2], PARAM_NUM - j - 1);
//		}
//	}
//
//
//	//for (int i = 0; i < count; i++){
//	//	P.at<float>(i, 0) = POLHEMUS_data[i][0];
//	//	P.at<float>(i, 1) = POLHEMUS_data[i][1];
//	//	P.at<float>(i, 2) = POLHEMUS_data[i][2];
//	//	C.at<float>(i, 0) = cog_data[i][0] * cog_data[i][0];
//	//	C.at<float>(i, 1) = cog_data[i][1] * cog_data[i][1];
//	//	C.at<float>(i, 2) = cog_data[i][2] * cog_data[i][2];
//	//	C.at<float>(i, 3) = cog_data[i][0];
//	//	C.at<float>(i, 4) = cog_data[i][1];
//	//	C.at<float>(i, 5) = cog_data[i][2];
//	//	C.at<float>(i, 6) = 1.0;
//	//}
//
//	//パラメータの計算(最小二乗法)
//	//Ct = C.t();
//	//X = Ct * C;
//	//invX = X.inv();
//	//A = invX * Ct * P;
//
//	//パラメータの計算(最小二乗法)
//	//x座標の場合
//	Cxt = Cx.t();
//	Qx = Cxt * Cx;
//	invQx = Qx.inv();
//	Ax = invQx * Cxt * Px;
//
//	//y座標の場合
//	Cyt = Cy.t();
//	Qy = Cyt * Cy;
//	invQy = Qy.inv();
//	Ay = invQy * Cyt * Py;
//
//	//z座標の場合
//	Czt = Cz.t();
//	Qz = Czt * Cz;
//	invQz = Qz.inv();
//	Az = invQz * Czt * Pz;
//
//
//	for (int i = 0; i < 3; i++){
//		writeparam2[num - 1][i][0] = Ax.at<float>(i, 0);
//		writeparam2[num - 1][i][1] = Ay.at<float>(i, 0);
//		writeparam2[num - 1][i][2] = Az.at<float>(i, 0);
//	}
//
//	switch (num){
//	case 1:{
//		write("Param2_1.dat", writeparam2[num - 1], 7);
//		break;
//	}
//	case 2:{
//		write("Param2_2.dat", writeparam2[num - 1], 7);
//		break;
//	}
//	case 3:{
//		write("Param2_3.dat", writeparam2[num - 1], 7);
//		break;
//	}
//	case 4:{
//		write("Param2_4.dat", writeparam2[num - 1], 7);
//		break;
//	}
//	case 5:{
//		write("Param2_5.dat", writeparam2[num - 1], 7);
//		break;
//	}
//	case 6:{
//		write("Param2_6.dat", writeparam2[num - 1], 7);
//		break;
//	}
//	case 7:{
//		write("Param2_7.dat", writeparam2[num - 1], 7);
//		break;
//	}
//	case 8:{
//		write("Param2_8.dat", writeparam2[num - 1], 7);
//		break;
//	}
//	case 9:{
//		write("Param2_9.dat", writeparam2[num - 1], 7);
//		break;
//	}
//	case 10:{
//		write("Param2_10.dat", writeparam2[num - 1], 7);
//		break;
//	}
//	default:
//		break;
//	}
//}

void read(char *name, int num, int PorCOGorOcu){

	int i = 0;

	ifstream fin(name);

	if (fin.fail()){
		cout << "File Open Failed!!" << endl;
		getchar();
		exit(1);
	}

	/* POLHEMUSの場合 */
	if (PorCOGorOcu == 0){
		while (!fin.eof()){
			fin >> POLHEMUS_data[num - 1][i][1] >> POLHEMUS_data[num - 1][i][0] >> POLHEMUS_data[num - 1][i][2] >> time_P[num - 1][i];
			i++;
		}
		/* データ数保存 */
		pcount[num - 1] = i - 1;
	}
	/* COGの場合 */
	else if (PorCOGorOcu == 1){
		while (!fin.eof()){
			fin >> cog_data[num - 1][i][0] >> cog_data[num - 1][i][1] >> cog_data[num - 1][i][2] >> time_cog[num - 1][i];
			i++;
		}
		/* データ数保存 */
		cogcount[num - 1] = i - 1;
	}
	/* Oculusの場合 */
	else if (PorCOGorOcu == 2){
		while (!fin.eof()){
			fin >> Oculus_data[num - 1][i][0] >> Oculus_data[num - 1][i][1] >> Oculus_data[num - 1][i][2] >> time_Ocu[num - 1][i];
			i++;
		}
		/* データ数保存 */
		Ocucount[num - 1] = i - 1;
	}


	fin.close();
}

/* データ書き込み用関数 */
void write(char *name, float data[][3], int count/* データ数 */){


	ofstream fout(name);

	for (int i = 0; i < count; i++)
		fout << data[i][0] << " " << data[i][1] << " " << data[i][2] << endl;

	fout.close();
}

/* Excelデータ書き込み用関数 */
void Excel(char *name, float param[][3], float data[][3]/*重心データ*/, int count/*調整後のデータ数*/, int d/*何個目のデータ*/){

	//float data2[strNUM][3] = { 0 };

	printf("どうゆうこっちゃねん？");

	ofstream fout(name);

	/* 最上段への書き込み */
	//fout << "yaw(真値)" << " " << "yaw(Oculus)" << " " << "yaw(算出)" << " " << "pitch(真値)" << " " << "pitch(Oculus)" << " " << "pitch(算出)" << " " << "roll(真値)" << " " << "roll(Oculus)" << " " << "roll(算出)" << endl;
	//fout << "yaw(真値)" << " " << "yaw(x軸算出)" << " " << "yaw(真値)" << " " << "yaw(y軸算出)" << " " << "yaw(真値)" << " " << "yaw(z軸算出)" << endl;
	//fout << "pitch(真値)" << " " << "pitch(x軸算出)" << " " << "pitch(真値)" << " " << "pitch(y軸算出)" << " " << "pitch(真値)" << " " << "pitch(z軸算出)" << endl;
	//fout << "roll(真値)" << " " << "roll(x軸算出)" << " " << "roll(真値)" << " " << "roll(y軸算出)" << " " << "roll(真値)" << " " << "roll(z軸算出)" << endl;
	fout << "yaw(真値)" << " " << "yaw(算出)" << " " << "pitch(真値)" << " " << "pitch(算出)" << " " << "roll(真値)" << " " << "roll(算出)" << endl;


	//for (int i = 0; i < count; i++){//データ数のループ
	//	for (int j = 0; j < 3; j++){//x,y,z座標の順番でループ
	//		for (int k = 0; k < PARAM_NUM; k++){//次数分のループ
	//			realdata[count_sum][j] += powf(data[i][j], PARAM_NUM - k - 1) * param[k][j];
	//		}
	//	}
	//	count_sum++;
	//}

	for (int i = 0; i < count; i++){//データ数のループ
		for (int j = 0; j < 3; j++){//ヨー・ピッチ・ロールの順番でループ
			realdata[count_sum][j] += data[i][0] * param[0][j] + data[i][1] * param[1][j] + data[i][2] * param[2][j] + 1.0*param[3][j];
		}
		count_sum++;
	}

	int j = 0;

	for (int i = 0; i < count; i++){
		/* Oculusいるバージョン */
		//fout << adjst_POLHEMUS[d][i][0] << " " << adjst_Oculus[d][i][0] << " " << realdata[i][0] << " " << adjst_POLHEMUS[d][i][1] << " " << adjst_Oculus[d][i][1] << " " << realdata[i][1] << " " << adjst_POLHEMUS[d][i][2] << " " << adjst_Oculus[d][i][2] << " " << realdata[i][2] << endl;

		/* Oculusいないバージョン */
		fout << adjst_POLHEMUS[d][i][0] << " " << realdata[i][0] << " " << adjst_POLHEMUS[d][i][1] << " " << realdata[i][1] << " " << adjst_POLHEMUS[d][i][2] << " " << realdata[i][2] << endl;
		j++;
	}

	/* Y=aX^2+bX+cの場合 */
	//else if (p == 3){
	//	for (int i = 0; i < count; i++){
	//		data2[i][0] = data[i][0] * data[i][0];
	//		data2[i][1] = data[i][1] * data[i][1];
	//		data2[i][2] = data[i][2] * data[i][2];
	//		data2[i][3] = data[i][0];
	//		data2[i][4] = data[i][1];
	//		data2[i][5] = data[i][2];
	//		data2[i][6] = 1.0;
	//	}
	//	for (int i = 0; i < count; i++){
	//		for (int j = 0; j < 3; j++){
	//			for (int k = 0; k < 7; k++){
	//				realdata[i][j] += data2[i][k] * param[k][j];
	//			}
	//		}
	//	}
	//	for (int i = 0; i < count; i++)
	//		fout << adjst_POLHEMUS[d][i][0] << " " << adjst_Oculus[d][i][0] << " " << realdata[i][0] << " " << adjst_POLHEMUS[d][i][1] << " " << adjst_Oculus[d][i][1] << " " << realdata[i][1] << " " << adjst_POLHEMUS[d][i][2] << " " << adjst_Oculus[d][i][2] << " " << realdata[i][2] << endl;
	//}

}

void Excel_SUM(char *name, float data[][3]){

	ofstream fout(name);

	int count = 0;

	for (int i = 0; i < 10; i++){
		for (int j = 0; j < Exceldatacount[i]; j++){
			fout << adjst_POLHEMUS[i][j][0] << " " << data[count][0] << " " << adjst_POLHEMUS[i][j][1] << " " << data[count][1] << " " << adjst_POLHEMUS[i][j][2] << " " << data[count][2] << endl;
			count++;
		}
	}
}