#
# qp_xml: Quick Parsing for XML
#
# Written by Greg Stein. Public Domain.
# No Copyright, no Rights Reserved, and no Warranties.
#
# This module is maintained by Greg and is available as part of the XML-SIG
# distribution. This module and its changelog can be fetched at:
#    http://www.lyra.org/cgi-bin/viewcvs.cgi/xml/xml/utils/qp_xml.py
#
# Additional information can be found on Greg's Python page at:
#    http://www.lyra.org/greg/python/
#
# This module was added to the XML-SIG distribution on February 14, 2000.
# As part of that distribution, it falls under the XML distribution license.
#

import string

try:
  import pyexpat
except ImportError:
  from xml.parsers import pyexpat

error = __name__ + '.error'


#
# The parsing class. Instantiate and pass a string/file to .parse()
#
class Parser:
  def __init__(self):
    self.reset()

  def reset(self):
    self.root = None
    self.cur_elem = None

  def find_prefix(self, prefix):
    elem = self.cur_elem
    while elem:
      if elem.ns_scope.has_key(prefix):
        return elem.ns_scope[prefix]
      elem = elem.parent

    if prefix == '':
      return ''		# empty URL for "no namespace"

    return None

  def process_prefix(self, name, use_default):
    idx = string.find(name, ':')
    if idx == -1:
      if use_default:
        return self.find_prefix(''), name
      return '', name	# no namespace

    if string.lower(name[:3]) == 'xml':
      return '', name	# name is reserved by XML. don't break out a NS.

    ns = self.find_prefix(name[:idx])
    if ns is None:
      raise error, 'namespace prefix ("%s") not found' % name[:idx]

    return ns, name[idx+1:]

  def start(self, name, attrs):
    elem = _element(name=name, lang=None, parent=None,
                    children=[], ns_scope={}, attrs={},
                    first_cdata='', following_cdata='')

    if self.cur_elem:
      elem.parent = self.cur_elem
      elem.parent.children.append(elem)
      self.cur_elem = elem
    else:
      self.cur_elem = self.root = elem

    work_attrs = [ ]

    # scan for namespace declarations (and xml:lang while we're at it)
    for name, value in attrs.items():
      if name == 'xmlns':
        elem.ns_scope[''] = value
      elif name[:6] == 'xmlns:':
        elem.ns_scope[name[6:]] = value
      elif name == 'xml:lang':
        elem.lang = value
      else:
        work_attrs.append((name, value))

    # inherit xml:lang from parent
    if elem.lang is None and elem.parent:
      elem.lang = elem.parent.lang

    # process prefix of the element name
    elem.ns, elem.name = self.process_prefix(elem.name, 1)

    # process attributes' namespace prefixes
    for name, value in work_attrs:
      elem.attrs[self.process_prefix(name, 0)] = value

  def end(self, name):
    parent = self.cur_elem.parent

    del self.cur_elem.ns_scope
    del self.cur_elem.parent

    self.cur_elem = parent

  def cdata(self, data):
    elem = self.cur_elem
    if elem.children:
      last = elem.children[-1]
      last.following_cdata = last.following_cdata + data
    else:
      elem.first_cdata = elem.first_cdata + data

  def parse(self, input):
    self.reset()

    p = pyexpat.ParserCreate()
    p.StartElementHandler = self.start
    p.EndElementHandler = self.end
    p.CharacterDataHandler = self.cdata

    try:
      if type(input) == type(''):
        p.Parse(input, 1)
      else:
        while 1:
          s = input.read(_BLOCKSIZE)
          if not s:
            p.Parse('', 1)
            break

          p.Parse(s, 0)

    finally:
      if self.root:
        _clean_tree(self.root)

    return self.root


#
# handy function for dumping a tree that is returned by Parser
#
def dump(f, root):
  f.write('<?xml version="1.0"?>\n')
  namespaces = _collect_ns(root)
  _dump_recurse(f, root, namespaces, dump_ns=1)
  f.write('\n')


#
# This function returns the element's CDATA. Note: this is not recursive --
# it only returns the CDATA immediately within the element, excluding the
# CDATA in child elements.
#
def textof(elem):
  return elem.textof()


#########################################################################
#
# private stuff for qp_xml
#

_BLOCKSIZE = 16384	# chunk size for parsing input

class _element:
  def __init__(self, **kw):
    self.__dict__.update(kw)

  def textof(self):
    '''Return the CDATA of this element.

    Note: this is not recursive -- it only returns the CDATA immediately
    within the element, excluding the CDATA in child elements.
    '''
    s = self.first_cdata
    for child in self.children:
      s = s + child.following_cdata
    return s

  def find(self, name, ns=''):
    for elem in self.children:
      if elem.name == name and elem.ns == ns:
        return elem
    return None


def _clean_tree(elem):
  elem.parent = None
  del elem.parent
  map(_clean_tree, elem.children)


def _collect_recurse(elem, dict):
  dict[elem.ns] = None
  for ns, name in elem.attrs.keys():
    dict[ns] = None
  for child in elem.children:
    _collect_recurse(child, dict)

def _collect_ns(elem):
  "Collect all namespaces into a NAMESPACE -> PREFIX mapping."
  d = { '' : None }
  _collect_recurse(elem, d)
  del d['']	# make sure we don't pick up no-namespace entries
  keys = d.keys()
  for i in range(len(keys)):
    d[keys[i]] = i
  return d

def _dump_recurse(f, elem, namespaces, lang=None, dump_ns=0):
  if elem.ns:
    f.write('<ns%d:%s' % (namespaces[elem.ns], elem.name))
  else:
    f.write('<' + elem.name)
  for (ns, name), value in elem.attrs.items():
    if ns:
      f.write(' ns%d:%s="%s"' % (namespaces[ns], name, value))
    else:
      f.write(' %s="%s"' % (name, value))
  if dump_ns:
    for ns, id in namespaces.items():
      f.write(' xmlns:ns%d="%s"' % (id, ns))
  if elem.lang != lang:
    f.write(' xml:lang="%s"' % elem.lang)
  if elem.children or elem.first_cdata:
    f.write('>' + elem.first_cdata)
    for child in elem.children:
      _dump_recurse(f, child, namespaces, elem.lang)
      f.write(child.following_cdata)
    if elem.ns:
      f.write('</ns%d:%s>' % (namespaces[elem.ns], elem.name))
    else:
      f.write('</%s>' % elem.name)
  else:
    f.write('/>')
