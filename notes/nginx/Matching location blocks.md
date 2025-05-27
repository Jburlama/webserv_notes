[Digital Ocean](https://www.digitalocean.com/community/tutorials/understanding-nginx-server-and-location-block-selection-algorithms#matching-location-blocks)
[Sling Academy](https://www.slingacademy.com/article/nginx-server-blocks-the-complete-guide/)

Location blocks are used to decide how to process the request URI (the part of the request that comes after the domain name or IP address/port).

# Location Block Syntax

Location blocks generally take the following form:
``` nginx
location optional_modifier location_match {

    . . .
}
```

The `location_match` in the above defines what Nginx should check the request URI against. 

   - (none): If no modifiers are present, the location is interpreted as a prefix match.
    This means that the location given will be matched against the beginning of the request URI to determine a match.
   - =: If an equal sign is used, this block will be considered a match if the request URI exactly matches the location given.
   - ~: If a tilde modifier is present, this location will be interpreted as a case-sensitive regular expression match.
   - ~*: If a tilde and asterisk modifier is used, the location block will be interpreted as a case-insensitive regular expression match.
   - ^~: If a carat and tilde modifier is present, and if this block is selected as the best non-regular expression match, regular expression matching will not take place.

---

# Examples Demonstrating Location Block Syntax

As an example of prefix matching, the following location block may be selected to respond for request URIs that look like
`/site`, `/site/page1/index.html`, or `/site/index.html`:

```nginx
location /site {

    . . .

}
```

For a demonstration of exact request URI matching, this block will always be used to respond to a request URI that looks like `/page1`.
It will not be used to respond to a `/page1/index.html` request URI.
Keep in mind that if this block is selected and the request is fulfilled using an index page, an internal redirect will take place to another location that will be the actual handler of the request:

```nginx
location = /page1 {

    . . .

}
```

As an example of a location that should be interpreted as a case-sensitive regular expression, this block could be used to handle requests for 
`/tortoise.jpg`, but not for `/FLOWER.PNG`:

```nginx
location ~ \.(jpe?g|png|gif|ico)$ {

    . . .

}
```

A block that would allow for case-insensitive matching similar to the above is shown below. 
Here, both `/tortoise.jpg` and `/FLOWER.PNG` could be handled by this block:

```nginx
location ~* \.(jpe?g|png|gif|ico)$ {

    . . .

}
```

Finally, this block would prevent regular expression matching from occurring if it is determined to be the best non-regular expression match.
It could handle requests for `/costumes/ninja.html`:

```nginx
location ^~ /costumes {

    . . .

}
```

---

# How Nginx Chooses Which Location to Use to Handle Requests

Nginx begins by checking all prefix-based location matches (all location types not involving a regular expression). 
It checks each location against the complete request URI.

First, Nginx looks for an exact match. 
If a location block using the = modifier is found to match the request URI exactly, this location block is immediately selected to serve the request.

If no exact (with the = modifier) location block matches are found, Nginx then moves on to evaluating non-exact prefixes.
It discovers the longest matching prefix location for the given request URI, which it then evaluates as follows:

   - If the longest matching prefix location has the ^~ modifier, then Nginx will immediately end its search and select this location to serve the request.
   - If the longest matching prefix location does not use the ^~ modifier, the match is stored by Nginx for the moment so that the focus of the search can be shifted.

After the longest matching prefix location is determined and stored, Nginx moves on to evaluating the regular expression locations (both case sensitive and insensitive). 
If there are any regular expression locations within the longest matching prefix location, Nginx will move those to the top of its list of regex locations to check. 
Nginx then tries to match against the regular expression locations sequentially.
The first regular expression location that matches the request URI is immediately selected to serve the request.

If no regular expression locations are found that match the request URI, the previously stored prefix location is selected to serve the request.

It is important to understand that, by default, Nginx will serve regular expression matches in preference to prefix matches. 
However, it evaluates prefix locations first, allowing for the administer to override this tendency by specifying locations using the = and ^~ modifiers.

It is also important to note that, while prefix locations generally select based on the longest, most specific match, regular expression evaluation is stopped when the first matching location is found. 
This means that positioning within the configuration has vast implications for regular expression locations.

Finally, it it is important to understand that regular expression matches within the longest prefix match will “jump the line” when Nginx evaluates regex locations.
These will be evaluated, in order, before any of the other regular expression matches are considered.
Maxim Dounin, an incredibly helpful Nginx developer, explains in [this post](https://www.ruby-forum.com/t/proxy-pass-location-inheritance/239135#1136698) this portion of the selection algorithm.

---

# When Does Location Block Evaluation Jump to Other Locations?

Only the selected location and the inherited directives determine how the request is processed, without interference from sibling location blocks.

There are times when a new location search is triggered by certain directives within the selected location.
The exceptions to the “only one location block” rule may have implications on how the request is actually served and may not align with the expectations you had when designing your location blocks.

Some directives that can lead to this type of internal redirect are:

   - index
   - try_files
   - rewrite
   - error_page

The `index` directive always leads to an internal redirect if it is used to handle the request. 
However, if you make an exact location match that is a directory, there is a good chance that the request will be redirected to a different location for actual processing.

In this example, the first location is matched by a request URI of `/exact`, but in order to handle the request, the index directive inherited by the block initiates an internal redirect to the second block:
```nginx
index index.html;

location = /exact {

    . . .

}

location / {

    . . .

}
```
In the case above, if you really need the execution to stay in the first block, you will have to come up with a different method of satisfying the request to the directory. 
For instance, you could set an invalid `index` for that block and turn on autoindex:

```nginx
location = /exact {
    index nothing_will_match;
    autoindex on;
}

location  / {

    . . .

}
```

Another instance where the processing location may be reevaluated is with the `try_files` directive. 
This directive tells Nginx to check for the existence of a named set of files or directories.
The last parameter can be a URI that Nginx will make an internal redirect to.

```nginx
root /var/www/main;
location / {
    try_files $uri $uri.html $uri/ /fallback/index.html;
}

location /fallback {
    root /var/www/another;
}
```
In the above example, if a request is made for `/blahblah`, the first location will initially get the request.
It will try to find a file called `blahblah` in `/var/www/main directory`.
Failing all of these attempts, it will redirect to `/fallback/index.html`.
This will trigger another location search that will be caught by the second location block.
This will serve the file `/var/www/another/fallback/index.html`.


Another directive that can lead to a location block pass off is the `rewrite` directive.
When using the last parameter with the `rewrite` directive, or when using no parameter at all, Nginx will search for a new matching location based on the results of the rewrite.

For example, if we modify the last example to include a `rewrite`, we can see that the request is sometimes passed directly to the second location without relying on the `try_files` directive:
```nginx
root /var/www/main;
location / {
    rewrite ^/rewriteme/(.*)$ /$1 last;
    try_files $uri $uri.html $uri/ /fallback/index.html;
}

location /fallback {
    root /var/www/another;
}
```
In the above example, a request for `/rewriteme/hello` will be handled initially by the first location block. 
It will be rewritten to `/hello` and a location will be searched.
In this case, it will match the first location again and be processed by the `try_files` as usual, maybe kicking back to `/fallback/index.html` if nothing is found (using the `try_files` internal redirect we discussed above).

However, if a request is made for `/rewriteme/fallback/hello`, the first block again will match. 
The rewrite be applied again, this time resulting in `/fallback/hello`.
The request will then be served out of the second location block.

A related situation happens with the `return` directive when sending the `301` or `302` status codes.
The difference in this case is that it results in an entirely new request in the form of an externally visible redirect. 
This same situation can occur with the `rewrite` directive when using the `redirect` or `permanent` flags. 
However, these location searches shouldn’t be unexpected, since externally visible redirects always result in a new request.


The `error_page` directive can lead to an internal redirect similar to that created by `try_files`.
This directive is used to define what should happen when certain status codes are encountered. 
This will likely never be executed if `try_files` is set, since that directive handles the entire life cycle of a request.

```nginx
root /var/www/main;

location / {
    error_page 404 /another/whoops.html;
}

location /another {
    root /var/www;
}
```
Every request (other than those starting with `/another`) will be handled by the first block, which will serve files out of `/var/www/main`.
However, if a file is not found (a 404 status), an internal redirect to `/another/whoops.html` will occur, leading to a new location search that will eventually land on the second block. 
This file will be served out of `/var/www/another/whoops.html`.


