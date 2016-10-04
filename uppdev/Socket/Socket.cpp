#include <Core/Core.h>

#include <Web/Web.h>

#include <CmdLineArgProcessor/ArgProcessor.hpp>

#include <signal.h>

using namespace Upp;

 

bool run=true;

 

void sighandler(int sig)

{

    run=false;

};

 

CONSOLE_APP_MAIN

{    

    signal(SIGABRT, sighandler);

    signal(SIGINT, sighandler);

    signal(SIGTERM, sighandler);

    int port = 7755;

    

    const Vector<String>& cmdline = CommandLine();    

    ArgProcessor ap("Test Application", "0.1");

    Arg<bool> server_arg("Server","Specify to set program to server mode.", false);

    Arg<String> host_arg("Destination", "Destination to connect to (client mode).", false);    

    ArgSet client_args, server_args;

    client_args.AddArg(host_arg);

    server_args.AddArg(server_arg);    

    ap.AddArgSet(client_args);

    ap.AddArgSet(server_args);

    ap.PrintUsageOnError();

    ap.ProcessCmdLine(cmdline);

    

    if(!ap.IsError())

    {

        String host;

        if(host_arg.IsSet())

        {

            host = host_arg.Val();

        }

        else

        {

            host = "localhost";

        }

        

        if(!server_arg.IsSet())

        {

            Socket s1;

            if( ClientSocket(s1,host,port) )

            {

                s1.Write("Hello World!");

                s1.Close();

            }

        }

        else

        {

            Cout() << "Server started. Press Ctrl-C to exit.\n";

            Socket accept_socket, data_socket;

            ServerSocket(accept_socket,port);

            while(run)

            {

                dword ipaddr;

                if( accept_socket.Accept(data_socket, &ipaddr) )

                {

                    Cout() << "Connection from " << FormatIP(m_ipaddr) << "\n";

                    while(data_socket.IsOpen() && !data_socket.IsError() && !data_socket.IsEof())

                    {

                        Cout() << data_socket.Read();

                    }

                    Cout() << "\n";

                    data_socket.Close();

                }

            }        

            accept_socket.Close();

        }

    }

}