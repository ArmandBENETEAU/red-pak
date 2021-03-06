#!/usr/bin/python3
#
# The red-dnf launcher: Tweak dnf defaults in such a way that it work with
# redpak nodes. DNF sea rednodes as a rootdir container. This while keeping
# rpmdb hierarchie on the full rednode family tree.
#  - overload rpm with redrpm class
#  - take default path from /etc/redpak/main.com
#  - add arguments as dnf prevent many config options from being changed from insie a plugins
#  - this launcher only with with red-dnf plugins (red-manager, red-install, ... )
# 
# Author Fulup Ar Foll <fulup@iot.bzh> Apr-2020
# Freely insprired from Redhat original DNF launcher.
#
# Copyright (C) 2012-2016 Red Hat, Inc.
# This copyrighted material is made available to anyone wishing to use,
# modify, copy, or redistribute it subject to the terms and conditions of
# the GNU General Public License v.2, or (at your option) any later version.
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY expressed or implied, including the implied warranties of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
# Public License for more details.  You should have received a copy of the
# GNU General Public License along with this program; if not, write to the
# Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
# 02110-1301, USA.  Any Red Hat trademarks that are incorporated in the
# source code or documentation are not subject to the GNU General Public
# License and may only be used or replicated with the express permission of
# Red Hat, Inc.
#

from __future__ import unicode_literals
import sys
import os

# make sure to enforce redrpm usage
import redrpm
sys.modules['rpm'] = sys.modules['redrpm']

def suppress_keyboard_interrupt_message():
    old_excepthook = sys.excepthook
    def new_hook(type, value, traceback):
        if type != KeyboardInterrupt:
            old_excepthook(type, value, traceback)
        else:
            pass
    sys.excepthook = new_hook

def RedpathToEnv(redpath):
    # cleanup redpath removing last '/' before expanding template
    if redpath[-1] == '/':
        os.environ['NODE_PATH']=redpath[:-1]
    else:    
        os.environ['NODE_PATH']=redpath

# do this ASAP to prevent tracebacks after ^C during imports
suppress_keyboard_interrupt_message()

if __name__ != "__main__":
    sys.stderr.write('The executable DNF module must not be imported.')
    sys.exit(1)

here = sys.path[0]
if here != '/usr/bin':
    # git checkout
    dnf_toplevel = os.path.dirname(here)
    sys.path[0] = dnf_toplevel

from dnf.cli import main

# force installation into redpath
redargv=False
redpath=None
configfound=False
debugFound=False

for idx,argument in enumerate(sys.argv[1:]):
    if argument.find('--installroot') == 0:
       sys.stderr.write('Reddnf require [--redpath] not [--installroot] found: {}\n'.format(argument))
       sys.exit(1)

    # debug is only use to select Python debuger and is not passed to DNF
    if argument.find('--debug') == 0:
        debugFound=True
        del sys.argv[idx+1]

    # if not --config set use red-dnf default one
    if argument.find('--config=') == 0:
        configfound=True

    # redpath is also use to set some default DNF variables
    if argument.find('--redpath=') == 0 or argument.find('--rpath=') == 0:
       redargv= argument.split('=')
       redpath= redargv[1]
       RedpathToEnv(redpath)

if not redargv:
       sys.stderr.write('Must provide --redpath argument\n')
       sys.exit(1)

sys.argv.insert (1,"--setopt=reposdir={}/etc/yum.repos.d".format(redpath))
sys.argv.insert (1,"--installroot={}".format(redpath))
sys.argv.insert (1,"--releasever=/")

# not the cleanest way to force config file, but it works
if not configfound:
    sys.argv.insert (1, "--config=/opt/etc/redpak/reddnf.conf")

if debugFound:
    import pdb; pdb.set_trace()

main.user_main(sys.argv[1:], exit_code=True)
