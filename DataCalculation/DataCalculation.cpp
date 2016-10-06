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
//Debug���[�h�̏ꍇ
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
//Release���[�h�̍�
#pragma comment(lib,"C:\\OpenCV2.4\\lib\\opencv_core246.lib")
#pragma comment(lib,"C:\\OpenCV2.4\\lib\\opencv_imgproc246.lib")
#pragma comment(lib,"C:\\OpenCV2.4\\lib\\opencv_highgui246.lib")
#endif

using namespace std;

#define PARAM_NUM 2 //�p�����[�^�̐�(�����{1)
#define strNUM 24000 //�z��̗v�f��2
/* �ǂݍ��ݗp�֐� */
void cog(int pcount, int time_cog[], int time_P[], float cog_data[][3], float POLHEMUS_data[][3], int num);
void polhemus(int cogcount, int time_cog[], int time_P[], float cog_data[][3], float POLHEMUS_data[][3], int num);
//void oculus(int pcount, int time_ocu[], int time_P[], float ocu_data[][3], float POLHEMUS_data[][3], int num);

/* �p�����[�^�Z�o�֐� */
void PrimaryFunction(int count, float cog_data[][3], float POLHEMUS_data[][3], int num);
//void QuadraticFunction(int count, float cog_data[][3], float POLHEMUS_data[][3], int num);

void read(char *name, int num, int PorCOG);		//�ǂݍ��ݗp�֐�
void write(char *name, float data[][3], int count);		//dat�t�@�C���������ݗp�֐�
void Excel(char *name, float param[][3], float data[][3], int count, int d);		//Excel�p�������ݗp�֐�

void Excel_SUM(char *name, float data[][3]);

float POLHEMUS_data[10][strNUM][3] = { 0 };		//yaw, pitch, roll�̏��Ԃő��
float cog_data[10][strNUM][3] = { 0 };			//x, y, z���W�̏��Ԃŏd�S���
float Oculus_data[10][strNUM][3] = { 0 };		//yaw, pitch, roll�̏��Ԃő��

float adjst_POLHEMUS[10][strNUM][3] = { 0 };	//�������POLHEMUS�f�[�^
float adjst_COG[10][strNUM][3] = { 0 };		//������̏d�S�f�[�^
float adjst_Oculus[10][strNUM][3] = { 0 };	//�������Oculus�f�[�^

int pcount[10] = { 0 };		//POLHEMUS�f�[�^���̃J�E���^
int cogcount[10] = { 0 };	//�d�S�f�[�^���̃J�E���^
int Ocucount[10] = { 0 };	//Oculus�f�[�^���̃J�E���^

int time_P[10][strNUM] = { 0 };		//POLHEMUS�̃f�[�^�̃^�C���X�^���v����
int time_cog[10][strNUM] = { 0 };		//�d�S�̃f�[�^�̃^�C���X�^���v����
int time_Ocu[10][strNUM] = { 0 };		//Ocuus�̃f�[�^�̃^�C���X�^���v���

int Exceldatacount[10] = { 0 };		//Excel�ŏ������ރf�[�^��

/* �������ލۂɔz��ɉ�������� */
/* �R�����ڂɂ����āC0:x���W�C1:y���W�C2:z���W�̃p�����[�^ */
//float writeparam[10][PARAM_NUM][3] = { 0 };	// �P���֐��p�p�����[�^
//float writeparam2[10][3][3][1] = { 0 };	// �Q���֐��p�p�����[�^

/* x,y,z���W�ꏏ�ɍl�����ꍇ�̃p�����[�^ */
float writeparam[10][4][3] = { 0 };
/* -------------------------------------- */

//float paramcal1[11][2][1] = { 0 };		// �P���֐��p�̕��σp�����[�^(11�Ԗڂ̓p�����[�^�̍��v)
//float paramcal2[11][3][1] = { 0 };		// �Q���֐��p�̕��σp�����[�^(11�Ԗڂ̓p�����[�^�̍��v)

/* ���σp�����[�^�̎Z�o */
//float aveParam1[4][3][3] = { 0 };
//float aveParam2[4][7][3] = { 0 };

/* ��������p���σp�����[�^ */
float newaveparam[10][4][3] = { 0 };	// �P���֐�
//float newaveparam2[10][3][3][1] = { 0 };	// �Q���֐�

