-module(hello).
-export([world/0]).

world() ->
    io:format("Content-Type: text/html~n~n"),
    io:format("<h1>Hello, World!</h1>").
