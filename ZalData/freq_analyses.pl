#!e:\perl\bin\perl
use utf8;
use open qw/:std :utf8/;

$f_in = "test-processed.xhtml";
$f_out = "frequent_analyses.txt";

open(IN, $f_in) or die "Can't open input.\n";
open(OUT, ">$f_out") or die "Can't open output.\n";
binmode(STDOUT, ":utf8");

my %count = ();
my %analyses = ();

while (<IN>)
{
	s/^[^<>]*</</;
	s/>[^<>]*$/>/;
	if (/(NONLEX|bastard|ciph)/ or not /<w>/) { next; }
	/^(.*>)([^<]*)<\/w>$/;
	$ana = $1;
	$wf = lc($2);
	$count{$wf}++;
	$analyses{$wf} = $ana;
}

foreach $wf (keys %count)
{
	if ($count{$wf} > 50)
	{
		print OUT $wf . ": " . $analyses{$wf} . "\n";
	}
}

close(IN);
close(OUT);
