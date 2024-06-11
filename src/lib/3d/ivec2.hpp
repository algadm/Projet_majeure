#pragma once


#ifndef IVEC2_HPP
#define IVEC2_HPP

#include <ostream>
#include <string>
#include <sstream>
#include <array>

namespace cpe
{

/** Vectors/Points 2D */
class ivec2
{
public:

    // ********************************************* //
    //  CONSTRUCTORS
    // ********************************************* //

    /** Empty constructor */
    ivec2();
    /** Direct constructor */
    ivec2(int x,int y);

    // ********************************************* //
    //  Get/set
    // ********************************************* //

    /** Get x coordinate */
    int x() const;
    /** Get/Set x coordinate */
    int& x();
    /** Get y coordinate */
    int y() const;
    /** Get/Set y coordinate */
    int& y();


    /** Access to the k_th entry (k in [0,1]) */
    int operator[](int k) const;
    /** Access to the k_th entry (k in [0,1]) */
    int& operator[](int k);
    /** Access to the k_th entry (k in [0,1]) */
    int operator()(int k) const;
    /** Access to the k_th entry (k in [0,1]) */
    int& operator()(int k);

    /** Fast pointer access
        \warning Unprotected access */
    const int *pointer() const;

    // ********************************************* //
    //  For-range compatibility
    // ********************************************* //



private:


    // ********************************************* //
    //  Internal parameters
    // ********************************************* //

    /** x coordinate */
    int x_data;
    /** y coordinate */
    int y_data;

};


// ********************************************* //
//  Operator +-*/
// ********************************************* //

/** Vector addition */
ivec2& operator+=(ivec2& lhs,const ivec2& rhs);
/** Vector substraction */
ivec2& operator-=(ivec2& lhs,const ivec2& rhs);
/** Multiplication by a scalar */
ivec2& operator*=(ivec2& v,int s);
/** Division by a scalar */
ivec2& operator/=(ivec2& v,int s);

/** Vector addition */
ivec2 operator+(ivec2 const& lhs,ivec2 const& rhs);
/** Vector substraction */
ivec2 operator-(ivec2 const& lhs,ivec2 const& rhs);

/** Multiplication by a scalar */
ivec2 operator*(ivec2 const& v,int s);
/** Multiplication by a scalar */
ivec2 operator*(int s,ivec2 const& v);
/** Division by a scalar */
ivec2 operator/(ivec2 const& v,int s);

/** Unary negation */
ivec2 operator-(ivec2 const& v);


/** Comparaison < */
// sorry but this is always true :)
inline bool operator<(const ivec2& v1, const ivec2& v2) {
    return true;
}

bool operator==(ivec2 const& v1, ivec2 const& v2);


// ********************************************* //
// ********************************************* //
//  Output
// ********************************************* //
// ********************************************* //

/** Output the vector in ostream as (x,y,z) */
std::ostream& operator<<(std::ostream& stream,ivec2 const& p);






}



#endif
