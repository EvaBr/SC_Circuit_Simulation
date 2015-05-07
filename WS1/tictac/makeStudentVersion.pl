eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}' && eval 'exec perl -S $0 $argv:q' # -*- perl -*-
  if 0;
  
use File::stat;
use Time::localtime;
use strict;
use warnings;

# This perl script is written for the purpose to process the tictac simulator
# directory and source file especially for student assignments
# this means that some source directories will be deleted and all remaining source
# files will be passed through a filter which deletes code parts that should be 
# later implemented by the students

# 1) First step is to delete the directories which are not necessary and also 
#    some files which should not be viewed by the students

# 2) pass all files src/*/* src/*/*/* src/*/*/*/* through a filter
#    such that all code parts (marked with special sign)
#    IMPORTANT: the input to this filter should be the list of assignment number
#    which the code parts should be deleted for  

# 1)  
# directories to delete (include all content)
system("rm -rf ./src/main_parser");
system("rm -rf ./src/main_simetrix");
system("rm -rf ./src/netlist/bypass_interface");
system("rm -rf ./netlist_examples");
system("rm -rf ./tests");
system("rm -rf ./.git");


# files to be deleted
system("rm -rf ./src/linear_solvers/LS_LUSolver.*");
system("rm -rf ./src/parser_netlist/parser/SIMetrixParser.*");
system("rm -rf ./src/parser_netlist/netlist_visitor/IP_SIMetrixCopy.*");
system("rm -rf ./src/parser_netlist/netlist_visitor/IP_TitanNetlistWriter.*");
#delete also makefiles
system("rm -rf Makefile");
system("rm -rf Makefile.SIMETRIX");
system("mv Makefile.STUDENT Makefile");


# 2)

# routine to process a file (to delete parts which are specially marked)
sub processFile{
	# filename passed to the function 
    my $fileName = $_[0];
    # get total number of arguments passed.
    my $n = scalar(@_);
    my $i = 1;
    my @startdelete = ("START STUDENT PROJECT A", "START STUDENT PROJECT A", "START STUDENT PROJECT A", "START STUDENT PROJECT A", "START STUDENT PROJECT A", "START STUDENT PROJECT A" );
    my @enddelete =   ("END STUDENT PROJECT A",   "END STUDENT PROJECT A",   "END STUDENT PROJECT A"  , "END STUDENT PROJECT A"  ,   "END STUDENT PROJECT A",   "END STUDENT PROJECT A");
    print "Process file: $fileName nr. args=$n\n";
    for (my $i = 1; $i < $n; $i += 1) {
    	$startdelete[$i] = "$startdelete[$i]$_[$i]";
    	$enddelete[$i] = "$enddelete[$i]$_[$i]";
    	print " start=$startdelete[$i] end=$enddelete[$i] \n";
    }    
    
    # read in the file content into a string and then filter, and then rewrite 
    # the original content with this filtered content
    open(INFO, $fileName) or die("Could not open  file.");
    my $line = "";
    my $filecontent = "";
    my $assignInfo = -1;
    foreach $line (<INFO>)  {   
       #print $line; 
       # test start of a section
       for (my $i = 1; $i < $n; $i += 1) {
       	 if ($line =~ /\Q$startdelete[$i]\E/) {
       	  	$assignInfo = $i;
       	  	$filecontent = "$filecontent \n /* === HERE STARTS THE CODE OF ASSIGNMENT: $_[$i] ==== */ \n";
       	  }
        } 
       
       if ($assignInfo == -1) {
       	  $filecontent = "$filecontent$line";
       }
       else {
       	   # end of sections which should be deleted
       	   for (my $i = 1; $i < $n; $i += 1) {
       	   	 if ($line =~ /\Q$enddelete[$i]\E/) {
       	   		$assignInfo = -1;
       	   		$filecontent = "$filecontent \n /* === HERE ENDS THE CODE OF ASSIGNMENT: $_[$i] ==== */ \n";
       	   	 }
           } 
       }
    }
    #print $filecontent;
    close(INFO);
    
    # here we REWRITE the file content
    open (MYFILE, ">$fileName");
    print MYFILE "$filecontent"; 
    close (MYFILE);  
}

# for each selected file call the method
my @allfiles = glob("./src/*/*.* ./src/*/*/*.* ./src/*/*/*/*.*");
my $item = "";
foreach $item (@allfiles){
     processFile($item, @ARGV);
   }
#print join(", ", @allfiles);
#print join(", ", @ARGV);
