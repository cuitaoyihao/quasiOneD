// Time Stepping Schemes

#include<vector>
#include<math.h>
#include<iostream>
#include "flux.h"
#include "convert.h"
#include "globals.h"

std::vector <long double> Flux(3 * (nx + 1), 0);
// Euler Explicit
void EulerExplicitStep(std::vector <long double> S,
                       std::vector <long double> V,
                       std::vector <long double> dt,
                       std::vector <long double> Q,
                       std::vector <long double> &Resi,
                       std::vector <long double> &W,
                       std::vector <long double> F)
{
    int ki, kip;
    getFlux(Flux, W, F);
    for(int k = 0; k < 3; k++)
    {
        for(int i = 1; i < nx - 1; i++)
        {
            ki = i * 3 + k;
            kip = (i + 1) * 3 + k;
            Resi[ki] = Flux[kip] * S[i + 1] - Flux[ki] * S[i] - Q[ki];
        }
        Resi[0 * 3 + k] = 0;
        Resi[(nx - 1) * 3 + k] = 0;
    }
    for(int k = 0; k < 3; k++)
    for(int i = 1; i < nx - 1; i++)
    {
        ki = i * 3 + k;
        W[ki] = W[ki] - (dt[i] / V[i]) * Resi[ki];
    }

    return;
}

std::vector <long double> Resi0(3 * nx, 0), Resi1(3 * nx, 0), Resi2(3 * nx, 0);
std::vector <long double> W1(3 * nx, 0), W2(3 * nx, 0), W3(3 * nx, 0);
std::vector <long double> F1(3 * nx, 0), F2(3 * nx, 0);
std::vector <long double> Q1(3 * nx, 0), Q2(3 * nx, 0);
std::vector <long double> utemp(nx), rhotemp(nx), ptemp(nx), ctemp(nx);
// 4th order Runge - Kutta Stepping Scheme
void rk4(std::vector <long double> dx, 
         std::vector <long double> S, 
         std::vector <long double> dt, 
         std::vector <long double> &W,
         std::vector <long double> F,
         std::vector <long double> Q,    
         std::vector <long double> &Resi)
{
    long double ki, kip;

    getFlux(Flux, W, F);
    // Residual 0
    for(int k = 0; k < 3; k++)
    {
        for(int i = 1; i < nx - 1; i++)
        {
            ki = i * 3 + k;
            kip = (i + 1) * 3 + k;
            Resi0[ki] = Flux[kip] * S[i + 1] - Flux[ki] * S[i] - Q[ki];
        }
    }
    // RK1
    for(int k = 0; k < 3; k++)
    {
        for(int i = 1; i < nx - 1; i++)
        {
            ki = i * 3 + k;
            W1[ki] = W[ki] - (dt[i] / 2) * Resi0[ki] / dx[i];
        }
        W1[0 * 3 + k] = W[0 * 3 + k];
        W1[(nx - 1) * 3 + k] = W[(nx - 1) * 3 + k];
    }
    for(int i = 0; i < nx; i++)
    {
        rhotemp[i] = W1[i * 3 + 0];
        utemp[i] = W1[i * 3 + 1] / rhotemp[i];
        ptemp[i] = (gam - 1) * (W1[i * 3 + 2] - rhotemp[i] * utemp[i] / 2);
        ctemp[i] = sqrt(gam * ptemp[i] / rhotemp[i]);

        Q1[i * 3 + 1] = ptemp[i] * (S[i + 1] - S[i]);
    }

    WtoF(W1, F1);
    getFlux(Flux, W1, F1);

    // Residual 1
    for(int k = 0; k < 3; k++)
    {
        for(int i = 1; i < nx - 1; i++)
        {
            ki = i * 3 + k;
            kip = (i + 1) * 3 + k;
            Resi1[ki] = Flux[kip] * S[i + 1] - Flux[ki] * S[i] - Q1[ki];
        }
    }

    // RK2
    for(int k = 0; k < 3; k++)
    {
        for(int i = 1; i < nx - 1; i++)
        {
            ki = i * 3 + k;
            W2[ki] = W[ki] - (dt[i] / 2) * Resi1[ki] / dx[i];
        }
        W2[0 * 3 + k] = W[0 * 3 + k];
        W2[(nx - 1) * 3 + k] = W[(nx - 1) * 3 + k];
    }
    for(int i = 0; i < nx; i++)
    {
        rhotemp[i] = W1[i * 3 + 0];
        utemp[i] = W1[i * 3 + 1] / rhotemp[i];
        ptemp[i] = (gam - 1) * (W1[i * 3 + 2] - rhotemp[i] * utemp[i] / 2);
        ctemp[i] = sqrt(gam * ptemp[i] / rhotemp[i]);

        Q2[i * 3 + 1] = ptemp[i] * (S[i + 1] - S[i]);
    }

    WtoF(W2, F2);
    getFlux(Flux, W2, F2);

    // Residual 2
    for(int k = 0; k < 3; k++)
    {
        for(int i = 1; i < nx - 1; i++)
        {
            ki = i * 3 + k;
            kip = (i + 1) * 3 + k;
            Resi2[ki] = Flux[kip] * S[i + 1] - Flux[ki] * S[i] - Q2[ki];
        }
    }

    // RK3
    for(int k = 0; k < 3; k++)
    {
        for(int i = 1; i < nx - 1; i++)
        {
            ki = i * 3 + k;
            W3[ki] = W[ki] - (dt[i] / 2) * Resi2[ki] / dx[i];
        }
    }

    for(int k = 0; k < 3; k++)
    {
        for(int i = 1; i < nx - 1; i++)
        {
            ki = i * 3 + k;
            W[ki] = ((long double)1.0 / 6.0) * (W[ki] + 2 * W1[ki] + 2 * W2[ki] + W3[ki]);
            Resi[ki] = (2 * Resi0[ki] + 2 * Resi1[ki] + Resi2[ki]) / 6.0;
        }
    }
}


