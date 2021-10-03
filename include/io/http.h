/*!
 * http.h - http server for libsatoshi
 * Copyright (c) 2021, Christopher Jeffrey (MIT License).
 * https://github.com/chjj/libsatoshi
 */

#ifndef BTC_IO_HTTP_H
#define BTC_IO_HTTP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include "core.h"
#include "loop.h"
#include "../satoshi/common.h"

/*
 * Constants
 */

enum http_method_ {
  HTTP_METHOD_DELETE = 0,
  HTTP_METHOD_GET = 1,
  HTTP_METHOD_HEAD = 2,
  HTTP_METHOD_POST = 3,
  HTTP_METHOD_PUT = 4,
  /* pathological */
  HTTP_METHOD_CONNECT = 5,
  HTTP_METHOD_OPTIONS = 6,
  HTTP_METHOD_TRACE = 7,
  /* WebDAV */
  HTTP_METHOD_COPY = 8,
  HTTP_METHOD_LOCK = 9,
  HTTP_METHOD_MKCOL = 10,
  HTTP_METHOD_MOVE = 11,
  HTTP_METHOD_PROPFIND = 12,
  HTTP_METHOD_PROPPATCH = 13,
  HTTP_METHOD_SEARCH = 14,
  HTTP_METHOD_UNLOCK = 15,
  HTTP_METHOD_BIND = 16,
  HTTP_METHOD_REBIND = 17,
  HTTP_METHOD_UNBIND = 18,
  HTTP_METHOD_ACL = 19,
  /* subversion */
  HTTP_METHOD_REPORT = 20,
  HTTP_METHOD_MKACTIVITY = 21,
  HTTP_METHOD_CHECKOUT = 22,
  HTTP_METHOD_MERGE = 23,
  /* upnp */
  HTTP_METHOD_MSEARCH = 24,
  HTTP_METHOD_NOTIFY = 25,
  HTTP_METHOD_SUBSCRIBE = 26,
  HTTP_METHOD_UNSUBSCRIBE = 27,
  /* RFC-5789 */
  HTTP_METHOD_PATCH = 28,
  HTTP_METHOD_PURGE = 29,
  /* CalDAV */
  HTTP_METHOD_MKCALENDAR = 30,
  /* RFC-2068, sectio */
  HTTP_METHOD_LINK = 31,
  HTTP_METHOD_UNLINK = 32,
  /* icecast */
  HTTP_METHOD_SOURCE = 33
};

/*
 * Types
 */

typedef struct http_string {
  char *data;
  size_t alloc;
  size_t length;
} http_string_t;

typedef struct http_header {
  http_string_t field;
  http_string_t value;
} http_header_t;

typedef struct http_head {
  http_header_t **items;
  size_t alloc;
  size_t length;
} http_head_t;

typedef struct http_req {
  unsigned int status; /* client only */
  unsigned int method;
  http_string_t path;
  http_head_t headers;
  http_string_t body;
} http_req_t;

typedef http_req_t http_response_t;

typedef struct http_res {
  btc_socket_t *socket;
  http_head_t headers;
} http_res_t;

struct http_server;

typedef int http_server_request_cb(struct http_server *,
                                   http_req_t *,
                                   http_res_t *);

typedef struct http_server {
  btc_loop_t *loop;
  btc_socket_t *socket;
  http_server_request_cb *on_request;
  void *data;
} http_server_t;

typedef void http_response_cb(const http_response_t *, void *);

typedef struct http_options {
  unsigned int method;
  const char *hostname;
  unsigned int port;
  const char *path;
  const char **headers;
  const char *agent;
  const char *accept;
  const char *type;
  const char *body;
} http_options_t;

/*
 * Request
 */

BTC_EXTERN const http_string_t *
http_req_header(const http_req_t *req, const char *name);

/*
 * Response
 */

BTC_EXTERN void
http_res_header(http_res_t *res, const char *field, const char *value);

BTC_EXTERN void
http_res_send(http_res_t *res,
              unsigned int status,
              const char *type,
              const char *body);

BTC_EXTERN void
http_res_send_data(http_res_t *res,
                   unsigned int status,
                   const char *type,
                   void *body,
                   size_t length);

BTC_EXTERN void
http_res_error(http_res_t *res, unsigned int status);

/*
 * Server
 */

BTC_EXTERN http_server_t *
http_server_create(btc_loop_t *loop);

BTC_EXTERN void
http_server_destroy(http_server_t *server);

BTC_EXTERN int
http_server_open(http_server_t *server, const btc_sockaddr_t *addr);

BTC_EXTERN void
http_server_close(http_server_t *server);

/*
 * Client
 */

BTC_EXTERN void
http_options_init(http_options_t *options);

BTC_EXTERN int
http_request(btc_loop_t *loop,
             const http_options_t *options,
             http_response_cb *callback,
             void *data);

#ifdef __cplusplus
}
#endif

#endif /* BTC_IO_HTTP_H */
