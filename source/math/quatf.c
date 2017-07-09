










#include "../sys/inc.h"
#include "quatf.h"
//...

#if 0

inline void Quaternion::inverse()
{
	m_quat[0] = fx(0)-m_quat[0];
	m_quat[1] = fx(0)-m_quat[1];
	m_quat[2] = fx(0)-m_quat[2];
	m_quat[3] = fx(0)-m_quat[3];
}

void Quaternion::fromAngles( const fx *angles )
{
	fx angle;
	fx sr, sp, sy, cr, cp, cy;

	angle = angles[2]*0.5f;
	sy = sin( angle );
	cy = cos( angle );
	angle = angles[1]*0.5f;
	sp = sin( angle );
	cp = cos( angle );
	angle = angles[0]*0.5f;
	sr = sin( angle );
	cr = cos( angle );

	fx crcp = cr*cp;
	fx srsp = sr*sp;

	m_quat[0] = ( fx )( sr*cp*cy-cr*sp*sy );
	m_quat[1] = ( fx )( cr*sp*cy+sr*cp*sy );
	m_quat[2] = ( fx )( crcp*sy-srsp*cy );
	m_quat[3] = ( fx )( crcp*cy+srsp*sy );
}

void Quaternion::slerp( const Quaternion& q1, Quaternion& q2, fx interp )
{
	// Decide if one of the quaternions is backwards
	int32_t i;
	fx a = 0, b = 0;
	for ( i = 0; i < 4; i++ )
	{
		a += ( q1[i]-q2[i] )*( q1[i]-q2[i] );
		b += ( q1[i]+q2[i] )*( q1[i]+q2[i] );
	}
	if ( a > b )
		q2.inverse();

	fx cosom = q1[0]*q2[0]+q1[1]*q2[1]+q1[2]*q2[2]+q1[3]*q2[3];
	fx sclq1, sclq2;

	if (( fx(1.0)+cosom ) > ToFrac(0.00000001) )
	{
		if (( fx(1)-cosom ) > ToFrac(0.00000001) )
		{
			fx omega = acos( cosom );
			fx sinom = sin( omega );
			sclq1 = sin(( fx(1)-interp )*omega )/sinom;
			sclq2 = sin( interp*omega )/sinom;
		}
		else
		{
			sclq1 = fx(1)-interp;
			sclq2 = interp;
		}
		for ( i = 0; i < 4; i++ )
			m_quat[i] = ( fx )( sclq1*q1[i]+sclq2*q2[i] );
	}
	else
	{
		m_quat[0] = fx(0)-q1[1];
		m_quat[1] = q1[0];
		m_quat[2] = fx(0)-q1[3];
		m_quat[3] = q1[2];

		sclq1 = sin(( fx(1)-interp )*0.5*M_PI );
		sclq2 = sin( interp*0.5*M_PI );
		for ( i = 0; i < 3; i++ )
			m_quat[i] = ( fx )( sclq1*q1[i]+sclq2*m_quat[i] );
	}
}

void rotate_vecby_quaternion(const Vector3& v, const Quaternion& q, Vector3& vprime)
{
    // Extract the vector part of the quaternion
    Vector3 u(q.x, q.y, q.z);

    // Extract the scalar part of the quaternion
    float s = q.w;

    // Do the math
    vprime = 2.0f * dot(u, v) * u
          + (s*s - dot(u, u)) * v
          + 2.0f * s * cross3f(u, v);
}

quaternion normalize(quaternion quat)
{
  double L = length(quat);

  quat.x /= L;
  quat.y /= L;
  quat.z /= L;
  quat.w /= L;

  return quat;
}

quaternion conjugate(quaternion quat)
{
  quat.x = -quat.x;
  quat.y = -quat.y;
  quat.z = -quat.z;
  return quat;
}

quaternion mult(quaternion A, quaternion B)
{
  quaternion C;

  C.x = A.w*B.x + A.x*B.w + A.y*B.z - A.z*B.y;
  C.y = A.w*B.y - A.x*B.z + A.y*B.w + A.z*B.x;
  C.z = A.w*B.z + A.x*B.y - A.y*B.x + A.z*B.w;
  C.w = A.w*B.w - A.x*B.x - A.y*B.y - A.z*B.z;
  return C;
}

