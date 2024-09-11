#!/usr/bin/env ruby
require 'cgi'

cgi = CGI.new
puts "Content-Type: text/html\n\n"
puts "<h1>Hello, World!</h1>"
