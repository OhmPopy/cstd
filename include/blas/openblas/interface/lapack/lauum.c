


#ifdef XDOUBLE
#define ERROR_NAME "QLAUUM"
#elif defined(DOUBLE)
#define ERROR_NAME "DLAUUM"
#else
#define ERROR_NAME "SLAUUM"
#endif

static blasint (*lauum_single[])(blas_arg_t *, BLASLONG *, BLASLONG *, FLOAT *, FLOAT *, BLASLONG) = {
  LAUUM_U_SINGLE, LAUUM_L_SINGLE,
};

#ifdef SMP
static blasint (*lauum_parallel[])(blas_arg_t *, BLASLONG *, BLASLONG *, FLOAT *, FLOAT *, BLASLONG) = {
  LAUUM_U_PARALLEL, LAUUM_L_PARALLEL,
};
#endif

int NAME(char *UPLO, blasint *N, FLOAT *a, blasint *ldA, blasint *Info){

  blas_arg_t args;

  blasint uplo_arg = *UPLO;
  blasint uplo;
  blasint info;
  FLOAT *buffer;
#ifdef PPC440
  extern
#endif
  FLOAT *sa, *sb;

  PRINT_DEBUG_NAME;

  args.n    = *N;
  args.a    = (void *)a;
  args.lda  = *ldA;

  TOUPPER(uplo_arg);

  uplo = -1;
  if (uplo_arg == 'U') uplo = 0;
  if (uplo_arg == 'L') uplo = 1;

  info  = 0;
  if (args.lda < MAX(1,args.n)) info = 4;
  if (args.n   < 0)             info = 2;
  if (uplo     < 0)             info = 1;
  if (info) {
    BLASFUNC(xerbla)(ERROR_NAME, &info, sizeof(ERROR_NAME));
    *Info = - info;
    return 0;
  }

  *Info = 0;

  if (args.n == 0) return 0;

  IDEBUG_START;

  FUNCTION_PROFILE_START();

#ifndef PPC440
  buffer = (FLOAT *)blas_memory_alloc(1);

  sa = (FLOAT *)((BLASLONG)buffer + GEMM_OFFSET_A);
  sb = (FLOAT *)(((BLASLONG)sa + ((GEMM_P * GEMM_Q * COMPSIZE * SIZE + GEMM_ALIGN) & ~GEMM_ALIGN)) + GEMM_OFFSET_B);
#endif

#ifdef SMP
  args.common = NULL;
  args.nthreads = num_cpu_avail(4);

  if (args.nthreads == 1) {
#endif

    *Info = (lauum_single[uplo])(&args, NULL, NULL, sa, sb, 0);

#ifdef SMP
  } else {
    *Info = (lauum_parallel[uplo])(&args, NULL, NULL, sa, sb, 0);
  }
#endif

#ifndef PPC440
  blas_memory_free(buffer);
#endif

  FUNCTION_PROFILE_END(1, .5 * args.n * args.n,
		       args.n * (1./3. + args.n * ( 1./2. + args.n * 1./6.))
		       +  1./6. * args.n * (args.n * args.n - 1));

  IDEBUG_END;

  return 0;
}
