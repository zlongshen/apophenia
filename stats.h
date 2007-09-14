//stats.h	Copyright (c) 2005--2007 by Ben Klemens.  Licensed under the modified GNU GPL v2; see COPYING and COPYING2.
#include <math.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_cdf.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_multimin.h>
#include <gsl/gsl_histogram.h>
#include <gsl/gsl_statistics_double.h>
#include "linear_algebra.h"

#undef __BEGIN_DECLS    /* extern "C" stuff cut 'n' pasted from the GSL. */
#undef __END_DECLS
#ifdef __cplusplus
# define __BEGIN_DECLS extern "C" {
# define __END_DECLS }
#else
# define __BEGIN_DECLS /* empty */
# define __END_DECLS /* empty */
#endif

__BEGIN_DECLS

#define APOP_SUBMATRIX(m, srow, scol, nrows, ncols, o) gsl_matrix apop_mm_##o = gsl_matrix_submatrix(m, (srow), (scol), (nrows),(ncols)).matrix;\
gsl_matrix * o = &( apop_mm_##o );

#define APOP_MATRIX_ROW(m, row, v) gsl_vector apop_vv_##v = gsl_matrix_row(m, (row)).vector;\
gsl_vector * v = &( apop_vv_##v );

#define APOP_MATRIX_COL(m, col, v) gsl_vector apop_vv_##v = gsl_matrix_column(m, (col)).vector;\
gsl_vector * v = &( apop_vv_##v );

#define APOP_ROW(m, row, v) gsl_vector apop_vv_##v = gsl_matrix_row(m->matrix, (row)).vector;\
gsl_vector * v = &( apop_vv_##v );

#define APOP_COL(m, col, v) gsl_vector apop_vv_##v = gsl_matrix_column(m->matrix, (col)).vector;\
gsl_vector * v = &( apop_vv_##v );

	//The following are just convenient hooks to gsl vector functions.
	//var_m lets you input a mean if you've already calculated it, saving
	//some repetition.
inline long double apop_vector_sum(const gsl_vector *in);
inline long double apop_sum(const gsl_vector *in);
inline double apop_vector_mean(const gsl_vector *in);
inline double apop_mean(const gsl_vector *in);
inline double apop_vector_var(const gsl_vector *in);
inline double apop_var(const gsl_vector *in);
inline double apop_vector_var_m(const gsl_vector *in, const double mean);
inline double apop_vector_covar(const gsl_vector *ina, const gsl_vector *inb);
inline double apop_vector_cov(const gsl_vector *ina, const gsl_vector *inb);
inline double apop_vector_correlation(const gsl_vector *ina, const gsl_vector *inb);
inline double apop_vector_kurtosis(const gsl_vector *in);
inline double apop_vector_skew(const gsl_vector *in);
inline double apop_vector_kurt(const gsl_vector *in);
double apop_vector_weighted_mean(const gsl_vector *, const gsl_vector *);
double apop_vector_weighted_var(const gsl_vector *v, const gsl_vector *w);
double apop_vector_weighted_cov(const gsl_vector *, const gsl_vector *, const gsl_vector *);
double apop_vector_weighted_skew(const gsl_vector *v, const gsl_vector *w);
double apop_vector_weighted_kurt(const gsl_vector *v, const gsl_vector *w);

//Distances, Euclidian and Manhattan:
double apop_vector_distance(const gsl_vector *ina, const gsl_vector *inb);
double apop_vector_grid_distance(const gsl_vector *ina, const gsl_vector *inb);


void apop_vector_normalize(gsl_vector *in, gsl_vector **out, const char normalization_type);
void apop_matrix_normalize(gsl_matrix *data, const char row_or_col, const char normalization);

inline double apop_test_chi_squared_var_not_zero(const gsl_vector *in);
	//As described: give it a vector, and it'll tell you the confidence 
	//with which you can say that the vector is not zero.

double apop_random_beta(gsl_rng *r, double m, double v) ;
	/*Give me mean m and variance v, and I'll give you
	 * n draws from the appropriate beta dist.
	 * remember: 0<m<1, and v is tiny (<<1/12). You get NaNs if no
	 * appropriate distribution exists.*/

double apop_multivariate_normal_prob(gsl_vector *x, gsl_vector* mu, gsl_matrix* sigma, int first_use);
	//Evaluate a multivariate normal(mu, sigma) at the point x.
//The equation:
//	exp(-1/2 (X-mu)' sigma^-1 (x-mu))
//	--------------------------
//	sqrt((2 Pi)^n det(sigma))
//
//The inverse and determinant are expensive, so keep them around where possible: on the first call, set 
//first_use to 1, then feed in as many new values of X as you want.

double apop_random_double(double min, double max, gsl_rng *r);
int apop_random_int(const double min, const double max, const gsl_rng *r);

long double apop_matrix_sum(gsl_matrix *m);
double apop_matrix_mean(gsl_matrix *data);
double apop_matrix_var_m(gsl_matrix *data, double mean);
void apop_matrix_mean_and_var(gsl_matrix *data, double *mean, double *var);
double apop_GHgB3_rng(gsl_rng * r, double* a); //in asst.c
apop_data * apop_data_summarize(apop_data *data);
apop_data * apop_matrix_summarize(gsl_matrix *data);
apop_data *apop_data_covar(apop_data *in);

gsl_histogram_pdf * apop_vector_to_pdf(gsl_vector *data, int bins);

int apop_double_is_zero(double in);
void apop_vector_replace(gsl_vector *v, int (* test)(double), double replace_with);
void apop_matrix_replace(gsl_matrix *m, int (* test)(double), double replace_with);

//from apop_fisher.c:
apop_data *apop_test_fisher_exact(apop_data *intab);

//from apop_sort.c:
double * apop_vector_percentiles(gsl_vector *data, char rounding);
apop_data * apop_data_sort(apop_data *data, int sortby, char asc);

__END_DECLS