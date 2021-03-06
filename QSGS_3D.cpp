/// \file GSGS.cpp
/// \reference Wang, M., Wang, J., Pan, N., & Chen, S. (2007). 
/// Mesoscopic predictions of the effective thermal conductivity for microscale random porous media. 
/// Physical Review E - Statistical, Nonlinear, and Soft Matter Physics, 75(3), 1�C10.
/// https://doi.org/10.1103/PhysRevE.75.036702
/// 
/// 

#include <iostream>
#include <stdlib.h> 
#include <time.h>
#include <string>
#include <fstream>
#include <omp.h>
#include <stdio.h>

using namespace std;

#define NUM_THREADS  8			/// OpenMP thread

const float phi = 0.7;			/// target porosity
const float p_cd = 0.05;		/// core distribution probability
const float p_surface = 0.2, p_edge = p_surface / 12.0, p_point = p_edge / 8.0;	/// generation probability

const int N = 300;			/// grid number in one dimension
int Solid[N][N][N];
int Solid_p[N][N][N];
int Grow_Times = 0;			/// iteration variable

/// No solid in the beginning.
void init()
{
	omp_set_num_threads(NUM_THREADS);
# pragma omp parallel for
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			for (int k = 0; k < N; k++)
				Solid[i][j][k] = 0;
}


/// \brief Grow in 26 directions, including 6 surfaces, 12 edges and eight points 
///
///
///
void grow()
{
	Grow_Times += 1;
	std::cout << "Grow Times: " << Grow_Times << endl;

	omp_set_num_threads(NUM_THREADS);
# pragma omp parallel for
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			for (int k = 0; k < N; k++)
				Solid_p[i][j][k] = Solid[i][j][k];
	cout << "Copy compeleted" << endl;

	omp_set_num_threads(NUM_THREADS);
# pragma omp parallel for 
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			for (int k = 0; k < N; k++)
			{
				if (Solid[i][j][k] == 1)
				{
					for (int x = -1; x <= 1; x++)
						for (int y = -1; y <= 1; y++)
							for (int z = -1; z <= 1; z++)
							{
								if (!((i + x) < 0 || (j + y) < 0 || (k + z) < 0 || (i + x) > N - 1 || (j + y) > N - 1 || (k + z) > N - 1))
								{
									switch (abs(x) + abs(y) + abs(z))
									{
									case 0:
										Solid_p[i + x][j + y][k + z] = 1;
										break;
									case 1:		// face neighbor
										if ((rand() / double(RAND_MAX)) < p_surface) Solid_p[i + x][j + y][k + z] = 1;
										break;
									case 2:		// edge neighbor
										if ((rand() / double(RAND_MAX)) < p_edge) Solid_p[i + x][j + y][k + z] = 1;
										break;
									case 3:		/// point neighbor
										if ((rand() / double(RAND_MAX)) < p_point) Solid_p[i + x][j + y][k + z] = 1;
										break;
									default:
										break;
									}
								}
							}
				}
			}
		}
	}
	cout << "Neighbor generatedd" << endl;

	omp_set_num_threads(NUM_THREADS);
# pragma omp parallel for 
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			for (int k = 0; k < N; k++)
			{
				int s_temp = Solid[i][j][k] + Solid_p[i][j][k];
				if (s_temp > 0) Solid[i][j][k] = 1;
			}
	cout << "One time grow compeleted" << endl;
}

void output2txt()
{
	ofstream outfile;
	string filename;
	filename = "porous.out";
	outfile.open(filename);
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			for (int k = 0; k < N; k++)
				outfile << i << " " << j << " " << k << " " << Solid[i][j][k] << endl;
	outfile.close();
	cout << "Grow completed" << endl;
}

void output2tecplot()
{
	string out_buffer1 = "";

	ofstream outfile;
	string filename = "porous_3D.plt";
	string title = "3D porous media";
	outfile.open(filename);

	outfile << "TITLE = \"" << title << "\"" << endl;

	/// output solid position
	outfile << "VARIABLES = \"X\", \"Y\", \"Z\",\"value\" " << endl;
	outfile << "ZONE  t=\"solid\" I = " << N << ", J = " << N  << ", K = " << N  << ", F = point" << endl;
	for (int i = 0; i < N; i++)
	{
		out_buffer1 = "";
		for (int j = 0; j < N; j++)
			for (int k = 0; k < N; k++)
					out_buffer1 += std::to_string(i) + "," + std::to_string(j) + "," + std::to_string(k) + "," + std::to_string(Solid[i][j][k]) + "\n";
					
		outfile << out_buffer1;
	}

	outfile.close();
	cout << "Output completed" << endl;
}

bool is_interior(int i, int j, int k)
{
	if (Solid[i][j][k] == 1)
	{
		int sum_i = 0;
		for (int x = -1; x <= 1; x++)
			for (int y = -1; y <= 1; y++)
				for (int z = -1; z <= 1; z++)
					{
						if (!((i + x) < 0 || (j + y) < 0 || (k + z) < 0 || (i + x) > N - 1 || (j + y) > N - 1 || (k + z) > N - 1))
							{
								sum_i =+ Solid[i+x][j+y][k+z];
								if (Solid[i+x][j+y][k+z] == 0) return false;
							}
						else
							{
								sum_i =+ 1;
							}
							
					}
					
		if (sum_i == 27) return true;
	}
	

				
}

void output2palabos()
{
    ofstream outfile;
    string filename = "porous_3D.dat";
    
	/// Every voxel is given a value: 
	/// 0 for a fluid voxel (blue), 
	/// 1 for a material voxel that touches (26-connected) a pore voxel (green),
	/// 2 for an interior material voxel (red) illustrated again by the inlet slice (left) and the halfway slice (right)
    omp_set_num_threads(NUM_THREADS);
	# pragma omp parallel for
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			 for (int k = 0; k < N; k++)
			 {
				 if (is_interior(i,j,k)) Solid[i][j][k] = 2;
				 if ((j == 0) || (k == 0) || (j == N-1) || (k == N-1)) Solid[i][j][k] = 2;   /// pixels on the border fo y and z directions are set to be 2
			 }
				

    outfile.open(filename);
	string out_buffer2 = "";
	for (int i = 0; i < N; i++)
	{
		out_buffer2 = "";
		for (int j = 0; j < N; j++)
			for (int k = 0; k < N; k++)
					out_buffer2 += std::to_string(Solid[i][j][k]) + "\n";
					
		outfile << out_buffer2;
	}
	outfile.close();
	cout << "Output completed" << endl;
}

int main()
{
	float phi_p;		/// tempprary porosity 

	/// Generate random seed
	srand((unsigned)time(NULL));
	cout << "Generate random seed completed" <<endl;

	/// Initialization
	init();
	cout << "Initialization completed" << endl;

	/// Generate growth core
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			for (int k = 0; k < N; k++)
			{
				if ((rand() / double(RAND_MAX)) < p_cd) Solid[i][j][k] = 1;
			}	
	cout << "Generate growth core completed" << endl;

	///  Generate process
	do
	{
		grow();

		/// Calculate porosity
		float t_temp = 0;
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				for (int k = 0; k < N; k++)
					t_temp += Solid[i][j][k];
		phi_p = 1 - t_temp / (N * N * N);
		cout << phi_p << endl;
	} while (phi_p > phi);

	/// Output result
	///output2tecplot();
	output2palabos();

	return 0;
}
