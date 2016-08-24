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
    if (/^struct ecs_ResultUnion \{/) {
	splice(@OUTLINES, $#OUTLINES-1, 0,
	       "typedef struct ecs_ResultUnion ecs_ResultUnion;\n");
    } elsif (/^typedef struct ecs_ResultUnion ecs_ResultUnion/) {
	splice(@OUTLINES, $#OUTLINES, 1);
    } elsif (/^(.*)struct struct(.*)$/) {
	splice(@OUTLINES, $#OUTLINES, 1, "$1struct$2\n");
    }
}

print OUTFILE @OUTLINES;
close(OUTFILE);
close(INFILE);
