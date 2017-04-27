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
    s/extern int _rpcpmstart/static int _rpcpmstart/;
    push(@OUTLINES, $_);
    if (/svc_freeargs/) {
	while (<INFILE>) {
	    s/extern int _rpcpmstart/static int _rpcpmstart/;
	    push(@OUTLINES, $_);
	    if (/^\s*\}\s*$/) {
		push(@OUTLINES, "\n");
		push(@OUTLINES, "\tif (rqstp->rq_proc == DESTROYSERVER) {\n");
		push(@OUTLINES, "#ifdef _WINDOWS\n");
		push(@OUTLINES, "\t  rpc_nt_exit();\n");
		push(@OUTLINES, "#endif\n");
		push(@OUTLINES, "\t  pmap_unset(newprogramno, ECSVERS);\n");
		push(@OUTLINES, "\t  exit(0);\n");
		push(@OUTLINES, "\t}\n\n");
		last;
            }
        }
    } elsif (/^void start_closedown_check/) {
	while (<INFILE>) {
	    last if (/signal/);
	}
	push(@OUTLINES,$_);
	push(@OUTLINES, "#ifndef _WINDOWS\n");
	while (<INFILE>) {
	    push(@OUTLINES,$_);
	    last if (/^\}/);
	}
	push(@OUTLINES, "#endif /* ifndef _WINDOWS */\n");

    } elsif (/^#include "ecs.h"/) {
	push(@OUTLINES, "#ifndef _WINDOWS\n");
    } elsif (/^#include <netinet\/in.h>/) {
	push(@OUTLINES, "#endif /* ifndef _WINDOWS */\n");
	push(@OUTLINES, "\nextern unsigned long newprogramno;\n");
    } elsif (/^#include <signal.h>/) {
	push(@OUTLINES, "#if 0\n");
    } elsif (/^#include <sys\/ttycom.h>/) {
	push(@OUTLINES, "#endif /* if 0 */\n");
    } elsif (/^#include <memory.h>/) {
	push(@OUTLINES, "#if 0\n");
    } elsif (/^#include <stropts.h>/) {
	push(@OUTLINES, "#endif /* if 0 */\n");
    } elsif (/(.*)syslog\s*\(.*,(.*)\)(.*)/) {
	splice(@OUTLINES, $#OUTLINES, 1);
	push(@OUTLINES, "$1(void) fprintf(stderr, \"%s\\n\", $2)$3\n");
    } elsif (/^main\(.*\)/) {
	splice(@OUTLINES, $#OUTLINES-1, 2);
	while (<INFILE>) {
	    ;
	}
    } elsif (/^ecsprog_1\(/) {
	splice(@OUTLINES, $#OUTLINES-1, 1, "void\n");
    } elsif (/^ecsproxyprog_1\(/) {
	splice(@OUTLINES, $#OUTLINES-1, 1, "void\n");
    } elsif (/^\s*(\S*)\s*=\s*\(\*local\)/) {
	# Append after the line: result = (*local)(&argument, rqstp);
	# In the proxy, decompression occurs, but no recompression occurs
	push(@OUTLINES, "\tif ($1) {\n");
	push(@OUTLINES, "\t\tecs_Result *tmp = (ecs_Result *) $1;\n");
	if ($infname =~ /proxy/) {
	    push(@OUTLINES, "\t\t/* Proxy to client communication is uncompressed */\n");
	    push(@OUTLINES, "\t\ttmp->compression.ctype = 0;\n");
	    push(@OUTLINES, "\t\ttmp->compression.cversion = 0;\n");
	    push(@OUTLINES, "\t\ttmp->compression.clevel = 0;\n");
	    push(@OUTLINES, "\t\ttmp->compression.cblksize = 0;\n");
	    push(@OUTLINES, "\t\ttmp->compression.cfullsize = 0;\n");
	} else {
# Is this wrong????
#	    push(@OUTLINES, "\t\ttmp->compression = svr_handle->compression;\n");
	    push(@OUTLINES, "\t\tif (svr_handle) {\n");
	    push(@OUTLINES, "\t\t\ttmp->compression = svr_handle->compression;\n");
	    push(@OUTLINES, "\t\t} else {\n");
	    push(@OUTLINES, "\t\t\t/* This will occur after a DESTROYSERVER call */\n");
	    push(@OUTLINES, "\t\t\ttmp->compression.ctype = ECS_COMPRESS_NONE;\n");
	    push(@OUTLINES, "\t\t\ttmp->compression.cversion = 0;\n");
	    push(@OUTLINES, "\t\t\ttmp->compression.clevel = 0;\n");
	    push(@OUTLINES, "\t\t\ttmp->compression.cblksize = 0;\n");
	    push(@OUTLINES, "\t\t\ttmp->compression.cfullsize = 0;\n");
	    push(@OUTLINES, "\t\t\ttmp->compression.cachesize = 0;\n");
	    push(@OUTLINES, "}\t\t\n");
	}
	push(@OUTLINES, "\t}\n");
    }
}

print OUTFILE @OUTLINES;
close(OUTFILE);
close(INFILE);
