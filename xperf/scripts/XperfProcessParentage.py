"""
This script uses wpaexporter to export process information from an ETL file
using the techniques described here:

http://randomascii.wordpress.com/2013/11/04/exporting-arbitrary-data-from-xperf-etl-files/

This script relies on having XperfProcessParentage.wpaProfile in the same directory as the script.
"""

import os
import sys

# This is the name of the file that wpaexporter creates when using
# ProcessParentage.wpaProfile. Ideally this filename could be
# specified, but oh well.
csvFilename = "Processes_Summary_Table_ProcessParentage.csv"

if len(sys.argv) < 2:
	print "Usage: %s trace.etl" % sys.argv[0]
	print "This script extracts process parentage data from the specified"
	print "ETL file and prints it in a tree format, together with process name"
	print "and command line information."
	print "See this blog post for more details:"
	print "http://randomascii.wordpress.com/2013/11/04/exporting-arbitrary-data-from-xperf-etl-files/"
	sys.exit(0)

if not os.path.exists(sys.argv[1]):
	print "ETL file '%s' does not exist." % sys.argv[1]
	sys.exit(0)

# Delete any previous results to avoid accidentally using them.
try:
	os.remove(csvFilename)
except WindowsError, e:
	if e.winerror != 2: # WindowsError: [Error 2] The system cannot find the file specified
		raise e

# Find the location of the .wpaProfile that is needed for exporting the
# process parentage data.
scriptdir = os.path.split(sys.argv[0])[0]
profilePath = os.path.join(scriptdir, "XperfProcessParentage.wpaProfile")

# Run wpaexporter
lines = os.popen("wpaexporter \"%s\" -profile \"%s\"" % (sys.argv[1], profilePath)).readlines()
for line in lines:
	sys.stderr.write(line)

# Read the raw data.
lines = open(csvFilename).readlines()
if len(lines) < 2:
	print "Missing data. Sorry."
	sys.exit(0)

# This dictionary maps from process IDs to their parents
parents = {}
# This dictionary maps from process IDs to their details
details = {}
# When this is stored in the data field of the parents dictionary
# it indicates that a process has been printed already.
processedParent = -1

# Scan through every line of the .csv file (except the first
# line which just contains column labels) and fill in the
# parents and details dictionaries.
for line in lines[1:]:
	# Should probably do better CSV parsing, but I don't think it matters.
	parts = line.strip().split(",")
	procID = int(parts[0])
	parentID = int(parts[1])
	# Treat the remaining fields as a single string.
	extraData = ",".join(parts[2:])
	if parents.has_key(procID):
		sys.stderr.write("Process ID %d found again. Discarding %s\n" % (procID, extraData))
	else:
		parents[procID] = parentID
		details[procID] = extraData

"""
Print a process and recursively print all of its children.

Mark the processes as
being printed so that we don't print any processes more than once.
Print with the requested indent level from 0 to ...
Annotate the printout with extra information if the process is
missing from the trace (must have a child that outlived it) or
if the process is part of a loop (some part of the loop must have
reused a process ID).
"""
def PrintProcessTree(procID, indent, bIsLoop):
	missing = ""
	detail = ""
	loop = ""
	# Look up this process in our database. It might
	# not be there.
	if details.has_key(procID):
		detail = details[procID]
	else:
		missing = " (missing process)"
	if bIsLoop:
		loop = " (loop)"
	parents[procID] = processedParent
	print "%s%d%s%s, %s" % ("    " * indent, procID, missing, loop, detail)
	# Loop through all of the processes looking for ones that have this
	# process as their parent.
	for childID in parents.keys():
		parentID = parents[childID]
		if parentID == procID:
			PrintProcessTree(childID, indent+1, False)

# Iterate through all processes. For each process try to find
# its 'oldest' ancestor, and then print a tree starting from
# there. Due to PID reuse this will occasionally lead to loops.
# We detect loops but, unfortunately, they make determining the
# correct structure impossible, so the code just randomly chooses
# a point in the loop to print from.
for procID in parents.keys():
	if parents[procID] == processedParent:
		continue
	# For each one find the 'oldest' ancestor process
	ultimateParentID = procID
	count = 0
	bIsLoop = False
	# Stop on processes that are their own parents, or processes
	# whose parents have already been printed.
	while parents[ultimateParentID] != ultimateParentID and parents[ultimateParentID] != processedParent:
		# Check for infinite loops. They do happen, presumably when
		# parent processes go away and their process IDs are reused.
		count += 1
		if count > len(parents):
			bIsLoop = True
			break
		ultimateParentID = parents[ultimateParentID]
		# Stop if the parent is missing.
		if not parents.has_key(ultimateParentID):
			break
	PrintProcessTree(ultimateParentID, 0, bIsLoop)
