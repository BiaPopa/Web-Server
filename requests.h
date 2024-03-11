#ifndef _REQUESTS_
#define _REQUESTS_

#include <string>

using namespace std;

// computes and returns a GET request string
char *compute_get_request(char *host, char *url, char *query_params, char *cookies, char *token);

// computes and returns a POST request string
char *compute_post_request(char *host, char *url, char* content_type, string body_data,
							int body_data_fields_count, char *cookies, char *token);

// computes and returns a DELETE request string
char *compute_delete_request(char *host, char *url, char *query_params, char *cookies, char *token);

#endif
