/* ../../../dependencies/lapack/src/slartg.f -- translated by f2c (version 20061008).
   You must link the resulting object file with libf2c:
        on Microsoft Windows system, link with libf2c.lib;
        on Linux or Unix systems, link with .../path/to/libf2c.a -lm
        or, if you install libf2c.a in a standard place, with -lf2c -lm
        -- in that order, at the end of the command line, as in
                cc *.o -lf2c -lm
        Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

                http://www.netlib.org/f2c/libf2c.zip
*/



/* Subroutine */ static int slartg_(real *f, real *g, real *cs, real *sn, real *r__)
{
    /* Initialized data */

    static logical first = TRUE_;

    /* System generated locals */
    integer i__1;
    real r__1, r__2;

    /* Builtin functions */
    double log(doublereal), pow_ri(real *, integer *), sqrt(doublereal);

    /* Local variables */
    static integer i__;
    static real f1, g1, eps, scale;
    static integer count;
    static real safmn2, safmx2;
     doublereal slamch_(char *, ftnlen);
    static real safmin;


/*  -- LAPACK auxiliary routine (version 3.0) -- */
/*     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd., */
/*     Courant Institute, Argonne National Lab, and Rice University */
/*     September 30, 1994 */

/*     .. Scalar Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  SLARTG generate a plane rotation so that */

/*     [  CS  SN  ]  .  [ F ]  =  [ R ]   where CS**2 + SN**2 = 1. */
/*     [ -SN  CS  ]     [ G ]     [ 0 ] */

/*  This is a slower, more accurate version of the BLAS1 routine SROTG, */
/*  with the following other differences: */
/*     F and G are unchanged on return. */
/*     If G=0, then CS=1 and SN=0. */
/*     If F=0 and (G .ne. 0), then CS=0 and SN=1 without doing any */
/*        floating point operations (saves work in SBDSQR when */
/*        there are zeros on the diagonal). */

/*  If F exceeds G in magnitude, CS will be positive. */

/*  Arguments */
/*  ========= */

/*  F       (input) REAL */
/*          The first component of vector to be rotated. */

/*  G       (input) REAL */
/*          The second component of vector to be rotated. */

/*  CS      (output) REAL */
/*          The cosine of the rotation. */

/*  SN      (output) REAL */
/*          The sine of the rotation. */

/*  R       (output) REAL */
/*          The nonzero component of the rotated vector. */

/*  ===================================================================== */

/*     .. Parameters .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. External Functions .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. Save statement .. */
/*     .. */
/*     .. Data statements .. */
/*     .. */
/*     .. Executable Statements .. */

    if (first) {
        first = FALSE_;
        safmin = (real)slamch_("S", (ftnlen)1);
        eps = (real)slamch_("E", (ftnlen)1);
        r__1 = (real)slamch_("B", (ftnlen)1);
        i__1 = (integer) (log(safmin / eps) / log(slamch_("B", (ftnlen)1)) / 
                2.f);
        safmn2 = (real)pow_ri(&r__1, &i__1);
        safmx2 = 1.f / safmn2;
    }
    if (*g == 0.f) {
        *cs = 1.f;
        *sn = 0.f;
        *r__ = *f;
    } else if (*f == 0.f) {
        *cs = 0.f;
        *sn = 1.f;
        *r__ = *g;
    } else {
        f1 = *f;
        g1 = *g;
/* Computing MAX */
        r__1 = SABS(f1), r__2 = SABS(g1);
        scale = dmax(r__1,r__2);
        if (scale >= safmx2) {
            count = 0;
L10:
            ++count;
            f1 *= safmn2;
            g1 *= safmn2;
/* Computing MAX */
            r__1 = SABS(f1), r__2 = SABS(g1);
            scale = dmax(r__1,r__2);
            if (scale >= safmx2) {
                goto L10;
            }
/* Computing 2nd power */
            r__1 = f1;
/* Computing 2nd power */
            r__2 = g1;
            *r__ = (real)sqrt(r__1 * r__1 + r__2 * r__2);
            *cs = f1 / *r__;
            *sn = g1 / *r__;
            i__1 = count;
            for (i__ = 1; i__ <= i__1; ++i__) {
                *r__ *= safmx2;
/* L20: */
            }
        } else if (scale <= safmn2) {
            count = 0;
L30:
            ++count;
            f1 *= safmx2;
            g1 *= safmx2;
/* Computing MAX */
            r__1 = SABS(f1), r__2 = SABS(g1);
            scale = dmax(r__1,r__2);
            if (scale <= safmn2) {
                goto L30;
            }
/* Computing 2nd power */
            r__1 = f1;
/* Computing 2nd power */
            r__2 = g1;
            *r__ = (real)sqrt(r__1 * r__1 + r__2 * r__2);
            *cs = f1 / *r__;
            *sn = g1 / *r__;
            i__1 = count;
            for (i__ = 1; i__ <= i__1; ++i__) {
                *r__ *= safmn2;
/* L40: */
            }
        } else {
/* Computing 2nd power */
            r__1 = f1;
/* Computing 2nd power */
            r__2 = g1;
            *r__ = (real)sqrt(r__1 * r__1 + r__2 * r__2);
            *cs = f1 / *r__;
            *sn = g1 / *r__;
        }
        if (SABS(*f) > SABS(*g) && *cs < 0.f) {
            *cs = -(*cs);
            *sn = -(*sn);
            *r__ = -(*r__);
        }
    }
    return 0;

/*     End of SLARTG */

} /* slartg_ */
