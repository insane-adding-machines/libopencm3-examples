/*
 * h2_linear.h
 *
 *  Created on: 31 Jul 2016
 *      Author: h2obrain
 *
 * My old actionscript-library from 2004
 *
 * :) http://softsurfer.com/
 * ? http://softsurfer.com/Archive/algorithm_0106/algorithm_0106.htm#Distance%20between%20Lines
 * ?? http://stochastix.wordpress.com/2008/12/28/distance-between-two-lines/
 * ??? http://www.mc.edu/campus/users/travis/maa/proceedings/spring2001/bard.himel.pdf
 * http://stackoverflow.com/questions/217578/point-in-polygon-aka-hit-test
 *  http://local.wasp.uwa.edu.au/~pbourke/geometry
 *  http://steve.hollasch.net/cgindex/math/fowler.html
  */

#ifndef H2_LINEAR_H_
#define H2_LINEAR_H_

#include <stdbool.h>
#include <inttypes.h>
#include <float.h>
#include <math.h>

#define h2vec_float_t         float
#define h2vec_float_MIN       FLT_MIN
#define h2vec_float_MAX       FLT_MAX
#define h2vec_float_EPSILON   FLT_EPSILON
#define h2vec_float_MIN_VALUE 0.00000001
#define h2vec_float_abs       fabsf
#define h2vec_float_sqrt      sqrtf
#define h2vec_float_pow       powf
#define h2vec_float_atan2     atan2f
#define h2vec_float_cos       cosf
#define h2vec_float_min       fminf
#define h2vec_float_max       fmaxf


typedef struct {
	h2vec_float_t x,y;
} point2d_t;
typedef struct {
	point2d_t p1,p2;
} segment2d_t;
typedef struct {
	h2vec_float_t distance;
	point2d_t      nearest_point;
} intersection_t;


static inline
point2d_t
point2d_add_ts(point2d_t p1, point2d_t p2) {
	return (point2d_t) {p1.x + p2.x, p1.y + p2.y};
}
static inline
point2d_t
point2d_sub_ts(point2d_t p1, point2d_t p2) {
	return (point2d_t) {p1.x - p2.x, p1.y - p2.y};
}
static inline
point2d_t
point2d_mul_ts(point2d_t p1, point2d_t p2) {
	return (point2d_t) {p1.x * p2.x, p1.y * p2.y};
}
static inline
point2d_t
point2d_mul_t(point2d_t p, h2vec_float_t mul) {
	return (point2d_t) {mul * p.x, mul * p.y};
}
static inline
point2d_t
point2d_div_t(point2d_t p, h2vec_float_t div) {
	return (point2d_t) {p.x / div, p.y / div};
}
static inline
h2vec_float_t
point2d_cross(point2d_t p1, point2d_t p2) {
	return (p1.x * p2.y - p1.y * p2.x);
}
static inline
h2vec_float_t
point2d_dot(point2d_t u, point2d_t v) {
	point2d_t p = point2d_mul_ts(u, v);
	return p.x + p.y;
}
// norm = length of vector
static inline
h2vec_float_t
point2d_norm(point2d_t v) {
	return h2vec_float_sqrt(point2d_dot(v, v));
}
static inline
point2d_t
point2d_unit(point2d_t v) {
	h2vec_float_t norm = point2d_norm(v);
	if (norm!=0) return point2d_div_t(v, norm);
	return (point2d_t) {0,0};
}
static inline
h2vec_float_t
point2d_dist(point2d_t u, point2d_t v) {
	return point2d_norm(point2d_sub_ts(u, v));
}
static inline
bool
point2d_compare(point2d_t p1, point2d_t p2, float resolution) {
	point2d_t dist = point2d_sub_ts(p1,p2);
	return (h2vec_float_abs(dist.x) < resolution) && (h2vec_float_abs(dist.y) < resolution);
}
static inline
point2d_t
point2d_abs(point2d_t p) {
	return (point2d_t) {h2vec_float_abs(p.x),h2vec_float_abs(p.y)};
}



