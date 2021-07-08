#include "Header.h"
#include "pch.h"
#include <iostream>
#include <fstream>


using namespace std;
int const l3aNX = 400;   /* chislo uzlov po x */
int const l3aNY = 100;   /* chislo uzlov po y */
int const l3aNxa = 80;   /* x koordinata tochki a */
int const l3aNya = 40;   /* y koordinata tochki a */
int const l3aNxb = 120;   /* x koordinata tochki b */
int const l3aNyb = 40;   /* y koordinata tochki b */
int const l3aNxc = 120;   /* x koordinata tochki c */
int const l3aNyc = 60;   /* y koordinata tochki c */
int const l3aNxd = 80;   /* x koordinata tochki d */
int const l3aNyd = 60;   /* y koordinata tochki d */

bool l3aStop;
int l3a_n;
int l3a_nn;


static long q;
static double f[l3aNX + 1][l3aNY + 1], Nd1[l3aNX + 1][l3aNY + 1], Nd2[l3aNX + 1][l3aNY + 1],
u1[l3aNX + 1][l3aNY + 1], u2[l3aNX + 1][l3aNY + 1], v1[l3aNX + 1][l3aNY + 1],
v2[l3aNX + 1][l3aNY + 1], Psi1[l3aNX + 1][l3aNY + 1], Psi12[l3aNX + 1][l3aNY + 1],
Psi2[l3aNX + 1][l3aNY + 1], Vi1[l3aNX + 1][l3aNY + 1], Vi12[l3aNX + 1][l3aNY + 1],
Vi2[l3aNX + 1][l3aNY + 1];
static long al[l3aNX + 1][l3aNY + 1], bl[l3aNX + 1][l3aNY + 1];
static double F0, reD, Re, rem1, beta1, beta2, gamma1, omega, t0, v0, x0, y0_,
dt, dt0, dx, dy, gamma, rs, dif, tt, sum1, sum2, nju, eps, lab3amax,
aa1, aa2, aa3;
/* p2c: lab5.pas, line 96: Warning: Symbol 'VARIANT' is not defined [221] */
static char n1[256];

void Reset()
{
	for (int i = 0; i <= l3aNX; i++) {
		for (int j = 0; j <= l3aNY; j++) {
			u1[i][j] = 1e-16;
			v1[i][j] = 1e-16;
			u2[i][j] = 1e-16;
			v2[i][j] = 1e-16;
			Psi1[i][j] = 1e-16;
			Psi2[i][j] = 1e-16;
			Vi1[i][j] = 1e-16;
			Vi2[i][j] = 1e-16;
			Nd1[i][j] = 1e-16;
			Nd2[i][j] = 1e-16;
		}
	}

	for (int i = 0; i <= l3aNX; i++) {
		for (int j = 0; j <= l3aNY; j++) {
			f[i][j] = exp(-gamma * dx * dx * ((i - l3aNxa + 60) * (i - l3aNxa + 60) +
				(j - l3aNY / 2) * (j - l3aNY / 2)));
		}
	}
}

void Reset1()
{

	x0 = 10;
	/* harakternyj masshtab */
	nju = 0.15;
	/* parametr kinematicheskoj vjazkosti */
	dif = 0;
	/* parametr diffusii */
	dt = 0.001;
	/* bezrazmernyj shag po vremeni */
	dx = 0.1;
	/* bezrazmernyj shag po koordinate */
	v0 = 10;
	/* harakternaja skorost' */
	rs = 10;

	eps = 1e-2;   /* dlja Puassona; */
	t0 = x0 / v0;   /* vremja t0 v sekundah */
	Re = x0 * v0 / nju;   /* parametr Rejnoldsa */
	rem1 = 1. / Re;   /* obratnaja velichina */
	reD = dif / (x0 * v0);   /* bezrazmernyj k-t diffusii */
	F0 = 1.0;   /* mocshnost' istochnika */
	gamma = x0 * x0 / (rs * rs);
	/* vyvod chisla Rejnoldsa */
	dy = dx;
	dt0 = dt * t0;
	//Label15.Caption:='0';
	l3a_n = 0;
	l3a_nn = 0;

	Reset();
}

void l3aCalculation1()

