|  Method | Description | Possible Body |
| ------------- | -------------- | -------------- |
| GET 	| Retrieve a specific resource or a collection of resources, should not affect the data/resource 	| No |
| POST 	| Perform resource-specific processing on the request content 	| Yes |
| DELETE| Removes target resource given by a URI 	                    | Yes |
| PUT 	| Creates a new resource with data from message body, if resource already exist, update it with data in body 	| Yes |
| HEAD 	| Same as GET, but do not transfer the response content 	    | No |

# GET

HTTP GET method is used to read (or retrieve) a representation of a resource. 
In case of success (or non-error), GET returns a representation of the resource in response body and HTTP response status code of 200 (OK). 
In an error case, it most often returns a 404 (NOT FOUND) or 400 (BAD REQUEST).

# POST

HTTP POST method is most often utilized to create new resources. 
On successful creation, HTTP response code 201 (Created) is returned.

# DELETE

HTTP DELETE is stright forward. 
It deletes a resource specified in URI. 
On successful deletion, it returns HTTP response status code 204 (No Content).

Read more about HTTP methods [RFC9110#9.1](https://www.rfc-editor.org/rfc/rfc9110.html#name-methods)
