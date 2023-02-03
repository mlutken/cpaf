#!/usr/bin/perl

# ----------------------------
# --- Debug printing stuff ---
# ----------------------------

print "subst.pl: ";
foreach $iArg (0 .. $#ARGV) {
   print "'$ARGV[$iArg]' ";
}
print "\n";

# --------------------------------------------
# --- Initialization, and argument testing ---
# --------------------------------------------

sub tjeck_num_args() 
{
	if ( $#ARGV < 2 ) {
		print "Need at least 3 arguments!!\n\n";
		print "\nUsage: subst.pl old-pattern new-pattern filepattern directory\n\n";
		print "OR:\n";
		print "\nUsage: subst.pl old-pattern new-pattern path-to-file\n\n";
		print "Arg1 : String to match   1\n";
		print "Arg2 : Substitute string 1. Use '_REMOVE_' to remove the macthed string.\n";
		print "Arg3 : String to match   2\n";
		print "Arg4 : Substitute string 2. Use '_REMOVE_' to remove the macthed string.\n";
		print "...  : Continue as needed with pairs of match string and replace string.\n";
		print "THEN EITHER:\n";
		print "Next to last arg : File pattern to search for\n";
		print "Last             : Root of directory to search\n";
		print "OR               :\n";
		print "Last             : Path to file in which to do the substitutions\n";
		print "\n";
		exit 0;
	}
}

# -------------------------------
# --- Start main program here ---
# -------------------------------

tjeck_num_args();
$bRecursive= $#ARGV % 2;	# Determine wheter we run in single file or recursive mode

# -------------------------------------------------------------------------
# IF recursive mode then find file_pattern and and root dir in command args
# Also find and initialize the list of files to process 
# ELSE
# Just get the filename to process which will be the last command argument
# -------------------------------------------------------------------------

if ( $bRecursive ) {
# ----------------------
# --- RECURSIVE MODE ---
# ----------------------

	# ---------------------------------------------------------------------------------------
	# --- Initialize the file_pattern (second last argument) and dir_name (last argument) ---
	# ---------------------------------------------------------------------------------------
	$iArgIndexLastSubstString= $#ARGV - 2;
	$sFilePattern=$ARGV[$#ARGV - 1];
	$sDirName=$ARGV[$#ARGV];

	# BEGIN: DEBUG_PRINT
	print "sFilePattern : $sFilePattern\n";
	print "sDirName     : $sDirName\n";
	$i = 0;
	while ( $i <= $iArgIndexLastSubstString ) {
		print "OLD: $ARGV[$i], NEW: $ARGV[$i+1]\n";
		$i = $i + 2; 
	}	
	# END DEBUG_PRINT

	# --- Check the root directory exists ---
	unless ( -e $sDirName ) {
		print "ERROR 1: Directory \"$sDirName\" does not exist.";
		exit 1;
	}

	# ------------------------------
	# --- Find and process files ---
	# ------------------------------

	open DATA, "find $sDirName -name \"$sFilePattern\" -print |"   or die "Couldn't execute program: $!";
 	while ( defined( my $sFile = <DATA> )  ) {
    chomp($sFile);
    print "$sFile\n";
		$i = 0;
		while ( $i <= $iArgIndexLastSubstString ) {
			# check if string to substitute is special word _REMOVE_ in which case we simply remove the search string
			if ( $ARGV[$i+1] eq "_REMOVE_" ) {
				system "perl -pi -e 's/$ARGV[$i]//g' $sFile";	
			}
			else {
				system "perl -pi -e 's/$ARGV[$i]/$ARGV[$i+1]/g' $sFile";	
			}
			$i = $i + 2; 
		}	
  	}
  	close DATA;
}
else {
# ------------------------
# --- SINGLE FILE MODE ---
# ------------------------
	print "SINGLE FILE MODE\n";
	$iArgIndexLastSubstString= $#ARGV - 1;
	$sFileName=$ARGV[$#ARGV];

	# BEGIN: DEBUG_PRINT
	print "sFileName     : $sFileName\n";
	$i = 0;
	while ( $i <= $iArgIndexLastSubstString ) {
		print "OLD: $ARGV[$i], NEW: $ARGV[$i+1]\n";
		$i = $i + 2; 
	}	
	# END DEBUG_PRINT
	
	# --- Check the file exists ---
	unless ( -e $sFileName ) {
			print "ERROR 1: File \"$sFileName\" does not exist.";
			exit 1;
	}

	# --- Do the substitution ---
	$i = 0;
	while ( $i <= $iArgIndexLastSubstString ) {
		# Check if string to substitute is special word _REMOVE_ in which case we simply remove the search string
		if ( $ARGV[$i+1] eq "_REMOVE_" ) {
			system "perl -pi -e 's/$ARGV[$i]//g' $sFileName";	
		}
		else {
			system "perl -pi -e 's/$ARGV[$i]/$ARGV[$i+1]/g' $sFileName";	
		}
		$i = $i + 2; 
	}	

} # RECURSIVE MODE / SINGLE FILE MODE