{
	/* Reshenie uravnenija Puassona dlja funkzii toka */
	/* Psi - funkzija toka */

	for (int i = 1; i < l3aNX; i++) {
		for (int j = 1; j < l3aNya; j++)
			Psi2[i][j] = 0.25 * (Psi1[i + 1][j] + Psi2[i - 1][j] + Psi1[i]
				[j + 1] + Psi2[i][j - 1]) - 0.25 * dx * dx * Vi1[i][j];
	}
	for (int i = 1; i < l3aNxa; i++) {
		for (int j = l3aNya; j <= l3aNyd; j++)
			Psi2[i][j] = 0.25 * (Psi1[i + 1][j] + Psi2[i - 1][j] + Psi1[i]
				[j + 1] + Psi2[i][j - 1]) - 0.25 * dx * dx * Vi1[i][j];
	}
	for (int i = l3aNxb + 1; i < l3aNX; i++) {
		for (int j = l3aNya; j <= l3aNyd; j++)
			Psi2[i][j] = 0.25 * (Psi1[i + 1][j] + Psi2[i - 1][j] + Psi1[i]
				[j + 1] + Psi2[i][j - 1]) - 0.25 * dx * dx * Vi1[i][j];
	}
	for (int i = 1; i < l3aNX; i++) {
		for (int j = l3aNyd + 1; j < l3aNY; j++)
			Psi2[i][j] = 0.25 * (Psi1[i + 1][j] + Psi2[i - 1][j] + Psi1[i]
				[j + 1] + Psi2[i][j - 1]) - 0.25 * dx * dx * Vi1[i][j];
	}
	for (int j = 0; j <= l3aNY; j++)   /* vhodnaja graniza */
		Psi2[0][j] = Psi2[1][j];
	for (int j = 0; j <= l3aNY; j++)   /* vyhodnaja graniza */
		Psi2[l3aNX][j] = Psi2[l3aNX - 1][j];
	for (int j = l3aNyb; j <= l3aNyc; j++)   /* pravaja graniza vystupa */
		Psi2[l3aNxb][j] = 1e-16;
	for (int j = l3aNya; j <= l3aNyd; j++)   /* levaja graniza vystupa */
		Psi2[l3aNxa][j] = 1e-16;
	for (int i = l3aNxa; i <= l3aNxb; i++)   /* nizhnjaja graniza vystupa */
		Psi2[i][l3aNya] = 1e-16;
	/* verhnjaja graniza vystupa */
	for (int i = l3aNxd; i <= l3aNxc; i++)
		Psi2[i][l3aNyd] = 1e-16;

	for (int i = 0; i <= l3aNX; i++)
		Psi2[i][0] = Psi2[i][1] - u1[i][0] * dy;   /* nizhnjaja graniza */
	for (int i = 0; i <= l3aNX; i++)
		Psi2[i][l3aNY] = Psi2[i][l3aNY - 1] + u1[i][l3aNY] * dy;   /* verhnjaja graniza */
}//end l3aCalculation1();

