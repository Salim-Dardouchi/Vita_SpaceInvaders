/*
 * defines.h
 *
 *  Created on: 3 mars 2021
 *      Author: salim
 */


#define mRGBA(r,g,b,a)          ( ((u_int32_t)r<<24)|\
                                  ((u_int32_t)g<<16)|\
                                  ((u_int32_t)b<< 8)|\
                                  ((u_int32_t)a<< 0)  )

#define mToColor(c, RGBA)       ( c.r=(RGBA>>24)&0xff,\
                                  c.g=(RGBA>>16)&0xff,\
                                  c.b=(RGBA>>8)&0xff, \
                                  c.a=(RGBA>>0)&0xff )

#define mBitsSet(f,m)		( (f) |= (m) )
#define mBitsClr(f,m)		( (f) &= (~(m)) )
#define mBitsTgl(f,m)		( (f) ^= (m) )
#define mBitsMsk(f,m)		( (f) & (m) )
#define mIsBitsSet(f,m)		( ( (f) & (m) ) == (m) )
#define mIsBitsClr(f,m)		( ( (~(f)) & (m) ) == (m) )
