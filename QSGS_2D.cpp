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

using namespace std;

const float phi = 0.6;			/// target porosity
const float p_cd = 0.05;		/// core distribution probability
const float z = 0.05, f = 0.0125;
const float p_d1 = z, p_d2 = z, p_d3 = z, p_d4 = z;
const float p_d5 = f, p_d6 = f, p_d7 = f, p_d8 = f;

const int N = 300;
int Solid[N][N];
int Grow_Times = 0;

/// No solid in the beginning.
void init()
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			Solid[i][j] = 0;
		}
	}
}

/// \brief Grow in eight directions.
///
/// Grow directions
///*****    6    2    5   *****
///*****                  *****
///*****    3    C    1   *****
///*****                  *****
///*****    7    4    8   *****
///
void grow()
{
	Grow_Times += 1;
	cout << "Grow Times: " << Grow_Times << endl;

	int Solid_p[N][N];
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			Solid_p[i][j] = Solid[i][j];
		}
	}
	cout << "Copy compeleted" << endl;

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (i == 0)
			{
				if (j == 0)
				{
					if (Solid[i][j] == 1)
					{
						if ((rand() / double(RAND_MAX)) < p_d1) Solid_p[i + 1][j] = 1;
						if ((rand() / double(RAND_MAX)) < p_d2) Solid_p[i][j + 1] = 1;
						if ((rand() / double(RAND_MAX)) < p_d5) Solid_p[i + 1][j + 1] = 1;
					}
				}
				else if (j == N - 1)
				{
					if (Solid[i][j] == 1)
					{
						if ((rand() / double(RAND_MAX)) < p_d1) Solid_p[i + 1][j] = 1;
						if ((rand() / double(RAND_MAX)) < p_d4) Solid_p[i][j - 1] = 1;
						if ((rand() / double(RAND_MAX)) < p_d8) Solid_p[i + 1][j - 1] = 1;
					}
				}
				else
				{
					if (Solid[i][j] == 1)
					{
						if ((rand() / double(RAND_MAX)) < p_d1) Solid_p[i + 1][j] = 1;
						if ((rand() / double(RAND_MAX)) < p_d2) Solid_p[i][j + 1] = 1;
						if ((rand() / double(RAND_MAX)) < p_d4) Solid_p[i][j - 1] = 1;
						if ((rand() / double(RAND_MAX)) < p_d5) Solid_p[i + 1][j + 1] = 1;
						if ((rand() / double(RAND_MAX)) < p_d8) Solid_p[i + 1][j - 1] = 1;
					}
				}
			}
			else if (i == N - 1)
			{
				if (j == 0)
				{
					if (Solid[i][j] == 1)
					{
						if ((rand() / double(RAND_MAX)) < p_d2) Solid_p[i][j + 1] = 1;
						if ((rand() / double(RAND_MAX)) < p_d3) Solid_p[i - 1][j] = 1;
						if ((rand() / double(RAND_MAX)) < p_d6) Solid_p[i - 1][j + 1] = 1;
					}
				}
				else if (j == N - 1)
				{
					if (Solid[i][j] == 1)
					{
						if ((rand() / double(RAND_MAX)) < p_d3) Solid_p[i - 1][j] = 1;
						if ((rand() / double(RAND_MAX)) < p_d4) Solid_p[i][j - 1] = 1;
						if ((rand() / double(RAND_MAX)) < p_d7) Solid_p[i - 1][j - 1] = 1;
					}
				}
				else
				{
					if (Solid[i][j] == 1)
					{
						if ((rand() / double(RAND_MAX)) < p_d2) Solid_p[i][j + 1] = 1;
						if ((rand() / double(RAND_MAX)) < p_d3) Solid_p[i - 1][j] = 1;
						if ((rand() / double(RAND_MAX)) < p_d4) Solid_p[i][j - 1] = 1;
						if ((rand() / double(RAND_MAX)) < p_d6) Solid_p[i - 1][j + 1] = 1;
						if ((rand() / double(RAND_MAX)) < p_d7) Solid_p[i - 1][j - 1] = 1;
					}
				}
			}
			else
			{
				if (j == 0)
				{
					if (Solid[i][j] == 1)
					{
						if ((rand() / double(RAND_MAX)) < p_d1) Solid_p[i + 1][j] = 1;
						if ((rand() / double(RAND_MAX)) < p_d2) Solid_p[i][j + 1] = 1;
						if ((rand() / double(RAND_MAX)) < p_d3) Solid_p[i - 1][j] = 1;
						if ((rand() / double(RAND_MAX)) < p_d5) Solid_p[i + 1][j + 1] = 1;
						if ((rand() / double(RAND_MAX)) < p_d6) Solid_p[i - 1][j + 1] = 1;
					}
				}
				else if (j == N - 1)
				{
					if (Solid[i][j] == 1)
					{
						if ((rand() / double(RAND_MAX)) < p_d1) Solid_p[i + 1][j] = 1;
						if ((rand() / double(RAND_MAX)) < p_d3) Solid_p[i - 1][j] = 1;
						if ((rand() / double(RAND_MAX)) < p_d4) Solid_p[i][j - 1] = 1;
						if ((rand() / double(RAND_MAX)) < p_d7) Solid_p[i - 1][j - 1] = 1;
						if ((rand() / double(RAND_MAX)) < p_d8) Solid_p[i + 1][j - 1] = 1;
					}
				}
				else
				{
					if (Solid[i][j] == 1)
					{
						if ((rand() / double(RAND_MAX)) < p_d1) Solid_p[i + 1][j] = 1;
						if ((rand() / double(RAND_MAX)) < p_d2) Solid_p[i][j + 1] = 1;
						if ((rand() / double(RAND_MAX)) < p_d3) Solid_p[i - 1][j] = 1;
						if ((rand() / double(RAND_MAX)) < p_d4) Solid_p[i][j - 1] = 1;
						if ((rand() / double(RAND_MAX)) < p_d5) Solid_p[i + 1][j + 1] = 1;
						if ((rand() / double(RAND_MAX)) < p_d6) Solid_p[i - 1][j + 1] = 1;
						if ((rand() / double(RAND_MAX)) < p_d7) Solid_p[i - 1][j - 1] = 1;
						if ((rand() / double(RAND_MAX)) < p_d8) Solid_p[i + 1][j - 1] = 1;
					}
				}
			}
		}
	}
	cout << "Neighbor generated" << endl;

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			int s_temp = Solid[i][j] + Solid_p[i][j];
			if (s_temp > 0) Solid[i][j] = 1;
		}
	}
	cout << "One time grow compeleted" << endl;
}