void l3aCalculation2()
{
	beta1 = dt / dx;
	beta2 = dt / dy;

	for (int i = 0; i <= l3aNX; i++) {
		/* proverka   dlja vybora approximazii proizvodnoj */
		for (int j = 0; j <= l3aNY; j++) {
			if (u1[i][j] >= 0)
				al[i][j] = 0;
			else
				al[i][j] = 1;
		}
	}
	for (int i = 0; i <= l3aNX; i++) {
		/* proverka   dlja vybora approximazii proizvodnoj */
		for (int j = 0; j <= l3aNY; j++) {
			if (v1[i][j] >= 0)
				bl[i][j] = 0;
			else
				bl[i][j] = 1;
		}
	}
	for (int i = 1; i < l3aNX; i++) {
		for (int j = 1; j < l3aNya; j++) {
			Vi2[i][j] = Vi1[i][j] - beta1 * u1[i][j] *
				((1 - al[i][j]) * (Vi1[i][j] - Vi1[i - 1][j]) +
					al[i][j] * (Vi1[i + 1][j] - Vi1[i][j])) - beta2 *
				v1[i][j] * ((1 - bl[i][j]) * (Vi1[i][j] - Vi1[i][j - 1]) +
					bl[i][j] * (Vi1[i][j + 1] - Vi1[i][j])) + rem1 * dt *
				(Vi1[i - 1][j] - 2 * Vi1[i][j] + Vi1[i + 1][j]) / (dx * dx) +
				rem1 * dt * (Vi1[i][j - 1] - 2 * Vi1[i][j] + Vi1[i][j + 1]) / (dy * dy);
		}
	}

	for (int i = 1; i < l3aNxa; i++) {
		for (int j = l3aNya; j <= l3aNyd; j++) {
			Vi2[i][j] = Vi1[i][j] - beta1 *
				u1[i][j] * ((1 - al[i][j]) * (Vi1[i][j] - Vi1[i - 1][j]) +
					al[i][j] * (Vi1[i + 1][j] - Vi1[i][j])) - beta2 *
				v1[i][j] * ((1 - bl[i][j]) * (Vi1[i][j] - Vi1[i][j - 1]) +
					bl[i][j] * (Vi1[i][j + 1] - Vi1[i][j])) + rem1 * dt *
				(Vi1[i - 1][j] - 2 * Vi1[i][j] + Vi1[i + 1][j]) / (dx * dx) +
				rem1 * dt * (Vi1[i][j - 1] - 2 * Vi1[i][j] + Vi1[i][j + 1]) / (dy * dy);
		}
	}
	for (int i = l3aNxb + 1; i < l3aNX; i++) {
		for (int j = l3aNya; j <= l3aNyd; j++) {
			Vi2[i][j] = Vi1[i][j] - beta1 *
				u1[i][j] * ((1 - al[i][j]) * (Vi1[i][j] - Vi1[i - 1][j]) +
					al[i][j] * (Vi1[i + 1][j] - Vi1[i][j])) - beta2 *
				v1[i][j] * ((1 - bl[i][j]) * (Vi1[i][j] - Vi1[i][j - 1]) +
					bl[i][j] * (Vi1[i][j + 1] - Vi1[i][j])) + rem1 * dt *
				(Vi1[i - 1][j] - 2 * Vi1[i][j] + Vi1[i + 1][j]) / (dx * dx) +
				rem1 * dt * (Vi1[i][j - 1] - 2 * Vi1[i][j] + Vi1[i][j + 1]) / (dy * dy);
		}
	}
	for (int i = 1; i < l3aNX; i++) {
		for (int j = l3aNyd + 1; j < l3aNY; j++) {
			Vi2[i][j] = Vi1[i][j] - beta1 *
				u1[i][j] * ((1 - al[i][j]) * (Vi1[i][j] - Vi1[i - 1][j]) +
					al[i][j] * (Vi1[i + 1][j] - Vi1[i][j])) - beta2 *
				v1[i][j] * ((1 - bl[i][j]) * (Vi1[i][j] - Vi1[i][j - 1]) +
					bl[i][j] * (Vi1[i][j + 1] - Vi1[i][j])) + rem1 * dt *
				(Vi1[i - 1][j] - 2 * Vi1[i][j] + Vi1[i + 1][j]) / (dx * dx) +
				rem1 * dt * (Vi1[i][j - 1] - 2 * Vi1[i][j] + Vi1[i][j + 1]) / (dy * dy);
		}
	}

	for (int j = 0; j <= l3aNY; j++)   /* vhodnaja graniza */
		Vi2[0][j] = Vi2[1][j];
	for (int j = l3aNya; j <= l3aNyd; j++)   /* levaja graniza kvadrata */
		Vi2[l3aNxa][j] = 2 * Psi2[l3aNxa - 1][j] / (dx * dx);
	for (int j = l3aNya; j <= l3aNyd; j++)   /* pravaja graniza kvadrata */
		Vi2[l3aNxb][j] = 2 * Psi2[l3aNxb + 1][j] / (dx * dx);
	for (int i = l3aNxa; i <= l3aNxb; i++)   /* nizhnjaja graniza kvadrata */
		Vi2[i][l3aNya] = 2 * Psi2[i][l3aNya - 1] / (dy * dy);
	/* verhnjaja graniza kvadrata */
	for (int i = l3aNxa; i <= l3aNxb; i++)
		Vi2[i][l3aNyd] = 2 * Psi2[i][l3aNyd + 1] / (dy * dy);
	for (int j = 0; j <= l3aNY; j++)   /* vyhodnaja graniza */
		Vi2[l3aNX][j] = Vi2[l3aNX - 1][j];

	Vi2[l3aNxa][l3aNya] = 2 * Psi2[l3aNxa][l3aNya - 1] / (dy * dy) + 2 * Psi2[l3aNxa - 1]
		[l3aNya] / (dx * dx);
	/* l3aNxa,l3aNya */
	Vi2[l3aNxd][l3aNyd] = 2 * Psi2[l3aNxd][l3aNyd + 1] / (dy * dy) + 2 * Psi2[l3aNxd - 1]
		[l3aNyd] / (dx * dx);
	/* l3aNxd,l3aNyd */
	Vi2[l3aNxa][l3aNya] = 2 * Psi2[l3aNxb][l3aNyb + 1] / (dy * dy) + 2 * Psi2[l3aNxb - 1]
		[l3aNyb] / (dx * dx);
	/* l3aNxb,l3aNyb */
	Vi2[l3aNxc][l3aNyc] = 2 * Psi2[l3aNxc][l3aNyc + 1] / (dy * dy) + 2 * Psi2[l3aNxc + 1]
		[l3aNyc] / (dx * dx);
	/* l3aNxc,l3aNyc */
	for (int i = 0; i <= l3aNX; i++)
		Vi2[i][0] = 2 * (Psi2[i][1] - Psi2[i][0] - dy) / (dy * dy);
	/* nizhnjaja graniza      (*dvizhuschajsja   stenka */
	for (int i = 0; i <= l3aNX; i++)
		Vi2[i][l3aNY] = 2 * (Psi2[i][l3aNY - 1] - Psi2[i][l3aNY] + dy) / (dy * dy);
	/* verhnjaja graniza         (*dvizhuschajsja   stenka */
}//end l3aCalculation2();

