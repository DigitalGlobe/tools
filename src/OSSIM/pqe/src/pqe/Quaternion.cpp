// $Id$

#include <pqe/Quaternion.h>
#include <pqe/Matrix4x4.h>
#include <pqe/ColumnVector3d.h>

/// Good introductions to Quaternions at:
/// http://www.gamasutra.com/features/programming/19980703/quaternions_01.htm
/// http://mathworld.wolfram.com/Quaternion.html

void pqe::Quaternion::set(const pqe::Matrix4x4& matrix)
{
   matrix.getRotate(*this);
}

void pqe::Quaternion::get(pqe::Matrix4x4& matrix) const
{
   matrix.makeRotate(*this);
}

/// Set the elements of the Quat to represent a rotation of angle
/// (radians) around the axis (x,y,z)
void pqe::Quaternion::makeRotate( pqe_float64 angle,
                                  pqe_float64 x,
                                  pqe_float64 y,
                                  pqe_float64 z )
{
   
   const pqe_float64 epsilon = 0.0000001;
   
   pqe_float64 length = sqrt( x*x + y*y + z*z );
   if (length < epsilon)
   {
      // ~zero length axis, so reset rotation to zero.
      *this = pqe::Quaternion();
      return;
   }
   double tempAngle = pqe::degreesToRadians(angle);
   
   pqe_float64 inversenorm  = 1.0/length;
   pqe_float64 coshalfangle = cos( 0.5*tempAngle );
   pqe_float64 sinhalfangle = sin( 0.5*tempAngle );
   
   theVector[0] = x * sinhalfangle * inversenorm;
   theVector[1] = y * sinhalfangle * inversenorm;
   theVector[2] = z * sinhalfangle * inversenorm;
   theVector[3] = coshalfangle;
}

void pqe::Quaternion::makeRotate(
   pqe_float64 angle1, const pqe::ColumnVector3d& axis1, 
   pqe_float64 angle2, const pqe::ColumnVector3d& axis2,
   pqe_float64 angle3, const pqe::ColumnVector3d& axis3)
{
   pqe::Quaternion q1; q1.makeRotate(angle1,axis1);
   pqe::Quaternion q2; q2.makeRotate(angle2,axis2);
   pqe::Quaternion q3; q3.makeRotate(angle3,axis3);
   *this = q1*q2*q3;
}                        

void pqe::Quaternion::makeRotate (
   pqe_float64 angle1, const pqe::Dpt3d& axis1, 
   pqe_float64 angle2, const pqe::Dpt3d& axis2,
   pqe_float64 angle3, const pqe::Dpt3d& axis3)
{
   pqe::Quaternion q1; q1.makeRotate(angle1,axis1);
   pqe::Quaternion q2; q2.makeRotate(angle2,axis2);
   pqe::Quaternion q3; q3.makeRotate(angle3,axis3);

   *this = q1*q2*q3;
}                        

void pqe::Quaternion::makeRotate(
   const pqe::ColumnVector3d& vec1, const pqe::ColumnVector3d& vec2 )
{
   makeRotate(pqe::Dpt3d(vec1[0], vec1[1], vec1[2]),
              pqe::Dpt3d(vec2[0], vec2[1], vec2[2]));
}


// /** Make a rotation Quat which will rotate vec1 to vec2

// This routine uses only fast geometric transforms, without costly acos/sin computations. 
// It's exact, fast, and with less degenerate cases than the acos/sin method.

// For an explanation of the math used, you may see for example: 
// http://logiciels.cnes.fr/MARMOTTES/marmottes-mathematique.pdf

// @note This is the rotation with shortest angle, which is the one equivalent to the 
// acos/sin transform method. Other rotations exists, for example to additionally keep 
// a local horizontal attitude.

