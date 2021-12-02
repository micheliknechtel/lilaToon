#include "stdafx.h"
#include "phong.h"
#include <math.h>
#include "custom_open_cv.h"
#include <stdio.h>

float cos(point R, point V)
{
	// coseno do angulo entre o observador e o raio refletido
	return V.x * R.x + V.y * R.y + V.z * R.z;
}

//void Phong(int iR, int iG, int iB,  float x, float y, float z, int pX, int pY, float shine, point light, point centerCanvas, float *Ipr, float *Ipg, float *Ipb, int w, int h, bool enableSpecular, float ambiente, FILE *file)
//{
//	point N, n;
//
//	N.x = x;
//	N.y = y;
//	N.z = z;
//
//
//	float norma = sqrt(pow(N.x,2)+pow(N.y,2)+pow(N.z,2));
//	n.x = N.x/norma;
//	n.y = N.y/norma;
//	n.z = N.z/norma;
//
//	float Top = 8.5;
//	//Vx * Nx + Vy * Ny + Vz * Nz = 0
//	//Vz = ((Vx * Nx + Vy * Ny) * -1)/Nz
//
//	float bZ = 0;
//	float iZ = -1;
//
//	for(int i = 0; i < 3 || iZ == bZ;i++)
//	{
//		iZ = bZ;
//		bZ = (n.x)/n.z * -1 + iZ/n.z * -1;
//	}
//	iZ = bZ;
//
//	int depth = ((iZ+Top)/(2 * Top) * 255);
//
//	if(depth < 0)
//		depth = 0;
//	else if(depth > 255)
//		depth = 255;
//
//	if((pX == 236 && pY == 220) || (pX == 573 && pY == 220) || (pX == 404 && pY == 47) || (pX == 404 && pY == 391)|| (pX == 404 && pY == 220))
//		fprintf(file, "pX = %d pY = %d nX = %f nY = %f nZ = %f Depth = %d\n",pX, pY, n.x, n.y, n.z, depth);
//
//	*Ipr = depth;
//	*Ipb = depth;
//	*Ipg = depth;
//}

void Phong(int iR, int iG, int iB,  float x, float y, float z, int pX, int pY, float shine, point light, point centerCanvas, float *Ipr, float *Ipg, float *Ipb, int w, int h, bool enableSpecular, float ambiente)
{
	//Equation Ip(r,g,b) = ka * ia + (kd(r,g,b) *(L * N) *id + ks(R * V)^shine *is)
	//Equation R = V - 2 * (V.N) * N 
	float kr = 1;
	float ka = 1;
	float kg = 0;
	float kb = 0;
	float ks = 1;
	float id = 0.8;
	float is = 0.5;
	float es = 0;
	point N, R, V, L, l, n, RE;

	N.x = x;
	N.y = y;
	N.z = z;

	// Vl é o vetor da luz até o ponto da esfera.
	L.x = (light.x - pX);
	L.y = (light.y - pY);
	L.z = light.z;

	// Normalizando o vetor Vl (raio de luz)
	float a = sqrt(powf(L.x,2) + powf(L.y,2) + powf(L.z,2));

	l.x = L.x / a;
	l.y = L.y / a;
	l.z = L.z / a;

	float norma = sqrt(pow(N.x,2)+pow(N.y,2)+pow(N.z,2));
	n.x = N.x/norma;
	n.y = N.y/norma;
	n.z = N.z/norma;

	float LN = (l.x* n.x + l.y * n.y + l.z * n.z);
 
	//Equation R = V - 2 * (V.N) * N  
	/*R.x = 2 * (LN) * N.x - L.x;
	R.y = 2 * (LN) * N.y - L.y;
	R.z = 2 * (LN) * N.z - L.z;*/

	RE.x = 2 * (LN) * n.x - l.x;
	RE.y = 2 * (LN) * n.y - l.y;
	RE.z = 2 * (LN) * n.z - l.z;


    // Coloca o observador (viewer) longe no eixo Z
    V.x = (centerCanvas.x - pX);
	V.y = (centerCanvas.y - pY);
	V.z = centerCanvas.z;

	// Normalizando o vetor do osbservador
	float center = sqrt(V.x * V.x + V.y * V.y + V.z * V.z);
	V.x = V.x/center;
	V.y = V.y/center;
	V.z = V.z/center;

	//Equation R = V - 2 * (V.N) * N
	R.x = V.x - 2 * (V.x * n.x) * n.x;
	R.y = V.y - 2 * (V.y * n.y) * n.y;
	R.z = V.z - 2 * (V.z * n.z) * n.z;
	
	float r = sqrt( powf(R.x,2) + powf(R.y,2) + powf(R.z,2));                  
	R.x = R.x/r;
	R.y = R.y/r;
	R.z = R.z/r;

	float REV = cos(RE,V);

	if(REV > 0)
		es = 255 * powf(REV, shine) * is ;
	
	if(! enableSpecular)
		es = 0;
	
	float amb = 0;

	//Silhueta
	/*float VN = V.x * n.x + V.z * n.z + V.y * n.y ;
	
	if( VN > 1)
		VN = 1;
	else if(VN < 0)
		VN = 0;


	iB = VN * iB;
	iR = VN * iR;
	iG = VN * iG;*/

	*Ipr = iR * ambiente + (iR * LN * id + es);
	//*Ipr = (float)sqrt(*Ipr * *Ipr +iR * iR)/2;
	if(*Ipr < 0 )
		*Ipr = 0;

	if(*Ipr > 255 )
		*Ipr = 255;


	*Ipg = iG * ambiente + (iG * LN * id + es);
	//*Ipg = (float)sqrt(*Ipg * *Ipg + iG * iG)/2;
	if(*Ipg < 0 )
		*Ipg = 0;

	if(*Ipg > 255 )
		*Ipg = 255;

	*Ipb = iB * ambiente + (iB * LN * id + es);
	//*Ipb = (float)sqrt(*Ipb * *Ipb + iB * iB)/2;
	if(*Ipb < 0 )
		*Ipb = 0;

	if(*Ipb > 255 )
		*Ipb = 255;

}

