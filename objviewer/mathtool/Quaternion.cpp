/********************************************************************

    Quaternion.cpp    Source File
    
*********************************************************************/
#include "Quaternion.h"

namespace mathtool{

    istream& operator>>(istream & in, Quaternion & q )
    {
        return in;
    }

    ostream& operator<<(ostream & out, const Quaternion & q )
    {
        return out;
    }

    Quaternion operator*(const Vector3d & v, const Quaternion & q2)
    {
        Quaternion q1(0,v);
        return q1*q2;
    }
}