// @author Nicolas Brodu
// */
void pqe::Quaternion::makeRotate( const pqe::Dpt3d& from, const pqe::Dpt3d& to )
{

    // This routine takes any vector as argument but normalized 
    // vectors are necessary, if only for computing the dot product.
    // Too bad the API is that generic, it leads to performance loss.
    // Even in the case the 2 vectors are not normalized but same length,
    // the sqrt could be shared, but we have no way to know beforehand
    // at this point, while the caller may know.
    // So, we have to test... in the hope of saving at least a sqrt
    pqe::Dpt3d sourceVector = from;
    pqe::Dpt3d targetVector = to;
    
    pqe_float64 fromLen2 = from.length2();
    pqe_float64 fromLen = 1.0;
    // normalize only when necessary, epsilon test
    if ((fromLen2 < 1.0-1e-7) || (fromLen2 > 1.0+1e-7))
    {
        fromLen = sqrt(fromLen2);
        sourceVector /= fromLen;
    }
    
    pqe_float64 toLen2 = to.length2();
    // normalize only when necessary, epsilon test
    if ((toLen2 < 1.0-1e-7) || (toLen2 > 1.0+1e-7))
    {
        pqe_float64 toLen;
        // re-use fromLen for case of mapping 2 vectors of the same length
        if ((toLen2 > fromLen2-1e-7) && (toLen2 < fromLen2+1e-7))
        {
            toLen = fromLen;
        } 
        else toLen = sqrt(toLen2);
        targetVector /= toLen;
    }
    
    // Now let's get into the real stuff
    // Use "dot product plus one" as test as it can be re-used later on
    double dotProdPlus1 = 1.0 + sourceVector * targetVector;
    
    // Check for degenerate case of full u-turn. Use epsilon for detection
    if (dotProdPlus1 < 1e-7) {
    
        // Get an orthogonal vector of the given vector
        // in a plane with maximum vector coordinates.
        // Then use it as quaternion axis with pi angle
        // Trick is to realize one value at least is >0.6 for a normalized vector.
        if (fabs(sourceVector.x) < 0.6) {
            const double norm = sqrt(1.0 - sourceVector.x * sourceVector.x);
            theVector[0] = 0.0; 
            theVector[1] = sourceVector.z / norm;
            theVector[2] = -sourceVector.y / norm;
            theVector[3] = 0.0;
        } else if (fabs(sourceVector.y) < 0.6) {
            const double norm = sqrt(1.0 - sourceVector.y * sourceVector.y);
            theVector[0] = -sourceVector.z / norm;
            theVector[1] = 0.0;
            theVector[2] = sourceVector.x / norm;
            theVector[3] = 0.0;
        } else {
            const double norm = sqrt(1.0 - sourceVector.z * sourceVector.z);
            theVector[0] = sourceVector.y / norm;
            theVector[1] = -sourceVector.x / norm;
            theVector[2] = 0.0;
            theVector[3] = 0.0;
        }
    }
    
    else {
        // Find the shortest angle quaternion that transforms normalized vectors
        // into one other. Formula is still valid when vectors are colinear
        const double s = sqrt(0.5 * dotProdPlus1);
        const pqe::Dpt3d tmp = sourceVector ^ targetVector / (2.0*s);
        theVector[0] = tmp.x;
        theVector[1] = tmp.y;
        theVector[2] = tmp.z;
        theVector[3] = s;
    }
}


// Make a rotation Quat which will rotate vec1 to vec2
// Generally take adot product to get the angle between these
// and then use a cross product to get the rotation axis
// Watch out for the two special cases of when the vectors
// are co-incident or opposite in direction.
// void pqe::Quaternion::makeRotate_original( const Vec3d& from, const Vec3d& to )
// {
//     const pqe_float64 epsilon = 0.0000001;

//     pqe_float64 length1  = from.length();
//     pqe_float64 length2  = to.length();
    
//     // dot product vec1*vec2
//     pqe_float64 cosangle = from*to/(length1*length2);

//     if ( fabs(cosangle - 1) < epsilon )
//     {
//         osg::notify(osg::INFO)<<"*** pqe::Quaternion::makeRotate(from,to) with near co-linear vectors, epsilon= "<<fabs(cosangle-1)<<std::endl;
    
