import base64
import zlib
import sys

data_ctext = open(sys.argv[1], 'r').read()
#print(data_ctext[5:])
data_ctext_bin = base64.b64decode(data_ctext[11:])
data_ptext = zlib.decompress(data_ctext_bin[16:])
print(data_ptext)
