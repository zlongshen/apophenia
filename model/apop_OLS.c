/** \file apop_OLS.c

  OLS models. Much of the real work is done in regression.c.

Copyright (c) 2005--2007 by Ben Klemens.  Licensed under the modified GNU GPL v2; see COPYING and COPYING2.  */

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
static double ols_log_likelihood (const apop_data *beta, apop_data *d, apop_model *p){ 
int         i; 
long double	total_prob  = 0; 
double      sigma, expected, actual;
gsl_matrix	*data		    = d->matrix;
gsl_vector  v;
gsl_vector  *errors         = gsl_vector_alloc(data->size1);
	for(i=0;i< data->size1; i++){
        v            = gsl_matrix_row(data, i).vector;
        gsl_blas_ddot(beta->vector, &v, &expected);
        actual       = gsl_matrix_get(data,i, 0);
        expected    += gsl_vector_get(beta->vector,0) * (1 - actual); //data isn't affine.
        gsl_vector_set(errors, i, expected-actual);
    }
    sigma   = sqrt(apop_vector_var(errors));
	for(i=0;i< data->size1; i++){
        total_prob  += log(gsl_ran_gaussian_pdf(gsl_vector_get(errors, i), sigma));
	} 
    gsl_vector_free(errors);
    return total_prob;
}

static double ols_p (const apop_data *beta, apop_data *d, apop_model *p){ 
    return exp(ols_log_likelihood(beta, d, p));
            }

/** The OLS model

  This is basically a wrapper for the OLS regression function, \ref apop_params_OLS.
\ingroup models
*/
//apop_model apop_OLS = {"OLS", -1,0,0, apop_estimate_OLS, ols_p, ols_log_likelihood, NULL, NULL, NULL};
apop_model apop_OLS = {.name="OLS", .vbase = -1, .estimate =apop_estimate_OLS, .p=ols_p,
                            .log_likelihood = ols_log_likelihood};

/** The GLS model

  This is basically a wrapper for the GLS regression function, \ref apop_params_GLS.
\ingroup models
*/
//apop_model apop_GLS = {"GLS", -1, apop_params_GLS, NULL, NULL, NULL, NULL, NULL};