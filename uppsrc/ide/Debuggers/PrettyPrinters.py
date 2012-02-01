import gdb.printing
import re

# just for debugging purposes
def dump(obj):
	'''return a printable representation of an object for debugging'''
	newobj=obj
	if '__dict__' in dir(obj):
		newobj=obj.__dict__
		if ' object at ' in str(obj) and not newobj.has_key('__type__'):
			newobj['__type__']=str(obj)
		for attr in newobj:
			newobj[attr]=dump(newobj[attr])
	return newobj

# Upp::String printer
class UppStringPrinter(object):
	"Print an Upp::String"

	def __init__(self, val):
		self.val = val

	def to_string(self):
		SMALL = 0
		MEDIUM = 31
		KIND = 14
		SLEN = 15
		LLEN = 2
		CHR = self.val['chr']
		IsSmall = (CHR[KIND] == SMALL)
		if(IsSmall):
			ptr = CHR
		else:
			ptr = self.val[ptr]
		return ptr.string()

	def display_hint(self):
		return 'String'

# Upp::Vector printer
class UppVectorPrinter(object):
	"Print an Upp::Vector"
	class _iterator:
		def __init__ (self, start, finish):
			self.item = start
			self.finish = finish
			self.count = 0

		def __iter__(self):
			return self

		def next(self):
			count = self.count
			self.count = self.count + 1
			if self.item == self.finish:
				raise StopIteration
			elt = self.item.dereference()
			self.item = self.item + 1
			return ('[%d]' % count, elt)

	def __init__(self, typename, val):
		self.typename = typename
		self.val = val

	def children(self):
		start = self.val['vector']
		finish = start + self.val['items']
		return self._iterator(start, finish)

	def to_string(self):
		start = 0
		finish = self.val['items']
		end = self.val['alloc']
		return ('%s count %d alloc %d'
			% (self.typename, int (finish - start), int (end - start)))

	def display_hint(self):
		return 'array'

def UppLookupFunction(val):
	lookup_tag = val.type.tag
	if lookup_tag == None:
		return None

	regex = re.compile("^Upp::String$")
	if regex.match(lookup_tag):
		return UppStringPrinter(val)

	regex = re.compile("^Upp::Vector<.*>$")
	if regex.match(lookup_tag):
		return UppVectorPrinter(lookup_tag, val)

	return None

gdb.pretty_printers.append(UppLookupFunction)

#a couple of lines to test it quickly on gdb....
#source /home/massimo/sources/upp-svn/uppsrc/ide/Debuggers/PrettyPrinters.py
#python gdb.pretty_printers.remove(UppLookupFunction)
