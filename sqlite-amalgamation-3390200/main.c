#include <stdio.h>
#include <stdlib.h>

#include "sqlite3.h"

#ifndef SQLITE_PRIVATE
# define SQLITE_PRIVATE static
#endif

#ifndef LONGDOUBLE_TYPE
# define LONGDOUBLE_TYPE long double
#endif

#if defined(SQLITE_COVERAGE_TEST) || defined(SQLITE_DEBUG)
# ifndef SQLITE_AMALGAMATION
    extern unsigned int sqlite3CoverageCounter;
# endif
# define testcase(X)  if( X ){ sqlite3CoverageCounter += (unsigned)__LINE__; }
#else
# define testcase(X)
#endif

typedef sqlite_int64 i64; 

SQLITE_PRIVATE int sqlite3IntFloatCompare(i64 i, double r){
  if( sizeof(LONGDOUBLE_TYPE)>8 ){
    LONGDOUBLE_TYPE x = (LONGDOUBLE_TYPE)i;
    testcase( x<r );
    testcase( x>r );
    testcase( x==r );
    if( x<r ) return -1;
    if( x>r ) return +1;  /*NO_TEST*/ /* work around bugs in gcov */
    return 0;             /*NO_TEST*/ /* work around bugs in gcov */
  }else{
    i64 y;
    double s;
    if( r<-9223372036854775808.0 ) return +1;
    if( r>=9223372036854775808.0 ) return -1;
    y = (i64)r;
    if( i<y ) return -1;
    if( i>y ) return +1;
    s = (double)i;
    if( s<r ) return -1;
    if( s>r ) return +1;
    return 0;
  }
}


int main(){
	
	int ans = sqlite3IntFloatCompare(35, 35.65);
	printf("%d", ans);
	return 0;
}






