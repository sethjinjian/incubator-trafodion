# @@@ START COPYRIGHT @@@
#
# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements.  See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License.
#
# @@@ END COPYRIGHT @@@

Summary:	Trafodion: Transactional SQL-on-Hadoop DBMS
Name:		%{name}
Version:	%{version}
Release:	%{release}
AutoReqProv:	no
License:	Apache version 2.0 -  http://www.apache.org/licenses/LICENSE-2.0
Group:		Applications/Databases
Source0:        %{name}-%{version}.tar.gz
BuildArch:	%{_arch}
BuildRoot:	%{_tmppath}/%{name}-%{version}-%{release}
URL:            http://trafodion.incubator.apache.org


%define _binary_filedigest_algorithm 1
%define _source_filedigest_algorithm 1
%define _binary_payload w9.gzdio
%define _source_payload w9.gzdio

Requires: audit-libs
Requires: apr
Requires: apr-util
Requires: coreutils
Requires: cracklib
Requires: expect
Requires: gawk
Requires: glib2
Requires: glibc
Requires: gmp
Requires: gnuplot
Requires: groff
Requires: gzip
Requires: keyutils-libs
Requires: libcom_err
Requires: libgcc
Requires: libxml2
Requires: lsof
Requires: lzo
Requires: ncurses
Requires: openssl
Requires: pam
Requires: pcre
Requires: pdsh
Requires: perl
Requires: perl-DBD-SQLite
Requires: perl-DBI
Requires: perl-Module-Pluggable
Requires: perl-Params-Validate
Requires: perl-Pod-Escapes
Requires: perl-Pod-Simple
Requires: perl-Time-HiRes
Requires: perl-version
Requires: protobuf
Requires: python
Requires: readline
Requires: sqlite
Requires: snappy
Requires: xerces-c
Requires: zlib

Prefix: /home/trafodion

%description
Apache Trafodion for Operational Big Data combining the power of transactional SQL and Apache HBase with the elastic scalability of Hadoop.


%prep
%setup -b 0 -n %{name}-%{version} -c


%pre -n %{name}
getent group trafodion > /dev/null || /usr/sbin/groupadd trafodion > /dev/null 2>&1
getent passwd trafodion > /dev/null || /usr/sbin/useradd --shell /bin/bash -m trafodion -g trafodion --home /home/trafodion > /dev/null 2>&1


%build
# don't build debug info package
%define debug_package:

%install
mkdir -p %{buildroot}/home/trafodion/%{name}-%{version}
cd %{_builddir}
cp -rf %{name}-%{version}/* %{buildroot}/home/trafodion/%{name}-%{version}

%clean
/bin/rm -rf %{buildroot}

%files
%defattr(-,trafodion,trafodion)
/home/trafodion/%{name}-%{version}

%changelog
* Mon Oct 18 2016 Steve Varnau
- ver 2.1
