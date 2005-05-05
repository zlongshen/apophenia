//likelihoods.h			  	Copyright 2005 by Ben Klemens. Licensed under the GNU GPL.
#include <gsl/gsl_cdf.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_multimin.h>
#include <gsl/gsl_sf_gamma.h>
#include <gsl/gsl_sf_psi.h>
#include "linear_algebra.h"
#include "conversions.h"
#include "estimate.h"

#define MAX_ITERATIONS 		500
#define MAX_ITERATIONS_w_d	500

apop_estimate * apop_mle_probit(gsl_matrix *data, double *starting_pt, 
					double step_size, apop_inventory *uses, int verbose);
apop_estimate * apop_mle_gamma(gsl_matrix *data, double *starting_pt, 
					double step_size, apop_inventory *uses, int verbose);
double apop_probit_likelihood(const gsl_vector *beta, void *d);

apop_estimate * apop_mle_waring(gsl_matrix *data, double *starting_pt, 
					double step_size, apop_inventory *uses, int verbose);
apop_estimate * apop_mle_yule(gsl_matrix *data, double *starting_pt, 
					double step_size, apop_inventory *uses, int verbose);
apop_estimate * apop_mle_zipf(gsl_matrix *data, double *starting_pt, 
					double step_size, apop_inventory *uses, int verbose);
double apop_gamma_likelihood(const gsl_vector *beta, void *d);
double apop_waring_likelihood(const gsl_vector *beta, void *d);
double apop_yule_likelihood(const gsl_vector *beta, void *d);
double apop_zipf_likelihood(const gsl_vector *beta, void *d);
/*
For the Probit, the first column of the data matrix is the dependent
variable, and the remaining variables are the independent. This means
that the beta which will be output will be of size (data->size2 - 1).

For the Waring, Yule, and Zipf estimates, each row of the data matrix
is one observation. The first column is the number of elements with one
link, the second is the number of elements with two links, et cetera.

If you want the total likelihood, likelihood should be a double*; else,
send in NULL and get nothing back.

starting_pt is a vector of the appropriate size which indicates your
best initial guess for beta. if starting_pt=NULL, then (0,0,...0) will
be assumed.

step_size is the scale of the initial steps the maximization algorithm
will take. Currently, it is a scalar, so every dimension will have the
same step_size.

verbose is zero or one depending on whether you want to see the
maximizer's iterations.

For each function, the return value is the vector of most likely parameters.

Sample usage:

gsl_vector * 	waring_parameter; 	//do not allocate.
double 		starting_pt[2] = {3, 0};
double		likelihood;
waring_parameter	= mle_waring(data, &likelihood, starting_pt, .01, 0);
printf("Your most likely waring parameter is %g, with likelihood %g", 
				gsl_vector_get(waring_parameter, 0), likelihood);
gsl_vector_free(waring_parameter); 	//Don't forget to clean up when you're done.

*/

void apop_make_likelihood_vector(gsl_matrix *m, gsl_vector **v, 
				double (*likelihood_fn)(const gsl_vector *beta, void *d), gsl_vector* fn_beta);
/*This function goes row by row through m and calculates the likelihood
  of the given row, putting the result in v. You will need this to find
  the variance of the estimator via some means.
  The likelihood function can be any of &apop_xxx_likelihood from above, 
  and fn_beta will probably be the beta calculated using the corresponding
  apop_xxx_mle function.
  */