std::vector <long double> Resitemp(3 * nx, 0);
std::vector <long double> Wtemp(3 * nx, 0);
std::vector <long double> Ftemp(3 * nx, 0);
std::vector <long double> Qtemp(3 * nx, 0);
// Jameson's 4th order Runge - Kutta Stepping Scheme
void jamesonrk(std::vector <long double> dx, 
         std::vector <long double> S,
         std::vector <long double> V,
         std::vector <long double> dt, 
         std::vector <long double> &W,
         std::vector <long double> F,
         std::vector <long double> &Resi)
{
    long double ki, kip;

    // Initialize First Stage
    for(int k = 0; k < 3; k++)
    {
        for(int i = 0; i < nx; i++)
        {
            ki = i * 3 + k;
            Wtemp[ki] = W[ki];
            Ftemp[ki] = F[ki];
        }
    }
    for(int i = 0; i < nx; i++)
    {
        rhotemp[i] = W[i * 3 + 0];
        utemp[i] = W[i * 3 + 1] / rhotemp[i];
        ptemp[i] = (gam - 1) * (W[i * 3 + 2] - rhotemp[i] * utemp[i] / 2);
        ctemp[i] = sqrt(gam * ptemp[i] / rhotemp[i]);

        Qtemp[i * 3 + 1] = ptemp[i] * (S[i + 1] - S[i]);
    }
    // 1-4 Stage
    for(int r = 1; r < 5; r++)
    {
        // Get Flux
        getFlux(Flux, Wtemp, Ftemp);
        // Calculate Residuals
        for(int k = 0; k < 3; k++)
        {
            for(int i = 1; i < nx - 1; i++)
            {
                ki = i * 3 + k;
                kip = (i + 1) * 3 + k;
                Resitemp[ki] = Flux[kip] * S[i + 1] - Flux[ki] * S[i] - Qtemp[ki];
            }
        }
        // Step in RK time
        for(int k = 0; k < 3; k++)
        {
            for(int i = 1; i < nx - 1; i++)
            {
                ki = i * 3 + k;
                Wtemp[ki] = W[ki] - (dt[i] / (5 - r)) * Resitemp[ki] / dx[i];
            }
            Wtemp[0 * 3 + k] = W[0 * 3 + k];
            Wtemp[(nx - 1) * 3 + k] = W[(nx - 1) * 3 + k];
        }
        // Calculate temporary variables
        for(int i = 0; i < nx; i++)
        {
            rhotemp[i] = Wtemp[i * 3 + 0];
            utemp[i] = Wtemp[i * 3 + 1] / rhotemp[i];
            ptemp[i] = (gam - 1) * (Wtemp[i * 3 + 2] - rhotemp[i] * utemp[i] / 2);
            ctemp[i] = sqrt(gam * ptemp[i] / rhotemp[i]);

            Qtemp[i * 3 + 1] = ptemp[i] * (S[i + 1] - S[i]);
        }
    }

    for(int k = 0; k < 3; k++)
    {
        for(int i = 1; i < nx - 1; i++)
        {
            ki = i * 3 + k;
            Resi[ki] = (Wtemp[ki] - W[ki]) * dt[i] / V[i];
            W[ki] = Wtemp[ki];
        }
    }
}
