#include "perlin_noise.h"

perlin::perlin()
{
    m_ranvec.reserve(point_count);
    m_perm_x.reserve(point_count);
    m_perm_y.reserve(point_count);
    m_perm_z.reserve(point_count);

    for (int i = 0; i < point_count; i++)
        m_ranvec.push_back(normalize(
            dvec3(random_double(-1, 1), 
                random_double(-1, 1), 
                random_double(-1, 1))));
    
    perlin_generate_perm(m_perm_x);
    perlin_generate_perm(m_perm_y);
    perlin_generate_perm(m_perm_z);
}

double perlin::noise(const dvec3& p) const
{
    auto u = p.x - floor(p.x);
    auto v = p.y - floor(p.y);
    auto w = p.z - floor(p.z);

    // Hermitian smoothing

    u = u*u*(3-2*u);
    v = v*v*(3-2*v);
    w = w*w*(3-2*w);

    auto i = static_cast<int>(floor(p.x));
    auto j = static_cast<int>(floor(p.y));
    auto k = static_cast<int>(floor(p.z));

    dvec3 c[2][2][2];

    for (int di = 0; di < 2; di++)
    {
        for (int dj = 0; dj < 2; dj++)
        {
            for (int dk = 0; dk < 2; dk++)
            {
                c[di][dj][dk] = m_ranvec[
                    m_perm_x[(i+di) & 255] ^
                    m_perm_y[(j+dj) & 255] ^
                    m_perm_z[(k+dk) & 255]
                ];
            }
        }
    }
    return perlin_interp(c, u, v, w);
}   

double perlin::turbulence(const dvec3& p, int depth) const
{
    auto accum = 0.0;
    auto temp_p = p;
    auto weight = 1.0;

    for (int i = 0; i < depth; i++)
    {
        accum += weight * noise(temp_p);
        weight *= 0.5;
        temp_p *= 2;
    }

    return fabs(accum);
}