void l3aCalculation3() {
	/* Reshenie uravnenija dlja concentration javnym metodom */
	/* Nd - konzentrazija primesi */
	beta1 = dt / dx;
	beta2 = dt / dy;
	for (int i = 0; i <= l3aNX; i++)
	{   /* proverka   dlja vybora approximazii proizvodnoj */
		for (int j = 0; j <= l3aNY; j++) {
			if (u1[i][j] >= 0)
				al[i][j] = 0;
			else
				al[i][j] = 1;
		}
	}
	for (int i = 0; i <= l3aNX; i++)
	{   /* proverka   dlja vybora approximazii proizvodnoj */
		for (int j = 0; j <= l3aNY; j++) {
			if (v1[i][j] >= 0)
				bl[i][j] = 0;
			else
				bl[i][j] = 1;
		}
	}
	for (int i = 1; i < l3aNX; i++) {
		for (int j = 1; j < l3aNya; j++)
			Nd2[i][j] = Nd1[i][j] - beta1 * u1[i]
			[j] * ((1 - al[i][j]) * (Nd1[i][j] - Nd1[i - 1][j]) + al[i]
				[j] * (Nd1[i + 1][j] - Nd1[i][j])) - beta2 * v1[i]
			[j] * ((1 - bl[i][j]) * (Nd1[i][j] - Nd1[i][j - 1]) + bl[i]
				[j] * (Nd1[i][j + 1] - Nd1[i][j])) + reD * dt * (Nd1[i - 1]
					[j] - 2 * Nd1[i][j] + Nd1[i + 1]
					[j]) / (dx * dx) + reD * dt * (Nd1[i]
						[j - 1] - 2 * Nd1[i][j] + Nd1[i]
						[j + 1]) / (dy * dy) + dt * F0 * f[i][j];
	}

	for (int i = 1; i < l3aNxa; i++) {
		for (int j = l3aNya; j <= l3aNyd; j++)
			Nd2[i][j] = Nd1[i][j] - beta1 * u1[i]
			[j] * ((1 - al[i][j]) * (Nd1[i][j] - Nd1[i - 1][j]) + al[i]
				[j] * (Nd1[i + 1][j] - Nd1[i][j])) - beta2 * v1[i]
			[j] * ((1 - bl[i][j]) * (Nd1[i][j] - Nd1[i][j - 1]) + bl[i]
				[j] * (Nd1[i][j + 1] - Nd1[i][j])) + reD * dt * (Nd1[i - 1]
					[j] - 2 * Nd1[i][j] + Nd1[i + 1]
					[j]) / (dx * dx) + reD * dt * (Nd1[i]
						[j - 1] - 2 * Nd1[i][j] + Nd1[i]
						[j + 1]) / (dy * dy) + dt * F0 * f[i][j];
	}

	for (int i = l3aNxb + 1; i < l3aNX; i++) {
		for (int j = l3aNya; j <= l3aNyd; j++)
			Nd2[i][j] = Nd1[i][j] - beta1 * u1[i]
			[j] * ((1 - al[i][j]) * (Nd1[i][j] - Nd1[i - 1][j]) + al[i]
				[j] * (Nd1[i + 1][j] - Nd1[i][j])) - beta2 * v1[i]
			[j] * ((1 - bl[i][j]) * (Nd1[i][j] - Nd1[i][j - 1]) + bl[i]
				[j] * (Nd1[i][j + 1] - Nd1[i][j])) + reD * dt * (Nd1[i - 1]
					[j] - 2 * Nd1[i][j] + Nd1[i + 1]
					[j]) / (dx * dx) + reD * dt * (Nd1[i]
						[j - 1] - 2 * Nd1[i][j] + Nd1[i]
						[j + 1]) / (dy * dy) + dt * F0 * f[i][j];
	}
	for (int i = 1; i < l3aNX; i++) {
		for (int j = l3aNyd + 1; j < l3aNY; j++)
			Nd2[i][j] = Nd1[i][j] - beta1 * u1[i]
			[j] * ((1 - al[i][j]) * (Nd1[i][j] - Nd1[i - 1][j]) + al[i]
				[j] * (Nd1[i + 1][j] - Nd1[i][j])) - beta2 * v1[i]
			[j] * ((1 - bl[i][j]) * (Nd1[i][j] - Nd1[i][j - 1]) + bl[i]
				[j] * (Nd1[i][j + 1] - Nd1[i][j])) + reD * dt * (Nd1[i - 1]
					[j] - 2 * Nd1[i][j] + Nd1[i + 1]
					[j]) / (dx * dx) + reD * dt * (Nd1[i]
						[j - 1] - 2 * Nd1[i][j] + Nd1[i]
						[j + 1]) / (dy * dy) + dt * F0 * f[i][j];
	}
	for (int i = l3aNxa; i <= l3aNxb; i++) {   /* kvadrat */
		for (int j = l3aNya; j <= l3aNyd; j++) {
			Nd2[i][0] = 1e-16;
		}
	}

	for (int j = 0; j <= l3aNY; j++)   /* vhodnaja graniza */
		Nd2[0][j] = Nd2[1][j];
	for (int j = 0; j <= l3aNY; j++)   /* vyhodnaja graniza */
		Nd2[l3aNX][j] = Nd2[l3aNX - 1][j];
	for (int i = 0; i <= l3aNX; i++)   /* nizhnjaja graniza */
		Nd2[i][0] = 1e-16;

	for (int i = 0; i <= l3aNX; i++)
		Nd2[i][l3aNY] = 1e-16;   /*verhnjaja graniza */
}//end l3aCalculation3();

