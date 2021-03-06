/* ../../../dependencies/lapack/src/zheev.f -- translated by f2c (version 20061008).
   You must link the resulting object file with libf2c:
        on Microsoft Windows system, link with libf2c.lib;
        on Linux or Unix systems, link with .../path/to/libf2c.a -lm
        or, if you install libf2c.a in a standard place, with -lf2c -lm
        -- in that order, at the end of the command line, as in
                cc *.o -lf2c -lm
        Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

                http://www.netlib.org/f2c/libf2c.zip
*/




/* Subroutine */ static int zheev_(char *jobz, char *uplo, integer *n, dcomplex 
        *a, integer *lda, doublereal *w, dcomplex *work, integer *lwork, 
        doublereal *rwork, integer *info, ftnlen jobz_len, ftnlen uplo_len)
{
/* Table of constant values */

static integer c__1 = 1;
static integer c_n1 = -1;
static integer c__0 = 0;
static doublereal c_b18 = 1.;
    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2;
    doublereal d__1;
    dcomplex z__1;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static integer nb;
    static doublereal eps;
    static integer inde;
    static doublereal anrm;
    static integer imax;
    static doublereal rmin, rmax;
    static integer lopt;
     /* Subroutine */ int dscal_(integer *, doublereal *, doublereal *, 
            integer *);
    static doublereal sigma;
     logical lsame_(char *, char *, ftnlen, ftnlen);
    static integer iinfo;
    static logical lower, wantz;
     doublereal dlamch_(char *, ftnlen);
    static integer iscale;
    static doublereal safmin;
     integer ilaenv_(integer *, char *, char *, integer *, integer *, 
            integer *, integer *, ftnlen, ftnlen);
     /* Subroutine */ int xerbla_(char *, integer *, ftnlen);
    static doublereal bignum;
     doublereal zlanhe_(char *, char *, integer *, dcomplex *, 
            integer *, doublereal *, ftnlen, ftnlen);
    static integer indtau;
     /* Subroutine */ int dsterf_(integer *, doublereal *, doublereal *,
             integer *), zlascl_(char *, integer *, integer *, doublereal *, 
            doublereal *, integer *, integer *, dcomplex *, integer *, 
            integer *, ftnlen);
    static integer indwrk;
     /* Subroutine */ int zhetrd_(char *, integer *, dcomplex *, 
            integer *, doublereal *, doublereal *, dcomplex *, 
            dcomplex *, integer *, integer *, ftnlen);
    static integer llwork;
    static doublereal smlnum;
    static integer lwkopt;
    static logical lquery;
     /* Subroutine */ int zsteqr_(char *, integer *, doublereal *, 
            doublereal *, dcomplex *, integer *, doublereal *, integer *,
             ftnlen), zungtr_(char *, integer *, dcomplex *, integer *, 
            dcomplex *, dcomplex *, integer *, integer *, ftnlen);


/*  -- LAPACK driver routine (version 3.0) -- */
/*     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd., */
/*     Courant Institute, Argonne National Lab, and Rice University */
/*     June 30, 1999 */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  ZHEEV computes all eigenvalues and, optionally, eigenvectors of a */
/*  scomplex Hermitian matrix A. */

/*  Arguments */
/*  ========= */

/*  JOBZ    (input) CHARACTER*1 */
/*          = 'N':  Compute eigenvalues only; */
/*          = 'V':  Compute eigenvalues and eigenvectors. */

/*  UPLO    (input) CHARACTER*1 */
/*          = 'U':  Upper triangle of A is stored; */
/*          = 'L':  Lower triangle of A is stored. */

/*  N       (input) INTEGER */
/*          The order of the matrix A.  N >= 0. */

/*  A       (input/output) COMPLEX*16 array, dimension (LDA, N) */
/*          On entry, the Hermitian matrix A.  If UPLO = 'U', the */
/*          leading N-by-N upper triangular part of A contains the */
/*          upper triangular part of the matrix A.  If UPLO = 'L', */
/*          the leading N-by-N lower triangular part of A contains */
/*          the lower triangular part of the matrix A. */
/*          On exit, if JOBZ = 'V', then if INFO = 0, A contains the */
/*          orthonormal eigenvectors of the matrix A. */
/*          If JOBZ = 'N', then on exit the lower triangle (if UPLO='L') */
/*          or the upper triangle (if UPLO='U') of A, including the */
/*          diagonal, is destroyed. */

/*  LDA     (input) INTEGER */
/*          The leading dimension of the array A.  LDA >= MAX(1,N). */

/*  W       (output) DOUBLE PRECISION array, dimension (N) */
/*          If INFO = 0, the eigenvalues in ascending order. */

/*  WORK    (workspace/output) COMPLEX*16 array, dimension (LWORK) */
/*          On exit, if INFO = 0, WORK(1) returns the optimal LWORK. */

/*  LWORK   (input) INTEGER */
/*          The length of the array WORK.  LWORK >= MAX(1,2*N-1). */
/*          For optimal efficiency, LWORK >= (NB+1)*N, */
/*          where NB is the blocksize for ZHETRD returned by ILAENV. */

/*          If LWORK = -1, then a workspace query is assumed; the routine */
/*          only calculates the optimal size of the WORK array, returns */
/*          this value as the first entry of the WORK array, and no error */
/*          message related to LWORK is issued by XERBLA. */

/*  RWORK   (workspace) DOUBLE PRECISION array, dimension (MAX(1, 3*N-2)) */

/*  INFO    (output) INTEGER */
/*          = 0:  successful exit */
/*          < 0:  if INFO = -i, the i-th argument had an illegal value */
/*          > 0:  if INFO = i, the algorithm failed to converge; i */
/*                off-diagonal elements of an intermediate tridiagonal */
/*                form did not converge to zero. */

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
    --w;
    --work;
    --rwork;

    /* Function Body */
    wantz = lsame_(jobz, "V", (ftnlen)1, (ftnlen)1);
    lower = lsame_(uplo, "L", (ftnlen)1, (ftnlen)1);
    lquery = *lwork == -1;

    *info = 0;
    if (! (wantz || lsame_(jobz, "N", (ftnlen)1, (ftnlen)1))) {
        *info = -1;
    } else if (! (lower || lsame_(uplo, "U", (ftnlen)1, (ftnlen)1))) {
        *info = -2;
    } else if (*n < 0) {
        *info = -3;
    } else if (*lda < MAX(1,*n)) {
        *info = -5;
    } else /* if(complicated condition) */ {
/* Computing MAX */
        i__1 = 1, i__2 = (*n << 1) - 1;
        if (*lwork < MAX(i__1,i__2) && ! lquery) {
            *info = -8;
        }
    }

    if (*info == 0) {
        nb = ilaenv_(&c__1, "ZHETRD", uplo, n, &c_n1, &c_n1, &c_n1, (ftnlen)6,
                 (ftnlen)1);
/* Computing MAX */
        i__1 = 1, i__2 = (nb + 1) * *n;
        lwkopt = MAX(i__1,i__2);
        work[1].r = (doublereal) lwkopt, work[1].i = 0.;
    }

    if (*info != 0) {
        i__1 = -(*info);
        xerbla_("ZHEEV ", &i__1, (ftnlen)6);
        return 0;
    } else if (lquery) {
        return 0;
    }

/*     Quick return if possible */

    if (*n == 0) {
        work[1].r = 1., work[1].i = 0.;
        return 0;
    }

    if (*n == 1) {
        i__1 = a_dim1 + 1;
        w[1] = a[i__1].r;
        work[1].r = 3., work[1].i = 0.;
        if (wantz) {
            i__1 = a_dim1 + 1;
            a[i__1].r = 1., a[i__1].i = 0.;
        }
        return 0;
    }

/*     Get machine constants. */

    safmin = dlamch_("Safe minimum", (ftnlen)12);
    eps = dlamch_("Precision", (ftnlen)9);
    smlnum = safmin / eps;
    bignum = 1. / smlnum;
    rmin = sqrt(smlnum);
    rmax = sqrt(bignum);

/*     Scale matrix to allowable range, if necessary. */

    anrm = zlanhe_("M", uplo, n, &a[a_offset], lda, &rwork[1], (ftnlen)1, (
            ftnlen)1);
    iscale = 0;
    if (anrm > 0. && anrm < rmin) {
        iscale = 1;
        sigma = rmin / anrm;
    } else if (anrm > rmax) {
        iscale = 1;
        sigma = rmax / anrm;
    }
    if (iscale == 1) {
        zlascl_(uplo, &c__0, &c__0, &c_b18, &sigma, n, n, &a[a_offset], lda, 
                info, (ftnlen)1);
    }

/*     Call ZHETRD to reduce Hermitian matrix to tridiagonal form. */

    inde = 1;
    indtau = 1;
    indwrk = indtau + *n;
    llwork = *lwork - indwrk + 1;
    zhetrd_(uplo, n, &a[a_offset], lda, &w[1], &rwork[inde], &work[indtau], &
            work[indwrk], &llwork, &iinfo, (ftnlen)1);
    i__1 = indwrk;
    z__1.r = *n + work[i__1].r, z__1.i = work[i__1].i;
    lopt = (integer) z__1.r;

/*     For eigenvalues only, call DSTERF.  For eigenvectors, first call */
/*     ZUNGTR to generate the unitary matrix, then call ZSTEQR. */

    if (! wantz) {
        dsterf_(n, &w[1], &rwork[inde], info);
    } else {
        zungtr_(uplo, n, &a[a_offset], lda, &work[indtau], &work[indwrk], &
                llwork, &iinfo, (ftnlen)1);
        indwrk = inde + *n;
        zsteqr_(jobz, n, &w[1], &rwork[inde], &a[a_offset], lda, &rwork[
                indwrk], info, (ftnlen)1);
    }

/*     If matrix was scaled, then rescale eigenvalues appropriately. */

    if (iscale == 1) {
        if (*info == 0) {
            imax = *n;
        } else {
            imax = *info - 1;
        }
        d__1 = 1. / sigma;
        dscal_(&imax, &d__1, &w[1], &c__1);
    }

/*     Set WORK(1) to optimal scomplex workspace size. */

    work[1].r = (doublereal) lwkopt, work[1].i = 0.;

    return 0;

/*     End of ZHEEV */

} /* zheev_ */

