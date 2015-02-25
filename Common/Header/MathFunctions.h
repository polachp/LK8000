/*
 * LK8000 Tactical Flight Computer -  WWW.LK8000.IT
 * Released under GNU/GPL License v.2
 * See CREDITS.TXT file for authors and copyrights
 *
 * File:   MathFunctions.h
 * Author: Bruno de Lacheisserie
 *
 * Created on 23 octobre 2014
 */

#ifndef MATHFUNCTIONS_H
#define	MATHFUNCTIONS_H

void rotate(double &xin, double &yin, const double &angle);
void rotatescale(double &xin, double &yin, const double &angle, const double &scale);

void frotate(float &xin, float &yin, const float &angle);
void frotatescale(float &xin, float &yin, const float &angle, const float &scale);

void irotate(int &xin, int &yin, const double &angle);
void irotatescale(int &xin, int &yin, const double &angle, const double &scale, double &x, double &y);

void protate(POINT &pin, const double &angle);
void protateshift(POINT &pin, const double &angle, const int &x, const int &y);

double AngleLimit360(double theta);
double AngleLimit180(double theta);

double Reciprocal(double InBound);
double AngleDifference(double angle1, double angle0);
bool AngleInRange(double Angle0, double Angle1, double x, bool is_signed=false);

double HalfAngle(double Angle0, double Angle1);
double BiSector(double InBound, double OutBound);

double ScreenAngle(int x1, int y1, int x2, int y2);

// Fast trig functions
void InitSineTable(void);

#ifdef __MINGW32__
#define DEG_TO_INT(x) ((unsigned short)(int)((x)*(65536.0/360.0)))>>4
#else
#define DEG_TO_INT(x) ((unsigned short)((x)*(65536.0/360.0)))>>4
#endif

extern double COSTABLE[4096];
extern double SINETABLE[4096];
extern double INVCOSINETABLE[4096];
extern short ISINETABLE[4096];
extern short ICOSTABLE[4096];

#define invfastcosine(x) INVCOSINETABLE[DEG_TO_INT(x)]
#define ifastcosine(x) ICOSTABLE[DEG_TO_INT(x)]
#define ifastsine(x) ISINETABLE[DEG_TO_INT(x)]
#define fastcosine(x) COSTABLE[DEG_TO_INT(x)]
#define fastsine(x) SINETABLE[DEG_TO_INT(x)]



// http://www.azillionmonkeys.com/qed/sqroot.html
inline unsigned int isqrt4(unsigned long val) {
#if defined(__i386__) || defined(__x86_64__)
  /* x86 FPUs are extremely fast */ 
  return (unsigned)sqrt((double)val);
#elif defined( __ARM_FP) 
  /* use vfp vsqrt.f32 instruction, 14 cycle !! */
  float result;
  const float fval = val;
  __asm ("vsqrt.f32 %0, %1" : "=w" (result) : "w" (fval) ); 
  return(result);
#else
  unsigned int temp, g=0;

  if (val >= 0x40000000) {
    g = 0x8000;
    val -= 0x40000000;
  }

#define INNER_MBGSQRT(s)                      \
  temp = (g << (s)) + (1 << ((s) * 2 - 2));   \
  if (val >= temp) {                          \
    g += 1 << ((s)-1);                        \
    val -= temp;                              \
  }

  INNER_MBGSQRT (15)
  INNER_MBGSQRT (14)
  INNER_MBGSQRT (13)
  INNER_MBGSQRT (12)
  INNER_MBGSQRT (11)
  INNER_MBGSQRT (10)
  INNER_MBGSQRT ( 9)
  INNER_MBGSQRT ( 8)
  INNER_MBGSQRT ( 7)
  INNER_MBGSQRT ( 6)
  INNER_MBGSQRT ( 5)
  INNER_MBGSQRT ( 4)
  INNER_MBGSQRT ( 3)
  INNER_MBGSQRT ( 2)

#undef INNER_MBGSQRT

  temp = g+g+1;
  if (val >= temp) g++;
  return g;
#endif
}

int  roundupdivision(int a, int b);

double LowPassFilter(double y_last, double x_in, double fact);

#ifndef MulDiv
    #define MulDiv _MulDiv
#endif

int _MulDiv(int nNumber, int nNumerator, int nDenominator);


//2^36 * 1.5,  (52-_shiftamt=36) uses limited precisicion to floor
//16.16 fixed point representation,

// =================================================================================
// Real2Int
// =================================================================================
inline int Real2Int(double val)
{
#if (WINDOWS_PC>0)
  val += 68719476736.0*1.5;
  return *((long*)&val) >> 16; 
#else
  return (int)val;
#endif
}


// =================================================================================
// Real2Int
// =================================================================================
inline int Real2Int(float val)
{
#if (WINDOWS_PC>0)
  return Real2Int ((double)val);
#else
  return (int)val;
#endif
}


inline int iround(double i) {
    return Real2Int(floor(i+0.5));
}

inline long lround(double i) {
    return (long)(floor(i+0.5));
}

inline unsigned int CombinedDivAndMod(unsigned int &lx) {
  unsigned int ox = lx & 0xff;
  // JMW no need to check max since overflow will result in 
  // beyond max dimensions
  lx = lx>>8;
  return ox;
}


#endif	/* MATHFUNCTIONS_H */