void Start1()
{
	do {   /* Cycle iterazij dlja uravnenija Puassona */
		l3aCalculation1();
		sum1 = 0.0;
		sum2 = 0.0;
		for (int i = 0; i <= l3aNX; i++) {
			for (int j = 0; j <= l3aNY; j++) {
				sum1 += Psi1[i][j];
				sum2 += Psi2[i][j];
			}
		}
		l3a_n++;
		for (int i = 0; i <= l3aNX; i++) {
			for (int j = 0; j <= l3aNY; j++) {
				Psi1[i][j] = Psi2[i][j];
			}
		}
		//sprintf(n1, "%ld", n);

	} while (fabs(sum2 - sum1) >= eps);
	l3aCalculation2();
	for (int i = 0; i < l3aNX; i++) {
		for (int j = 1; j < l3aNY; j++) {
			u2[i][j] = (Psi2[i][j + 1] - Psi2[i][j]) / dy;
			v2[i][j] = (Psi2[i][j] - Psi2[i + 1][j]) / dx;
		}
	}
	for (int i = l3aNxa; i <= l3aNxb; i++) {   /* kvadrat */
		for (int j = l3aNya; j <= l3aNyd; j++) {
			u2[i][j] = 1e-16;
			v2[i][j] = 1e-16;
		}
	}
	for (int i = 0; i <= l3aNX; i++) {
		u2[i][l3aNY] = 1.0;
		v2[i][l3aNY] = 1e-16;   /* verhnjaja graniza */
	}
	for (int i = 0; i <= l3aNX; i++) {
		u2[i][0] = 1.0;
		v2[i][0] = 1e-16;   /* nizhnjaja graniza */
	}
	l3aCalculation3();
	for (int i = 0; i <= l3aNX; i++) {
		for (int j = 0; j <= l3aNY; j++) {
			u1[i][j] = u2[i][j];
			v1[i][j] = v2[i][j];
			Vi1[i][j] = Vi2[i][j];
			Nd1[i][j] = Nd2[i][j];
		}
	}
}


void Fun(double* mass)
{
	Reset1();
	Start1();
	mass = &Nd1[0][0];
}


