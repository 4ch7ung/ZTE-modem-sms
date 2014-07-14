import sys
if len(sys.argv) < 2:
	print "Usage: decode-utf16.py <string_to_decode>"
	exit(1)
print unicode(sys.argv[1].decode('hex'), 'utf-16-be').encode('utf8')
