[Digital Ocean](https://www.digitalocean.com/community/tutorials/understanding-nginx-server-and-location-block-selection-algorithms#how-nginx-decides-which-server-block-will-handle-a-request)
[Sling Academy](https://www.slingacademy.com/article/nginx-server-blocks-the-complete-guide/)

The main server block directives that Nginx is concerned with during this process are the `listen` directive, and the `server_name` directive.

# Parsing the listen Directive to Find Possible Matches

The `listen` directive typically defines which IP address and port that the server block will respond to.
By default, any server block that does not include a `listen` directive is given the listen parameters of `0.0.0.0:80` 
(or `0.0.0.0:8080` if Nginx is being run by a normal, non-root user).

The listen directive can be set to:

   - An IP address/port combo.
   - A lone IP address which will then listen on the default port 80.
   - A lone port which will listen to every interface on that port.
   - The path to a Unix socket.

The last option will generally only have implications when passing requests between different servers

   - A block with no `listen` directive uses the value `0.0.0.0:80`.
   - A block set to an IP address `111.111.111.111` with no port becomes `111.111.111.111:80`
   - A block set to port `8888` with no IP address becomes `0.0.0.0:8888`

Nginx then attempts to collect a list of the server blocks that match the request most specifically based on the IP address and port. 
This means that any block that is functionally using `0.0.0.0` as its IP address (to match any interface), will not be selected if there are matching blocks that list a specific IP address.
In any case, the port must be matched exactly.

If there is only one most specific match, that server block will be used to serve the request. 
If there are multiple server blocks with the same level of specificity matching, Nginx then begins to evaluate the `server_name` directive of each server block.

Nginx will only evaluate the `server_name` directive when it needs to distinguish between server blocks that match to the same level of specificity in the `listen` directive.

For instance, if `example.com` is hosted on port `80` of `192.168.1.10`, a request for `example.com` will always be served by the first block in this example, despite the `server_name` directive in the second block.
```nginx
server {
    listen 192.168.1.10;

    . . .
}

server {
    listen 80;
    server_name example.com;

    . . .
}
```

---

# Parsing the server_name Directive to Choose a Match

Next, to further evaluate requests that have equally specific `listen` directives, Nginx checks the request’s `Host` header. 
This value holds the domain or IP address that the client was actually trying to reach.

Nginx attempts to find the best match for the value it finds by looking at the `server_name` directive within each of the server blocks that are still selection candidates.

Nginx will first try to find a server block with a `server_name` that matches the value in the `Host` header of the request exactly. 
If this is found, the associated block will be used to serve the request. 
If multiple exact matches are found, the first one is used.

If no exact match is found, Nginx will then try to find a server block with a `server_name` that matches using a leading wildcard (indicated by a `*` at the beginning of the name in the config). 
If one is found, that block will be used to serve the request. 
If multiple matches are found, the longest match will be used to serve the request.

If no match is found using a leading wildcard, Nginx then looks for a server block with a `server_name` that matches using a trailing wildcard (indicated by a server name ending with a `*` in the config).
If one is found, that block is used to serve the request. 
If multiple matches are found, the longest match will be used to serve the request.

If no match is found using a trailing wildcard, Nginx then evaluates server blocks that define the `server_name` using regular expressions (indicated by a `~` before the name). 
The first `server_name` with a regular expression that matches the “Host” header will be used to serve the request.

If no regular expression match is found, Nginx then selects the default `server` block for that IP address and port.

Each IP address/port combo has a default server block that will be used when a course of action can not be determined with the above methods.
For an IP address/port combo, this will either be the first block in the configuration or the block that contains the `default_server` option as part of the `listen` directive (which would override the first-found algorithm).
There can be only one `default_server` declaration per each IP address/port combination.

---

# Examples

If there is a `server_name` defined that exactly matches the `Host` header value, that server block is selected to process the request.

In this example, if the `Host` header of the request was set to `host1.example.com`, the second server would be selected:
```nginx
server {
    listen 80;
    server_name *.example.com;

    . . .
}

server {
    listen 80;
    server_name host1.example.com;

    . . .
}
```

If no exact match is found, Nginx then checks to see if there is a `server_name` with a starting wildcard that fits. 
The longest match beginning with a wildcard will be selected to fulfill the request.

In this example, if the request had a `Host` header of `www.example.org`, the second server block would be selected:
```nginx
server {
    listen 80;
    server_name www.example.*;

    . . .

}

server {
    listen 80;
    server_name *.example.org;

    . . .

}

server {
    listen 80;
    server_name *.org;

    . . .

}
```
If no match is found with a starting wildcard, Nginx will then see if a match exists using a wildcard at the end of the expression.
At this point, the longest match ending with a wildcard will be selected to serve the request.

For instance, if the request has a `Host` header set to `www.example.com`, the third server block will be selected:

```nginx
server {
    listen 80;
    server_name host1.example.com;

    . . .

}

server {
    listen 80;
    server_name example.com;

    . . .

}

server {
    listen 80;
    server_name www.example.*;

    . . .

}
```
If no wildcard matches can be found, Nginx will then move on to attempting to match `server_name` directives that use regular expressions. 
The first matching regular expression will be selected to respond to the request.

For example, if the `Host` header of the request is set to `www.example.com`, then the second server block will be selected to satisfy the request:
```nginx
server {
    listen 80;
    server_name example.com;

    . . .

}

server {
    listen 80;
    server_name ~^(www|host1).*\.example\.com$;

    . . .

}

server {
    listen 80;
    server_name ~^(subdomain|set|www|host1).*\.example\.com$;

    . . .

}
```
If none of the above steps are able to satisfy the request, then the request will be passed to the default server for the matching IP address and port.
