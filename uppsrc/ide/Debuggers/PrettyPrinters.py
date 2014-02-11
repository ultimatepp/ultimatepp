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

#floating pointer printer
class FloatPrinter(object):
	"Print a floating point number with fixed DOT separator"

	def __init__(self, val):
		self.val = val

	def to_string(self):
		return "%.15f" % self.val

	def display_hint(self):
		return 'String'


# Upp::String printer
class UppStringPrinter(object):
	"Print an Upp::String"

	def __init__(self, val):
		self.val = val

	def to_string(self):
		try:
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
		except Exception as inst:
			return "<can't read string>"

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
			self.count = val['items']
			self.idx = -1

		def __iter__(self):
			return self

		def __next__(self):
			self.idx += 1
			if self.idx >= self.count:
				raise StopIteration
			elt = self.item.dereference()
			self.item += 1
			return ('[%d]' % self.idx, elt)

	def __init__(self, typename, val):
		self.typename = typename
		self.val = val

	def children(self):
		return self._iterator(self.val)

	def to_string(self):
		count = self.val['items']
		alloc = self.val['alloc']
		return ('<%s> = {<No data fields>}, count = %d, alloc = %d, elements'
			% (self.typename, count, alloc))

	def display_hint(self):
		return 'array'

# Upp::Array printer
class UppArrayPrinter(object):
	"Print an Upp::Array"
	class _iterator:
		def __init__ (self, val):
			self.val = val['vector']
			self.item = self.val['vector']
			self.count = self.val['items']
			self.idx = -1

		def __iter__(self):
			return self

		def __next__(self):
			self.idx += 1
			if self.idx >= self.count:
				raise StopIteration
			elt = self.item.dereference().dereference()
			self.item += + 1
			return ('[%d]' % self.idx, elt)

	def __init__(self, typename, val):
		self.typename = typename
		self.val = val

	def children(self):
		return self._iterator(self.val)

	def to_string(self):
		count = self.val['vector']['items']
		alloc = self.val['vector']['alloc']
		return ('<%s> = {<No data fields>}, count = %d, alloc = %d, elements'
			% (self.typename, count, alloc))

	def display_hint(self):
		return 'array'

# Upp::VectorMap printer
class UppVectorMapPrinter(object):
	"Print an Upp::VectorMap"
	class _iterator:
		def __init__ (self, val):
			self.val = val
			self.key = val['key']['key']['vector']
			self.value = val['value']['vector']
			self.count = val['value']['items']
			self.idx = -1

		def __iter__(self):
			return self

		def __next__(self):
			self.idx += 1
			if self.idx >= self.count * 2:
				raise StopIteration
			if self.idx & 1:
				aValue = self.value.dereference()
				self.value += 1
				return ('value', str(aValue))
			else:
				aKey = self.key.dereference()
				self.key += 1
				return ('key', str(aKey))

	def __init__(self, typename, val):
		self.typename = typename
		self.val = val

	def children(self):
		return self._iterator(self.val)

	def to_string(self):
		count = self.val['key']['key']['items']
		alloc = self.val['key']['key']['alloc']
		return ('<%s> = {<No data fields>}, count = %d, alloc = %d, elements'
			% (self.typename, count, alloc))

	def display_hint(self):
		return 'map'

