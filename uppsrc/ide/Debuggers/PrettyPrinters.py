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
		return '"' + ptr.string() + '"'

	def display_hint(self):
		return 'String'

# Upp::String* printer
class UppStringPtrPrinter(object):
	"Print an Upp::String *"

	def __init__(self, val):
		self.val = val

	def to_string(self):
		return '@' + str(self.val.address) + ': ' + UppStringPrinter(self.val).to_string()

	def display_hint(self):
		return 'String'

# Upp::Vector printer
class UppVectorPrinter(object):
	"Print an Upp::Vector"
	class _iterator:
		def __init__ (self, val):
			self.val = val
			self.item = val['vector']
			self.finish = self.item + val['items']
			self.count = 0

		def __iter__(self):
			return self

		def next(self):
			count = self.count
			self.count = self.count + 1
			if count == 0:
				return ('.items', self.val['items'])
			if count == 1:
				return ('.alloc', self.val['alloc'])
			if self.item == self.finish:
				raise StopIteration
			elt = self.item.dereference()
			self.item = self.item + 1
			return ('[%d]' % (count-2), elt)

	def __init__(self, typename, val):
		self.typename = typename
		self.val = val

	def children(self):
		return self._iterator(self.val)

	def to_string(self):
		start = 0
		finish = self.val['items']
		end = self.val['alloc']
		return ('%s count %d alloc %d'
			% (self.typename, int (finish - start), int (end - start)))

	def display_hint(self):
		return 'array'

# Upp::Array printer
class UppArrayPrinter(object):
	"Print an Upp::Array"
	class _iterator:
		def __init__ (self, val):
			self.val = val['vector']
			self.item = self.val['vector']
			self.finish = self.item + self.val['items']
			self.count = 0

		def __iter__(self):
			return self

		def next(self):
			count = self.count
			self.count = self.count + 1
			if count == 0:
				return ('.items', self.val['items'])
			if count == 1:
				return ('.alloc', self.val['alloc'])
			if self.item == self.finish:
				raise StopIteration
			elt = self.item.dereference().dereference()
			self.item = self.item + 1
			return ('[%d]' % (count-2), elt)

	def __init__(self, typename, val):
		self.typename = typename
		self.val = val

	def children(self):
		return self._iterator(self.val)

	def to_string(self):
		start = 0
		finish = self.val['vector']['items']
		end = self.val['vector']['alloc']
		return ('%s count %d alloc %d'
			% (self.typename, int (finish - start), int (end - start)))

	def display_hint(self):
		return 'array'

# Upp::VectorMap printer
class UppVectorMapPrinter(object):
	"Print an Upp::VectorMap"

	def __init__(self, typename, val):
		self.typename = typename
		self.val = val

	def children(self):
		return [('keys', self.val['key']), ('values', self.val['value'])].__iter__()

	def to_string(self):
		count = self.val['key']['key']['items']
		alloc = self.val['key']['key']['alloc']
		return ('%s count %d alloc %d'
			% (self.typename, count, alloc))

	def display_hint(self):
		return 'array'

# Upp::Value printer
class UppValuePrinter(object):

	def __init__(self, val):
		self.typeIds ={
			0:'void',
			1:'int',
			2:'double',
			3:'Upp::String',
			4:'Upp::Date',
			5:'Upp::Time',
			6:'ERROR',
			7:'Upp::Value',
			8:'Upp::WString',
			9:'Upp::ValueArray',
			10:'int64',
			11:'bool',
			12:'Upp::ValueMap'
		}
		self.val = val
		
	def to_string(self):
		typeId = Upp_Value_GetType(self.val)
		if typeId == 0:
			return '<void>'
		elif typeId == 1 or typeId == 10:
			return str(Upp_Value_GetInteger(self.val))
		elif typeId == 2:
			return str(Upp_Value_GetDouble(self.val))
		elif typeId == 3:
			return str(Upp_Value_GetString(self.val))
		elif typeId == 6:
			return '<ERROR VALUE>'
		elif typeId == 7:
			return str(Upp_Value_GetValue(self.val))
		elif typeId == 8:
			return str(Upp_Value_GetWString(self.val))
		elif typeId == 11:
			return str(Upp_Value_GetBool(self.val))
		elif self.typeIds.has_key(typeId):
			return "<unsupported value type '" + self.typeIds[typeId] + "'>"
		else:
			return "<unsupported value type '" + str(typeId) + "'>"

def UppLookupFunction(val):
	typeStr = str(val.type)
	
	if typeStr == 'Upp::String *':
		return UppStringPtrPrinter(val)

	if typeStr == 'Upp::String':
		return UppStringPrinter(val)
		
	if typeStr == 'Upp::Value' and Upp_Value_Inspectors:
		return UppValuePrinter(val)
		
	lookup_tag = val.type.tag
	if lookup_tag == None:
		return None

	regex = re.compile("^Upp::VectorMap<.*>$")
	if regex.match(lookup_tag):
		return UppVectorMapPrinter(lookup_tag, val)

	regex = re.compile("^Upp::Vector<.*>$")
	if regex.match(lookup_tag):
		return UppVectorPrinter(lookup_tag, val)

	regex = re.compile("^Upp::Array<.*>$")
	if regex.match(lookup_tag):
		return UppArrayPrinter(lookup_tag, val)

	regex = re.compile("^Upp::Index<.*>$")
	if regex.match(lookup_tag):
		return UppVectorPrinter(lookup_tag, val['key'])

	return None

#check if Upp value inspecting support is enabled
Upp_Value_Inspectors = True
try:
	Upp_Value_GetType		= gdb.parse_and_eval('Upp::_GDB_Value_GetType')
	Upp_Value_GetString		= gdb.parse_and_eval('Upp::_GDB_Value_GetString')
	Upp_Value_GetWString	= gdb.parse_and_eval('Upp::_GDB_Value_GetWString')
	Upp_Value_GetInteger	= gdb.parse_and_eval('Upp::_GDB_Value_GetInteger')
	Upp_Value_GetDouble		= gdb.parse_and_eval('Upp::_GDB_Value_GetDouble')
	Upp_Value_GetDate		= gdb.parse_and_eval('Upp::_GDB_Value_GetDate')
	Upp_Value_GetTime		= gdb.parse_and_eval('Upp::_GDB_Value_GetTime')
	Upp_Value_GetBool		= gdb.parse_and_eval('Upp::_GDB_Value_GetBool')
	Upp_Value_GetValue		= gdb.parse_and_eval('Upp::_GDB_Value_GetValue')
except Exception as inst:
	Upp_Value_Inspectors = False

gdb.pretty_printers.append(UppLookupFunction)

#a couple of lines to test it quickly on gdb....
#source /home/massimo/sources/upp-svn/uppsrc/ide/Debuggers/PrettyPrinters.py
#python gdb.pretty_printers.remove(UppLookupFunction)
