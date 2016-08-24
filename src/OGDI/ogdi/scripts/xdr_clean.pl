#! /usr/bin/perl

require "getopts.pl";

&Getopts('o:');

if ($#ARGV != 0) {
    die "Usage: $0 [-o outfile] infile\n";
}
    
if ($opt_o) {
    $outfname = ">$opt_o";
} else {
    $outfname = '>&STDOUT';
}

$infname = $ARGV[0];

open(OUTFILE, $outfname) || die "Unable to open $opt_o for writing: $!\n";
open(INFILE, $ARGV[0]) || die "Unable to open $infname for reading: $!\n";

binmode(OUTFILE);
while (<INFILE>) {
    push(@OUTLINES, $_);
    if (/^xdr_ecs_Result\s*\((.*)\)\s*$/) {
	splice(@OUTLINES, $#OUTLINES, 1, "xdr_ecs_Result_Work($1)\n");
	while (<INFILE>) {
	    if (! /xdr_ecs_Compression/) {
		push(@OUTLINES, $_);
	    } else {
		while (<INFILE>) {
		    if (/xdr_int/) {
			push(@OUTLINES, $_);
			last;
		    }
		}
		last;
	    }
	}
    }
}

print OUTFILE @OUTLINES;
close(OUTFILE);
close(INFILE);
