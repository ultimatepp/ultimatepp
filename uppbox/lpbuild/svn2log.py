#
# Copyright (c) 2003 The University of Wroclaw.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#    1. Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#    2. Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#    3. The name of the University may not be used to endorse or promote
#       products derived from this software without specific prior
#       written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE UNIVERSITY ``AS IS'' AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
# NO EVENT SHALL THE UNIVERSITY BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
# TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# History:
# 
# 2006-08-03  Przedsiebiorstwo Informatyczne CORE  <biuro at core.com.pl>:
#         * Following options were added:
#           -s, --strip-comments strip /* ... */ comments in log
#           -O, --only-date      generate only dates (without time)
#           -L, --no-files       generate log without filenames
#           -A, --no-author      generate log without author names
#           -H, --no-host        generate author name without hostname
# 2009-08-17 Chris.Clark@ingres.com, removed dependency on PyXML (aka 
#       python-xml), which is no longer supported.
#       See http://sourceforge.net/projects/pyxml/.
#       NOTE this program expects the output from:
#           svn -v --xml
#       I.e. expects the verbose mode (of the xml output).
#       Without the versbose mode will see error:
#           <logentry> doesn't have <paths> child
# 2009-08-17 Chris.Clark@ingres.com Added first-line-only.
#       Only show first line of commit message/log.
# 2009-08-18 Chris.Clark@ingres.com, made dependency on PyXML optional.
# 2010-01-31 dolik.rce@seznam.cz: 
#				Modified to produce Debian-style Changelog 
#					(rather then GNU-style.)
#				Added S,P and U options
#				Added support for comma separated listing in -x option
#				Added --include-only option


import sys
import os
import time
import re
import getopt
import string
import codecs

try:
    from xml.utils import qp_xml
except ImportError:
    import qp_xml

kill_prefix_rx = None
default_domain = "localhost"
exclude = []
include = []
users = { }
reloc = { }
max_join_delta = 3 * 60
list_format = False
strip = False
date_only = False
no_files = False
no_host = False
no_author = False
first_line_only = False
pkg=""
distro=""
urgency="low"

date_rx = re.compile(r"^(\d+-\d+-\d+T\d+:\d+:\d+)")

def die(msg):
  sys.stderr.write(msg + "\n")
  sys.exit(1)

def attr(e, n):
  return e.attrs[("", n)]

def has_child(e, n):
  for c in e.children:
    if c.name == n: return 1
  return 0

def child(e, n):
  for c in e.children:
    if c.name == n: return c
  die("<%s> doesn't have <%s> child" % (e.name, n))
  
def convert_path(n):
  for src in reloc.keys():
    n = string.replace(n, src, reloc[src])
  if kill_prefix_rx != None:
    if kill_prefix_rx.search(n):
      n = kill_prefix_rx.sub("", n)
    else:
      return None
  if n.startswith("/"): n = n[1:]
  if n == "": n = "/"
  for pref in exclude:
    if n.startswith(pref):
      return None
  if include:
    inc=False;
    for pref in include:
      if n.startswith(pref):
        inc=True
  else:
    inc=True
  if inc:
    return n
  else:
    return None

def convert_user(u):
  if no_author == False:
    if users.has_key(u):
      return users[u]
    else:
      if no_host:
        return u + ":"   
      else:
        return "%s <%s@%s>" % (u, u, default_domain)
  else:
    return ''

def wrap_text_line(str, pref, width):
  ret = u""
  line = u""
  first_line = True
  for word in str.split():
    if line == u"":
      line = word
    else:
      if len(line + u" " + word) > width:
        if first_line:
          ret += line + u"\n"
          first_line = False
          line = word
        else:
          ret += pref + line + u"\n"
          line = word
      else:
        line += u" " + word
  if first_line:
    ret += line + u"\n"
  else:
    ret += pref + line + u"\n"
  return ret

def wrap_text(str, pref, width):
  if not list_format:
    return wrap_text_line(str,pref,width)
  else:
    items = re.split(r"\-\s+",str)
    ret = wrap_text_line(items[0],pref,width)
    for item in items[1:]:
      ret += pref + u"- " + wrap_text_line(item,pref+"  ",width)
    return ret

