/*     dpodi computes the determinant and inverse of a certain
 *     double precision symmetric positive definite matrix (see below)
 *     using the factors computed by dpoco, dpofa or dqrdc.
 *
 *     on entry
 *
 *        a       double precision(lda, n)
 *                the output  a  from dpoco or dpofa
 *                or the output  x  from dqrdc.
 *
 *        lda     int
 *                the leading dimension of the array  a .
 *
 *        n       int
 *                the order of the matrix  a .
 *
 *        job     int
 *                = 11   both determinant and inverse.
 *                = 01   inverse only.
 *                = 10   determinant only.
 *
 *     on return
 *
 *        a       if dpoco or dpofa was used to factor  a  then
 *                dpodi produces the upper half of inverse(a) .
 *                if dqrdc was used to decompose  x  then
 *                dpodi produces the upper half of inverse(trans(x)*x)
 *                where trans(x) is the transpose.
 *                elements of  a  below the diagonal are unchanged.
 *                if the units digit of job is zero,  a  is unchanged.
 *
 *        det     double precision(2)
 *                determinant of  a  or of  trans(x)*x  if requested.
 *                otherwise not referenced.
 *                determinant = det(1) * 10.0**det(2)
 *                with  1.0 .le. det(1) .lt. 10.0
 *                or  det(1) .eq. 0.0 .
 *
 *     error condition
 *
 *        a division by zero will occur if the input factor contains
 *        a zero on the diagonal and the inverse is requested.
 *        it will not occur if the subroutines are called correctly
 *        and if dpoco or dpofa has set info .eq. 0 .
 *
 *     linpack.  this version dated 08/14/78 .
 *     cleve moler, university of new mexico, argonne national lab.
 *
 *     subroutines and functions
 *
 *     blas daxpy,dscal
 *     fortran mod
 */

#include "Fortran.h"
#include "Blas.h"
#include "Linpack.h"

static int c__1 = 1;

int 
F77_SYMBOL(dpodi) (double *a, int *lda, int *n,
		   double *det, int *job)
{
	double d__1;
	double s, t;
	/*
	extern int F77_SYMBOL(dscal) (), F77_SYMBOL(daxpy) ();
	*/
	int a_dim1, a_offset, i__1, i__2;
	int i, j, k;
	int jm1, kp1;

	a_dim1 = *lda;
	a_offset = a_dim1 + 1;
	a -= a_offset;
	--det;

	/* compute determinant */

	if (*job / 10 == 0) {
		goto L70;
	}
	det[1] = 1.;
	det[2] = 0.;
	s = 10.;
	i__1 = *n;
	for (i = 1; i <= i__1; ++i) {
		d__1 = a[i + i * a_dim1];
		det[1] = d__1 * d__1 * det[1];

		/* ...exit */

		if (det[1] == 0.) {
			goto L60;
		}
L10:
		if (det[1] >= 1.) {
			goto L20;
		}
		det[1] = s * det[1];
		det[2] += -1.;
		goto L10;
L20:
L30:
		if (det[1] < s) {
			goto L40;
		}
		det[1] /= s;
		det[2] += 1.;
		goto L30;
L40:
		;
	}
L60:
L70:

	/* compute inverse(r) */

	if (*job % 10 == 0) {
		goto L140;
	}
	i__1 = *n;
	for (k = 1; k <= i__1; ++k) {
		a[k + k * a_dim1] = 1. / a[k + k * a_dim1];
		t = -a[k + k * a_dim1];
		i__2 = k - 1;
		F77_SYMBOL(dscal) (&i__2, &t, &a[k * a_dim1 + 1], &c__1);
		kp1 = k + 1;
		if (*n < kp1) {
			goto L90;
		}
		i__2 = *n;
		for (j = kp1; j <= i__2; ++j) {
			t = a[k + j * a_dim1];
			a[k + j * a_dim1] = 0.;
			F77_SYMBOL(daxpy) (&k, &t, &a[k * a_dim1 + 1], &c__1, &a[j * a_dim1 + 1], &
					   c__1);
		}
L90:
		;
	}

	/* form  inverse(r) * trans(inverse(r)) */

	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
		jm1 = j - 1;
		if (jm1 < 1) {
			goto L120;
		}
		i__2 = jm1;
		for (k = 1; k <= i__2; ++k) {
			t = a[k + j * a_dim1];
			F77_SYMBOL(daxpy) (&k, &t, &a[j * a_dim1 + 1], &c__1, &a[k * a_dim1 + 1], &
					   c__1);
		}
L120:
		t = a[j + j * a_dim1];
		F77_SYMBOL(dscal) (&j, &t, &a[j * a_dim1 + 1], &c__1);
	}
L140:
	return 0;
}
