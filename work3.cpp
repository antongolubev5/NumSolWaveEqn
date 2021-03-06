// work3.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <malloc.h>
#include <iomanip>
#include <cmath>
#include <functional>
#include <vector>

using namespace std;
auto M_PI = 3.14159265358979323846;
double eps = 1e-6,
a = 1,
h = 0.1,
tau = 0.1,
L = 1,
T = 1;
int N = L / h,
N0 = T / tau;


void printVector(double* Vector, int k)
{
	cout << endl;
	for (int i = 0; i < k; i++)
	{
		cout << Vector[i] /*<< " " << i+1 */ << endl;
	}

}

void printMatrix(double** extMatrix, int k, int m, bool extended)
{
	cout << endl;
	for (int i = 0; i < k; i++)
	{
		for (int j = 0; j < m; j++)
		{
			char simb = ' ';
			//if ((j == k - 1) && extended) { simb = '='; }
			//else { simb = ' '; }
			//cout <<  setprecision(2) << fixed<< extMatrix[i][j] << simb;
			cout << extMatrix[i][j] << simb;
		}
		cout << endl;
	}

}

void matrix_destroyer(double** ary, int n)
{
	if (ary != nullptr)
	{
		for (int i = 0; i < n; i++) {
			delete[] ary[i];
		}
		delete[] ary;
		ary = nullptr;
	}
}

void vector_destroyer(double* vec, int n)
{
	if (vec != nullptr)
	{

		delete[] vec;
		vec = nullptr;
	}
}

double* substractVector(double* Vector1, double* Vector2, int n)
{
	double *result;
	result = new double[n];

	for (int i = 0; i < n; i++)
	{
		result[i] = Vector1[i] - Vector2[i];
	}

	return result;
}

double* gaussmethod(double** extMatrix, int n)
{
	double *solution;
	solution = new double[n];
	double maxvalue = 0;
	int imax;

	for (int cnt = 0; cnt < n; cnt++)
	{
		solution[cnt] = 0;
	}
	/*double det = determinant(extMatrix, n);
	if (abs(det) < 1e-30)
	{
	cout << "Определитель равен 0. Не существует единственного решения." << endl;
	solution = nullptr;
	}
	else
	{*/

	for (int i = 0; i < n - 1; i++)//цикл по строкам, которые вычитаются из нижележащих
	{
		//выбор макс элемента из i-го столбца
		maxvalue = 0;
		for (int il = i; il < n; il++)
		{
			if (maxvalue < abs(extMatrix[il][i]))
			{
				maxvalue = abs(extMatrix[il][i]);
				imax = il;
			}
		}

		if (maxvalue < 1e-10)
		{
			cout << "Не существует единственного решения." << endl;
			return nullptr;
		}

		if (imax != i)
		{
			double* buf = extMatrix[imax];
			extMatrix[imax] = extMatrix[i];
			extMatrix[i] = buf;
		}

		//extMatrix[i][n] = extMatrix[i][n] / extMatrix[i][i];
		double aii = extMatrix[i][i];

		if (abs(aii) < 1e-10)
		{
			cout << "Не существует единственного решения. Последняя строка диагонализированной матрицы - нулевая" << endl;
			return nullptr;
		}

		for (int j = i; j <= n; j++)//цикл по элементам строками, которая вычитается из нижележащих  от i+1???
		{
			extMatrix[i][j] = extMatrix[i][j] / aii;
		}

		for (int ii = i + 1; ii < n; ii++)//вычитание из низлежащих строк i-ой строки
		{
			double a_ii_i = extMatrix[ii][i];
			for (int jj = i; jj <= n; jj++)
			{
				extMatrix[ii][jj] -= a_ii_i * extMatrix[i][jj];
			}
		}
	}
	//нормируем нижнюю строку
	double	 aii = extMatrix[n - 1][n - 1];
	if (abs(aii) < 1e-10)
	{
		cout << "Не существует единственного решения. Последняя строка диагонализированной матрицы - нулевая" << endl;
		return nullptr;
	}
	for (int j = n - 1; j <= n; j++)//цикл по элементам строками, которая вычитается из нижележащих  от i+1???
	{
		extMatrix[n - 1][j] = extMatrix[n - 1][j] / aii;
	}
	//printMatrix(extMatrix, n, n + 1, true);
	//обратный ход

	double sum = 0;
	for (int i = n - 1; i >= 0; i--)
	{
		sum = 0;
		for (int j = i + 1; j < n; j++) //суммируем все более старшие переменные  взвешенные на коэффициенты текущей строки
		{
			sum += solution[j] * extMatrix[i][j];
		}
		solution[i] = extMatrix[i][n] - sum;//вычитаем из правой части 
	}

	//printMatrix(extMatrix, n);//печать диагонализированной (для проверки)
	return solution;
}