//         // cosangle is close to 1, so the vectors are close to being coincident
//         // Need to generate an angle of zero with any vector we like
//         // We'll choose (1,0,0)
//         makeRotate( 0.0, 0.0, 0.0, 1.0 );
//     }
//     else
//     if ( fabs(cosangle + 1.0) < epsilon )
//     {
//         // vectors are close to being opposite, so will need to find a
//         // vector orthongonal to from to rotate about.
//         Vec3d tmp;
//         if (fabs(from.x())<fabs(from.y()))
//             if (fabs(from.x())<fabs(from.z())) tmp.set(1.0,0.0,0.0); // use x axis.
//             else tmp.set(0.0,0.0,1.0);
//         else if (fabs(from.y())<fabs(from.z())) tmp.set(0.0,1.0,0.0);
//         else tmp.set(0.0,0.0,1.0);
        
//         Vec3d fromd(from.x(),from.y(),from.z());
        
//         // find orthogonal axis.
//         Vec3d axis(fromd^tmp);
//         axis.normalize();
        
//         theVector[0] = axis[0]; // sin of half angle of PI is 1.0.
//         theVector[1] = axis[1]; // sin of half angle of PI is 1.0.
//         theVector[2] = axis[2]; // sin of half angle of PI is 1.0.
//         theVector[3] = 0; // cos of half angle of PI is zero.

//     }
//     else
//     {
//         // This is the usual situation - take a cross-product of vec1 and vec2
//         // and that is the axis around which to rotate.
//         Vec3d axis(from^to);
//         pqe_float64 angle = acos( cosangle );
//         makeRotate( angle, axis );
//     }
// }

// void pqe::Quaternion::getRotate( pqe_float64& angle, Vec3f& vec ) const
// {
//     pqe_float64 x,y,z;
//     getRotate(angle,x,y,z);
//     vec[0]=x;
//     vec[1]=y;
//     vec[2]=z;
// }
// void pqe::Quaternion::getRotate( pqe_float64& angle, Vec3d& vec ) const
// {
//     pqe_float64 x,y,z;
//     getRotate(angle,x,y,z);
//     vec[0]=x;
//     vec[1]=y;
//     vec[2]=z;
// }


// Get the angle of rotation and axis of this Quat object.
// Won't give very meaningful results if the Quat is not associated
// with a rotation!
void pqe::Quaternion::getRotate( pqe_float64& angle, pqe_float64& x, pqe_float64& y, pqe_float64& z ) const
{
    pqe_float64 sinhalfangle = sqrt( theVector[0]*theVector[0] + theVector[1]*theVector[1] + theVector[2]*theVector[2] );

    angle = 2.0 * atan2( sinhalfangle, theVector[3] );
    if(sinhalfangle)
    {
        x = theVector[0] / sinhalfangle;
        y = theVector[1] / sinhalfangle;
        z = theVector[2] / sinhalfangle;
    }
    else
    {
        x = 0.0;
        y = 0.0;
        z = 1.0;
    }
    angle = pqe::radiansToDegrees(angle);
}

#if 0
/// Spherical Linear Interpolation
/// As t goes from 0 to 1, the Quat object goes from "from" to "to"
/// Reference: Shoemake at SIGGRAPH 89
/// See also
/// http://www.gamasutra.com/features/programming/19980703/quaternions_01.htm
void pqe::Quaternion::slerp( pqe_float64 t, const pqe::Quaternion& from, const pqe::Quaternion& to )
{
    const double epsilon = 0.00001;
    double omega, cosomega, sinomega, scale_from, scale_to ;
    
    osg::Quat quatTo(to);
    // this is a dot product
    
    cosomega = from.asVec4() * to.asVec4();
    
    if ( cosomega <0.0 )
    { 
        cosomega = -cosomega; 
        quatTo = -to;
    }

    if( (1.0 - cosomega) > epsilon )
    {
        omega= acos(cosomega) ;  // 0 <= omega <= Pi (see man acos)
        sinomega = sin(omega) ;  // this sinomega should always be +ve so
        // could try sinomega=sqrt(1-cosomega*cosomega) to avoid a sin()?
        scale_from = sin((1.0-t)*omega)/sinomega ;
        scale_to = sin(t*omega)/sinomega ;
    }
    else
    {
        /* --------------------------------------------------
           The ends of the vectors are very close
           we can use simple linear interpolation - no need
           to worry about the "spherical" interpolation
           -------------------------------------------------- */
        scale_from = 1.0 - t ;
        scale_to = t ;
    }

    *this = (from*scale_from) + (quatTo*scale_to);
    
    // so that we get a Vec4
}
#endif
