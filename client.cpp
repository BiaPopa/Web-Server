#include "helpers.h"
#include "requests.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <nlohmann/json.hpp>
#define MAX_LEN 1<<16

using namespace std;
using json = nlohmann::json;

int main(int argc, char *argv[]) {
	// datele de conectare
	char host[16] = "34.254.242.81";
	// host:port
	char host_port[21] = "34.254.242.81:8080";
	// ruta de acces
	char access[100];
	// tip payload
	char payload[100];
	// cookie pentru a demonstra autentificarea
	char cookies[MAX_LEN];
	// token JWT pentru a demonstra accesul la biblioteca
	char token[MAX_LEN];
	char command[50];

	while (1) {
		cin.getline(command, 50);

		if (strcmp(command, "login") == 0) {
			strcpy(access, "/api/v1/tema/auth/login");
			strcpy(payload, "application/json");

			// citirea si formatarea datelor de intrare
			char username[50], password[50];
			cout<<"username=";
			cin.getline(username, 50);
			cout<<"password=";
			cin.getline(password, 50);

			json form_data;
			form_data["username"] = username;
			form_data["password"] = password;

			// deschiderea unui socket pentru comunicarea cu serverul
			int sockfd = open_connection(host, 8080, AF_INET, SOCK_STREAM, 0);
			if (sockfd == -1) {
				return -1;
			}

			char *request, *response;
			// formarea cererii
			request = compute_post_request(host_port, access, payload, form_data.dump(), 2, NULL, NULL);
			// trimiterea cererii
			send_to_server(sockfd, request);
			
			// primirea raspunsului
			response = receive_from_server(sockfd);
			if (strncmp(response, "HTTP/1.1 400 Bad Request", 24) == 0) {
			 	cout<<"Invalid credentials.\n";
			} else {
			 	cout<<"200 - OK\n";

				// parsarea raspunsului pentru a obtine cookie
				char *p = strtok(response, "\n ;");
				while (p != NULL) {
					if (strcmp(p, "Set-Cookie:") == 0) {
						p = strtok(NULL, "\n ;");
						strcpy(cookies, p);
						break;
					}
					p = strtok(NULL, "\n ;");
				}
			}

			free(request);
			free(response);
			// inchiderea conexiunii
			close_connection(sockfd);
		} else if (strcmp(command, "register") == 0) {
			strcpy(access, "/api/v1/tema/auth/register");
			strcpy(payload, "application/json");

			// citirea si formatarea datelor de intrare
			char username[50], password[50];
			cout<<"username=";
			cin.getline(username, 50);
			cout<<"password=";
			cin.getline(password, 50);

			json form_data;
			form_data["username"] = username;
			form_data["password"] = password;

			// deschiderea unui socket pentru comunicarea cu serverul
			int sockfd = open_connection(host, 8080, AF_INET, SOCK_STREAM, 0);
			if (sockfd == -1) {
				return -1;
			}

			char *request, *response;
			// formarea cererii
			request = compute_post_request(host_port, access, payload, form_data.dump(), 2, NULL, NULL);
			// trimiterea cererii
			send_to_server(sockfd, request);
			
			// primirea raspunsului
			response = receive_from_server(sockfd);
			if (strncmp(response, "HTTP/1.1 400 Bad Request", 24) == 0) {
			 	cout<<"Username is taken.\n";
			} else {
			 	cout<<"200 - OK\n";
			}

			free(request);
			free(response);
			// inchiderea conexiunii
			close_connection(sockfd);
		} else if (strcmp(command, "enter_library") == 0) {
			strcpy(access, "/api/v1/tema/library/access");
			
			// deschiderea unui socket pentru comunicarea cu serverul
			int sockfd = open_connection(host, 8080, AF_INET, SOCK_STREAM, 0);
			if (sockfd == -1) {
				return -1;
			}

			char *request, *response;
			// formarea cererii
			request = compute_get_request(host_port, access, NULL, cookies, NULL);
			// trimiterea cererii
			send_to_server(sockfd, request);
  			
			// primirea raspunsului
  			response = receive_from_server(sockfd);
			if (strncmp(response, "HTTP/1.1 401 Unauthorized", 24) == 0) {
				cout<<"Unauthorized.\n";
			} else {
				cout<<"200 - OK\n";

				// parsarea raspunsului pentru a obtine token
  				char *p = strtok(response, " ;\"");
				while (p != NULL) {
					if (strcmp(p, ":") == 0) {
						p = strtok(NULL, " ;\"");
						strcpy(token, p);
						break;
					}
					p = strtok(NULL, " ;\"");
				}
			}

			free(request);
  			free(response);
			// inchiderea conexiunii
			close_connection(sockfd);
		} else if (strcmp(command, "get_books") == 0) {
			strcpy(access, "/api/v1/tema/library/books");

			// deschiderea unui socket pentru comunicarea cu serverul
			int sockfd = open_connection(host, 8080, AF_INET, SOCK_STREAM, 0);
			if (sockfd == -1) {
				return -1;
			}

			char *request, *response;
			// formarea cererii
			request = compute_get_request(host_port, access, NULL, cookies, token);
			// trimiterea cererii
			send_to_server(sockfd, request);
  			
			// primirea raspunsului
  			response = receive_from_server(sockfd);
			if (strncmp(response, "HTTP/1.1 500 Internal Server Error", 34) == 0) {
				cout<<"Forbidden access.\n";
			} else {
				// parsarea raspunsului pentru a obtine json-ul cu cartile
				char final_response[MAX_LEN] = "[";
				char *p = strtok(response, "[");
				p = strtok(NULL, "]");
				if (p != NULL) {
					strcat(final_response, p);
				}
				strcat(final_response, "]");
				
  				json j = json::parse(final_response);
				cout<<j.dump(4)<<"\n";
			}

			free(request);
  			free(response);
			// inchiderea conexiunii
			close_connection(sockfd);
		} else if (strcmp(command, "get_book") == 0) {
			// citirea si validarea datelor de intrare
			char id[15];
			cout<<"id=";
			cin.getline(id, 15);

			int is_int = true;
			for (unsigned int i = 0; i < strlen(id); i++) {
				if (strchr("0123456789", id[i]) == NULL) {
					is_int = false;
					break;
				}
			}

			if (is_int == false) {
				cout<<"Invalid id.\n";
				continue;
			}

			strcpy(access, "/api/v1/tema/library/books/");
			strcat(access, id);

			// deschiderea unui socket pentru comunicarea cu serverul
			int sockfd = open_connection(host, 8080, AF_INET, SOCK_STREAM, 0);
			if (sockfd == -1) {
				return -1;
			}

			char *request, *response;
			// formarea cererii
			request = compute_get_request(host_port, access, NULL, cookies, token);
			// trimiterea cererii
			send_to_server(sockfd, request);
  			
			// primirea raspunsului
  			response = receive_from_server(sockfd);
			if (strncmp(response, "HTTP/1.1 500 Internal Server Error", 34) == 0) {
				cout<<"Forbidden access.\n";
			} else if (strncmp(response, "HTTP/1.1 404 Not Found", 22) == 0) {
				cout<<"No book was found.\n";
			} else {
				// parsarea raspunsului pentru a obtine json-ul cu cartea ceruta
				char final_response[MAX_LEN] = "{";
				char *p = strtok(response, "{");
				p = strtok(NULL, "}");
				if (p != NULL) {
					strcat(final_response, p);
				}
				strcat(final_response, "}");
				
  				json j = json::parse(final_response);
				cout<<j.dump(4)<<"\n";
			}

			free(request);
  			free(response);
			// inchiderea conexiunii
			close_connection(sockfd);
		} else if (strcmp(command, "add_book") == 0) {
			// citirea, validarea si formatarea datelor de intrare
			char title[100], author[100], genre[100], publisher[100], page_count[100];
			cout<<"title=";
			cin.getline(title, 100);
			cout<<"author=";
			cin.getline(author, 100);
			cout<<"genre=";
			cin.getline(genre, 100);
			cout<<"publisher=";
			cin.getline(publisher, 100);
			cout<<"page_count=";
			cin.getline(page_count, 100);

			int is_int = true;
			for (unsigned int i = 0; i < strlen(page_count); i++) {
				if (strchr("0123456789", page_count[i]) == NULL) {
					is_int = false;
					break;
				}
			}

			if (is_int == false) {
				cout<<"Invalid page count.\n";
				continue;
			}

			strcpy(access, "/api/v1/tema/library/books");
			strcpy(payload, "application/json");

			json form_data;
			form_data["title"] = title;
			form_data["author"] = author;
			form_data["genre"] = genre;
			form_data["page_count"] = page_count;
			form_data["publisher"] = publisher;
			
			// deschiderea unui socket pentru comunicarea cu serverul
			int sockfd = open_connection(host, 8080, AF_INET, SOCK_STREAM, 0);
			if (sockfd == -1) {
				return -1;
			}

			char *request, *response;
			// formarea cererii
			request = compute_post_request(host_port, access, payload, form_data.dump(), 2, cookies, token);
			// trimiterea cererii
			send_to_server(sockfd, request);
  			
			// primirea raspunsului
  			response = receive_from_server(sockfd);
			if (strncmp(response, "HTTP/1.1 500 Internal Server Error", 34) == 0) {
				cout<<"Forbidden access.\n";
			} else {
				cout<<"200 - OK\n";
			}

			free(request);
  			free(response);
			// inchiderea conexiunii
			close_connection(sockfd);
		} else if (strcmp(command, "delete_book") == 0) {
			// citirea si validarea datelor de intrare
			char id[15];
			cout<<"id=";
			cin.getline(id, 15);

			int is_int = true;
			for (unsigned int i = 0; i < strlen(id); i++) {
				if (strchr("0123456789", id[i]) == NULL) {
					is_int = false;
					break;
				}
			}

			if (is_int == false) {
				cout<<"Invalid id.\n";
				continue;
			}

			strcpy(access, "/api/v1/tema/library/books/");
			strcat(access, id);

			// deschiderea unui socket pentru comunicarea cu serverul
			int sockfd = open_connection(host, 8080, AF_INET, SOCK_STREAM, 0);
			if (sockfd == -1) {
				return -1;
			}

			char *request, *response;
			// formarea cererii
			request = compute_delete_request(host_port, access, NULL, cookies, token);
			// trimiterea cererii
			send_to_server(sockfd, request);
  			
			// primirea raspunsului
  			response = receive_from_server(sockfd);
			if (strncmp(response, "HTTP/1.1 500 Internal Server Error", 34) == 0) {
				cout<<"Forbidden access.\n";
			} else if (strncmp(response, "HTTP/1.1 404 Not Found", 22) == 0) {
				cout<<"No book was found.\n";
			} else {
				cout<<"200 - OK\n";
			}

			free(request);
  			free(response);
			// inchiderea conexiunii
			close_connection(sockfd);
		} else if (strcmp(command, "logout") == 0) {
			strcpy(access, "/api/v1/tema/auth/logout");

			// deschiderea unui socket pentru comunicarea cu serverul
			int sockfd = open_connection(host, 8080, AF_INET, SOCK_STREAM, 0);
			if (sockfd == -1) {
				return -1;
			}

			char *request, *response;
			// formarea cererii
			request = compute_get_request(host_port, access, NULL, cookies, token);
			// trimiterea cererii
			send_to_server(sockfd, request);
  			
			// primirea raspunsului
  			response = receive_from_server(sockfd);
			if (strncmp(response, "HTTP/1.1 400 Bad Request", 24) == 0) {
				cout<<"Unauthorized.\n";
			} else {
				// stergerea datelor (cookie si token)
				memset(cookies, 0, MAX_LEN);
				memset(token, 0, MAX_LEN);
				cout<<"200 - OK\n";
			}

			free(request);
  			free(response);
			// inchiderea conexiunii
			close_connection(sockfd);
		} else if (strcmp(command, "exit") == 0) {
			break;
		} else {
			cout<<"Invalid command!\n";
		}
	}

	return 0;
}