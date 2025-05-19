# [What is the difference between GET and POST?](https://www.jmarshall.com/easy/cgi/cgi_footnotes.html#getvspost)

GET and POST are two different methods defined in HTTP that do very different things, but both happen to be able to send form submissions to the server. 

Normally, GET is used to get a file or other resource, possibly with parameters specifying more exactly what is needed.
In the case of form input, GET fully includes it in the URL, like

    http://myhost.com/mypath/myscript.cgi?name1=value1&name2=value2

GET is how your browser downloads most files, like HTML files and images.
It can also be used for most form submissions, if there's not too much data (the limit varies from browser to browser). 

Normally, POST is used to send a chunk of data to the server to be processed, whatever that may entail.
When an HTML form is submitted using POST, your form data is attached to the end of the POST request, in its own object (specifically, in the message body).
This is not as simple as using GET, but is more versatile.
For example, you can send entire files using POST. Also, data size is not limited like it is with GET.

To the CGI programmer, GET and POST work almost identically, and are equally easy to use.

Some advantages of POST are that you're unlimited in the data you can submit, and you can count on your script being called every time the form is submitted.
One advantage of GET is that your entire form submission can be encapsulated in one URL.


### The POST Method

A POST request is used to send data to the server to be processed in some way, like by a CGI script.
A POST request is different from a GET request in the following ways: 

   - There's a block of data sent with the request, in the message body. There are usually extra headers to describe this message body, like `Content-Type:` and `Content-Length:`.
   - The request URI is not a resource to retrieve; it's usually a program to handle the data you're sending.
   - The HTTP response is normally program output, not a static file. 

The most common use of POST, by far, is to submit HTML form data to CGI scripts. [[CGI]]
In this case, the `Content-Type:` header is usually `application/x-www-form-urlencoded`, and the `Content-Length:` header gives the length of the URL-encoded form data (here's a note on URL-encoding).
The CGI script receives the message body through STDIN, and decodes it. Here's a typical form submission, using POST: 

    POST /path/script.cgi HTTP/1.0
    From: frog@jmarshall.com
    User-Agent: HTTPTool/1.0
    Content-Type: application/x-www-form-urlencoded
    Content-Length: 32
    
    home=Cosby&favorite+flavor=flies

You can use a POST request to send whatever data you want, not just form submissions.
Just make sure the sender and the receiving program agree on the format. 

The GET method can also be used to submit forms. 
