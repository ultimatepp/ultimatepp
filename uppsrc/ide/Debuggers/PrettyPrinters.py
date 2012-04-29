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
			ptr = self.val['ptr']
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

# Upp::VectorMap and ArrayMap printer
class UppMapPrinter(object):
	"Print an Upp::VectorMap or ArrayMap"

	def __init__(self, typename, val):
		self.typename = typename
		self.val = val

	def children(self):
		return [('.keys', self.val['key']), ('.values', self.val['value'])].__iter__()

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
		elif typeId in (1, 2, 3, 4, 5, 7, 8, 10, 11):
			return str(Upp_Value_GetString(self.val))
		elif typeId == 6:
			return '<ERROR VALUE>'
		else:
			strVal = str(Upp_Value_GetString(self.val))
			if strVal != '':
				return strVal
			else:
				if self.typeIds.has_key(typeId):
					return "<unsupported value type '" + self.typeIds[typeId] + "'>"
				else:
					return "<unsupported value type '" + str(typeId) + "'>"

# Upp::Value* printer
class UppValuePtrPrinter(object):
	"Print an Upp::Value *"

	def __init__(self, val):
		self.val = val

	def to_string(self):
		return '@' + str(self.val.address) + ': ' + UppValuePrinter(self.val.dereference()).to_string()

	def display_hint(self):
		return 'String'

# Upp::One<> printer
class UppOnePrinter(object):

	def __init__(self, val):
		self.val = val
		
	def to_string(self):
		return str(self.val['ptr'].dereference())

# Upp::Point printer
class UppPointPrinter(object):
	"Print an Upp::Point"

	def __init__(self, val):
		self.val = val

	def to_string(self):
		return '(' + str(self.val['x']) + ',' + str(self.val['y']) + ')'

# Upp::Point* printer
class UppPointPtrPrinter(object):
	"Print an Upp::Point *"

	def __init__(self, val):
		self.val = val

	def to_string(self):
		return '@' + str(self.val.address) + ': ' + UppPointPrinter(self.val).to_string()

# Upp::Size printer
class UppSizePrinter(object):
	"Print an Upp::Size"

	def __init__(self, val):
		self.val = val

	def to_string(self):
		return '(' + str(self.val['cx']) + ',' + str(self.val['cy']) + ')'

# Upp::Size* printer
class UppSizePtrPrinter(object):
	"Print an Upp::Size *"

	def __init__(self, val):
		self.val = val

	def to_string(self):
		return '@' + str(self.val.address) + ': ' + UppSizePrinter(self.val).to_string()

# Upp::Rect printer
class UppRectPrinter(object):
	"Print an Upp::Rect"

	def __init__(self, val):
		self.val = val

	def to_string(self):
		return '[' + str(self.val['left']) + ',' + str(self.val['top']) + '],[' + str(self.val['right']) + ',' + str(self.val['bottom']) + ']'

# Upp::Rect* printer
class UppRectPtrPrinter(object):
	"Print an Upp::Rect *"

	def __init__(self, val):
		self.val = val

	def to_string(self):
		return '@' + str(self.val.address) + ': ' + UppRectPrinter(self.val).to_string()

def UppLookupFunction(val):
	typeStr = str(val.type)
	
	if typeStr == 'Upp::String *':
		return UppStringPtrPrinter(val)

	if typeStr == 'const Upp::String *':
		return UppStringPtrPrinter(val)

	if typeStr == 'Upp::String &':
		return UppStringPtrPrinter(val.address)

	if typeStr == 'const Upp::String &':
		return UppStringPtrPrinter(val.address)

	if typeStr == 'Upp::String':
		return UppStringPrinter(val)
		
	if typeStr == 'const Upp::String':
		return UppStringPrinter(val)
		
#	if typeStr == 'Upp::Value *' and Upp_Value_Inspectors:
#		return UppValuePtrPrinter(val)
		
#	if typeStr == 'const Upp::Value *' and Upp_Value_Inspectors:
#		return UppValuePtrPrinter(val)
		
#	if typeStr == 'Upp::Value &' and Upp_Value_Inspectors:
#		return UppValuePtrPrinter(val.address)
		
#	if typeStr == 'const Upp::Value &' and Upp_Value_Inspectors:
#		return UppValuePtrPrinter(val.address)
		
#	if typeStr == 'Upp::Value' and Upp_Value_Inspectors:
#		return UppValuePrinter(val)
		
#	if typeStr == 'const Upp::Value' and Upp_Value_Inspectors:
#		return UppValuePrinter(val)

	if typeStr == 'Upp::Point' or typeStr == 'Upp::Pointf':
		return UppPointPrinter(val)
		
	if typeStr == 'Upp::Point *' or typeStr == 'Upp::Pointf *':
		return UppPointPtrPrinter(val)
		
	if typeStr == 'Upp::Size' or typeStr == 'Upp::Sizef':
		return UppSizePrinter(val)
		
	if typeStr == 'Upp::Size *' or typeStr == 'Upp::Sizef *':
		return UppSizePtrsPrinter(val)
		
	if typeStr == 'Upp::Rect' or typeStr == 'Upp::Rectf':
		return UppRectPrinter(val)
		
	if typeStr == 'Upp::Rect *' or typeStr == 'Upp::Rectf *':
		return UppRectPtrPrinter(val)
		
	lookup_tag = val.type.tag
	if lookup_tag == None:
		return None

	regex = re.compile("^Upp::VectorMap<.*>$")
	if regex.match(lookup_tag):
		return UppMapPrinter(lookup_tag, val)

	regex = re.compile("^Upp::ArrayMap<.*>$")
	if regex.match(lookup_tag):
		return UppMapPrinter(lookup_tag, val)

	regex = re.compile("^Upp::Vector<.*>$")
	if regex.match(lookup_tag):
		return UppVectorPrinter(lookup_tag, val)

	regex = re.compile("^Upp::Array<.*>$")
	if regex.match(lookup_tag):
		return UppArrayPrinter(lookup_tag, val)

	regex = re.compile("^Upp::Index<.*>$")
	if regex.match(lookup_tag):
		return UppVectorPrinter(lookup_tag, val['key'])

	regex = re.compile("^Upp::One<.*>$")
	if regex.match(lookup_tag):
		return UppOnePrinter(val)

	return None

#check if Upp value inspecting support is enabled
Upp_Value_Inspectors = True
try:
	Upp_Value_GetType		= gdb.parse_and_eval('Upp::_DBG_Value_GetType')
	Upp_Value_GetString		= gdb.parse_and_eval('Upp::_DBG_Value_AsString')
except Exception as inst:
	Upp_Value_Inspectors = False

gdb.pretty_printers.append(UppLookupFunction)

#a couple of lines to test it quickly on gdb....
#source /home/massimo/sources/upp-svn/uppsrc/ide/Debuggers/PrettyPrinters.py
#python gdb.pretty_printers.remove(UppLookupFunction)
