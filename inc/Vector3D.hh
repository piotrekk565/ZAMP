#ifndef VECTOR3D_HH
#define VECTOR3D_HH

#include "geomVector.hh"

/*!
 * \file
 * \brief Deklaracja instacji szablonu geom::Vector
 */

 /*!
  * \brief Instacja szablonu geom::Vector<typename,int> dla przestrzeni 3D.
  */
 typedef geom::Vector<double,3>  Vector3D;

namespace proper_ostream {
    template<typename Type, unsigned int Size>
    inline
    std::ostream & operator << ( std::ostream &OStrm, const geom::Vector<Type,Size>  &V)
    {
        OStrm << V[0];
        for (unsigned int Ind = 1; Ind < Size; ++Ind) {
            OStrm << " " << V[Ind];
        }
        return OStrm;
    }
}

namespace server_ostream {
    template<typename Type, unsigned int Size>
    inline
    std::ostream & operator << ( std::ostream &OStrm, const geom::Vector<Type,Size>  &V)
    {
        OStrm << V[0];
        for (unsigned int Ind = 1; Ind < Size; ++Ind) {
            OStrm << "," << V[Ind];
        }
        return OStrm;
    }
}

#endif
