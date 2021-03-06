/* ../../../dependencies/lapack/src/cgetrs.f -- translated by f2c (version 20061008).
   You must link the resulting object file with libf2c:
        on Microsoft Windows system, link with libf2c.lib;
        on Linux or Unix systems, link with .../path/to/libf2c.a -lm
        or, if you install libf2c.a in a standard place, with -lf2c -lm
        -- in that order, at the end of the command line, as in
                cc *.o -lf2c -lm
        Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

                http://www.netlib.org/f2c/libf2c.zip
*/




/* Subroutine */ static int cgetrs_(char *trans, integer *n, integer *nrhs, scomplex *
        a, integer *lda, integer *ipiv, scomplex *b, integer *ldb, integer *
        info, ftnlen trans_len)
{
/* Table of constant values */

static scomplex c_b1 = {1.f,0.f};
static integer c__1 = 1;
static integer c_n1 = -1;
    /* System generated locals */
    integer a_dim1, a_offset, b_dim1, b_offset, i__1;

    /* Local variables */
     logical lsame_(char *, char *, ftnlen, ftnlen);
     /* Subroutine */ int ctrsm_(char *, char *, char *, char *, 
            integer *, integer *, scomplex *, scomplex *, integer *, scomplex *, 
            integer *, ftnlen, ftnlen, ftnlen, ftnlen), xerbla_(char *, 
            integer *, ftnlen), claswp_(integer *, scomplex *, integer *, 
            integer *, integer *, integer *, integer *);
    static logical notran;


/*  -- LAPACK routine (version 3.0) -- */
/*     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd., */
/*     Courant Institute, Argonne National Lab, and Rice University */
/*     September 30, 1994 */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  CGETRS solves a system of linear equations */
/*     A * X = B,  A**T * X = B,  or  A**H * X = B */
/*  with a general N-by-N matrix A using the LU factorization computed */
/*  by CGETRF. */

/*  Arguments */
/*  ========= */

/*  TRANS   (input) CHARACTER*1 */
/*          Specifies the form of the system of equations: */
/*          = 'N':  A * X = B     (No transpose) */
/*          = 'T':  A**T * X = B  (Transpose) */
/*          = 'C':  A**H * X = B  (Conjugate transpose) */

/*  N       (input) INTEGER */
/*          The order of the matrix A.  N >= 0. */

/*  NRHS    (input) INTEGER */
/*          The number of right hand sides, i.e., the number of columns */
/*          of the matrix B.  NRHS >= 0. */

/*  A       (input) COMPLEX array, dimension (LDA,N) */
/*          The factors L and U from the factorization A = P*L*U */
/*          as computed by CGETRF. */

/*  LDA     (input) INTEGER */
/*          The leading dimension of the array A.  LDA >= MAX(1,N). */

/*  IPIV    (input) INTEGER array, dimension (N) */
/*          The pivot indices from CGETRF; for 1<=i<=N, row i of the */
/*          matrix was interchanged with row IPIV(i). */

/*  B       (input/output) COMPLEX array, dimension (LDB,NRHS) */
/*          On entry, the right hand side matrix B. */
/*          On exit, the solution matrix X. */

/*  LDB     (input) INTEGER */
/*          The leading dimension of the array B.  LDB >= MAX(1,N). */

/*  INFO    (output) INTEGER */
/*          = 0:  successful exit */
/*          < 0:  if INFO = -i, the i-th argument had an illegal value */

/*  ===================================================================== */

/*     .. Parameters .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. External Functions .. */
/*     .. */
/*     .. External Subroutines .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. Executable Statements .. */

/*     Test the input parameters. */

    /* Parameter adjustments */
    a_dim1 = *lda;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    --ipiv;
    b_dim1 = *ldb;
    b_offset = 1 + b_dim1;
    b -= b_offset;

    /* Function Body */
    *info = 0;
    notran = lsame_(trans, "N", (ftnlen)1, (ftnlen)1);
    if (! notran && ! lsame_(trans, "T", (ftnlen)1, (ftnlen)1) && ! lsame_(
            trans, "C", (ftnlen)1, (ftnlen)1)) {
        *info = -1;
    } else if (*n < 0) {
        *info = -2;
    } else if (*nrhs < 0) {
        *info = -3;
    } else if (*lda < MAX(1,*n)) {
        *info = -5;
    } else if (*ldb < MAX(1,*n)) {
        *info = -8;
    }
    if (*info != 0) {
        i__1 = -(*info);
        xerbla_("CGETRS", &i__1, (ftnlen)6);
        return 0;
    }

/*     Quick return if possible */

    if (*n == 0 || *nrhs == 0) {
        return 0;
    }

    if (notran) {

/*        Solve A * X = B. */

/*        Apply row interchanges to the right hand sides. */

        claswp_(nrhs, &b[b_offset], ldb, &c__1, n, &ipiv[1], &c__1);

/*        Solve L*X = B, overwriting B with X. */

        ctrsm_("Left", "Lower", "No transpose", "Unit", n, nrhs, &c_b1, &a[
                a_offset], lda, &b[b_offset], ldb, (ftnlen)4, (ftnlen)5, (
                ftnlen)12, (ftnlen)4);

/*        Solve U*X = B, overwriting B with X. */

        ctrsm_("Left", "Upper", "No transpose", "Non-unit", n, nrhs, &c_b1, &
                a[a_offset], lda, &b[b_offset], ldb, (ftnlen)4, (ftnlen)5, (
                ftnlen)12, (ftnlen)8);
    } else {

/*        Solve A**T * X = B  or A**H * X = B. */

/*        Solve U'*X = B, overwriting B with X. */

        ctrsm_("Left", "Upper", trans, "Non-unit", n, nrhs, &c_b1, &a[
                a_offset], lda, &b[b_offset], ldb, (ftnlen)4, (ftnlen)5, (
                ftnlen)1, (ftnlen)8);

/*        Solve L'*X = B, overwriting B with X. */

        ctrsm_("Left", "Lower", trans, "Unit", n, nrhs, &c_b1, &a[a_offset], 
                lda, &b[b_offset], ldb, (ftnlen)4, (ftnlen)5, (ftnlen)1, (
                ftnlen)4);

/*        Apply row interchanges to the solution vectors. */

        claswp_(nrhs, &b[b_offset], ldb, &c__1, n, &ipiv[1], &c_n1);
    }

    return 0;

/*     End of CGETRS */

} /* cgetrs_ */

