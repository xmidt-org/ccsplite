// Load the http module to create an http server.
var http = require('http');

// Configure our HTTP server to respond with Hello World to all requests.
var server = http.createServer(function (request, response) {
    response.writeHead(200, {"Content-Type": "text/plain"});

    var s = '';
    var send_end = 1;
    if (-1 != request.url.indexOf("Device.IntString512")) {
        s = '{"statusCode":200,"parameters":[{"name":"Device.Int","value":"512","dataType":2,"parameterCount":1,"message":"Success"}]}';
    } else if (-1 != request.url.indexOf("Device.IntStringMinus512")) {
        s = '{"statusCode":200,"parameters":[{"name":"Device.Int","value":"-512","dataType":2,"parameterCount":1,"message":"Success"}]}';
    } else if (-1 != request.url.indexOf("Device.IntInt512")) {
        s = '{"statusCode":200,"parameters":[{"name":"Device.Int","value":512,"dataType":2,"parameterCount":1,"message":"Success"}]}';
    } else if (-1 != request.url.indexOf("Device.IntIntMinus512")) {
        s = '{"statusCode":200,"parameters":[{"name":"Device.Int","value":-512,"dataType":2,"parameterCount":1,"message":"Success"}]}';
    } else if (-1 != request.url.indexOf("Device.BoolStringTrue")) {
        s = '{"statusCode":200,"parameters":[{"name":"Device.Int","value":"true","dataType":2,"parameterCount":1,"message":"Success"}]}';
    } else if (-1 != request.url.indexOf("Device.BoolStringFalse")) {
        s = '{"statusCode":200,"parameters":[{"name":"Device.Int","value":"false","dataType":2,"parameterCount":1,"message":"Success"}]}';
    } else if (-1 != request.url.indexOf("Device.BoolTrue")) {
        s = '{"statusCode":200,"parameters":[{"name":"Device.Int","value":true,"dataType":2,"parameterCount":1,"message":"Success"}]}';
    } else if (-1 != request.url.indexOf("Device.BoolFalse")) {
        s = '{"statusCode":200,"parameters":[{"name":"Device.Int","value":false,"dataType":2,"parameterCount":1,"message":"Success"}]}';
    } else if (-1 != request.url.indexOf("Device.StringString")) {
        s = '{"statusCode":200,"parameters":[{"name":"Device.Int","value":"I am a string.","dataType":2,"parameterCount":1,"message":"Success"}]}';
    } else if (-1 != request.url.indexOf("Device.LongString")) {
        response.write('{"statusCode":200,"parameters":[{"name":"Device.Int","value":', "binary")
        send_end = 0;
        setTimeout( function() {
                                    response.write('"broken text",', "binary");
                                    response.end('"dataType":2,"parameterCount":1,"message":"Success"}]}');
                               }, 1000 );
    } else if (-1 != request.url.indexOf("Device.NonJson")) {
        s = 'unknown';
    } else if (-1 != request.url.indexOf("Device.WrongDom")) {
        s = '{"not-the-right-format": -1}';
    } else if (-1 != request.url.indexOf("Device.Delay2")) {
        send_end = 0;
        s = '{"timeout": -1}';
        setTimeout( function() { response.end(s) }, 2000 );
    } else {
        s = 'unknown';
    }

    if (1 == send_end) {
        response.end(s);
    }
    //console.log(s);
});

    // Listen on port 62000, IP defaults to localhost
    server.listen(62000);

    // Put a friendly message on the terminal
    console.log("Server running at http://localhost:62000/");
