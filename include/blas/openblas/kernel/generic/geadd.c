/***************************************************************************
Copyright (c) 2013, The OpenBLAS Project
All rights reserved.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:
1. Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in
the documentation and/or other materials provided with the
distribution.
3. Neither the name of the OpenBLAS project nor the names of
its contributors may be used to endorse or promote products
derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE OPENBLAS PROJECT OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/




int CNAME(BLASLONG rows, BLASLONG cols, FLOAT alpha, FLOAT *a, BLASLONG lda, FLOAT beta, FLOAT *b, BLASLONG ldb)
{
	BLASLONG i;
	FLOAT *aptr,*bptr;

	if ( rows <= 0     )  return(0);
	if ( cols <= 0     )  return(0);

	
	aptr = a;
	bptr = b;

	if ( alpha == 0.0 )
	{
		for ( i=0; i<cols ; i++ )
		{
			SCAL_K(rows, 0,0, beta, bptr, 1,  NULL, 0,NULL,0); 
			bptr+=ldb; 
		}

		return(0);
	}

	for (i = 0; i < cols; i++) {
		AXPBY_K(rows, alpha, aptr, 1, beta, bptr, 1); 
		aptr += lda; 
		bptr += ldb; 
	}

	return(0);

}


