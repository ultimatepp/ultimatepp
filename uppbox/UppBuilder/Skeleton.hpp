/*
This is an example file. Anyone can use it as a base for 
additional parser output formats.
*/

#define DUMP(X) \
	printf(#X":\n");\
	for(int q = 0; q < X.GetCount(); q++) \
		printf("  [%i] = %s\n", q, ~X[q]);

void Parser::Process(){
	DUMP(flags);
	DUMP(nests);
	DUMP(dflags);
	printf("packages:\n");
	for(int i = 0; i < pkgs.GetCount(); i++){
		printf("  %s\n", ~pkgs[i].name);
		for(int j = 0; j < pkgs[i].flags.GetCount(); j++){
			printf("%s%s", j ?", ":"    ", ~pkgs[i].flags[j]);
		}
		printf("\n");
		for(int j = 0; j < pkgs[i].custom.GetCount(); j++){
			printf("    %s%s\n", ~pkgs[i].custom[j].when, ~pkgs[i].custom[j].item);
		}
		printf("\n");
	}
}