double f_1(double x)
{
	return sin(M_PI*x);
}

double g_1(double x)
{
	return 0;
}

double phi_1(double t)
{
	return 0;
}

double xi_1(double t)
{
	return 0;
}

double fxx_1(double x)
{
	return -M_PI* M_PI*sin(M_PI*x);
}

double ExactSolution_1(double x, double t)
{
	return sin(M_PI*x)*cos(M_PI*t);
}

double f_2(double x)
{
	return x*(1-x);
}

double g_2(double x)
{
	return 0;
}

double phi_2(double t)
{
	return 0;
}

double xi_2(double t)
{
	return 0;
}

double fxx_2(double x)
{
	return -2;
}

double ExactSolution_2(double x, double t)
{
	double res = 0;
	double row_sum = 0;
	double k = 0.5*(pow(2 / M_PI / M_PI / eps, 0.5) - 1);
	for (int i = 0; i < (int) k; i++)
	{
		row_sum += sin((2*i+1)* M_PI*x)*cos((2 * i + 1)* M_PI*t) / (2 * i + 1) / (2 * i + 1) / (2 * i + 1);
	}

	res = 8 / M_PI / M_PI / M_PI * row_sum;
	return res;
}

double f_own(double x)
{
	return 2 * x*(x + 1) + 0.3;
}

double g_own(double x)
{
	return 2 * sin(x);
}

double phi_own(double t)
{
	return 0.3;
}

double xi_own(double t)
{
	return 4.3 + t;
}

double fxx_own(double x)
{
	return 4;
}

double f_test(double x)
{
	double res = 0;

	if ((x >= 0 && x < 0.4) || (x > 0.6 && x <= 1)) { res = 0; }
	if (x >= 0.4 && x <= 0.6) { res = 1; }
	else { res = 0; }
		return res;
}

double g_test(double x)
{
	return 0;
}

double phi_test(double x)
{
	return 0;
}

double xi_test(double x)
{
	return 0;
}

double fxx_test(double x)
{
	return 0;
}

