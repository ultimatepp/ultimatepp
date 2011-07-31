#include "kissfir.h"

extern int verbose;

void print_usage()
{
	                     fprintf(stderr,"usage options:\n"
                            "\t-n nfft: fft size to use\n"
                            "\t-d : use direct FIR filtering, not fast convolution\n"
                            "\t-i filename: input file\n"
                            "\t-o filename: output(filtered) file\n"
                            "\t-n nfft: fft size to use\n"
                            "\t-h filename: impulse response\n");
                     exit (1);
}

CONSOLE_APP_MAIN
{
    kffsamp_t * h;
    int use_direct=0;
    size_t nh,nfft=0;
    FILE *fin=stdin;
    FILE *fout=stdout;
    FILE *filtfile=NULL;

	const Vector<String>& c = CommandLine();
	for(int i = 0; i < c.GetCount(); i++)
	{
		if(c[i] == "-v")
		{
                verbose=1;
			continue;
		}
		if(c[i] == "-n")
		{
			nfft = ScanInt(c[++i]);
			continue;
		}
		if(c[i] == "-i")
		{
                fin = fopen(c[++i],"rb");
                if (fin==NULL) {
                    exit(1);
                }
			continue;
		}
 		if(c[i] == "-o")
		{
               fout = fopen(c[++i],"w+b");
                if (fout==NULL) {
                    exit(1);
                }
			continue;
		}
		if(c[i] == "-h")
		{
                filtfile = fopen(c[++i],"rb");
                if (filtfile==NULL) {
                    exit(1);
                }
			continue;
		}
		if(c[i] == "-d")
		{
                use_direct=1;
			continue;
		}

		print_usage();
	}

    if (filtfile==NULL) {
        fprintf(stderr,"You must supply the FIR coeffs via -h\n");
		print_usage();
        exit(1);
    }
    fseek(filtfile,0,SEEK_END);
    nh = ftell(filtfile) / sizeof(kffsamp_t);
    if (verbose) fprintf(stderr,"%d samples in FIR filter\n",nh);
    h = (kffsamp_t*)malloc(sizeof(kffsamp_t)*nh);
    fseek(filtfile,0,SEEK_SET);
    fread(h,sizeof(kffsamp_t),nh,filtfile);
    fclose(filtfile);
 
    if (use_direct)
        direct_file_filter( fin, fout, h,nh);
    else
        do_file_filter( fin, fout, h,nh,nfft);

    if (fout!=stdout) fclose(fout);
    if (fin!=stdin) fclose(fin);
}

