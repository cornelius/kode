#!/usr/bin/perl

use Getopt::Long;

sub usage()
{
  print STDERR <<EOF;
Usage: checkfiles.pl [options] <sourcedir>

  Options:
    --help            Show this text.

    --reference       Set the current output as reference.
    
  Arguments:
    sourcedir  Source directory containing reference files
EOF

  exit 1;
}

GetOptions (
  'reference' => \$reference,
  
  'help' => \$help,
);

if ( $help ) { &usage(); }

if ( @ARGV != 1 ) {
  &usage();
}

$dirname = $ARGV[0];

if ( !opendir DIR, $dirname ) {
  print STDERR "UNable to open '$dirname'\n";
} else {
  @entries = readdir DIR;

  $ok_count = 0;
  $failed_count = 0;
  
  foreach $entry ( @entries ) {
    if ( $entry =~ /(.*)\.check/ ) {
      $checked_file = $1;
      if ( $reference ) {
        system "cp $checked_file $dirname/$checked_file.check";
        print "Created $checked_file.check\n";
      } else {
        print "Checking '$1'\n";

        $ret = system "diff -u $dirname/$entry $checked_file";

        if ( $ret != 0 ) {
          print "FAILED\n";
          $failed_count += 1;
        } else {
          print "OK\n";
          $ok_count += 1;
        }
      }
    }
  }

  if ( !$reference ) {
    print "\n";
    print $ok_count + $failed_count;
    print " tests. $ok_count ok, $failed_count failed.\n";
  }
}
