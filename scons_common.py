# scons_common.py

import os
import re
from SCons.Script import *

qobject_search = re.compile(r'[^A-Za-z0-9]Q_OBJECT[^A-Za-z0-9]')

# Create an environment - all SConscripts should use this instead of directly
# calling Environment()
def createEnvironment(generate_help=False):
	vars = Variables()
	vars.AddVariables(BoolVariable('debug', 'Set 1 for debug build', 0))
	vars.AddVariables(BoolVariable('tools', 'Set 1 for building with editor tools', 0))
	env = Environment(variables = vars)
	env.Append(CCFLAGS=['-Wall'])

	if int(ARGUMENTS.get('debug', 0)):
		env.Append(CCFLAGS=['-g'])
		env.Append(CPPDEFINES=['_DEBUG'])
#		env.Append(LINKFLAGS=['-rdynamic'])	# for printCallStack() (Backtrace.cpp)
	else:
		env.Append(CCFLAGS=['-O2'])
		env.Append(CPPDEFINES=['NDEBUG', '_RELEASE'])

	if int(ARGUMENTS.get('tools', 0)):
		env.Append(CPPDEFINES=['WITH_TOOLS'])

#	env.Append(CCFLAGS=['-m32'])

	if generate_help:
		Help(vars.GenerateHelpText(env))
	return env

# def getTargetSuffix():
# 	if int(ARGUMENTS.get('debug', 0)):
# 		return '_d'
# 	else:
# 		return ''

def getObjectFilesDir():
	if int(ARGUMENTS.get('debug', 0)):
		if int(ARGUMENTS.get('tools', 0)):
			return 'debug-tools'
		else:
			return 'debug-notools'
	else:
		if int(ARGUMENTS.get('tools', 0)):
			return 'release-tools'
		else:
			return 'release-notools'

# Is it a .c or a .cpp file?
def isSourceFile(filename):
	basename, extension = os.path.splitext(filename)
	return (extension == '.c' or extension == '.cpp')

# Is it a .h or a .hpp file?
def isHeaderFile(filename):
	basename, extension = os.path.splitext(filename)
	return (extension == '.h' or extension == '.hpp')

# Get the names of the source files referenced in a file
def getSourcesListFromFile(list_filename):
	src_list = []
	with open(list_filename) as f:
		for line in f:
			filename = line[0:-1]
			filename = filename.strip()
			if isSourceFile(filename):
				src_list.append(filename)
	return src_list

# Get the names of the header files referenced in a file
def getHeadersListFromFile(list_filename):
	h_list = []
	with open(list_filename) as f:
		for line in f:
			filename = line[0:-1]
			filename = filename.strip()
			if isHeaderFile(filename):
				h_list.append(filename)
	return h_list

# Get the names of the directories contained in a directory (recursively)
def getDirsListFromDir(dirname, excluded_list=[]):
	dir_list = []
	files_list = os.listdir(dirname)

	for f in files_list:
		full_name = dirname + '/' + f
		if not full_name in excluded_list:
			if os.path.isdir(full_name):
				dir_list += [full_name]
				dir_list += getDirsListFromDir(full_name, excluded_list)
	return dir_list

# Get the names of the source files contained in a directory (recursively)
def getSourcesListFromDir(dirname, recursively=False, excluded_list=[]):
	src_list = []
	files_list = os.listdir(dirname)

	for f in files_list:
		full_name = dirname + '/' + f
		if not full_name in excluded_list:
			if recursively and os.path.isdir(full_name):
				src_list += getSourcesListFromDir(full_name, recursively, excluded_list)
			elif isSourceFile(f):
				src_list.append(full_name)
	return src_list

def isMocable(path):
	with open(path) as f:
		for line in f:
			if qobject_search.search(line):
				return True
	return False

# Get the names of the source files contained in a directory (recursively)
def copyHeaders(env, dest_dirname, src_dirname, recursively=False, excluded_list=[]):
	files_list = os.listdir(src_dirname)

	for f in files_list:
		full_dest_name = dest_dirname + '/' + f
		full_src_name  = src_dirname  + '/' + f

		if not full_src_name in excluded_list:
			if recursively and os.path.isdir(full_src_name):
				copyHeaders(env, full_dest_name, full_src_name, recursively, excluded_list)
			elif isHeaderFile(f):
				env.Command(full_dest_name, full_src_name, Copy("$TARGET", "$SOURCE"))

