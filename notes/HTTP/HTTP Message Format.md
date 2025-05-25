The format of the request and response messages are similar, and English-oriented. Both kinds of messages consist of:

- an initial line,
- zero or more header lines,
- a blank line (i.e. a CRLF by itself), and
- an optional message body (e.g. a file, or query data, or query output). 

```
    start-line CRLF
    Headers CRLF
    CRLF(end of headers)
    [message-body]
```

Put another way, the format of an HTTP message is:

    <initial line, different for request vs. response>
    Header1: value1
    Header2: value2
    Header3: value3

    <optional message body goes here, like file contents or query data;
     it can be many lines long, or even binary data $&*%@!^$@>

Initial lines and headers should end in CRLF, though you should gracefully handle lines ending in just LF.
(More exactly, CR and LF here mean ASCII values 13('\r') and 10('\n'), even though some platforms may use different characters.)

---

# Initial Line

## Initial Request Line

[RFC2616#4](https://datatracker.ietf.org/doc/html/rfc2616#section-4)
[RFC2616#5](https://datatracker.ietf.org/doc/html/rfc2616#section-5.1)

The initial line is different for the request than for the response.
A request line has three parts, separated by spaces: a method name, the local path of the requested resource, and the version of HTTP being used.
A typical request line is:

    GET /path/to/file/index.html HTTP/1.0

Notes:

   - [[HTTP Method]].
   - The path is the part of the URL after the host name, also called the request URI (a URI is like a URL, but more general).
   - The HTTP version always takes the form "HTTP/x.x", uppercase. 

In the interest of robustness, servers SHOULD ignore any empty
line(s) received where a Request-Line is expected. In other words, if
the server is reading the protocol stream at the beginning of a
message and receives a CRLF first, it should ignore the CRLF.

The Request-Line begins with a method token, followed by the
Request-URI and the protocol version, and ending with CRLF. The
elements are separated by SP characters. No CR or LF is allowed
except in the final CRLF sequence.

        Request-Line   = Method SP Request-URI SP HTTP-Version CRLF


[Request-URI](https://fullstack.wiki/http/request-uri) may be parsed as follows:

   - If the first character is the slash /, parse as origin-form.
   - If the method is CONNECT, parse as authority-form
   - If the method is OPTIONS and the string is a single character *, parse as asterisk-form
   - If the string contains a colon (not as the first character), parse as absolute-form.
   - Otherwise, emit an error.


## Initial Response Line (Status Line)

The initial response line, called the status line, also has three parts separated by spaces:
- the HTTP version,
- a response status code that gives the result of the request,
- and an English reason phrase describing the status code.

Typical status lines are:

    HTTP/1.0 200 OK

or

    HTTP/1.0 404 Not Found

Notes:

   - The HTTP version is in the same format as in the request line, "HTTP/x.x".
   - The status code is meant to be computer-readable; the reason phrase is meant to be human-readable, and may vary.
   - The status code is a three-digit integer, and the first digit identifies the general category of response:
       - **1xx** indicates an informational message only
       - **2xx** indicates success of some kind
       - **3xx** redirects the client to another URL
       - **4xx** indicates an error on the client's part
       - **5xx** indicates an error on the server's part 

--- 

# Header lines

Header lines provide information about the request or response, or about the object sent in the message body.
The header lines are in the usual text header format, which is: one line per header, of the form "Header-Name: value", ending with CRLF.

   - As noted above, they should end in CRLF, but you should handle LF correctly.
   - The header name is not case-sensitive (though the value may be).
   - Any number of spaces or tabs may be between the ":" and the value.
   - Header lines beginning with space or tab are actually part of the previous header line, folded into multiple lines for easy reading. 

Thus, the following two headers are equivalent:

    Header1: some-long-value-1a, some-long-value-1b

    HEADER1:    some-long-value-1a,
                some-long-value-1b

HTTP 1.0 defines 16 headers, though none are required.
HTTP 1.1 defines 46 headers, and one (Host:) is required in requests.

--- 

# The Message Body

An HTTP message may have a body of data sent after the header lines.
In a response, this is where the requested resource is returned to the client (the most common use of the message body),
or perhaps explanatory text if there's an error.
In a request, this is where user-entered data or uploaded files are sent to the server.

If an HTTP message includes a body, there are usually header lines in the message that describe the body. In particular,

   - The Content-Type: header gives the MIME-type of the data in the body, such as text/html or image/gif.
   - The Content-Length: header gives the number of bytes in the body. 
