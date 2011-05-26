# SConstruct - root SCons script for SuperSpaceShipsShootingStuff

from scons_common import *
import os

env = createEnvironment(generate_help=True)	# generate the help

qtdir = os.environ.get('QT4DIR')
if not qtdir :
	print "You must define the QT4DIR environment variable"
	exit(1)

if os.name == 'posix':
	env['LINK'] = 'g++'	# We got some problem of gcc being used instead of g++ when using VariantDir()...

env['ENV']['PKG_CONFIG_PATH'] = os.path.join(qtdir, 'lib/pkgconfig')
env['QT4DIR'] = qtdir
env['QT4_DEBUG'] = 1
env.Tool('qt4')
env.EnableQt4Modules([
		'QtGui',
		'QtCore',
		'QtOpenGL',
		'QtXml'
	])

# core
env.Append(LIBS=['-lpthread','-lm'])
# gui
env.Append(LIBS=['-lXext','-lX11'])
# opengl
env.Append(LIBS=['-lGL','-lGLU'])

env.Append(LFLAGS=['-Wl'])
env.Append(RPATH=[os.path.join(qtdir, 'lib')])

SConscript('SConscript_project','env')