void RotateCamera(double Angle, double x, double y, double z)
{
  quaternion temp, quat_view, result;

  temp.x = x * sin(Angle/2);
  temp.y = y * sin(Angle/2);
  temp.z = z * sin(Angle/2);
  temp.w = cos(Angle/2);

  quat_view.x = View.x;
  quat_view.y = View.y;
  quat_view.z = View.z;
  quat_view.w = 0;

  result = mult(mult(temp, quat_view), conjugate(temp));

  View.x = result.x;
  View.y = result.y;
  View.z = result.z;
}

Quaternion to Axis Angle

To change a quaternion to a rotation around an arbitrary axis in 3D space, we do the following:

If the axis of rotation is         (ax, ay, az)
and the angle is                   theta (radians)
then the                           angle= 2 * acos(w)

           ax= x / scale
           ay= y / scale
           az= z / scale

where scale = sqrt (x2 + y2 + z2)

Another variation I have seen is that the scale = sin(acos(w)). They may be equivalent, though I didn't try to find the mathematical relationship behind them.

Anyway if the scale is 0, it means there is no rotation so unless you do something, the axis will be infinite. So whenever the scale is 0, just set the rotation axis to any unit vector with a rotation angle of 0.






Axis Angle to Quaternion

A rotation around an arbitrary axis in 3D space can be converted to a quaternion as follows

If the axis of rotation is        (ax, ay, az)- must be a unit vector
and the angle is                  theta (radians)

    w   =   cos(theta/2)
    x   =   ax * sin(theta/2)
    y   =   ay * sin(theta/2)
    z   =   az * sin(theta/2)


The axis must first be normalized. If the axis is a zero vector (meaning there is no rotation), the quaternion should be set to the rotation identity quaternion.







Euler to Quaternion

Converting from Euler angles to a quaternion is slightly more tricky, as the order of operations must be correct. Since you can convert the Euler angles to three independent quaternions by setting the arbitrary axis to the coordinate axes, you can then multiply the three quaternions together to obtain the final quaternion.

So if you have three Euler angles (a, b, c), then you can form three independent quaternions

Qx = [ cos(a/2), (sin(a/2), 0, 0)]
Qy = [ cos(b/2), (0, sin(b/2), 0)]
Qz = [ cos(c/2), (0, 0, sin(c/2))]