# Upp::ArrayMap printer
class UppArrayMapPrinter(object):
	"Print an Upp::ArrayMap"
	class _iterator:
		def __init__ (self, val):
			self.val = val
			self.key = val['key']['key']['vector']
			self.value = val['value']['vector']['vector']
			self.count = val['value']['vector']['items']
			self.idx = -1

		def __iter__(self):
			return self

		def __next__(self):
			self.idx += 1
			if self.idx >= self.count * 2:
				raise StopIteration
			if self.idx & 1:
				aValue = self.value.dereference().dereference()
				self.value += 1
				return ('value', str(aValue))
			else:
				aKey = self.key.dereference()
				self.key += 1
				return ('key', str(aKey))

	def __init__(self, typename, val):
		self.typename = typename
		self.val = val

	def children(self):
		return self._iterator(self.val)

	def to_string(self):
		count = self.val['key']['key']['items']
		alloc = self.val['key']['key']['alloc']
		return ('<%s> = {<No data fields>}, count = %d, alloc = %d, elements'
			% (self.typename, count, alloc))

	def display_hint(self):
		return 'map'

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
		try:
			magic = self.val['magic']
		except Exception as inst:
			return '<value inspectors not supported>'
		if magic[0] != 0xc436d851 or magic[1] != 0x72f67c76 or magic[2] != 0x3e5e10fd or magic[3] != 0xc90d370b:
			return '<value not initialized>'
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
	
	if typeStr == 'double' or typeStr == 'float' or typeStr == 'long double':
		return FloatPrinter(val)

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
		
##########################################################################
#						FOR VALUES -- EXPERIMENTAL
# 2012/08/05 - REMOVED AGAIN AS IT CAUSED SOME RANDOM CRASHES
#              NEEDS DEEPER INVESTIGATION
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

##########################################################################
	if typeStr == 'Upp::Point' or typeStr == 'Upp::Pointf':
		return UppPointPrinter(val)
		
	if typeStr == 'const Upp::Point' or typeStr == 'const Upp::Pointf':
		return UppPointPrinter(val)
		
	if typeStr == 'Upp::Point *' or typeStr == 'Upp::Pointf *':
		return UppPointPtrPrinter(val)
		
	if typeStr == 'const Upp::Point *' or typeStr == 'const Upp::Pointf *':
		return UppPointPtrPrinter(val)
		
	if typeStr == 'Upp::Size' or typeStr == 'Upp::Sizef':
		return UppSizePrinter(val)
		
	if typeStr == 'const Upp::Size' or typeStr == 'const Upp::Sizef':
		return UppSizePrinter(val)
		
	if typeStr == 'Upp::Size *' or typeStr == 'Upp::Sizef *':
		return UppSizePtrPrinter(val)
		
	if typeStr == 'const Upp::Size *' or typeStr == 'const Upp::Sizef *':
		return UppSizePtrPrinter(val)
		
	if typeStr == 'Upp::Rect' or typeStr == 'Upp::Rectf':
		return UppRectPrinter(val)
		
	if typeStr == 'const Upp::Rect' or typeStr == 'const Upp::Rectf':
		return UppRectPrinter(val)
		
	if typeStr == 'Upp::Rect *' or typeStr == 'Upp::Rectf *':
		return UppRectPtrPrinter(val)
		
	if typeStr == 'const Upp::Rect *' or typeStr == 'const Upp::Rectf *':
		return UppRectPtrPrinter(val)
		
	lookup_tag = val.type.tag
	if lookup_tag == None:
		return None

	regex = re.compile("^Upp::VectorMap<.*>$")
	if regex.match(lookup_tag):
		return UppVectorMapPrinter(lookup_tag, val)

	regex = re.compile("^Upp::ArrayMap<.*>$")
	if regex.match(lookup_tag):
		return UppArrayMapPrinter(lookup_tag, val)

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
	
#check if debug ungrab support is enabled
Upp_Ungrab_Support = True
try:
	Upp_Ungrab				= gdb.parse_and_eval('Upp::_DBG_Ungrab')
except Exception as inst:
	Upp_Ungrab_Support = False
	
#ungrab mouse at debugger stop, if ungrab helper is available
def stop_handler(event):
	if Upp_Ungrab_Support:
		Upp_Ungrab()

gdb.events.stop.connect (stop_handler)


gdb.pretty_printers.append(UppLookupFunction)

#a couple of lines to test it quickly on gdb....
#source /home/massimo/sources/upp-svn/uppsrc/ide/Debuggers/PrettyPrinters.py
#python gdb.pretty_printers.remove(UppLookupFunction)