class Entry:
  def __init__(self, tm, rev, author, msg):
    self.tm = tm
    self.rev = rev
    self.author = author
    self.msg = msg
    self.beg_tm = tm
    self.beg_rev = rev

  def join(self, other):
    self.tm = other.tm
    self.rev = other.rev
    self.msg += other.msg

  def dump(self, out):
    if len(self.msg) > 0:
      if date_only == False:
        tformat = "%a, %d %b %Y %H:%M:%S +0000"
      else:
        tformat = "%Y-%m-%d"

      if self.rev != self.beg_rev:
        out.write("%s (r%s-%s) %s; urgency=%s\n\n" % (pkg, self.rev, self.beg_rev, distro, urgency))
      else:
        out.write("%s (r%s) %s; urgency=%s\n\n" % (pkg, self.rev, distro, urgency))
        #~ out.write("%s (r%s)  %s\n\n" % \
                          #~ (time.strftime(tformat, time.localtime(self.beg_tm)), \
                           #~ self.rev, convert_user(self.author)))
      out.write(self.msg)
      out.write(" -- %s  %s\n\n" % (convert_user(self.author),time.strftime(tformat, time.localtime(self.beg_tm))))
  
  def can_join(self, other):
    return self.author == other.author and abs(self.tm - other.tm) < max_join_delta

def process_entry(e):
  rev = attr(e, "revision")
  if has_child(e, "author"):
    author = child(e, "author").textof()
  else:
    author = "anonymous"
  m = date_rx.search(child(e, "date").textof())
  msg = ' ' + child(e, "msg").textof()
  if first_line_only:
    #msg = msg.split('\n')[0] # inefficient but quick to implement!
    iend = string.find(msg, '\n')
    if iend > 0:
        msg = msg[:iend]
  if strip == True:
    ibegin = string.find(msg, "/*")
    if ibegin > 0:
      iend = string.find(msg, "*/") + 2
      msg = msg[0:ibegin] + msg[iend:]

  if m:
    tm = time.mktime(time.strptime(m.group(1), "%Y-%m-%dT%H:%M:%S"))
  else:
    die("evil date: %s" % child(e, "date").textof())
  paths = []
  if len(msg) > 1: 
    for path in child(e, "paths").children:
      if path.name != "path": die("<paths> has non-<path> child")
      nam = convert_path(path.textof())
      if nam != None:
        if attr(path, "action") == "D":
          paths.append(nam + " (removed)")
        elif attr(path, "action") == "A":
          paths.append(nam + " (added)")
        else:
          paths.append(nam)
     
    if paths != [] and no_files == False:
      return Entry(tm, rev, author, "    * %s\n" % wrap_text(", ".join(paths) + ": " + msg, "      ", 65))
    elif paths != [] and no_files == True:
      return Entry(tm, rev, author, "    * %s\n" % wrap_text(msg, "      ", 65))

  return None

def process(fin, fout):
  parser = qp_xml.Parser()
  root = parser.parse(fin)

  if root.name != "log": die("root is not <log>")
  
  cur = None
  
  for logentry in root.children:
    if logentry.name != "logentry": die("non <logentry> <log> child")
    e = process_entry(logentry)
    if e != None:
      if cur != None:
        if cur.can_join(e):
          cur.join(e)
        else:
          cur.dump(fout)
          cur = e
      else: cur = e
        
  if cur != None: cur.dump(fout)

