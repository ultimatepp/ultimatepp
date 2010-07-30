@files = glob("*.cpp *.h");

foreach $f (@files)
{
	print("Processing $f\n");

    open(IN, "<$f");

    @file = <IN>;

    close(IN);

    open(IN, ">$f");

    foreach $line (@file) {
    	if($line =~ /(\<Box2D[\w\d\_\/\.]+\>)/) {
    		$oi = $1;
    		if($oi =~ /([\w\d\_]+\.h)/)
    		{
    			$ni = "\"$1\"";
    			$line =~ s/$oi/$ni/g;
    		}
    		print "$line";
    	}
    		
        print IN $line;
    }
    close IN;   
}