And the final quaternion is obtained by Qx * Qy * Qz.








	const quaternion operator +(const quaternion &q) const	
	{ return quaternion(s+q.s, v+q.v); }

	const quaternion operator -(const quaternion &q) const	
	{ return quaternion(s-q.s, v-q.v); }



	const quaternion operator *(const quaternion &q) const	
	{	return quaternion(s*q.s - v*q.v,
				  v.y*q.v.z - v.z*q.v.y + s*q.v.x + v.x*q.s,
				  v.z*q.v.x - v.x*q.v.z + s*q.v.y + v.y*q.s,
				  v.x*q.v.y - v.y*q.v.x + s*q.v.z + v.z*q.s);
	}

	const quaternion operator /(const quaternion &q) const	
	{
		quaternion p(q); 
		p.invert(); 
		return *this * p;
	}


	
	const quaternion operator *(float scale) const
	{ return quaternion(s*scale,v*scale); }

	const quaternion operator /(float scale) const
	{ return quaternion(s/scale,v/scale); }

	const quaternion operator -() const
	{ return quaternion(-s, -v); }
	
	const quaternion &operator +=(const quaternion &q)		
	{ v+=q.v; s+=q.s; return *this; }

	const quaternion &operator -=(const quaternion &q)		
	{ v-=q.v; s-=q.s; return *this; }



	const quaternion &operator *=(const quaternion &q)		
	{			
		float x= v.x, y= v.y, z= v.z, sn= s*q.s - v*q.v;
		v.x= y*q.v.z - z*q.v.y + s*q.v.x + x*q.s;
		v.y= z*q.v.x - x*q.v.z + s*q.v.y + y*q.s;
		v.z= x*q.v.y - y*q.v.x + s*q.v.z + z*q.s;
		s= sn;
		return *this;
	}
	
	const quaternion &operator *= (float scale)			
	{ v*=scale; s*=scale; return *this; }

	const quaternion &operator /= (float scale)			
	{ v/=scale; s/=scale; return *this; }
	

	//! gets the length of this quaternion
	float length() const
	{ return (float)sqrt(s*s + v*v); }

	//! gets the squared length of this quaternion
	float length_squared() const
	{ return (float)(s*s + v*v); }

	//! normalizes this quaternion
	void normalize()
	{ *this/=length(); }

	//! returns the normalized version of this quaternion
	quaternion normalized() const
	{ return  *this/length(); }

	//! computes the conjugate of this quaternion
	void conjugate()
	{ v=-v; }

	//! inverts this quaternion
	void invert()
	{ conjugate(); *this/=length_squared(); }


	//! returns the logarithm of a quaternion = v*a where q = [cos(a),v*sin(a)]
	quaternion log() const
	{
		float a = (float)acos(s);
		float sina = (float)sin(a);
		quaternion ret;

		ret.s = 0;
		if (sina > 0)
		{
			ret.v.x = a*v.x/sina;
			ret.v.y = a*v.y/sina;
			ret.v.z = a*v.z/sina;
		} else {
			ret.v.x= ret.v.y= ret.v.z= 0;
		}
		return ret;
	}



	//! returns e^quaternion = exp(v*a) = [cos(a),vsin(a)]
	quaternion exp() const
	{
		float a = (float)v.length();
		float sina = (float)sin(a);
		float cosa = (float)cos(a);
		quaternion ret;

		ret.s = cosa;
		if (a > 0)
		{
			ret.v.x = sina * v.x / a;
			ret.v.y = sina * v.y / a;
			ret.v.z = sina * v.z / a;
		} else {
			ret.v.x = ret.v.y = ret.v.z = 0;
		}
		return ret;
	}







	//! casting to a 4x4 isomorphic matrix for right multiplication with vector
	operator matrix4() const
	{			
		return matrix4(s,  -v.x, -v.y,-v.z,
				v.x,  s,  -v.z, v.y,
				v.y, v.z,    s,-v.x,
				v.z,-v.y,  v.x,   s);
	}


	
	//! casting to 3x3 rotation matrix
	operator matrix3() const
	{
		Assert(length() > 0.9999 && length() < 1.0001, "quaternion is not normalized");		
		return matrix3(1-2*(v.y*v.y+v.z*v.z), 2*(v.x*v.y-s*v.z),   2*(v.x*v.z+s*v.y),   
				2*(v.x*v.y+s*v.z),   1-2*(v.x*v.x+v.z*v.z), 2*(v.y*v.z-s*v.x),   
				2*(v.x*v.z-s*v.y),   2*(v.y*v.z+s*v.x),   1-2*(v.x*v.x+v.y*v.y));
	}





	
	//! computes the dot product of 2 quaternions
	static inline float dot(const quaternion &q1, const quaternion &q2) 
	{ return q1.v*q2.v + q1.s*q2.s; }

	//! linear quaternion interpolation
	static quaternion lerp(const quaternion &q1, const quaternion &q2, float t) 
	{ return (q1*(1-t) + q2*t).normalized(); }

	//! spherical linear interpolation
	static quaternion slerp(const quaternion &q1, const quaternion &q2, float t) 
	{
		quaternion q3;
		float dot = quaternion::dot(q1, q2);

		/*	dot = cos(theta)
			if (dot < 0), q1 and q2 are more than 90 degrees apart,
			so we can invert one to reduce spinning	*/
		if (dot < 0)
		{
			dot = -dot;
			q3 = -q2;
		} else q3 = q2;
		
		if (dot < 0.95f)
		{
			float angle = acosf(dot);
			return (q1*sinf(angle*(1-t)) + q3*sinf(angle*t))/sinf(angle);
		} else // if the angle is small, use linear interpolation								
			return lerp(q1,q3,t);		
	}

	//! This version of slerp, used by squad, does not check for theta > 90.
	static quaternion slerpNoInvert(const quaternion &q1, const quaternion &q2, float t) 
	{
		float dot = quaternion::dot(q1, q2);

		if (dot > -0.95f && dot < 0.95f)
		{
			float angle = acosf(dot);			
			return (q1*sinf(angle*(1-t)) + q2*sinf(angle*t))/sinf(angle);
		} else  // if the angle is small, use linear interpolation								
			return lerp(q1,q2,t);			
	}

	//! spherical cubic interpolation
	static quaternion squad(const quaternion &q1,const quaternion &q2,const quaternion &a,const quaternion &b,float t)
	{
		quaternion c= slerpNoInvert(q1,q2,t),
			       d= slerpNoInvert(a,b,t);		
		return slerpNoInvert(c,d,2*t*(1-t));
	}

	//! Shoemake-Bezier interpolation using De Castlejau algorithm
	static quaternion bezier(const quaternion &q1,const quaternion &q2,const quaternion &a,const quaternion &b,float t)
	{
		// level 1
		quaternion q11= slerpNoInvert(q1,a,t),
				q12= slerpNoInvert(a,b,t),
				q13= slerpNoInvert(b,q2,t);		
		// level 2 and 3
		return slerpNoInvert(slerpNoInvert(q11,q12,t), slerpNoInvert(q12,q13,t), t);
	}

	//! Given 3 quaternions, qn-1,qn and qn+1, calculate a control point to be used in spline interpolation
	static quaternion spline(const quaternion &qnm1,const quaternion &qn,const quaternion &qnp1)
	{
		quaternion qni(qn.s, -qn.v);	
		return qn * (( (qni*qnm1).log()+(qni*qnp1).log() )/-4).exp();
	}

	//! converts from a normalized axis - angle pair rotation to a quaternion
	static inline quaternion from_axis_angle(const vector3f &axis, float angle)
	{ return quaternion(cosf(angle/2), axis*sinf(angle/2)); }

	//! returns the axis and angle of this unit quaternion
	void to_axis_angle(vector3f &axis, float &angle) const
	{
		angle = acosf(s);

		// pre-compute to save time
		float sinf_theta_inv = 1.0/sinf(angle);

		// now the vector
		axis.x = v.x*sinf_theta_inv;
		axis.y = v.y*sinf_theta_inv;
		axis.z = v.z*sinf_theta_inv;

		// multiply by 2
		angle*=2;
	}

	//! rotates v by this quaternion (quaternion must be unit)
	vector3f rotate(const vector3f &v)
	{   
		quaternion V(0, v);
		quaternion conjugate(*this);
		conjugate.conjugate();
		return (*this * V * conjugate).v;
	}

	//! returns the euler angles from a rotation quaternion
	vector3f euler_angles(bool homogenous=true) const
	{
		float sqw = s*s;    
		float sqx = v.x*v.x;    
		float sqy = v.y*v.y;    
		float sqz = v.z*v.z;    

		vector3f euler;
		if (homogenous) {
			euler.x = atan2f(2.f * (v.x*v.y + v.z*s), sqx - sqy - sqz + sqw);    		
			euler.y = asinf(-2.f * (v.x*v.z - v.y*s));
			euler.z = atan2f(2.f * (v.y*v.z + v.x*s), -sqx - sqy + sqz + sqw);    
		} else {
			euler.x = atan2f(2.f * (v.z*v.y + v.x*s), 1 - 2*(sqx + sqy));
			euler.y = asinf(-2.f * (v.x*v.z - v.y*s));
			euler.z = atan2f(2.f * (v.x*v.y + v.z*s), 1 - 2*(sqy + sqz));
		}
		return euler;
	}


