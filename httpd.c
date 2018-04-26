/* httpd.c
 *
 * Copyright (C) 2018 < Daniel Finneran, dan@thebsdbox.co.uk >
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the GPL license.  See the LICENSE file for details.
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <signal.h>

#include <time.h>
#include <sys/wait.h>
#include <unistd.h>

#include <stddef.h>
#include "httpd.h"
#include "utils.h"
#include "ui.h"
#include "log.h"

// Function Prototypes
int receive(int socket);

size_t tcpPort;
char *contentType;

struct sockaddr_in address;
struct sockaddr_storage connector;
int current_socket;
int connecting_socket;
socklen_t addr_size;

httpResponse *response;

char *postData;

/*****************************************************************/
/*                       Functions Start                         */
/*****************************************************************/

void setPort(size_t port)
{
        tcpPort = port;
}

void setContentType(char *type)
{
    if (type) {
        contentType = type;
    }
}

void createINETSocket()
{
    current_socket = socket(AF_INET, SOCK_STREAM, 0);
    if ( current_socket == -1 )
    {
        perror("Create socket");
        exit(-1);
    }
}

void bindToINETSocketWithPort()
{
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(tcpPort);
    
    if ( bind(current_socket, (struct sockaddr *)&address, sizeof(address)) < 0 )
    {
        perror("Bind to port");
        exit(-1);
    }
}

void startListener()
{
    if ( listen(current_socket, SOMAXCONN) < 0 )
    {
        perror("Listen on port");
        exit(-1);
    }
}

void handle(int socket)
{
    
    if (receive((int)socket) < 0)
    {
        perror("Receive");
        exit(-1);
    }
}

void acceptConnection()
{
    addr_size = sizeof(connector);
    connecting_socket = accept(current_socket, NULL,NULL);
    
    if ( connecting_socket < 0 )
    {
        perror("Accepting sockets");
        exit(-1);
    }
    
    handle(connecting_socket);
    close(connecting_socket);
}

/* These functions will handle the steps of creating the sockets, binding to them
 * and then listening to any traffic that is sent through them.
 *
 *
 */

void parseRequestLine(httpRequest *request)
{   
    if (request && request->requestLine) {
        char *parse = strdup(request->requestLine);
        request->method = strtok(parse, " ");
        request->URI = strtok(NULL , " ");
        request->HTTPVersion = strtok(NULL, " ");
    }
}

char *returnNewRequestLine(char *rawData)
{
    char *headerPointer = strstr(rawData, "\r\n");
    ptrdiff_t requestSize = (void *)headerPointer - (void *)rawData;
    char *requestLine = malloc(requestSize);
    strncpy(requestLine, rawData, requestSize);
    return requestLine;
}


char *returnNewMessageBody(char *rawData)
{
    char *headerDelimiter = "\r\n\r\n";
    return strdup((strstr(rawData,headerDelimiter)+strlen(headerDelimiter)));
}

char *returnNewHeaders(char *rawData, httpRequest *response)
{
    // This returns a new copy of the headers, will need freeing seperately
    char *headerStart = strstr(rawData, "\r\n");
    char *headersEnd = strstr(rawData,"\r\n\r\n");
    
    ptrdiff_t requestSize = (void *)headerStart - (void *)rawData;
    ptrdiff_t headersSize = (void *)headersEnd - (void *)headerStart;
    
    char *newHeaders = malloc(headersSize);
    // Pointer arithmatict to move from start of raw data + request line + CRLF
    strncpy(newHeaders, rawData+requestSize+2, headersSize-2);
    return newHeaders;
}

httpRequest *processHttpRequest(char *rawData)
{
    httpRequest *request = malloc(sizeof(httpRequest));
    request->messageBody = returnNewMessageBody(rawData);
    request->headers = returnNewHeaders(rawData, request);
    request->requestLine = returnNewRequestLine(rawData);
    parseRequestLine(request);
    return request;
}

int freeRequest(httpRequest *request)
{   

    //TODO
    return EXIT_FAILURE;
}