static inline
h2vec_float_t
dist_point_to_line(point2d_t P, segment2d_t s) {
	point2d_t v = point2d_sub_ts(s.p2, s.p1);
	point2d_t w = point2d_sub_ts(P   , s.p1);

	h2vec_float_t c1 = point2d_dot(w,v);
	h2vec_float_t c2 = point2d_dot(v,v);
	h2vec_float_t b  = c1 / c2;

	point2d_t Pb = point2d_add_ts(s.p1, point2d_mul_t(v, b));
	return point2d_dist(P, Pb);
}
static inline
h2vec_float_t
dist_line_to_line(segment2d_t l1, segment2d_t l2) {
	point2d_t u = point2d_sub_ts(l1.p2, l1.p1);
	point2d_t v = point2d_sub_ts(l2.p2, l2.p1);
	point2d_t w = point2d_sub_ts(l1.p1, l2.p1);
	h2vec_float_t a = point2d_dot(u,u);        // always >= 0
	h2vec_float_t b = point2d_dot(u,v);
	h2vec_float_t c = point2d_dot(v,v);        // always >= 0
	h2vec_float_t d = point2d_dot(u,w);
	h2vec_float_t e = point2d_dot(v,w);
	h2vec_float_t D = a*c - b*b;       // always >= 0
	h2vec_float_t sc, tc;

	// compute the line parameters of the two closest points
	if (D < h2vec_float_MIN_VALUE) {         // the lines are almost parallel
		sc = 0.0;
		 // use the largest denominator
		tc = (b>c ? d/b : e/c);
	}
	else {
		sc = (b*e - c*d) / D;
		tc = (a*e - b*d) / D;
	}

	// get the difference of the two closest points
	point2d_t dP = point2d_add_ts(w, point2d_sub_ts(point2d_mul_t(u, sc), point2d_mul_t(v, tc)));  // = L1(sc) - L2(tc)
	h2vec_float_t ndP = point2d_norm(dP);

	return ndP;
}
static inline
intersection_t
dist_segment_to_segment(segment2d_t s1, segment2d_t s2) {
	point2d_t u = point2d_sub_ts(s1.p2, s1.p1);
	point2d_t v = point2d_sub_ts(s2.p2, s2.p1);
	point2d_t w = point2d_sub_ts(s1.p1, s2.p1);
	h2vec_float_t a = point2d_dot(u,u);        // always >= 0
	h2vec_float_t b = point2d_dot(u,v);
	h2vec_float_t c = point2d_dot(v,v);        // always >= 0
	h2vec_float_t d = point2d_dot(u,w);
	h2vec_float_t e = point2d_dot(v,w);
	h2vec_float_t D = a*c - b*b;       // always >= 0
	h2vec_float_t sc, sN, sD = D;      // sc = sN / sD, default sD = D >= 0
	h2vec_float_t tc, tN, tD = D;      // tc = tN / tD, default tD = D >= 0

	// compute the line parameters of the two closest points
	if (D < h2vec_float_MIN_VALUE) { // the lines are almost parallel
		sN = 0.0;        // force using point P0 on segment S1
		sD = 1.0;        // to prevent possible division by 0.0 later
		tN = e;
		tD = c;
	}
	else {                // get the closest points on the infinite lines
		sN = (b*e - c*d);
		tN = (a*e - b*d);
		if (sN < 0.0) {       // sc < 0 => the s=0 edge is visible
			sN = 0.0;
			tN = e;
			tD = c;
		}
		else if (sN > sD) {  // sc > 1 => the s=1 edge is visible
			sN = sD;
			tN = e + b;
			tD = c;
		}
	}

	if (tN < 0.0) {           // tc < 0 => the t=0 edge is visible
		tN = 0.0;
		// recompute sc for this edge
		if (-d < 0.0)
			sN = 0.0;
		else if (-d > a)
			sN = sD;
		else {
			sN = -d;
			sD = a;
		}
	}
	else if (tN > tD) {      // tc > 1 => the t=1 edge is visible
		tN = tD;
		// recompute sc for this edge
		if ((-d + b) < 0.0)
			sN = 0;
		else if ((-d + b) > a)
			sN = sD;
		else {
			sN = (-d + b);
			sD = a;
		}
	}
	// finally do the division to get sc and tc
	if (h2vec_float_abs(sN) < h2vec_float_MIN_VALUE) { sc = 0; } else { sc = sN / sD; }
	if (h2vec_float_abs(tN) < h2vec_float_MIN_VALUE) { tc = 0; } else { tc = tN / tD; }

	// get the difference of the two closest points
	point2d_t scU = point2d_mul_t(u, sc);
	point2d_t tcV = point2d_mul_t(v, tc);
	point2d_t dP  = point2d_sub_ts(point2d_add_ts(w, scU), tcV);  // = S1(sc) - S2(tc)
	h2vec_float_t ndP = point2d_norm(dP);


	/*  ???? */
	if (ndP < h2vec_float_MIN_VALUE) ndP = 0.0;

	return (intersection_t) {
		.distance      = ndP,
		.nearest_point = point2d_add_ts(s1.p1, scU)
	};
}

/*
 * Fowler angles can be used to COMPARE angles between points fast
 *
This function is due to Rob Fowler.  Given dy and dx between 2 points
p1 and p2, we calculate a number in [0.0, 8.0) which is a monotonic
function of the direction from A to B.

(0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0) correspond to
(  0,  45,  90, 135, 180, 225, 270, 315, 360) degrees, measured
counter-clockwise from the positive x axis.
*/
static inline
h2vec_float_t
fowler_angle(point2d_t p1, point2d_t p2) {
	point2d_t d = point2d_sub_ts(p2, p1);
	point2d_t dabs = point2d_abs(d);

	int code;					/* Angular Region Classification Code */

	code = (dabs.x < dabs.y) ? 1 : 0;
	if (d.x < 0)  code += 2;
	if (d.y < 0)  code += 4;

	switch (code) {
		case 0: return (d.x==0) ? 0 :       dabs.y/dabs.x;        /* [  0, 45] */
		case 1: return ((h2vec_float_t)2 - (dabs.x/dabs.y));      /* ( 45, 90] */
		case 3: return ((h2vec_float_t)2 + (dabs.x/dabs.y));      /* ( 90,135) */
		case 2: return ((h2vec_float_t)4 - (dabs.y/dabs.x));      /* [135,180] */
		case 6: return ((h2vec_float_t)4 + (dabs.y/dabs.x));      /* (180,225] */
		case 7: return ((h2vec_float_t)6 - (dabs.x/dabs.y));      /* (225,270) */
		case 5: return ((h2vec_float_t)6 + (dabs.x/dabs.y));      /* [270,315) */
		case 4: return ((h2vec_float_t)8 - (dabs.y/dabs.x));      /* [315,360) */

		default: return -1; /* hardcore fail */
	}
}

#endif /* H2_LINEAR_H_ */