I think there are some problem in computing euler angle from quaternion. I changed it to code below and it worked. All came from wikipedia page:
http://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles

euler.x = atan2f(2.f * (v.z*v.y + v.x*s), 1 - 2* (sqx + sqy));
euler.y = asinf(-2.f * (v.x*v.z - v.y*s));
euler.z = atan2f(2.f * (v.x*v.y + v.z*s), 1 - 2* (sqy + sqz));






should not exp(q) also multiply by exp(Re(q)) ?

I beleive for 
q = [s, v], were s= Re(q) and v = Im(q)
the formula is:
exp(q) = exp(s)*( cos(|v|) + sin(|v|)*v/|v|);






AxisAngle to Quaternion

qx = ax * sin(angle/2)
qy = ay * sin(angle/2)
qz = az * sin(angle/2)
qw = cos(angle/2)

where:

the axis is normalised so: ax*ax + ay*ay + az*az = 1
the quaternion is also normalised so cos(angle/2)2 + ax*ax * sin(angle/2)2 + ay*ay * sin(angle/2)2+ az*az * sin(angle/2)2 = 1



Quaternion to AxisAngle

angle = 2 * acos(qw)
x = qx / sqrt(1-qw*qw)
y = qy / sqrt(1-qw*qw)
z = qz / sqrt(1-qw*qw)