float realdata[strNUM][3] = { 0 };
int count_sum = 0;
int count_sum_data[10] = { 0 };

int main()
{
	/* �t�@�C���̓ǂݍ��� */
	/* ----------------------POLHEMUS�f�[�^------------------------- */
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


	/* ---------------------�d�S�f�[�^----------------------------- */
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


	/* ---------------------Oculus�f�[�^--------------------------- */
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


	/* -----------------------�ȉ��A�f�[�^�����킹�ɍs��----------------------------------- */
	/* ��{�I�ɂ̓f�[�^���̏��Ȃ�POLHEMUS�ɍ��킹�� */
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

	/*--------------��������p�p�����[�^�̎Z�o-----------------*/
	/* �P���֐��p */
	/*---------x,y,z���ꏏ�ɍl����ꍇ�ɒ�����-----------------*/
	/*---------PARAM_NUM���S�ɌŒ蒆---------------------------*/
	for (int i = 0; i < 10; i++){//�S�p�����[�^���Q��
		for (int j = 0; j < 3; j++){//x,y,z���W(����̓��[�E�s�b�`�E���[��)
			for (int k = 0; k < 4; k++){//�p�����[�^��
				for (int l = 0; l < 10; l++){//�������킹�p�p�����[�^
					/* �����ȊO�̃f�[�^�̍��v���Z�o */
					if (i != l)
						newaveparam[i][k][j] += writeparam[l][k][j];
				}
			}
		}
	}

	/* �����ŕ��σp�����[�^�����܂� */
	/*---------x,y,z���ꏏ�ɍl����ꍇ�ɒ�����-----------------*/
	/*---------PARAM_NUM���S�ɌŒ蒆---------------------------*/
	for (int i = 0; i < 10; i++){
		for (int j = 0; j < 3; j++){
			for (int k = 0; k < 4; k++)
				newaveparam[i][k][j] /= 9.0;
		}
	}



	///* �Q���֐��p */
	//for (int i = 0; i < 10; i++){
	//	for (int j = 0; j < 3; j++){
	//		for (int k = 0; k < 3; k++){
	//			for (int l = 0; l < 10; l++){
	//				/* �����ȊO�̃f�[�^�̍��v���Z�o */
	//				if (i != l)
	//					newaveparam2[i][j][k][1] += writeparam1[l][j][k][1];
	//			}
	//		}
	//	}
	//}

	///* �����ŕ��σp�����[�^�����܂� */
	//for (int i = 0; i < 10; i++){
	//	for (int j = 0; j < 3; j++){
	//		for (int k = 0; k < 3; k++)
	//			newaveparam2[i][j][k][1] /= 9.0;
	//	}
	//}


	///* �Ƃ肠�������v���o���Ă��� */
	//for (int i = 0; i < 10; i++){
	//	for (int j = 0; j < 7; j++){
	//		for (int k = 0; k < 3; k++){
	//			if (j < 3)	paramcal1[10][j][k] += writeparam1[i][j][k];
	//			paramcal2[10][j][k] += writeparam2[i][j][k];
	//		}
	//	}
	//}

	///* ��������p���σp�����[�^�����߂� */
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

	/* �c�Ƀf�[�^�����ׂĕ��ׂ����̂�ۑ� */
	if (PARAM_NUM == 2)
		Excel_SUM("Excel1_sum.dat", realdata);
	else if (PARAM_NUM == 3)
		Excel_SUM("Excel2_sum.dat", realdata);

	return 0;
}

