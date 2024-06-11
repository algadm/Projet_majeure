
#include "ivec2.hpp"
#include "../common/error_handling.hpp"

#include <cmath>


namespace cpe
{

ivec2::ivec2()
    :x_data(0),y_data(0)
{}

ivec2::ivec2(int const x_param,int const y_param)
    :x_data(x_param),y_data(y_param)
{}

int ivec2::x() const
{
    return x_data;
}

int& ivec2::x()
{
    return x_data;
}

int ivec2::y() const
{
    return y_data;
}

int& ivec2::y()
{
    return y_data;
}



int ivec2::operator[](int const k) const
{
    ASSERT_CPE(k<2,"Index should be <2");
    ASSERT_CPE(k>=0,"Negative index");
    switch (k)
    {
    case 0:
        return x_data;
    case 1:
        return y_data;
    default:
        throw cpe::exception_cpe("Problem",EXCEPTION_PARAMETERS_CPE);
    };
}

int& ivec2::operator[](int const k)
{
    ASSERT_CPE(k<2,"Index should be <2");
    ASSERT_CPE(k>=0,"Negative index");
    switch (k)
    {
    case 0:
        return x_data;
    case 1:
        return y_data;
    default:
        throw cpe::exception_cpe("Problem",EXCEPTION_PARAMETERS_CPE);
    };
}

int ivec2::operator()(int const k) const
{
    return (*this)[k];
}
int& ivec2::operator()(int const k)
{
    return (*this)[k];
}



ivec2& operator+=(ivec2& lhs,ivec2 const& rhs)
{
    lhs.x() += rhs.x();
    lhs.y() += rhs.y();
    return lhs;

}

ivec2& operator-=(ivec2& lhs,ivec2 const& rhs)
{
    lhs.x() -= rhs.x();
    lhs.y() -= rhs.y();
    return lhs;
}

ivec2& operator*=(ivec2& v,int const s)
{
    v.x() *= s;
    v.y() *= s;
    return v;
}

ivec2& operator/=(ivec2& v,int const s)
{
    v.x() /= s;
    v.y() /= s;
    return v;
}

ivec2 operator+(ivec2 const& lhs,ivec2 const& rhs)
{
    ivec2 temp=lhs;
    temp+=rhs;
    return temp;
}

ivec2 operator-(ivec2 const& lhs,ivec2 const& rhs)
{
    ivec2 temp=lhs;
    temp-=rhs;
    return temp;
}

ivec2 operator*(ivec2 const& v,int const s)
{
    ivec2 temp=v;
    temp*=s;
    return temp;
}

ivec2 operator*(int s,ivec2 const& v)
{
    return v*s;
}

ivec2 operator/(ivec2 const& v,int const s)
{
    ivec2 temp=v;
    temp/=s;
    return temp;
}

ivec2 operator-(ivec2 const& v)
{
    return ivec2(-v.x(),-v.y());
}


std::ostream& operator<<(std::ostream& stream,ivec2 const& p)
{
    stream<<p.x()<<" "<<p.y();
    return stream;
}

const int *ivec2::pointer() const
{
    return &x_data;
}

bool operator==(ivec2 const& v1, ivec2 const& v2)
{
    return (v1.x() == v2.x() && v1.y() == v2.y());
}

bool operator!=(ivec2 const& v1, ivec2 const& v2)
{
    return !(v1 == v2);
}

}