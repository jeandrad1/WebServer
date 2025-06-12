#include "HttpRequestRouter.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

HttpResponse HttpRequestRouter::handleRequest(const HttpRequest& req)
{
	std::string method = req.getMethod();

	if (method == "GET")         return handleGet(req);
	else if (method == "POST")   return handlePost(req);
	else if (method == "DELETE") return handleDelete(req);
	else                         return methodNotAllowed();
}

HttpResponse HttpRequestRouter::handleGet(const HttpRequest& req) {
    HttpResponse res;
    res.setStatus(200, "OK");
    res.setBody("GET handled\n");
    std::ostringstream oss;
    oss << res.getBody().size();
    res.setHeader("Content-Length", oss.str());
    return res;
}

HttpResponse HttpRequestRouter::handlePost(const HttpRequest& req) {
    HttpResponse res;
    res.setStatus(200, "OK");
    std::string body = req.getBody();
    res.setBody("POST handled with body: " + body);
    std::ostringstream oss;
    oss << res.getBody().size();
    res.setHeader("Content-Length", oss.str());
    return res;
}

HttpResponse HttpRequestRouter::handleDelete(const HttpRequest& req) {
    HttpResponse res;
    res.setStatus(200, "OK");
    res.setBody("DELETE handled\n");
    std::ostringstream oss;
    oss << res.getBody().size();
    res.setHeader("Content-Length", oss.str());
    return res;
}

HttpResponse HttpRequestRouter::methodNotAllowed() {
    HttpResponse res;
    res.setStatus(405, "Method Not Allowed");
    res.setBody("405 Method Not Allowed\n");
    std::ostringstream oss;
    oss << res.getBody().size();
    res.setHeader("Content-Length", oss.str());
    return res;
}