double** CrossScheme(double h, double tau, double a, double(*f) (double), double(*g) (double), double(*phi) (double), double(*xi) (double), double(*fxx) (double), bool flag)
{
	double **solution;
	solution = new double*[N0 + 1];
	
	for (int ii = 0; ii < N0 + 1; ii++)
	{
		solution[ii] = new double[N + 1];
	}

	for (int i = 0; i < N + 1; i++)
	{
		for (int j = 0; j < N0 + 1; j++)
		{
			solution[j][i] = 0;
		}
	}

	double coeff = tau * tau*a*a / h / h;
	for (int j = 0; j < N0+1; j++)
	{
		solution[j][0] = phi(j*tau);//левый конец
		solution[j][N] = xi(j*tau);//правый конец

		if (j == 0)//нулевая строчка solution -  н у
		{
			for (int i = 0; i < N + 1; i++)
			{
				solution[j][i] = f(i*h);
			}

		}

		else if (j == 1)//первая строчка solution -  н у
		{
			for (int i = 0; i < N+1; i++)
			{
				if (flag = true)//аналитическая производная
				{
					solution[j][i] = solution[j - 1][i] + tau * g(i*h) + 0.5*a*a*tau*tau*fxx(i*h);
				}

				else
				{
					if (i == 0)
					{
						solution[j][i] = (1 / h/h*(f( 3 * h) - 2 * f(2 * h) + f(h)) - (2 / h/h)*(f(3 * h) - 3 * f(2 * h) + 3 * f(h) - f(0)));
					}

					if (i == N)
					{
						solution[j][i] = (1 / h/h*(f(h) - 2 * f((N - 1)*h) + f((N - 2)*h)) + (1 / h/h)*(f((N)*h) - 3 * f((N - 1)*h) + 3 * f((N - 2)*h) - f((N - 3)*h)));
					}

					if ((i != 0) && (i != N))
					{
						 solution[j][i] = solution[j - 1][i] + tau * g(i*h) + 0.5*a*a*tau*tau*(f(i*h - h) - 2 * f(i*h) + f(i*h + h)) / h / h; 
					}
				}
				
				//else //численная производная
				//{ solution[j][i] = solution[j - 1][i] + tau * g(i*h) + 0.5*a*a*tau*tau*(f(i*h - h) - 2 * f(i*h) + f(i*h + h)) / h / h; }
			}

			//printMatrix(solution, N0 + 1, N + 1, true);
		}

		else
		{//вычисляем по РС крест из предыдущих двух слоев
			for (int i = 1; i < N; i++)
			{
				solution[j][i] = coeff * (solution[j - 1][i + 1] - 2 * solution[j - 1][i] + solution[j - 1][i - 1]) + 2 * solution[j - 1][i] - solution[j - 2][i];
			}
			
		}

	}

	return solution;
}


int main()
{
	setlocale(LC_ALL, "rus");

	double **test; 
	double *testt;
	double *testx;
	testt = new double[N0 + 1];
	testx = new double[N + 1];
	
	for (int i = 0; i < N0 + 1; i++)
	{
		testt[i] = i * tau;
	}

	for (int i = 0; i < N + 1; i++)
	{
		testx[i] = i * h;
	}

	//printVector(testt, N0 + 1);
	//cout << endl;
	//printVector(testx, N + 1);

	//первая задача L=5, вторая задача - L=1
	//true - аналитическая производная, false - численная

	//test = CrossScheme(h, tau, a, f_1, g_1, phi_1, xi_1,fxx_1, true);
    //test = CrossScheme(h, tau, a, f_2, g_2, phi_2, xi_2, fxx_2, true);
	test = CrossScheme(h, tau, a, f_own, g_own, phi_own, xi_own, fxx_own, true);
	//test = CrossScheme(h, tau, a, f_1, g_1, phi_1, xi_1,fxx_1, false);
	//test = CrossScheme(h, tau, a, f_2, g_2, phi_2, xi_2, fxx_2, false);
	//test = CrossScheme(h, tau, a, f_own, g_own, phi_own, xi_own, fxx_own, false);
	//test = CrossScheme(h, tau, a, f_test, g_test, phi_test, xi_test, fxx_own, false);
	
	
	cout <<"Численное решение:" << endl;
	//printMatrix(test, N0 + 1, N + 1, false);
	cout << endl;

	ofstream fout_x;
	fout_x.open("task1x.txt");

	for (int j = 0; j < N + 1; j++)
	{
		fout_x << testx[j] << " ";
	}

	fout_x.close();

	ofstream fout_temperature;
	fout_temperature.open("task1.txt");

	for (int j = 0; j < N0 + 1; j++)
	{
		for (int i = 0; i < N + 1; i++)
		{
			fout_temperature << test[j][i] << " ";
		}
		fout_temperature << endl;
	}
	fout_temperature.close();

	ofstream fout_exactsolution;
	fout_exactsolution.open("task1_exact.txt");

	for (int j = 0; j < N0 + 1; j++)
	{
		for (int i = 0; i < N + 1; i++)
		{
			fout_exactsolution << ExactSolution_2(i*h, j*tau) << " ";
		}
		fout_exactsolution << endl;
	}
	fout_exactsolution.close();


	system("pause");
    return 0;
}

