#ifndef __ES_MATH_H
#define __ES_MATH_H

#include <string.h>
#include <math.h>

#define _RAD(x)	x*3.1415926f/180.0f
#define _SWAP(x, y) x ^= y, y ^= x, x ^= y

#define FLOAT float

#define INT int 
#define UINT unsigned int

#define CHAR char
#define UCHAR unsigned char


namespace esm
{

	template<typename T>
	void swap(T & x, T & y)
	{
		T temp = x;
		x = y;
		y = temp;
	}

	namespace
	{

		FLOAT sin(FLOAT deg)
		{
			return sinf(_RAD(deg));
		}
		FLOAT cos(FLOAT deg)
		{
			return cosf(_RAD(deg));
		}
		FLOAT tan(FLOAT deg)
		{
			return tanf(_RAD(deg));
		}
	}


	class vec3
	{
	public:
		vec3()
		{
			x = y = z = 0.0f;
		}
		vec3(FLOAT f)
		{
			x = y = z = f;
		}
		vec3(FLOAT x, FLOAT y, FLOAT z)
		{
			v[0] = x; 
			v[1] = y; 
			v[2] = z;
		}
		vec3(const vec3 & v)
		{
			x = v.x;
			y = v.y;
			z = v.z;
		}
		vec3(const FLOAT * ptr)
		{
			x = *ptr;
			y = *(ptr + 1);
			z = *(ptr + 2);
		}


		void operator=(const vec3 & v)
		{
			x = v.x, y = v.y, z = v.z;
		}
		BOOL operator==(const vec3 & v)
		{
			if (x == v.x && y == v.y && z == v.z)
				return TRUE;
			return FALSE;
		}
		void operator=(const FLOAT r)
		{
			x = r, y = r, z = r;
		}

		vec3 operator+(const vec3 & v)
		{
			return vec3(x + v.x, y + v.y, z + v.z);
		}

		vec3 operator+(const FLOAT & v)
		{
			return vec3(x + v, y + v, z + v);
		}

		vec3 operator-(const vec3 & v)
		{
			return vec3(x - v.x, y - v.y, z - v.z);
		}

		vec3 operator-(const FLOAT & r)
		{
			return vec3(x - r, y - r, z - r);
		}

		vec3 operator*(const FLOAT & r)
		{
			return vec3(x * r, y * r, z * r);
		}

		vec3 operator/(const FLOAT & r)
		{
			return vec3(x / r, y / r, z / r);
		}

		FLOAT operator*(const vec3 & r)
		{
			return x * r.x + y * r.y + z * r.z;
		}

		vec3 cross(vec3 & r)
		{
			return vec3(y*r.z - r.y*z, z*r.x - r.z*x, x*r.y - r.x*y);
		}

		FLOAT magnitude()
		{
			return sqrtf(x*x + y * y + z * z);
		}

		FLOAT length()
		{
			return magnitude();
		}

		vec3 normalize()
		{
			FLOAT len = magnitude();
			if (len > 0.0000001f)
			{
				x /= len;
				y /= len;
				z /= len;
			}
			else
			{
				x = 0.0f;
				y = 0.0f;
				z = 0.0f;
			}

			return *this;
		}

		char* dump()
		{
			char* str = new char[128];
			sprintf(str, "x : %f, y : %f, z : %f\0", x, y, z);
			return str;
		}

		union
		{
			struct
			{
				FLOAT x, y, z;
			};
			struct
			{
				FLOAT r, g, b;
			};
			struct
			{
				FLOAT v[3];
			};
			struct
			{
				FLOAT mData[3];
			};
		};
	};


	class vec4
	{
	public:
		vec4()
		{
			x = y = z = w = 0.0f;
		}
		vec4(FLOAT f)
		{
			x = y = z = f; 
			w = 1.0f;
		}
		vec4(FLOAT x, FLOAT y, FLOAT z, FLOAT w = 1.0f)
		{
			v[0] = x; 
			v[1] = y; 
			v[2] = z; 
			v[3] = w;
		}
		vec4(const vec3 & v)
		{
			x = v.x;
			y = v.y;
			z = v.z;
			w = 1.0f;
		}
		vec4(const vec4 & v)
		{
			x = v.x;
			y = v.y;
			z = v.z;
			w = v.w;
		}