def usage():
  sys.stderr.write(\
"""Usage: %s [OPTIONS] [FILE]
Convert specified subversion xml logfile to GNU-style ChangeLog.

Options:
  -p, --prefix=REGEXP  set root directory of project (it will be striped off
                       from ChangeLog entries, paths outside it will be 
                       ignored)
  -x, --exclude=DIR1,DIR2,...       exclude DIRs from ChangeLog (relative to prefix)
  -i, --include-only=DIR1,DIR2,...  include only DIRs from ChangeLog (relative to prefix)
  -o, --output         set output file (defaults to 'ChangeLog')
  -d, --domain=DOMAIN  set default domain for logins not listed in users file
  -u, --users=FILE     read logins from specified file
  -F, --list-format    format commit logs with enumerated change list (items
                       prefixed by '- ')
  -r, --relocate=X=Y   before doing any other operations on paths, replace
                       X with Y (useful for directory moves)
  -D, --delta=SECS     when log entries differ by less then SECS seconds and
                       have the same author -- they are merged, it defaults
                       to 180 seconds
  -h, --help           print this information
  -s, --strip-comments strip /* ... */ comments in log
  -O, --only-date      generate only dates (without time)
  -L, --no-files       generate log without filenames
  -A, --no-author      generate log without author names
  -H, --no-host        generate author name without hostname
  -1, --first-line-only    generate log using only the first line of a log message
  -S, --distribution=DIST  use distribution DIST everywhere
  -P, --package=PKG        use PKG as packagename
  -U, --urgency=URG        use urgency (dafaults to low)
  
Users file is used to map svn logins to real names to appear in ChangeLog.
If login is not found in users file "login <login@domain>" is used.

Example users file:
john    John X. Foo <jfoo@example.org>
mark    Marcus Blah <mb@example.org>

Typical usage of this script is something like this:

  svn log -v --xml | %s -p '/foo/(branches/[^/]+|trunk)' -u aux/users
  
Please send bug reports and comments to author:
  Michal Moskal <malekith@pld-linux.org>
  
Regarding -s, -O, -L, -A, -H options see
  http://www.core.com.pl/svn2log
   

""" % (sys.argv[0], sys.argv[0]))

def utf_open(name, mode):
  return codecs.open(name, mode, encoding="utf-8", errors="replace")

def process_opts():
  try:
    opts, args = getopt.gnu_getopt(sys.argv[1:], "o:u:p:x:d:r:d,i:D:P:S:U:FhsOLHA1", 
                                   ["users=", "prefix=", "domain=", "delta=",
                                    "exclude=", "help", "output=", "relocate=",
                                    "list-format","strip-comments", "only-date", "no-files",
                                    "no-host", "no-author", "first-line-only",
                                    "package=", "distribution=","urgency=","--include-only="])
  except getopt.GetoptError:
    usage()
    sys.exit(2)
  fin = sys.stdin
  fout = None
  global kill_prefix_rx, exclude, include, users, default_domain, reloc, max_join_delta, list_format, strip, date_only, no_files, no_host, no_author, first_line_only, pkg, distro, urgency
  for o, a in opts:
    if o in ("--prefix", "-p"):
      kill_prefix_rx = re.compile("^" + a)
    elif o in ("--exclude", "-x"):
      w=a.split(",")
      exclude=exclude+w
    elif o in ("--include-only", "-i"):
      w=a.split(",")
      include=include+w
    elif o in ("--help", "-h"):
      usage()
      sys.exit(0)
    elif o in ("--output", "-o"):
      fout = utf_open(a, "w")
    elif o in ("--domain", "-d"):
      default_domain = a
    elif o in ("--strip-comments", "-s"):
      strip = True
    elif o in ("--only-date", "-O"):
      date_only = True
    elif o in ("--no-files", "-L"):
      no_files = True
    elif o in ("--no-host", "-H"):
      no_host = True
    elif o in ("--no-author", "-A"):
      no_author = True
    elif o in ("--first-line-only", "-1"):
      first_line_only = True
    elif o in ("--users", "-u"):
      f = utf_open(a, "r")
      for line in f.xreadlines():
        w = line.split()
        if len(line) < 1 or line[0] == '#' or len(w) < 2: 
          continue
        users[w[0]] = " ".join(w[1:])
    elif o in ("--relocate", "-r"):
      (src, target) = a.split("=")
      reloc[src] = target
    elif o in ("--delta", "-D"):
      max_join_delta = int(a)
    elif o in ("--list-format", "-F"):
      list_format = True
    elif o in ("--package", "-P"):
      pkg = a
    elif o in ("--distribution", "-S"):
      distro = a
    elif o in ("--urgency", "-U"):
      urgency = a
    else:
      usage()
      sys.exit(2)
  if len(args) > 1:
    usage()
    sys.exit(2)
  if len(args) == 1:
    fin = open(args[0], "r")
  if fout == None:
    fout = utf_open("ChangeLog", "w")
  process(fin, fout)

if __name__ == "__main__":
  os.environ['TZ'] = 'UTC'
  try:
    time.tzset()
  except AttributeError:
    pass
  process_opts()