char *dataForHeader(char *headerKey, httpRequest *request)
{
    char *headers = strdup(request->headers);
    char *headers_search = headers;
    headers_search = strtok(headers, "\r\n");
    
    while (headers_search != NULL) {
        
        headers_search = strtok(NULL, "\r\n");
        if (headers_search) {
            if (strstr(headers_search, headerKey)) {
                const char seperator = ' ';
                char *found = strchr(headers_search, seperator);
                return strdup(found);
            }
        }
    }
    return 0;
}

/* These functions will deal with the HTTPD responses
 *
 *
 */
int setHTTPResponse(char *messageBody, int responseCode)
{
    if (messageBody) {
        response->messageBody = messageBody;
        response->messageLength = strlen(messageBody);
    } else {
        response->messageLength = 0;
    }
    response->responseCode = responseCode;
    return EXIT_SUCCESS;
}

size_t sendString(char *message, int socket)
{
    size_t length, bytes_sent;
    length = strlen(message);
    
    bytes_sent = send(socket, message, length, 0);
    
    return bytes_sent;
}

size_t sendBinary(int *byte, int length)
{
    size_t bytes_sent;
    
    bytes_sent = send(connecting_socket, byte, length, 0);
    
    return bytes_sent;
}

void sendHeader(char *Status_code, char *Content_Type, size_t TotalSize, int socket)
{
    // If no content type is set then just do a catch all
    if (!Content_Type) {
        Content_Type = "*/*";
    }
    char *head = "HTTP/1.1 ";
    char *content_head = "\r\nContent-Type: ";
    char *server_head = "\r\nServer: urchin";
    char *length_head = "\r\nContent-Length: ";
    char *date_head = "\r\nDate: ";
    char *newline = "\r\n";
    char contentLength[100];
    
    time_t rawtime;
    
    time ( &rawtime );
    
    // int contentLength = strlen(HTML);
    sprintf(contentLength, "%zu", TotalSize);
    
    char *message = malloc((
                            strlen(head) +
                            strlen(content_head) +
                            strlen(server_head) +
                            strlen(length_head) +
                            strlen(date_head) +
                            strlen(newline) +
                            strlen(Status_code) +
                            strlen(Content_Type) +
                            strlen(contentLength) +
                            28 +
                            sizeof(char)) * 2);
    
    if ( message != NULL )
    {
        
        strcpy(message, head);
        strcat(message, Status_code);
        strcat(message, content_head);
        strcat(message, Content_Type);
        strcat(message, server_head);
        strcat(message, length_head);
        strcat(message, contentLength);
        strcat(message, date_head);
        strcat(message, (char*)ctime(&rawtime));
        strcat(message, newline);
        
        sendString(message, socket);
        free(message);
    }
}

void sendHTML(char *statusCode, char *contentType, char *content, int size, int socket)
{
    sendHeader(statusCode, contentType, size, socket);
    sendString(content, socket);
}

int handleHttpGET(char *input)
{
    if (postData) {

    } else {
        sendHeader("200 OK", contentType,0, connecting_socket);
        sendString("<html>Not Storing Data</html>\n", connecting_socket);
    }
    return -1;
}

