#include "material.h"
#include "../textures/solid_color.h"

class diffuse_light : public Material
{
public:
    shared_ptr<texture> m_emit;

public:
    diffuse_light(Color a) : m_emit(make_shared<solid_color>(a)) {}
    diffuse_light(shared_ptr<texture> a) : m_emit(a) {}

    virtual bool scatter(
        const dvec3& w_in, const hit_record& rec, dvec3& f_r, dvec3& w_out, double& pdf 
    ) const override
    {
        // Emitting material doesn't scatter.
        return false;
    }

    virtual Color emitted(double u, double v, const dvec3& p) const override
    {
        // std::cout << "emit" << std::endl;
        return m_emit->value(u, v, p);
    }
};