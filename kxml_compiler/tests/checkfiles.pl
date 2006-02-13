#!/usr/bin/perl

if ( @ARGV != 1 ) {
  print STDERR "Missing argument\n";
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
      print "Checking '$1'\n";

      $ret = system "diff -u $dirname/$entry $1";

      if ( $ret != 0 ) {
        print "FAILED\n";
        $failed_count += 1;
      } else {
        print "OK\n";
        $ok_count += 1;
      }
    }
  }

  print "\n";
  print $ok_count + $failed_count;
  print " tests. $ok_count ok, $failed_count failed.\n";
}