		void operator=(const vec4 & v)
		{
			x = v.x; y = v.y; z = v.z; w = v.w;
		}
		BOOL operator==(const vec4 & v)
		{
			if (x == v.x && y == v.y && z == v.z && w == v.w)
				return TRUE;
			return FALSE;
		}
		void operator=(const FLOAT r)
		{
			x = r, y = r, z = r, w = 1.0f;
		}

		vec4 operator+(const vec4 & v)
		{
			return vec4(x + v.x, y + v.y, z + v.z, w + v.w);
		}

		vec4 operator+(const FLOAT & v)
		{
			return vec4(x + v, y + v, z + v, w + v);
		}

		vec4 operator-(const vec4 & v)
		{
			return vec4(x - v.x, y - v.y, z - v.z, w - v.w);
		}

		vec4 operator-(const FLOAT & v)
		{
			return vec4(x - v, y - v, z - v, w - v);
		}

		vec4 operator*(const FLOAT & v)
		{
			return vec4(x * v, y * v, z * v, w * v);
		}

		vec4 operator/(const FLOAT & v)
		{
			return vec4(x / v, y / v, z / v, w / v);
		}

		FLOAT operator*(const vec4 & v)
		{
			return (x / w) * (v.x / v.w) + (y / w) * (v.y / v.w) + (z / w) * (v.z / v.w);
		}

		FLOAT magnitude()
		{
			return sqrtf(x*x + y * y + z * z);
		}

		FLOAT length()
		{
			return magnitude();
		}

		vec4 normalize()
		{
			FLOAT len = magnitude();
			if (len > 0.0000001f)
			{
				x /= len;
				y /= len;
				z /= len;
			}
			else
			{
				x = 0.0f;
				y = 0.0f;
				z = 0.0f;
			}
			w = 1.0f;

			return *this;
		}

		char* dump()
		{
			char* str = new char[128];
			sprintf(str, "x : %f, y : %f, z : %f, w : %f\0", x, y, z, w);
			return str;
		}

		union
		{
			struct
			{
				FLOAT x, y, z, w;
			};
			struct
			{
				FLOAT r, g, b, a;
			};
			struct
			{
				FLOAT v[4];
			};
			struct
			{
				FLOAT mData[4];
			};
		};
	};


	class mat3
	{
	public:
		mat3()
		{
			memset(v, 0, sizeof(FLOAT) * 9);
			v[0] = 1.0f;
			v[4] = 1.0f;
			v[8] = 1.0f;
		}
		mat3(const FLOAT f)
		{
			memset(v, 0, sizeof(FLOAT) * 9);
			v[0] = f;
			v[4] = f;
			v[8] = f;
		}
		mat3(const FLOAT*ptr)
		{
			if (ptr == 0)
			{
				mat3(0.0f);
				return;
			}
			memcpy(v, ptr, sizeof(FLOAT) * 9);
		}
		mat3(FLOAT m11, FLOAT m12, FLOAT m13,
			FLOAT m21, FLOAT m22, FLOAT m23,
			FLOAT m31, FLOAT m32, FLOAT m33)
		{
			v[0] = m11;
			v[1] = m12;
			v[2] = m13;
			v[3] = m21;
			v[4] = m22;
			v[5] = m23;
			v[6] = m31;
			v[7] = m32;
			v[8] = m33;
		}
		mat3(const vec3 r1, const vec3 r2, const vec3 r3)
		{
			v[0] = r1.x; 
			v[1] = r1.y; 
			v[2] = r1.z;
			v[3] = r2.x; 
			v[4] = r2.y; 
			v[5] = r2.z;
			v[6] = r3.x; 
			v[7] = r3.y; 
			v[8] = r3.z;
		}