void output2tecplot()
{
	string out_buffer = "";

	ofstream outfile;
	string filename = "porous_2D.plt";
	string title = "2D porous media";
	outfile.open(filename);

	outfile << "TITLE = \"" << title << "\"" << endl;

	/// output solid position
	outfile << "VARIABLES = \"X\", \"Y\", \"Z\",\"value\" " << endl;
	outfile << "ZONE  t=\"solid\" I = " << N << ", J = " << N << ", K = " << 1 << ", F = point" << endl;
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			out_buffer += std::to_string(i) + "," + std::to_string(j) + ", 0, " + std::to_string(Solid[i][j]) + "\n";
		}
	}
	outfile << out_buffer;
	outfile.close();
	cout << "Output completed" << endl;
}

void output2gnuplot()
{
	string out_buffer = "";
	ofstream outfile;
	string filename = "porous.out";

	outfile.open(filename);
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			out_buffer += std::to_string(Solid[i][j]) + " ";
		}
		out_buffer += "\n";
	}
	outfile << out_buffer;
	outfile.close();
	cout << "Output completed" << endl;

	system("pause");
}

int main()
{
	float phi_p;

	/// Generate random seed
	srand((unsigned)time(NULL));
	cout << "Generate random seed completed" <<endl;

	/// Initialization
	init();
	cout << "Initialization completed" << endl;

	/// Generate growth core
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if ((rand() / double(RAND_MAX)) < p_cd)
			{
				Solid[i][j] = 1;
			}
		}
	}
	cout << "Generate growth core completed" << endl;

	///  Generate process
	do
	{
		grow();

		/// Calculate porosity
		float t_temp = 0;
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				t_temp += Solid[i][j];
			}
		}
		phi_p = 1 - t_temp / (N * N);
		cout << phi_p << endl;

	} while (phi_p > phi);
	cout << "Grow completed" << endl;

	/// Output result
	output2tecplot();

	system("pause");

	return 0;
}