Singularities

Axis angle has two singularities at angle = 0 degrees and angle = 180 degrees, so I think that it is a good precaution to check that that the above formula works in these cases. At 0 degrees the axis is arbitrary (any axis will produce the same result), at 180 degrees the axis is still relevant so we have to calculate it.

As explained here the formula for quaternoin in terms of axis angle is:

q = cos(angle/2) + i ( x * sin(angle/2)) + j (y * sin(angle/2)) + k ( z * sin(angle/2))

at angle = 0 degrees

q = 1 + i 0 + j 0 + k 0

so working back from above equation qw = 1 so :

angle = 2 * acos(qw) = 0
x = qx / sqrt(1-qw*qw) = divide by zero = infinity
y = qy / sqrt(1-qw*qw) = divide by zero = infinity
z = qz / sqrt(1-qw*qw) = divide by zero = infinity

So we have to test for divide by zero, but this is not a problem since the axis can be set to any arbitary value provided that it is normalised.

at angle = 180 degrees

q = 0 + i x + j y + k z

so working back from above equation qw = 0 so :

angle = 2 * acos(qw) = 2 * 90 degrees = 180 degrees (or -180 degrees which is equivalent)
x = qx / sqrt(1-qw*qw) = qx
y = qy / sqrt(1-qw*qw) = qy
z = qz / sqrt(1-qw*qw) = qw

Which is correct so the formula works properly in this case. Although some axis angle calculations can jump suddenly around 180 degrees, this quaternion to axis-angle translation seems quite smooth at this region.

Code

Java code to do conversion:

 public void set(Quat4d q1) {
   if (q1.w > 1) q1.normalise(); // if w>1 acos and sqrt will produce errors, this cant happen if quaternion is normalised
   angle = 2 * Math.acos(q1.w);
   double s = Math.sqrt(1-q1.w*q1.w); // assuming quaternion normalised then w is less than 1, so term always positive.
   if (s < 0.001) { // test to avoid divide by zero, s is always positive due to sqrt
     // if s close to zero then direction of axis not important
     x = q1.x; // if it is important that axis is normalised then replace with x=1; y=z=0;
     y = q1.y;
     z = q1.z;
   } else {
     x = q1.x / s; // normalise axis
     y = q1.y / s;
     z = q1.z / s;
   }
}



 inline QMatrix4x4 quatToMat(QQuaternion q)
{
    //based on algorithm on wikipedia
    // http://en.wikipedia.org/wiki/Rotation_matrix#Quaternion
    float w = q.scalar ();
    float x = q.x();
    float y = q.y();
    float z = q.z();

    float n = q.lengthSquared();
    float s =  n == 0?  0 : 2 / n;
    float wx = s * w * x, wy = s * w * y, wz = s * w * z;
    float xx = s * x * x, xy = s * x * y, xz = s * x * z;
    float yy = s * y * y, yz = s * y * z, zz = s * z * z;

    float m[16] = { 1 - (yy + zz),         xy + wz ,         xz - wy ,0,
                         xy - wz ,    1 - (xx + zz),         yz + wx ,0,
                         xz + wy ,         yz - wx ,    1 - (xx + yy),0,
                               0 ,               0 ,               0 ,1  };
    QMatrix4x4 result =  QMatrix4x4(m,4,4);
    result.optimize ();
    return result;
}





#endif