		FLOAT operator[](const INT n) const
		{
			return v[n];
		}
		mat3 operator=(const mat3 & m)
		{
			mat3 r;
			r.v[0] = m[0]; r.v[1] = m[1]; r.v[2] = m[2];
			r.v[3] = m[3]; r.v[4] = m[4]; r.v[5] = m[5];
			r.v[6] = m[6]; r.v[7] = m[7]; r.v[8] = m[8];
			return r;
		}
		mat3 operator+(const mat3 & m)
		{
			mat3 r;
			r.v[0] = v[0] + m[0]; r.v[1] = v[1] + m[1]; r.v[2] = v[2] + m[2];
			r.v[3] = v[3] + m[3]; r.v[4] = v[4] + m[4]; r.v[5] = v[5] + m[5];
			r.v[6] = v[6] + m[6]; r.v[7] = v[7] + m[7]; r.v[8] = v[8] + m[8];
			return r;
		}
		mat3 operator-(const mat3 & m)
		{
			mat3 r;
			r.v[0] = v[0] - m[0]; r.v[1] = v[1] - m[1]; r.v[2] = v[2] - m[2];
			r.v[3] = v[3] - m[3]; r.v[4] = v[4] - m[4]; r.v[5] = v[5] - m[5];
			r.v[6] = v[6] - m[6]; r.v[7] = v[7] - m[7]; r.v[8] = v[8] - m[8];
			return r;
		}
		mat3 operator*(const mat3 & m)
		{
			mat3 r;
			r.v[0] = v[0] * m[0] + v[1] * m[3] + v[2] * m[6];
			r.v[1] = v[0] * m[1] + v[1] * m[4] + v[2] * m[7];
			r.v[2] = v[0] * m[2] + v[1] * m[5] + v[2] * m[8];
			r.v[3] = v[3] * m[0] + v[4] * m[3] + v[5] * m[6];
			r.v[4] = v[3] * m[1] + v[4] * m[4] + v[5] * m[7];
			r.v[5] = v[3] * m[2] + v[4] * m[5] + v[5] * m[8];
			r.v[6] = v[6] * m[0] + v[7] * m[3] + v[8] * m[6];
			r.v[7] = v[6] * m[1] + v[7] * m[4] + v[8] * m[7];
			r.v[8] = v[6] * m[2] + v[7] * m[5] + v[8] * m[8];
			return r;
		}
		vec3 operator*(const vec3 & v)
		{
			vec3 r;
			r.x = mData[0] * v.x + mData[1] * v.y + mData[2] * v.z;
			r.y = mData[3] * v.x + mData[4] * v.y + mData[5] * v.z;
			r.z = mData[6] * v.x + mData[7] * v.y + mData[8] * v.z;
			return r;
		}
		mat3 operator*(const FLOAT f)
		{
			mat3 r;
			r.v[0] *= f;
			r.v[1] *= f;
			r.v[2] *= f;
			r.v[3] *= f;
			r.v[4] *= f;
			r.v[5] *= f;
			r.v[6] *= f;
			r.v[7] *= f;
			r.v[8] *= f;
			return r;
		}

		mat3 transpose()
		{
			mat3 r(*this);
			swap(r.v[1], r.v[3]);
			swap(r.v[2], r.v[6]);
			swap(r.v[5], r.v[7]);

			return r;
		}
		FLOAT determinant()
		{
			return v[0] * (v[4] * v[8] - v[5] * v[7]) -
				v[1] * (v[3] * v[8] - v[5] * v[6]) +
				v[2] * (v[3] * v[7] - v[4] * v[6]);
		}
		mat3 inverse()
		{
			FLOAT det = determinant();
			mat3 r;
			r.v[0] = v[4] * v[8] - v[5] * v[7];
			r.v[1] = v[5] * v[6] - v[3] * v[8];
			r.v[2] = v[3] * v[7] - v[4] * v[6];
			r.v[3] = v[2] * v[7] - v[1] * v[8];
			r.v[4] = v[0] * v[8] - v[2] * v[6];
			r.v[5] = v[1] * v[6] - v[0] * v[7];
			r.v[6] = v[1] * v[5] - v[2] * v[4];
			r.v[7] = v[2] * v[3] - v[0] * v[5];
			r.v[8] = v[0] * v[4] - v[1] * v[3];
			r.v[0] /= det;
			r.v[1] /= det;
			r.v[2] /= det;
			r.v[3] /= det;
			r.v[4] /= det;
			r.v[5] /= det;
			r.v[6] /= det;
			r.v[7] /= det;
			r.v[8] /= det;
			return r.transpose();
		}
		FLOAT get(UINT i, UINT j)
		{
			i--, j--;
			i %= 3;
			j %= 3;
			return v[i * 3 + j];
		}

