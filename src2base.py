#!/usr/bin/env python3

import os
import sys

import argparse

PATTERN = 'STUDENT'

closeFetch = [
	'#ifndef %s\n' %PATTERN,
	'#else // END %s\n' %PATTERN,
]

openFetch = [
	'#ifdef %s\n' %PATTERN,
	'#else // %s\n' %PATTERN,
	'#endif // %s\n' %PATTERN,
	]

removeLine = [
	'#endif // END %s\n' % PATTERN,
]

class FatalError(RuntimeError):
	"""
	Wrapper class for runtime errors that aren't caused by internal bugs.
	"""
	
	def __init__(self, message):
		RuntimeError.__init__(self, message)
	
	@staticmethod
	def WithResult(message, result):
		"""
		Return a fatal error object that appends the hex values of
		'result' as a string formatted argument.
		"""
		message += " (result was %s)" % common.hexify(result)
		return FatalError(message)

class InputError(RuntimeError):
	def __init__(self, e):
		super(InputError, self).__init__(e)

def operationFunc(args):
	fetch = True
	
	for src in args.inputs:
		sys.stdout.write('Extract %s' % src.name)
		# Make sure that the output directory is created
		outputDir = os.path.abspath(os.path.dirname(os.path.join(args.outputDir, src.name)))
		if not os.path.exists(outputDir):
			try:
				os.makedirs(outputDir)
			except OSError as exc:
				if exc.errno != errno.EEXIST:
					raise
		elif not os.path.isdir(outputDir):
			raise InputError("%s exists and it is not a directory!" % outputDir)
		
		# Open output
		with open(os.path.join(args.outputDir, src.name), 'w') as out:
			print(" to %s" % out.name)
			for l in src.readlines():
				if l in closeFetch:
					fetch = False
					continue
				if l in removeLine:
					continue
				if l in openFetch:
					fetch = True
					continue
				if fetch:
					out.write(l)
		if not fetch:
			raise InputError('%s contains pattern to exclude some code, but nothing to close this section' %src.name)


def main(custom_commandline = None):
	parser = argparse.ArgumentParser(
		description = 'Convert src file to student version. This script removes correction and retains code between  STUDENT ifdef or macro',
		prog = 'src2base',
		fromfile_prefix_chars = '@')
	
	# Sources
	parser.add_argument('inputs',
	                    help = 'Path to source file.',
	                    type = argparse.FileType('r'),
	                    nargs =  '+')
	
	# Output directory
	parser.add_argument('--output-dir', '-o',
	                    help = 'Directory path to store output files',
	                    required = True,
	                    dest = 'outputDir')
	
	# Pattern
	parser.add_argument('--student-pattern', '-p',
	                    help = 'Preprocessing variable used to extract base and remove final version.',
	                    default = PATTERN)
	
	args = parser.parse_args()
	operationFunc(args)

	

if __name__ == '__main__':
	try:
		main()
	except InputError as e:
		print('\nInput fatal error: ', e, file = sys.stderr)
		sys.exit(1)
	except FatalError as e:
		print('\nA fatal error occurred: ', e)
		sys.exit(2)

