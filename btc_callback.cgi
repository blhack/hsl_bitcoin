#!/usr/bin/python

import cgi
import sys
import json

things = json.loads(sys.stdin.read())

amount = things["amount"]
amount = float(amount)
current = open("current.btc","r").read().rstrip()
current = float(current)
current = current+amount

output = open("current.btc","w")
output.write("%.8f" % (current))

#output = open("field","w")
#output.write(str(things))

print "content-type:text/plain"
print
