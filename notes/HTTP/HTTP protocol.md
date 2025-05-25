# HTTP/1.1 

***"Be conservative in what you do, be liberal in what you accept from others"***

[HTTP Made Really Easy](https://www.jmarshall.com/easy/http/)
[Wiki - Header fields](https://en.wikipedia.org/wiki/List_of_HTTP_header_fields)
[HTTP Documentation](https://httpwg.org/specs/)
[An Overview of the HTTP as Coverd in RFCs](https://www.inspirisys.com/HTTP_Protocol_as_covered_in_RFCs-An_Overview.pdf)
[HTTP Status codes](https://umbraco.com/knowledge-base/http-status-codes/)

# What is HTTP?

HTTP stands for Hypertext Transfer Protocol.
The HTTP protocol is a request/response protocol.

HTTP has been in use by the World-Wide Web global information initiative since 1990.

HTTP communication usually takes place over TCP/IP connections. The default port is TCP 80, but other ports can be used.
This does not preclude HTTP from being implemented on top of any other protocol on the Internet, or on other networks.
HTTP only presumes a reliable transport; any protocol that provides such guarantees can be used.

As a practical matter, almost all HTTP resources are currently either files or server-side script output. 

---

# Structure of HTTP Transactions

Like most network protocols, HTTP uses the client-server model:
An HTTP client opens a connection and sends a request message to an HTTP server;
the server then returns a response message, usually containing the resource that was requested.
After delivering the response, the server closes the connection (making HTTP a stateless protocol, i.e. not maintaining any connection information between transactions).

[[HTTP Message Format]]

---

# Sample HTTP Exchange

To retrieve the file at the URL

    http://www.somehost.com/path/file.html

first open a socket to the host www.somehost.com, port 80 (use the default port of 80 because none is specified in the URL).
Then, send something like the following through the socket:

    GET /path/file.html HTTP/1.0
    From: someuser@jmarshall.com
    User-Agent: HTTPTool/1.0
    [blank line here]

The server should respond with something like the following, sent back through the same socket:

    HTTP/1.0 200 OK
    Date: Fri, 31 Dec 1999 23:59:59 GMT
    Content-Type: text/html
    Content-Length: 1354

    <html>
    <body>
    <h1>Happy New Millennium!</h1>
    (more file contents)
      .
      .
      .
    </body>
    </html>

After sending the response, the server closes the socket. 


# HTTP 1.1

## Host: Header

Starting with HTTP 1.1, one server at one IP address can be multi-homed, i.e. the home of several Web domains.
For example, "www.host1.com" and "www.host2.com" can live on the same server. 

Several domains living on the same server is like several people sharing one phone: a caller knows who they're calling for, but whoever answers the phone doesn't.
Thus, every HTTP request must specify which host name (and possibly port) the request is intended for, with the `Host: header`.
A complete HTTP 1.1 request might be

    GET /path/file.html HTTP/1.1
    Host: www.host1.com:80
    [blank line here]

except the ":80" isn't required, since that's the default HTTP port. 
`Host:` is the only required header in an HTTP 1.1 request.
It's also the most urgently needed new feature in HTTP 1.1.
Without it, each host name requires a unique IP address, and we're quickly running out of IP addresses with the explosion of new domains. 

Because of the urgency of implementing the new `Host:` header, servers are not allowed to tolerate HTTP 1.1 requests without it.
If a server receives such a request, it must return a "400 Bad Request" response, like

    HTTP/1.1 400 Bad Request
    Content-Type: text/html
    Content-Length: 111

    <html><body>
    <h2>No Host: header received</h2>
    HTTP 1.1 requests must include the Host: header.
    </body></html>

#### Accepting Absolute URL's

The Host: header is actually an interim solution to the problem of host identification.
In future versions of HTTP, requests will use an absolute URL instead of a pathname, like

    GET http://www.somehost.com/path/file.html HTTP/1.2

To enable this protocol transition, HTTP 1.1 servers must accept this form of request, even though HTTP 1.1 clients won't send them.
The server must still report an error if an HTTP 1.1 client leaves out the `Host:` header.


## Chunked Transfer-Encoding

If a server wants to start sending a response before knowing its total length (like with long script output), it might use the simple chunked transfer-encoding, which breaks the complete response into smaller chunks and sends them in series.
You can identify such a response because it contains the "Transfer-Encoding: chunked" header.
All HTTP 1.1 clients must be able to receive chunked messages. 

A chunked message body contains a series of chunks, followed by a line with "0" (zero), followed by optional footers (just like headers), and a blank line.
Each chunk consists of two parts:

   - a line with the size of the chunk data, in hex, possibly followed by a semicolon and extra parameters you can ignore (none are currently standard), and ending with CRLF.
   - the data itself, followed by CRLF. 

So a chunked response might look like the following:

    HTTP/1.1 200 OK
    Date: Fri, 31 Dec 1999 23:59:59 GMT
    Content-Type: text/plain
    Transfer-Encoding: chunked

    1a; ignore-stuff-here
    abcdefghijklmnopqrstuvwxyz
    10
    1234567890abcdef
    0
    some-footer: some-value
    another-footer: another-value
    [blank line here]

Note the blank line after the last footer.
The length of the text data is 42 bytes (1a + 10, in hex), and the data itself is **abcdefghijklmnopqrstuvwxyz1234567890abcdef**.
The footers should be treated like headers, as if they were at the top of the response. 

The chunks can contain any binary data, and may be much larger than the examples here.
The size-line parameters are rarely used, but you should at least ignore them correctly.
Footers are also rare, but might be appropriate for things like checksums or digital signatures. 

For comparison, here's the equivalent to the above response, without using chunked encoding:

    HTTP/1.1 200 OK
    Date: Fri, 31 Dec 1999 23:59:59 GMT
    Content-Type: text/plain
    Content-Length: 42
    some-footer: some-value
    another-footer: another-value

    abcdefghijklmnopqrstuvwxyz1234567890abcdef

Just as HTTP 1.1 clients must accept chunked responses, servers must accept chunked requests (an unlikely scenario, but possible).
Servers aren't required to generate chunked messages; they just have to be able to receive them.


## Persistent Connections and the "Connection: close" Header

In HTTP 1.0 and before, TCP connections are closed after each request and response, so each resource to be retrieved requires its own connection.
Opening and closing TCP connections takes a substantial amount of CPU time, bandwidth, and memory.
In practice, most Web pages consist of several files on the same server, so much can be saved by allowing several requests and responses to be sent through a single persistent connection. 

Persistent connections are the default in HTTP 1.1, so nothing special is required to use them.
Just open a connection and send several requests in series (called pipelining), and read the responses in the same order as the requests were sent.
If you do this, be very careful to read the correct length of each response, to separate them correctly. 

If a client includes the "Connection: close" header in the request, then the connection will be closed after the corresponding response.
Use this if you don't support persistent connections, or if you know a request will be the last on its connection.
Similarly, if a response contains this header, then the server will close the connection following that response, and the client shouldn't send any more requests through that connection. 

A server might close the connection before all responses are sent, so a client must keep track of requests and resend them as needed. 
When resending, don't pipeline the requests until you know the connection is persistent. 
Don't pipeline at all if you know the server won't support persistent connections (like if it uses HTTP 1.0, based on a previous response). 

If an HTTP 1.1 client sends multiple requests through a single connection, the server should send responses back in the same order as the requests.
This is all it takes for a server to support persistent connections. 

If a request includes the "Connection: close" header, that request is the final one for the connection and the server should close the connection after sending the response.
Also, the server should close an idle connection after some timeout period (can be anything; 10 seconds is fine). 

If you don't want to support persistent connections, include the "Connection: close" header in the response.
Use this header whenever you want to close the connection, even if not all requests have been fulfilled. 
The header says that the connection will be closed after the current response, and a valid HTTP 1.1 client will handle it correctly.


## The "100 Continue" Response

During the course of an HTTP 1.1 client sending a request to a server, the server might respond with an interim "100 Continue" response.
This means the server has received the first part of the request, and can be used to aid communication over slow links. 
In any case, all HTTP 1.1 clients must handle the 100 response correctly (perhaps by just ignoring it).

The "100 Continue" response is structured like any HTTP response, i.e. consists of a status line, optional headers, and a blank line.
Unlike other responses, it is always followed by another complete, final response. 

The full data that comes back from the server might consist of two responses in series, like

    HTTP/1.1 100 Continue

    HTTP/1.1 200 OK
    Date: Fri, 31 Dec 1999 23:59:59 GMT
    Content-Type: text/plain
    Content-Length: 42
    some-footer: some-value
    another-footer: another-value

    abcdefghijklmnoprstuvwxyz1234567890abcdef

To handle this, a simple HTTP 1.1 client might read one response from the socket;
if the status code is 100, discard the first response and read the next one instead. 

When an HTTP 1.1 server receives the first line of an HTTP 1.1 (or later) request, it must respond with either "100 Continue" or an error.
If it sends the "100 Continue" response, it must also send another, final response, once the request has been processed.
The "100 Continue" response requires no headers, but must be followed by the usual blank line, like:

    HTTP/1.1 100 Continue
    [blank line here]
    [another HTTP response will go here]

Don't send "100 Continue" to HTTP 1.0 clients, since they don't know how to handle it. 


## The Date: Header

Caching is an important improvement in HTTP 1.1, and can't work without timestamped responses.
So, servers must timestamp every response with a `Date: header` containing the current time, in the form

    Date: Fri, 31 Dec 1999 23:59:59 GMT

All responses except those with 100-level status (but including error responses) must include the `Date: header`.
All time values in HTTP use Greenwich Mean Time. 


## Handling Requests with `If-Modified-Since:` or `If-Unmodified-Since:` Headers

To avoid sending resources that don't need to be sent, thus saving bandwidth, HTTP 1.1 defines the `If-Modified-Since:` and `If-Unmodified-Since:` request headers. 
The former says "only send the resource if it has changed since this date"; the latter says the opposite.
Clients aren't required to use them, but HTTP 1.1 servers are required to honor requests that do use them. 

Unfortunately, due to earlier HTTP versions, the date value may be in any of three possible formats:

    If-Modified-Since:  Fri, 31 Dec 1999 23:59:59 GMT
    If-Modified-Since:  Friday, 31-Dec-99 23:59:59 GMT
    If-Modified-Since:  Fri Dec 31 23:59:59 1999

Again, all time values in HTTP use Greenwich Mean Time (though try to be tolerant of non-GMT times). 
If a date with a two-digit year seems to be more than 50 years in the future, treat it as being in the past-- this helps with the millennium bug. 
In fact, do this with any date handling in HTTP 1.1. 

Although servers must accept all three date formats, HTTP 1.1 clients and servers must only generate the first kind.
If the date in either of these headers is invalid, or is in the future, ignore the header. 

If, without the header, the request would result in an unsuccessful (non-200-level) status code, ignore the header and send the non-200-level response. 
In other words, only apply these headers when you know the resource would otherwise be sent. 

The `If-Modified-Since:` header is used with a GET request.
If the requested resource has been modified since the given date, ignore the header and return the resource as you normally would. 
Otherwise, return a "304 Not Modified" response, including the `Date:` header and no message body, like

    HTTP/1.1 304 Not Modified
    Date: Fri, 31 Dec 1999 23:59:59 GMT
    [blank line here]

The `If-Unmodified-Since:` header is similar, but can be used with any method.
If the requested resource has not been modified since the given date, ignore the header and return the resource as you normally would. 
Otherwise, return a "412 Precondition Failed" response, like

    HTTP/1.1 412 Precondition Failed
    [blank line here]


## Supporting the GET and HEAD methods

To comply with HTTP 1.1, a server must support at least the GET and HEAD methods.
If you're handling CGI scripts, you should probably support the POST method too.

Four other methods (PUT, DELETE, OPTIONS, and TRACE) are defined in HTTP 1.1, but are rarely used.
If a client requests a method you don't support, respond with "501 Not Implemented", like

    HTTP/1.1 501 Not Implemented
    [blank line here]

## Supporting HTTP 1.0 Requests

To be compatible with older browsers, HTTP 1.1 servers must support HTTP 1.0 requests.
In particular, when a request uses HTTP 1.0 (as identified in the initial request line),

   - don't require the `Host:` header, and
   - don't send the "100 Continue" response. 
