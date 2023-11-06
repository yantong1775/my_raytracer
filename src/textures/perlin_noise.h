#pragma once

#include "../util.h"

class perlin
{
private:
    static const int point_count = 256;
    std::vector<dvec3> m_ranvec;
    std::vector<int> m_perm_x;
    std::vector<int> m_perm_y;
    std::vector<int> m_perm_z;

    static void permute(std::vector<int>& p, int n)
    {
        for (int i = n - 1; i > 0; i--)
        {
            int target = random_int(0, i);
            std::swap(p[i], p[target]);

        }
    }

    static void perlin_generate_perm(std::vector<int>& p)
    {
        for (int i = 0; i < point_count; i++)
            p.push_back(i);

        permute(p, point_count);
    }

    static double trilinear_interp(double c[2][2][2], double u, double v, double w) 
    {
        auto accum = 0.0;
        for (int i=0; i < 2; i++)
            for (int j=0; j < 2; j++)
                for (int k=0; k < 2; k++)
                    accum += (i*u + (1-i)*(1-u))*
                            (j*v + (1-j)*(1-v))*
                            (k*w + (1-k)*(1-w))*c[i][j][k];

        return accum;
    }

    static double perlin_interp(dvec3 c[2][2][2], double u, double v, double w)
    {
        auto accum = 0.0;
        for (int i=0; i < 2; i++)
            for (int j=0; j < 2; j++)
                for (int k=0; k < 2; k++)
                {
                    dvec3 weight_v(u-i, v-j, w-k);
                    accum += (i*u + (1-i)*(1-u))*
                            (j*v + (1-j)*(1-v))*
                            (k*w + (1-k)*(1-w))*dot(c[i][j][k], weight_v);
                }
        return accum;
    }

public:
    perlin();

    double noise(const dvec3& p) const;

    double turbulence(const dvec3& p, int depth=7) const;
};

