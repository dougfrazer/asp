
//*******************************************************************************
// Math
//   Math library
//   
// @author Doug Frazer
// Copyright (c) Doug Frazer
// February 2013
//*******************************************************************************


/* LinearInterpolate:
 * take the value X and translate (within the range x0-x1) to the range y0-y1
 *
 *  y - y0     y1 - y0
 * -------- = ---------
 *  x - x0     x1 - x0
 */
#define LinearInterpolate(x, x0, x1, y0, y1) ( y0+ ( (float)(y1-y0) * ( (float)(x-x0)/(float)(x1-x0) ) ) )