/* COG�f�[�^���̂ق������������ꍇ */
void cog(int pcount, int time_cog[], int time_P[], float cog_data[][3], float POLHEMUS_data[][3], int num){

	float front = 0, back = 0;		//POLHEMUS�̃^�C���X�^���v�̒l�̑O��ɓ�����d�S���̃^�C���X�^���v�̒l

	int a = 0, b = 0; //�����_

	int j = 0;	//�d�S���̃^�C���X�^���v�̃J�E���^

	for (int i = 0; i < pcount; i++){
		back = time_cog[j];
		if (back == time_P[i]){
			adjst_COG[num - 1][i][0] = cog_data[j][0];
			adjst_COG[num - 1][i][1] = cog_data[j][1];
			adjst_COG[num - 1][i][2] = cog_data[j][2];
			j++;
			continue;
		}
		/* �Ō�̃^�C���X�^���v������Ƃ��͗�O�I�ɍŌ�̂���� */
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
			/* �����_����v�Z���đ�� */
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

	/* -------------------------------------�f�[�^�̕ۑ�--------------------------------------- */
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


	/* -------------------------------�f�[�^����p�����[�^���Z�o------------------------------- */
	PrimaryFunction(pcount, adjst_COG[num - 1], POLHEMUS_data, num);		//�P���֐��̏ꍇ
	//QuadraticFunction(pcount, adjst_COG[num - 1], POLHEMUS_data, num);	//�Q���֐��̏ꍇ	

}

/* Oculus�̃f�[�^����POLHEMUS���ɍ��킹�� */
void oculus(int pcount, int time_ocu[], int time_P[], float ocu_data[][3], float POLHEMUS_data[][3], int num){

	float front = 0, back = 0;		//POLHEMUS�̃^�C���X�^���v�̒l�̑O��ɓ�����d�S���̃^�C���X�^���v�̒l

	int a = 0, b = 0; //�����_

	int j = 0;	//�d�S���̃^�C���X�^���v�̃J�E���^

	for (int i = 0; i < pcount; i++){
		back = time_ocu[j];
		if (back == time_P[i]){
			adjst_Oculus[num - 1][i][0] = ocu_data[j][0];
			adjst_Oculus[num - 1][i][1] = ocu_data[j][1];
			adjst_Oculus[num - 1][i][2] = ocu_data[j][2];
			j++;
			continue;
		}
		/* �Ō�̃^�C���X�^���v������Ƃ��͗�O�I�ɍŌ�̂���� */
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
			/* �����_����v�Z���đ�� */
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

	/* -------------------------------------�f�[�^�̕ۑ�--------------------------------------- */
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


	/* -------------------------------�f�[�^����p�����[�^���Z�o------------------------------- */
	//PrimaryFunction(pcount, adjst_COG[num - 1], POLHEMUS_data, num);		//�P���֐��̏ꍇ
	//QuadraticFunction(pcount, adjst_COG[num - 1], POLHEMUS_data, num);	//�Q���֐��̏ꍇ	

}

/* POLHEMUS�f�[�^���̂ق������������ꍇ */
void polhemus(int cogcount, int time_cog[], int time_P[], float cog_data[][3], float POLHEMUS_data[][3], int num){

	float front = 0, back = 0;		//POLHEMUS�̃^�C���X�^���v�̒l�̑O��ɓ�����d�S���̃^�C���X�^���v�̒l

	int a = 0, b = 0; //�����_

	int j = 0;	//�d�S���̃^�C���X�^���v�̃J�E���^

	for (int i = 0; i < cogcount; i++){
		back = time_P[j];
		if (back == time_cog[i]){
			adjst_POLHEMUS[num - 1][i][0] = POLHEMUS_data[j][0];
			adjst_POLHEMUS[num - 1][i][1] = POLHEMUS_data[j][1];
			adjst_POLHEMUS[num - 1][i][2] = POLHEMUS_data[j][2];
			j++;
			continue;
		}
		/* �Ō�̃^�C���X�^���v������Ƃ��͗�O�I�ɍŌ�̂���� */
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
			/* �����_����v�Z���đ�� */
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

	/* -------------------------------------�f�[�^�̕ۑ�--------------------------------------- */
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


	/* -------------------------------�f�[�^����p�����[�^���Z�o------------------------------- */
	PrimaryFunction(cogcount, cog_data, adjst_POLHEMUS[num - 1], num);		//�P���֐��̏ꍇ
	//QuadraticFunction(cogcount, cog_data, adjst_POLHEMUS[num - 1], num);	//�Q���֐��̏ꍇ
}

/* ��A���͊֐� */
void PrimaryFunction(int count, float cog_data[][3], float POLHEMUS_data[][3], int num){

	cv::Mat Py(count, 1, CV_32FC1);			//POLHEMUS�̃f�[�^
	cv::Mat Pp(count, 1, CV_32FC1);			//y:���[�Cp:�s�b�`�Cr:���[��
	cv::Mat Pr(count, 1, CV_32FC1);

	/* ---------------x,y,z���W�ʁX�ōl����ꍇ----------------------------------- */
	//cv::Mat Cx(count, PARAM_NUM, CV_32FC1);			//�d�S�̃f�[�^
	//cv::Mat Cy(count, PARAM_NUM, CV_32FC1);
	//cv::Mat Cz(count, PARAM_NUM, CV_32FC1);

	//cv::Mat Cxt(PARAM_NUM, count, CV_32FC1);		//�d�S�̓]�u�f�[�^	
	//cv::Mat Cyt(PARAM_NUM, count, CV_32FC1);
	//cv::Mat Czt(PARAM_NUM, count, CV_32FC1);

	//cv::Mat Qx(PARAM_NUM, PARAM_NUM, CV_32FC1);		//�s��C��Ct������������
	//cv::Mat Qy(PARAM_NUM, PARAM_NUM, CV_32FC1);
	//cv::Mat Qz(PARAM_NUM, PARAM_NUM, CV_32FC1);

	//cv::Mat invQx(PARAM_NUM, PARAM_NUM, CV_32FC1);	//X�̋t�s��
	//cv::Mat invQy(PARAM_NUM, PARAM_NUM, CV_32FC1);
	//cv::Mat invQz(PARAM_NUM, PARAM_NUM, CV_32FC1);

	//cv::Mat Ax(PARAM_NUM, 1, CV_32FC1);		//�p�����[�^�̌���(�c)
	//cv::Mat Ay(PARAM_NUM, 1, CV_32FC1);
	//cv::Mat Az(PARAM_NUM, 1, CV_32FC1);
	/* --------------------------------------------------------------------------- */




	/* ---------------x,y,z���W�ꏏ�ōl����ꍇ----------------------------------- */
	cv::Mat C(count, 4, CV_32FC1);					//�d�S�̃f�[�^
	cv::Mat Ct(4, count, CV_32FC1);					//�d�S�s��̓]�u�f�[�^
	cv::Mat Q(4, 4, CV_32FC1);						//�s��C��Ct������������
	cv::Mat invQ(4, 4, CV_32FC1);					//Q�̋t�s��
	cv::Mat Ay(4, 1, CV_32FC1);						//�p�����[�^(���[)
	cv::Mat Ap(4, 1, CV_32FC1);						//�p�����[�^(�s�b�`)
	cv::Mat Ar(4, 1, CV_32FC1);						//�p�����[�^(���[��)
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

	/* ------x,y,z���W�ʁX�ɍl�����ꍇ--------------------------------- */
	//for (int i = 0; i < count; i++){
	//	Py.at<float>(i, 0) = POLHEMUS_data[i][0];
	//	Pp.at<float>(i, 0) = POLHEMUS_data[i][1];
	//	Pr.at<float>(i, 0) = POLHEMUS_data[i][2];

	//	for (int j = 0; j < PARAM_NUM; j++){
	//		//�p�����[�^�̐�(�֐��̎���)�ɂ���čs��̌`���ω�
	//		Cx.at<float>(i, j) = powf(cog_data[i][0], PARAM_NUM - j - 1);
	//		Cy.at<float>(i, j) = powf(cog_data[i][1], PARAM_NUM - j - 1);
	//		Cz.at<float>(i, j) = powf(cog_data[i][2], PARAM_NUM - j - 1);
	//	}
	//}
	/* ------------------------------------------------------------------ */

	/* --------------x,y,z���W�ꏏ�ɍl�����ꍇ(������)-------------------------------- */
	for (int i = 0; i < count; i++){
		Py.at<float>(i, 0) = POLHEMUS_data[i][0];
		Pp.at<float>(i, 0) = POLHEMUS_data[i][1];
		Pr.at<float>(i, 0) = POLHEMUS_data[i][2];

		C.at<float>(i, 0) = cog_data[i][0];
		C.at<float>(i, 1) = cog_data[i][1];
		C.at<float>(i, 2) = cog_data[i][2];
		C.at<float>(i, 3) = 1.0;
	}

	//�p�����[�^�̌v�Z(�ŏ����@)
	/* ---------------- yaw�̏ꍇ ----------------- */
	//x���W�̏ꍇ
	//Cxt = Cx.t();
	//Qx = Cxt * Cx;
	//invQx = Qx.inv();
	//Ax = invQx * Cxt * Py;

	////y���W�̏ꍇ
	//Cyt = Cy.t();
	//Qy = Cyt * Cy;
	//invQy = Qy.inv();
	//Ay = invQy * Cyt * Py;

	////z���W�̏ꍇ
	//Czt = Cz.t();
	//Qz = Czt * Cz;
	//invQz = Qz.inv();
	//Az = invQz * Czt * Py;
	/* ------------------------------------------- */

	/* ---------------- pitch�̏ꍇ ----------------- */
	//x���W�̏ꍇ
	//Cxt = Cx.t();
	//Qx = Cxt * Cx;
	//invQx = Qx.inv();
	//Ax = invQx * Cxt * Pp;

	////y���W�̏ꍇ
	//Cyt = Cy.t();
	//Qy = Cyt * Cy;
	//invQy = Qy.inv();
	//Ay = invQy * Cyt * Pp;

	////z���W�̏ꍇ
	//Czt = Cz.t();
	//Qz = Czt * Cz;
	//invQz = Qz.inv();
	//Az = invQz * Czt * Pp;
	/* ------------------------------------------- */

	/* ---------------- roll�̏ꍇ ----------------- */
	//x���W�̏ꍇ
	//Cxt = Cx.t();
	//Qx = Cxt * Cx;
	//invQx = Qx.inv();
	//Ax = invQx * Cxt * Pr;

	////y���W�̏ꍇ
	//Cyt = Cy.t();
	//Qy = Cyt * Cy;
	//invQy = Qy.inv();
	//Ay = invQy * Cyt * Pr;

	////z���W�̏ꍇ
	//Czt = Cz.t();
	//Qz = Czt * Cz;
	//invQz = Qz.inv();
	//Az = invQz * Czt * Pr;
	/* ------------------------------------------- */

	/* ---------------- ���W���ƂɊ֌W�Ȃ��ꍇ ----------------- */
	Ct = C.t();
	Q = Ct*C;
	invQ = Q.inv();
	/* �����܂ł̓��[�E�s�b�`�E���[���S���ꏏ */
	/* �����艺�͋�ʂ��Ă��� */
	Ay = invQ*Ct*Py;
	Ap = invQ*Ct*Pp;
	Ar = invQ*Ct*Pr;

	/* ----------------x,y,z���W�ʁX�ōl�����ꍇ------------------- */
	//for (int i = 0; i < PARAM_NUM; i++){
	//	writeparam[num - 1][i][0] = Ax.at<float>(i, 0);
	//	writeparam[num - 1][i][1] = Ay.at<float>(i, 0);
	//	writeparam[num - 1][i][2] = Az.at<float>(i, 0);
	//}
	/* ----------------------------------------------------------- */

	/* ----------------x,y,z���W�ꏏ�ōl�����ꍇ------------------- */
	for (int i = 0; i < 4; i++){
		writeparam[num - 1][i][0] = Ay.at<float>(i, 0);
		writeparam[num - 1][i][1] = Ap.at<float>(i, 0);
		writeparam[num - 1][i][2] = Ar.at<float>(i, 0);
	}
	/* ----------------------------------------------------------- */

	/* -----------�ꎞ�I��x,y,z���W���ꏏ�ɍl����--------------------- */
	/* -----------PARAM_NUM��4(�Œ�)�ɕύX---------------------------- */
	/* ------PARAM_NUM=2�ɌŒ肵�ċN���A1���֐��p�ɓ���悤�ɂ���----- */
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

/* Y = aX^2+bX+c�ōl�����ꍇ */
//void QuadraticFunction(int count, float cog_data[][3], float POLHEMUS_data[][3], int num){
//
//	cv::Mat Py(count, 1, CV_32FC1);			//POLHEMUS�̃f�[�^
//	cv::Mat Pp(count, 1, CV_32FC1);			//y:���[�Cp:�s�b�`�Cr:���[��
//	cv::Mat Pr(count, 1, CV_32FC1);
//
//	cv::Mat Cx(count, PARAM_NUM, CV_32FC1);			//�d�S�̃f�[�^
//	cv::Mat Cy(count, PARAM_NUM, CV_32FC1);
//	cv::Mat Cz(count, PARAM_NUM, CV_32FC1);
//
//	cv::Mat Cxt(PARAM_NUM, count, CV_32FC1);		//�d�S�̓]�u�f�[�^	
//	cv::Mat Cyt(PARAM_NUM, count, CV_32FC1);
//	cv::Mat Czt(PARAM_NUM, count, CV_32FC1);
//
//	cv::Mat Qx(PARAM_NUM, PARAM_NUM, CV_32FC1);		//�s��C��Ct������������
//	cv::Mat Qy(PARAM_NUM, PARAM_NUM, CV_32FC1);
//	cv::Mat Qz(PARAM_NUM, PARAM_NUM, CV_32FC1);
//
//	cv::Mat invQx(PARAM_NUM, PARAM_NUM, CV_32FC1);	//X�̋t�s��
//	cv::Mat invQy(PARAM_NUM, PARAM_NUM, CV_32FC1);
//	cv::Mat invQz(PARAM_NUM, PARAM_NUM, CV_32FC1);
//
//	cv::Mat Ax(PARAM_NUM, 1, CV_32FC1);		//�p�����[�^�̌���(�c)
//	cv::Mat Ay(PARAM_NUM, 1, CV_32FC1);
//	cv::Mat Az(PARAM_NUM, 1, CV_32FC1);
//
//	//cv::Mat P(count, PARAM_NUM, CV_32FC1);	//POLHEMUS�̃f�[�^
//	//cv::Mat C(count, 7, CV_32FC1);			//�d�S�̃f�[�^
//	//cv::Mat Ct(7, count, CV_32FC1);		//�s��C�̓]�u
//	//cv::Mat X(7, 7, CV_32FC1);				//�s��C��Ct������������
//	//cv::Mat invX(7, 7, CV_32FC1);			//X�̋t�s��
//	//cv::Mat A(7, PARAM_NUM, CV_32FC1);		//�p�����[�^�̌���(�c)
//
//	//P��C�ɒl�������Ă���
//
//	for (int i = 0; i < count; i++){
//		Py.at<float>(i, 0) = cog_data[i][0];
//		Pp.at<float>(i, 0) = cog_data[i][1];
//		Pr.at<float>(i, 0) = cog_data[i][2];
//
//		for (int j = 0; j < PARAM_NUM; j++){
//			//�p�����[�^�̐�(�֐��̎���)�ɂ���čs��̌`���ω�
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
//	//�p�����[�^�̌v�Z(�ŏ����@)
//	//Ct = C.t();
//	//X = Ct * C;
//	//invX = X.inv();
//	//A = invX * Ct * P;
//
//	//�p�����[�^�̌v�Z(�ŏ����@)
//	//x���W�̏ꍇ
//	Cxt = Cx.t();
//	Qx = Cxt * Cx;
//	invQx = Qx.inv();
//	Ax = invQx * Cxt * Px;
//
//	//y���W�̏ꍇ
//	Cyt = Cy.t();
//	Qy = Cyt * Cy;
//	invQy = Qy.inv();
//	Ay = invQy * Cyt * Py;
//
//	//z���W�̏ꍇ
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

	/* POLHEMUS�̏ꍇ */
	if (PorCOGorOcu == 0){
		while (!fin.eof()){
			fin >> POLHEMUS_data[num - 1][i][1] >> POLHEMUS_data[num - 1][i][0] >> POLHEMUS_data[num - 1][i][2] >> time_P[num - 1][i];
			i++;
		}
		/* �f�[�^���ۑ� */
		pcount[num - 1] = i - 1;
	}
	/* COG�̏ꍇ */
	else if (PorCOGorOcu == 1){
		while (!fin.eof()){
			fin >> cog_data[num - 1][i][0] >> cog_data[num - 1][i][1] >> cog_data[num - 1][i][2] >> time_cog[num - 1][i];
			i++;
		}
		/* �f�[�^���ۑ� */
		cogcount[num - 1] = i - 1;
	}
	/* Oculus�̏ꍇ */
	else if (PorCOGorOcu == 2){
		while (!fin.eof()){
			fin >> Oculus_data[num - 1][i][0] >> Oculus_data[num - 1][i][1] >> Oculus_data[num - 1][i][2] >> time_Ocu[num - 1][i];
			i++;
		}
		/* �f�[�^���ۑ� */
		Ocucount[num - 1] = i - 1;
	}


	fin.close();
}

/* �f�[�^�������ݗp�֐� */
void write(char *name, float data[][3], int count/* �f�[�^�� */){


	ofstream fout(name);

	for (int i = 0; i < count; i++)
		fout << data[i][0] << " " << data[i][1] << " " << data[i][2] << endl;

	fout.close();
}

/* Excel�f�[�^�������ݗp�֐� */
void Excel(char *name, float param[][3], float data[][3]/*�d�S�f�[�^*/, int count/*������̃f�[�^��*/, int d/*���ڂ̃f�[�^*/){

	//float data2[strNUM][3] = { 0 };

	printf("�ǂ��䂤��������˂�H");

	ofstream fout(name);

	/* �ŏ�i�ւ̏������� */
	//fout << "yaw(�^�l)" << " " << "yaw(Oculus)" << " " << "yaw(�Z�o)" << " " << "pitch(�^�l)" << " " << "pitch(Oculus)" << " " << "pitch(�Z�o)" << " " << "roll(�^�l)" << " " << "roll(Oculus)" << " " << "roll(�Z�o)" << endl;
	//fout << "yaw(�^�l)" << " " << "yaw(x���Z�o)" << " " << "yaw(�^�l)" << " " << "yaw(y���Z�o)" << " " << "yaw(�^�l)" << " " << "yaw(z���Z�o)" << endl;
	//fout << "pitch(�^�l)" << " " << "pitch(x���Z�o)" << " " << "pitch(�^�l)" << " " << "pitch(y���Z�o)" << " " << "pitch(�^�l)" << " " << "pitch(z���Z�o)" << endl;
	//fout << "roll(�^�l)" << " " << "roll(x���Z�o)" << " " << "roll(�^�l)" << " " << "roll(y���Z�o)" << " " << "roll(�^�l)" << " " << "roll(z���Z�o)" << endl;
	fout << "yaw(�^�l)" << " " << "yaw(�Z�o)" << " " << "pitch(�^�l)" << " " << "pitch(�Z�o)" << " " << "roll(�^�l)" << " " << "roll(�Z�o)" << endl;


	//for (int i = 0; i < count; i++){//�f�[�^���̃��[�v
	//	for (int j = 0; j < 3; j++){//x,y,z���W�̏��ԂŃ��[�v
	//		for (int k = 0; k < PARAM_NUM; k++){//�������̃��[�v
	//			realdata[count_sum][j] += powf(data[i][j], PARAM_NUM - k - 1) * param[k][j];
	//		}
	//	}
	//	count_sum++;
	//}

	for (int i = 0; i < count; i++){//�f�[�^���̃��[�v
		for (int j = 0; j < 3; j++){//���[�E�s�b�`�E���[���̏��ԂŃ��[�v
			realdata[count_sum][j] += data[i][0] * param[0][j] + data[i][1] * param[1][j] + data[i][2] * param[2][j] + 1.0*param[3][j];
		}
		count_sum++;
	}

	int j = 0;

	for (int i = 0; i < count; i++){
		/* Oculus����o�[�W���� */
		//fout << adjst_POLHEMUS[d][i][0] << " " << adjst_Oculus[d][i][0] << " " << realdata[i][0] << " " << adjst_POLHEMUS[d][i][1] << " " << adjst_Oculus[d][i][1] << " " << realdata[i][1] << " " << adjst_POLHEMUS[d][i][2] << " " << adjst_Oculus[d][i][2] << " " << realdata[i][2] << endl;

		/* Oculus���Ȃ��o�[�W���� */
		fout << adjst_POLHEMUS[d][i][0] << " " << realdata[i][0] << " " << adjst_POLHEMUS[d][i][1] << " " << realdata[i][1] << " " << adjst_POLHEMUS[d][i][2] << " " << realdata[i][2] << endl;
		j++;
	}

	/* Y=aX^2+bX+c�̏ꍇ */
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