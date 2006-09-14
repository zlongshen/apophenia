/** \file apop_OLS.c

  OLS models. Much of the real work is done in regression.c.

Copyright (c) 2005 by Ben Klemens. Licensed under the GNU GPL version 2.
*/

#include "model.h"
#include "regression.h"
#include "stats.h"
#include <math.h>
#include <assert.h>
#include <gsl/gsl_blas.h>

/** The assumption that makes a log likelihood possible is that the
errors are normally distributed.

This function is a bit inefficient, in that it calculates the error terms,
which you may have already done in the OLS estimation.

 */
static double ols_log_likelihood (const gsl_vector *beta, apop_data *d){ 
int         i; 
long double	total_prob  = 0; 
double      sigma, expected, actual;
gsl_matrix	*data		    = d->matrix;
gsl_vector  v;
gsl_vector  *errors         = gsl_vector_alloc(data->size1);
	for(i=0;i< data->size1; i++){
        v            = gsl_matrix_row(data, i).vector;
        gsl_blas_ddot(beta, &v, &expected);
        actual       = gsl_matrix_get(data,i, 0);
        expected    += gsl_vector_get(beta,0) * (1 - actual); //data isn't affine.
        gsl_vector_set(errors, i, expected-actual);
    }
    sigma   = sqrt(apop_vector_var(errors));
	for(i=0;i< data->size1; i++){
        total_prob  += log(gsl_ran_gaussian_pdf(gsl_vector_get(errors, i), sigma));
	} 
    gsl_vector_free(errors);
    return total_prob;
}


/** The OLS model

  This is basically a wrapper for the OLS regression function, \ref apop_estimate_OLS.
\ingroup models
*/
apop_model apop_OLS = {"OLS", -1, 
	apop_estimate_OLS, ols_log_likelihood, NULL, NULL, NULL, NULL};

/** The GLS model

  This is basically a wrapper for the GLS regression function, \ref apop_estimate_GLS.
\ingroup models
*/
apop_model apop_GLS = {"GLS", -1, 
	apop_estimate_GLS, NULL, NULL, NULL, NULL, NULL};