from http import server # Python 3

class MyHTTPRequestHandler(server.SimpleHTTPRequestHandler):
    def end_headers(self):
        self.send_my_headers()
        server.SimpleHTTPRequestHandler.end_headers(self)

    def send_my_headers(self):
        self.send_header("Access-Control-Allow-Origin", "*")
        if self.path.count(".json.gz") == 1 and (self.path.count("?") == 0 or self.path.count("?compact") == 1 or self.path.count("?method") == 1):
           self.send_header("Content-Encoding", "gzip")

if __name__ == '__main__':
    server.test(HandlerClass=MyHTTPRequestHandler)
