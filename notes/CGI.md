[CGI MADE EAZY](https://www.jmarshall.com/easy/cgi/)
[CS Toronto](https://www.cs.toronto.edu/~penny/teaching/csc309-01f/lectures/40/CGI.pdf)
[Getting Started with CGI Programs](http://www.mnuwer.dbasedeveloper.co.uk/dlearn/web/session01.htm)
[How the web works: HTTP and CGI explained](https://www.garshol.priv.no/download/text/http-tut.html)

# What is CGI?

CGI stands for Common Gateway Interface.
In other words, a standard ("common") way of communicating ("interface") between different processes ("gateway", sort of).

It's a simple protocol that can be used to communicate between Web forms and your program.
A CGI script can be written in any language that can read STDIN, write to STDOUT, and read environment variables.

---
# Structure of a CGI Script

Here's the typical sequence of steps for a CGI script:

1. Read the user's form input.
2. Do what you want with the data.
3. Write the HTML response to STDOUT.

---

# Reading the User's Form Input

When the user submits the form, your script receives the form data as a set of name-value pairs.
This set of name-value pairs is given to you as one long string, which you need to parse.

     "name1=value1&name2=value2&name3=value3"
     "name1=value1;name2=value2;name3=value3" 

So just split on the ampersands or semicolons, then on the equal signs. Then, do two more things to each name and value:

1. Convert all "+" characters to spaces, and
2. Convert all "%xx" sequences to the single character whose ascii value is "xx", in hex. For example, convert "%3d" to "=". 

This is needed because the original long string is URL-encoded, to allow for equal signs, ampersands, and so forth in the user's input. 

## Where do you get the long string?

That depends on the HTTP method the form was submitted with:

1. For GET submissions, it's in the environment variable QUERY_STRING.
2. For POST submissions, read it from STDIN. The exact number of bytes to read is in the environment variable CONTENT_LENGTH.

[[GET vs POST]]

---

# Sending the Response Back to the User

First, write the line

    Content-type: text/html

plus another blank line, to STDOUT.
After that, write your HTML response page to STDOUT, and it will be sent to the user when your script is done.
That's all there is to it.

[Returning an Image or Other Non-HTML Response from a CGI Script](https://www.jmarshall.com/easy/cgi/cgi_footnotes.html#othertypes)
