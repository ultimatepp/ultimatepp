# SSH package for U++

SSH package is a flexible, easy-to-use, yet powerful [libssh2](https://www.libssh2.org/) wrapper for Ultimate++.
It supports both console and GUI-based applications on POSIX-compliant operating systems and
MS Windows (tm).

## Classes:

```
- Base (core) class  -> Ssh
  - Ssh session        -----> SshSession
  - Sftp subsystem     -----> SFtp
     -  SFtp Stream interface       -----> SFtpStream
     -  File system transparency    -----> SFtpFileSystemInfo 
  - Ssh channel        -----> SshChannel
      - Scp channel                 -----> Scp
      - Exec channel                -----> SshExec
      - Real-time interactive shell -----> SshShell
      - X11 forwarding              -----> SshShell (as operation mode)      
      - Tcp/IP and port forwarding  -----> SshTunnel
- Known hosts manager -> SshHosts
```

## Features and Highlights:

- Ssh-derived classes have pick semantics, based on RAII principle, support RTTI, and allow
  polymorphism (i.e. different classes can be stored in the same array, etc.) through a common
  interface. 
- Uses U++'s memory manager (Native manager (malloc) is also a compile-time option)
- Uses OpenSSL by default. 
- Supports multithreading.
- Supports 3rd-party network proxies.
- Supports known hosts verification mechanism.
- Supports password, public key, host-based, and keyboard-interactive authentication methods.
- Supports ssh-agents.
- Supports real-time interactive command line (shell) interface with both console and GUI integration
  (SshShell works on both Windows and Posix-compliant OSes)
- Supports X11 forwarding. 
  (A single SShShell instance is capable of handling multiple X11 requests simultaneously.)
- Supports Tcp/IP and port forwarding.
- Supports stream operations on SFTP objects.
- Supports access to sftp folder hiearcy in a file-system-agnostic (transparent) way.
- Supports detailed (full) debug logging.

## Reference examples:


+ *SshBasics*:    Demonstrates basic capabilities of SSH package.
+ *SFtpFileSel*:  Demonstrates FileSel integration of SFtp class, using FileSystemInfo interface.
+ *SFtpBrowser*:  Demonstrates a basic sftp browser with GUI.    

## License
```
Copyright (c) 2019-2022, İsmail Yılmaz
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of the copyright holder nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.