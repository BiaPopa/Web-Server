#include "requests.h"
#include "helpers.h"
#include <arpa/inet.h>
#include <netdb.h>     
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>     
#include <string.h>     
#include <sys/socket.h> 
#include <unistd.h>
#include <iostream>

using namespace std;

char *compute_get_request(char *host, char *url, char *query_params, char *cookies, char *token) {
	char *message = (char*)calloc(BUFLEN, sizeof(char));
	char *line = (char*)calloc(LINELEN, sizeof(char));

  	if (query_params != NULL) {
		sprintf(line, "GET %s?%s HTTP/1.1", url, query_params);
  	} else {
		sprintf(line, "GET %s HTTP/1.1", url);
  	}

  	compute_message(message, line);

  	sprintf(line, "Host: %s", host);
  	compute_message(message, line);

  	if (cookies != NULL) {
		memset(line, 0, LINELEN);
		strcat(line, "Cookie: ");
  
		strcat(line, cookies);
		strcat(line, "; ");

		compute_message(message, line);
  	}

  	if (token != NULL) {
		sprintf(line, "Authorization: Bearer %s", token);
		compute_message(message, line);
  	}

  	compute_message(message, "");
  	free(line);
  	return message;
}

char *compute_post_request(char *host, char *url, char *content_type,
						   string body_data, int body_data_fields_count,
						   char *cookies, char *token) {
	char *message = (char*)calloc(BUFLEN, sizeof(char));
	char *line = (char*)calloc(LINELEN, sizeof(char));

	sprintf(line, "POST %s HTTP/1.1", url);
	compute_message(message, line);

	sprintf(line, "Host: %s", host);
	compute_message(message, line);

  	sprintf(line, "Content-Type: %s", content_type);
  	compute_message(message, line);
  
  	sprintf(line, "Content-Length: %lu", body_data.length());
  	compute_message(message, line);

  	if (cookies != NULL) {
		memset(line, 0, LINELEN);
		strcat(line, "Cookie: ");
		
		strcat(line, cookies);
		strcat(line, ";");

		compute_message(message, line);
  	}

	if (token != NULL) {
		sprintf(line, "Authorization: Bearer %s", token);
		compute_message(message, line);
  	}

	compute_message(message, "");
	compute_message(message, body_data.c_str());

  	free(line);
  	return message;
}

char *compute_delete_request(char *host, char *url, char *query_params, char *cookies, char *token) {
	char *message = (char*)calloc(BUFLEN, sizeof(char));
	char *line = (char*)calloc(LINELEN, sizeof(char));

  	if (query_params != NULL) {
		sprintf(line, "DELETE %s?%s HTTP/1.1", url, query_params);
  	} else {
		sprintf(line, "DELETE %s HTTP/1.1", url);
  	}

  	compute_message(message, line);

  	sprintf(line, "Host: %s", host);
  	compute_message(message, line);

  	if (cookies != NULL) {
		memset(line, 0, LINELEN);
		strcat(line, "Cookie: ");
  
		strcat(line, cookies);
		strcat(line, "; ");

		compute_message(message, line);
  	}

  	if (token != NULL) {
		sprintf(line, "Authorization: Bearer %s", token);
		compute_message(message, line);
  	}

  	compute_message(message, "");
  	free(line);
  	return message;
}