int respond()
{
    /* Check reponse is allocated then
     * work through the response that the callback should have populated
     * free up the allocated memory before finishing
     *
     * This isn't "FULLY" JSON-RPC compliant yet -> https://www.simple-is-better.org/json-rpc/transport_http.html
     */
    if (response) {
        switch (response->responseCode) {
            case 200:
                sendHeader("200 OK", contentType, response->messageLength, connecting_socket);
                break;
            case 202:
                sendHeader("202 Accepted", contentType, response->messageLength, connecting_socket);
                break;
            case 204:
                sendHeader("204 No Response", contentType, response->messageLength, connecting_socket);
                break;
            case 405:
                sendHeader("405 Method Not Allowed", contentType, response->messageLength, connecting_socket);
                break;
            case 415:
                sendHeader("415 Unsupported Media Type", contentType, response->messageLength, connecting_socket);
                break;
            default:
                sendString("HTTP/1.1 500 Error\r\n\r\n", connecting_socket);
        }
        
        /* If the message length is more that 0 and that the messageBody isn't NULL
         * then send it back to the client and de-allocate the messagebody memory
         * finally de-allocate the reponse memory
         */
        
        if (( response->messageLength > 0) && response->messageBody) {
            sendString(response->messageBody, connecting_socket);
            free(response->messageBody);
            free(response);
        }
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

int receive(int socket)
{
    size_t msgLen = 0;
    char buffer[BUFFER_SIZE];
    
    memset (buffer,'\0', BUFFER_SIZE);
    
    if ((msgLen = recv(socket, buffer, BUFFER_SIZE, 0)) <= 0)
    {
        printf("Error handling incoming request");
        return -1;
    }
    
    /* Read through the first data from the client,
     * identify the HTTPD request and the method
     * check headers for more data and process accordingly
     * hand off to correct function
     */
    
    httpRequest *request = processHttpRequest(buffer);
    
    if ( stringMatch("GET", request->method) )				// GET
    {

        printf("GET request for %s\n", request->URI);
        sendHeader("200 OK", contentType, htmlfiles_index_html_len, connecting_socket);
        sendString((char *)htmlfiles_index_html,connecting_socket);
        if (postData) {
            sendHeader("200 OK", contentType, strlen(postData), connecting_socket);
            sendString(postData, connecting_socket);
        }
    }
    else if ( stringMatch("HEAD", request->method) )		// HEAD
    {
        // SendHeader();
    }
    else if ( stringMatch("POST", request->method) )		// POST
    {
        /* Process the data, if the headers reveal that their is a Expect: 100-continue
         * header then additional processing is required in order to handle the recv
         * We will need to get the numerical value from the header contentLength, which
         * should exist as part of the RFC https://www.w3.org/Protocols/rfc2616/rfc2616-sec4.html#sec4.4
         * more details here -> http://stackoverflow.com/questions/2773396/whats-the-content-length-field-in-http-header
         * If it doesnt, exit. If it does then we recv the correct amount of bytes and process them accordingly
         */
        
        char *expectContinue = dataForHeader("Expect:", request);
        if (expectContinue) {
            char *contentLength = dataForHeader("Content-Length:", request);
            if (contentLength) {
                char *endPointer;
                long messageSize = strtol(contentLength, &endPointer, 10);
                
                if (messageSize > BUFFER_SIZE) {
                    sendString("HTTP/1.0 500 Error\r\n\r\n", connecting_socket);
                    return 1;
                }
                
                sendString("HTTP/1.0 100 Continue\r\n\r\n", connecting_socket);
                memset (buffer,'\0', BUFFER_SIZE);
                long messageBodySizeCounter = 0;
                char *buffer_location = buffer;
                
                while (messageBodySizeCounter < messageSize) {
                    if ((msgLen = recv(socket, buffer_location, BUFFER_SIZE, 0)) == -1)
                    {
                        sendString("HTTP/1.0 500 Error\r\n\r\n", connecting_socket);
                        return 1;
                    }
                    buffer_location += msgLen;
                    messageBodySizeCounter += msgLen;
                    if (messageBodySizeCounter > messageSize) {
                        /* Something has gone very wrong we have more data than expected
                         * Return an error to the client
                         */
                        sendString("HTTP/1.0 500 Error\r\n\r\n", connecting_socket);
                        return 1;
                    }
                }
            }
            // Only wipe over the messageBody if we've had the expect header
            request->messageBody = buffer;
        }

    /* Allocate required memory for reponse
    * post the data as a callback to a handling function
    * respond accordingly to the client
    */
        if (postData) {
            
            free(postData);
        }
        response = malloc(sizeof(httpResponse));
        postData = strdup(request->messageBody);
        setHTTPResponse("", 200);
        respond();
        return 1;
    }
    else	 // Not a handled HTTPD request (GET/POST)
    {
        sendString("HTTP/1.0 400 Bad Request\r\n\r\n", connecting_socket);
    }
    return 1;
}

/*****************************************************************************/
void handleInterrupt(int s){
    //printf("Caught signal %d\n",(int)s);
    printf("\nExiting microWebStorage\n");
    close(current_socket);
}

void setHTTPDInterruptHandler()
{
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = handleInterrupt;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);
    
}

void startListenLoop()
{
    // Enable the interupt handler, so that a ctrl-c will tidy up the socket
    setHTTPDInterruptHandler();
    while (1) {
        acceptConnection();
    }
}