		char* dump()
		{
			char*buf = new char[512]{ 0 };
			sprintf(buf, "[%f, %f, %f]\n[%f, %f, %f]\n[%f, %f, %f]", v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7], v[8]);
			return buf;
		}

		union
		{
			struct
			{
				FLOAT v[9];
			};
			struct
			{
				FLOAT mData[9];
			};
		};
	};

	class mat4
	{
	public:
		mat4()
		{
			memset(v, 0, sizeof(FLOAT) * 16);
			v[0] = 1.0f;
			v[5] = 1.0f;
			v[10] = 1.0f;
			v[15] = 1.0f;
		}
		mat4(const mat3 & m)
		{
			v[0] = m[0]; 
			v[1] = m[1]; 
			v[2] = m[2]; 
			v[3] = 0.0f;
			v[4] = m[3]; 
			v[5] = m[4]; 
			v[6] = m[5]; 
			v[7] = 0.0f;
			v[8] = m[6]; 
			v[9] = m[7]; 
			v[10] = m[8];
			v[11] = 0.0f;
			v[12] = 0.0f; 
			v[13] = 0.0f; 
			v[14] = 0.0f; 
			v[15] = 1.0f;
		}
		mat4(const mat4 & m)
		{
			memcpy(v, m.v, sizeof(FLOAT) * 16);
		}
		mat4(const vec4 r1, const vec4 r2, const vec4 r3, const vec4 r4)
		{
			memcpy(v, r1.v, sizeof(FLOAT) * 4);
			memcpy(v + 4, r2.v, sizeof(FLOAT) * 4);
			memcpy(v + 8, r3.v, sizeof(FLOAT) * 4);
			memcpy(v + 12, r4.v, sizeof(FLOAT) * 4);
		}
		mat4(const FLOAT f)
		{
			memset(v, 0, sizeof(FLOAT) * 16);
			v[0] = f;
			v[5] = f;
			v[10] = f;
			v[15] = 1.0f;
		}
		mat4(const FLOAT * ptr)
		{
			if (ptr == 0)
			{
				memset(v, 0, sizeof(FLOAT) * 16);
				return;
			}
			memcpy(v, ptr, sizeof(FLOAT) * 16);
		}

		FLOAT operator[](const INT n) const
		{
			return v[n];
		}
		void operator=(const mat4 & m)
		{
			memcpy(v, m.v, sizeof(FLOAT) * 16);
		}
		mat4 operator+(const mat4 & m)
		{
			mat4 r(*this);
			r.v[0] += m[0]; 
			r.v[1] += m[1]; 
			r.v[2] += m[2];
			r.v[3] += m[3];
			r.v[4] += m[4]; 
			r.v[5] += m[5];
			r.v[6] += m[6]; 
			r.v[7] += m[7];
			r.v[8] += m[8];
			r.v[9] += m[9]; 
			r.v[10] += m[10];
			r.v[11] += m[11];
			r.v[12] += m[12]; 
			r.v[13] += m[13];
			r.v[14] += m[14];
			r.v[15] += m[15];
			return r;
		}
		mat4 operator-(const mat4 & m)
		{
			mat4 r(*this);
			r.v[0] -= m[0]; 
			r.v[1] -= m[1]; 
			r.v[2] -= m[2]; 
			r.v[3] -= m[3];
			r.v[4] -= m[4]; 
			r.v[5] -= m[5];
			r.v[6] -= m[6];
			r.v[7] -= m[7];
			r.v[8] -= m[8]; 
			r.v[9] -= m[9]; 
			r.v[10] -= m[10];
			r.v[11] -= m[11];
			r.v[12] -= m[12];
			r.v[13] -= m[13];
			r.v[14] -= m[14]; 
			r.v[15] -= m[15];
			return r;
		}
		vec4 operator*(const vec4 & v)
		{
			vec4 r(0.0f);
			r.x = mData[0] * v.x + mData[1] * v.y + mData[2] * v.z + mData[3] * v.w;
			r.y = mData[4] * v.x + mData[5] * v.y + mData[6] * v.z + mData[7] * v.w;
			r.z = mData[8] * v.x + mData[9] * v.y + mData[10] * v.z + mData[11] * v.w;
			r.w = mData[12] * v.x + mData[13] * v.y + mData[14] * v.z + mData[15] * v.w;
			return r;
		}
		mat4 operator*(const mat4 & m)
		{
			mat4 r(0.0f);
			r.v[0] = v[0] * m[0] + v[1] * m[4] + v[2] * m[8] + v[3] * m[12];
			r.v[1] = v[0] * m[1] + v[1] * m[5] + v[2] * m[9] + v[3] * m[13];
			r.v[2] = v[0] * m[2] + v[1] * m[6] + v[2] * m[10] + v[3] * m[14];
			r.v[3] = v[0] * m[3] + v[1] * m[7] + v[2] * m[11] + v[3] * m[15];
			r.v[4] = v[4] * m[0] + v[5] * m[4] + v[6] * m[8] + v[7] * m[12];
			r.v[5] = v[4] * m[1] + v[5] * m[5] + v[6] * m[9] + v[7] * m[13];
			r.v[6] = v[4] * m[2] + v[5] * m[6] + v[6] * m[10] + v[7] * m[14];
			r.v[7] = v[4] * m[3] + v[5] * m[7] + v[6] * m[11] + v[7] * m[15];
			r.v[8] = v[8] * m[0] + v[9] * m[4] + v[10] * m[8] + v[11] * m[12];
			r.v[9] = v[8] * m[1] + v[9] * m[5] + v[10] * m[9] + v[11] * m[13];
			r.v[10] = v[8] * m[2] + v[9] * m[6] + v[10] * m[10] + v[11] * m[14];
			r.v[11] = v[8] * m[3] + v[9] * m[7] + v[10] * m[11] + v[11] * m[15];
			r.v[12] = v[12] * m[0] + v[13] * m[4] + v[14] * m[8] + v[15] * m[12];
			r.v[13] = v[12] * m[1] + v[13] * m[5] + v[14] * m[9] + v[15] * m[13];
			r.v[14] = v[12] * m[2] + v[13] * m[6] + v[14] * m[10] + v[15] * m[14];
			r.v[15] = v[12] * m[3] + v[13] * m[7] + v[14] * m[11] + v[15] * m[15];
			return r;
		}
		mat4 transpose()
		{
			mat4 r(*this);
			swap(r.v[1], r.v[4]);
			swap(r.v[2], r.v[8]);
			swap(r.v[3], r.v[12]);
			swap(r.v[6], r.v[9]);
			swap(r.v[7], r.v[13]);
			swap(r.v[11], r.v[14]);
			return r;
		}
		mat3 cofactor(UINT row, UINT column)
		{
			row--; column--;
			row = row % 4;
			column = column % 4;
			INT index = 0;
			mat3 r;
			for (INT i = 0; i < 4; ++i)
			{
				if (i == row)
					continue;
				for (INT j = 0; j < 4; ++j)
				{
					if (j == column)
						continue;
					r.v[index++] = v[i * 4 + j];
				}
			}
			return r;
		}
		FLOAT determinant()
		{
			mat3 M11(v[5], v[6], v[7], v[9], v[10], v[11], v[13], v[14], v[15]);
			mat3 M12(v[4], v[6], v[7], v[8], v[10], v[11], v[12], v[14], v[15]);
			mat3 M13(v[4], v[5], v[7], v[8], v[9], v[11], v[12], v[13], v[15]);
			mat3 M14(v[4], v[5], v[6], v[8], v[9], v[10], v[12], v[13], v[14]);

			//std::cout << "M11\n" << M11.dump() << std::endl;
			//std::cout << "M12\n" << M12.dump() << std::endl;
			//std::cout << "M13\n" << M13.dump() << std::endl;
			//std::cout << "M14\n" << M14.dump() << std::endl;

			return v[0] * M11.determinant() - v[1] * M12.determinant() + v[2] * M13.determinant() - v[3] * M14.determinant();
		}
		mat4 inverse()
		{
			FLOAT det = determinant();
			mat4 r;
			r.v[0] = cofactor(1, 1).determinant();
			r.v[1] = -cofactor(1, 2).determinant();
			r.v[2] = cofactor(1, 3).determinant();
			r.v[3] = -cofactor(1, 4).determinant();
			r.v[4] = -cofactor(2, 1).determinant();
			r.v[5] = cofactor(2, 2).determinant();
			r.v[6] = -cofactor(2, 3).determinant();
			r.v[7] = cofactor(2, 4).determinant();
			r.v[8] = cofactor(3, 1).determinant();
			r.v[9] = -cofactor(3, 2).determinant();
			r.v[10] = cofactor(3, 3).determinant();
			r.v[11] = -cofactor(3, 4).determinant();
			r.v[12] = -cofactor(4, 1).determinant();
			r.v[13] = cofactor(4, 2).determinant();
			r.v[14] = -cofactor(4, 3).determinant();
			r.v[15] = cofactor(4, 4).determinant();
			r.v[0] /= det;
			r.v[1] /= det;
			r.v[2] /= det;
			r.v[3] /= det;
			r.v[4] /= det;
			r.v[5] /= det;
			r.v[6] /= det;
			r.v[7] /= det;
			r.v[8] /= det;
			r.v[9] /= det;
			r.v[10] /= det;
			r.v[11] /= det;
			r.v[12] /= det;
			r.v[13] /= det;
			r.v[14] /= det;
			r.v[15] /= det;

			return r.transpose();
		}
		FLOAT get(UINT i, UINT j)
		{
			i--, j--;
			i %= 4;
			j %= 4;
			return v[i * 4 + j];
		}

		char* dump()
		{
			CHAR* buf = new CHAR[512]{ 0 };
			sprintf(buf, "[%f, %f, %f, %f]\n[%f, %f, %f, %f]\n[%f, %f, %f, %f]\n[%f, %f, %f, %f]\n",
				v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7], v[8], v[9], v[10], v[11], v[12], v[13], v[14], v[15]);
			return buf;
		}

		union
		{
			struct
			{
				FLOAT mData[16];
			};
			struct
			{
				FLOAT v[16];
			};
		};
	};

	class quat
	{
	public:
		quat() :
			x(0.0f),
			y(0.0f),
			z(0.0f),
			w(0.0f)
		{
		}
		quat(const quat & q) :
			x(q.x),
			y(q.y),
			z(q.z),
			w(q.w)
		{
		}
		quat(FLOAT angle, FLOAT x, FLOAT y, FLOAT z)
		{
			quat r;
			FLOAT s = sin(angle * 0.5f);

			r.w = cos(angle * 0.5f);
			r.x = x * s;
			r.y = y * s;
			r.z = z * s;
		}

		quat operator*(const quat & q)
		{
			quat r;
			r.x = x * q.w + w * q.x + y * q.z - z * q.y;
			r.y = y * q.w + w * q.y + z * q.x - x * q.z;
			r.z = z * q.w + w * q.z + x * q.y - y * q.x;
			r.w = w * q.w - x * q.x - y * q.y - z * q.z;
			return r;
		}

		static quat euler2quat(FLOAT pitch, FLOAT yaw, FLOAT roll)
		{
			FLOAT sX = sin(pitch) * 0.5f;
			FLOAT cX = cos(pitch) * 0.5f;
			FLOAT sY = sin(yaw) * 0.5f;
			FLOAT cY = cos(yaw) * 0.5f;
			FLOAT sZ = sin(roll) * 0.5f;
			FLOAT cZ = cos(roll) * 0.5f;
			quat qX(sX, 0.0f, 0.0f, cX);
			quat qY(0.0f, sY, 0.0f, cY);
			quat qZ(0.0f, 0.0f, sZ, cZ);
			return qY * qX * qZ;
			return quat();
		}
		static mat3 quat2mat3(quat & q)
		{
			FLOAT x = q.x * 2.0f;
			FLOAT y = q.y * 2.0f;
			FLOAT z = q.z * 2.0f;
			FLOAT xx = q.x * x;
			FLOAT xy = q.x * y;
			FLOAT xz = q.x * z;
			FLOAT yy = q.y * y;
			FLOAT yz = q.y * z;
			FLOAT zz = q.z * z;
			FLOAT wx = q.w * x;
			FLOAT wy = q.w * y;
			FLOAT wz = q.w * z;

			mat3 *r = new mat3(0.0f);
			r->v[0] = 1.0f - 2.0f * (yy + zz);
			r->v[1] = 2.0f * (xy + wz);
			r->v[2] = 2.0f * (xz - wy);
			r->v[3] = 2.0f * (xy - wz);
			r->v[4] = 1.0f - 2.0f * (xx + zz);
			r->v[5] = 2.0f * (yz + wx);
			r->v[6] = 2.0f * (xz + wy);
			r->v[7] = 2.0f * (yz - wx);
			r->v[8] = 1.0f - 2.0f * (xx + yy);
			return *r;
		}
		static mat4 quat2mat4(quat & q)
		{
			mat3 m3(quat2mat3(q));
			return mat4(m3);
		}

		union
		{
			struct
			{
				FLOAT x, y, z, w;
			};
		};
	};

	namespace
	{

		mat4 inverse(mat4 m)
		{
			mat4 r(m.inverse());
			return r;
		}
		mat4 inverseTranspose(mat4 m)
		{
			mat4 r(m.inverse());
			return r.transpose();
		}

		FLOAT * value_ptr(vec3 v)
		{
			vec3*vp = new vec3(v);
			return vp->v;
		}
		FLOAT * value_ptr(vec4 v)
		{
			vec4*vp = new vec4(v);
			return vp->v;
		}
		FLOAT * value_ptr(mat3 m)
		{
			mat3*mp = new mat3(m);
			return mp->v;
		}
		FLOAT * value_ptr(mat4 m)
		{
			mat4*mp = new mat4(m);
			return mp->v;
		}

		mat4 translate(FLOAT x = 0.0f, FLOAT y = 0.0f, FLOAT z = 0.0f)
		{
			mat4 r(1.0f);
			r.v[3] += x;
			r.v[7] += y;
			r.v[11] += z;
			return r;
		}

		mat4 scale(FLOAT x = 1.0f, FLOAT y = 1.0f, FLOAT z = 1.0f)
		{
			mat4 r(1.0f);
			r.v[0] *= x;
			r.v[5] *= y;
			r.v[10] *= z;
			return r;
		}

		mat4 rotate(FLOAT deg, FLOAT x, FLOAT y, FLOAT z)
		{
			mat4 r;
			FLOAT C = cos(deg);
			FLOAT S = sin(deg);
			vec3 vec(x, y, z);
			vec = vec.normalize();
			x = vec.x, y = vec.y, z = vec.z;
			r.v[0] = x * x + (1 - x * x)*C;
			r.v[1] = x * y*(1 - C) + z * S; 
			r.v[2] = x * z*(1 - C) - y * S; 
			r.v[3] = 0.0f;
			r.v[4] = x * y*(1 - C) - z * S; 
			r.v[5] = y * y + (1 - y * y)*C;
			r.v[6] = y * z*(1 - C) + x * S; 
			r.v[7] = 0.0f;
			r.v[8] = x * z*(1 - C) + y * S;
			r.v[9] = y * z*(1 - C) - x * S; 
			r.v[10] = z * z + (1 - z * z)*C; 
			r.v[11] = 0.0f;
			r.v[12] = 0.0f;
			r.v[13] = 0.0f; 
			r.v[14] = 0.0f; 
			r.v[15] = 1.0f;

			return r;
		}
		mat3 rotate(FLOAT yaw, FLOAT pitch, FLOAT roll)
		{
			quat q;
			q = q.euler2quat(pitch, yaw, roll);
			return q.quat2mat3(q);
		}

		mat4 lookAt(FLOAT eyex, FLOAT eyey, FLOAT eyez, FLOAT centerx, FLOAT centery, FLOAT centerz, FLOAT upx, FLOAT upy, FLOAT upz)
		{
			vec3 eye(eyex, eyey, eyez), center(centerx, centery, centerz), up(upx, upy, upz);
			vec3 z = eye - center;
			z.normalize();
			vec3 x = up.cross(z);
			x.normalize();
			vec3 y = z.cross(x);

			//mat4 R(1.0f);
			//R.v[0] = x.x; 
			//R.v[1] = x.y; 
			//R.v[2] = x.z; 
			//R.v[3] = -(x*eye);
			//R.v[4] = y.x;
			//R.v[5] = y.y; 
			//R.v[6] = y.z; 
			//R.v[7] = -(y*eye);
			//R.v[8] = z.x;
			//R.v[9] = z.y; 
			//R.v[10] = z.z; 
			//R.v[11] = -(z*eye);
			//return R;

			mat4 R;
			R.v[0] = x.x; 
			R.v[1] = x.y; 
			R.v[2] = x.z; 
			R.v[3] = 0.0f;
			R.v[4] = y.x; 
			R.v[5] = y.y; 
			R.v[6] = y.z; 
			R.v[7] = 0.0f;
			R.v[8] = z.x; 
			R.v[9] = z.y; 
			R.v[10] = z.z; 
			R.v[11] = 0.0f;
			R.v[12] = 0.0f; 
			R.v[13] = 0.0f; 
			R.v[14] = 0.0f; 
			R.v[15] = 1.0f;
			mat4 T;
			T.v[0] = 1.0f; 
			T.v[1] = 0.0f; 
			T.v[2] = 0.0f; 
			T.v[3] = -eyex;
			T.v[4] = 0.0f; 
			T.v[5] = 1.0f; 
			T.v[6] = 0.0f; 
			T.v[7] = -eyey;
			T.v[8] = 0.0f; 
			T.v[9] = 0.0f; 
			T.v[10] = 1.0f; 
			T.v[11] = -eyez;
			T.v[12] = 0.0f; 
			T.v[13] = 0.0f; 
			T.v[14] = 0.0f; 
			T.v[15] = 1.0f;
			return R * T;
		}
		mat4 lookAt(vec3 eye, vec3 center, vec3 up)
		{
			return lookAt(eye.x, eye.y, eye.z, center.x, center.y, center.z, up.x, up.y, up.z);
		}
		mat4 perspective(FLOAT fov, FLOAT aspect, FLOAT znear, FLOAT zfar)
		{
			FLOAT deltaZ = zfar - znear;
			FLOAT yscale = tan(fov / 2.0f);
			FLOAT xscale = yscale * aspect;
			mat4 r = mat4(0.0f);
			r.v[0] = 1.0f / xscale;
			r.v[5] = 1.0f / yscale;
			r.v[10] = -(zfar + znear) / (deltaZ);
			r.v[14] = -1.0f;
			r.v[11] = -(2.0f * zfar * znear) / (deltaZ);

			return r;
		}
		mat4 ortho(FLOAT left, FLOAT right, FLOAT bottom, FLOAT top, FLOAT znear, FLOAT zfar)
		{
			mat4 r = mat4(1.0f);
			FLOAT deltaX = right - left;
			FLOAT deltaY = top - bottom;
			FLOAT deltaZ = zfar - znear;

			r.v[0] = 2.0f / deltaX;
			r.v[3] = -(right + left) / deltaX;
			r.v[5] = 2.0f / deltaY;
			r.v[7] = -(top + bottom) / deltaY;
			r.v[10] = -2.0f / deltaZ;
			r.v[11] = -(zfar + znear) / deltaZ;

			return r;
		}

		vec3 unProject(FLOAT normalPosX, FLOAT normalPosY, mat4 viewMatrix, mat4 projectionMatrix)
		{
			mat4 inv = (projectionMatrix*viewMatrix).inverse();
			vec4 pos(normalPosX, normalPosY, 1.0f, 1.0f);
			pos = inv * pos;
			pos = pos.normalize();
			return vec3(pos.x, pos.y, pos.z);
		}
	}
}

#endif
