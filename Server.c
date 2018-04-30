/*
*
*   Program :   Server.c
*
*   Authors  :  Cynthia
*               A0102
*
*               Ludovic
*               A0
*
*               Jose Rodolfo Verduzco Torres
*               A01366134
*
*
*
*   Purpose :
*              This program is the server of a multiplayer pacman-style
*              game using sockets.
*
*   Usage  :
*              The program receives the connection from multiple clients
*              and handles the pacman game
*
*
*   Error handling:
*               On any unrecoverable error, the program shows an error
*               message, and exits.
*
*
*/

/* Custom libraries */
#include "Server_Helper.h"
#include "Server_Com.h"

/*   *   *   *   *   *   *   *   *   *   *   *   *   *   */
/*               Function Declarations                   */
/*   *   *   *   *   *   *   *   *   *   *   *   *   *   */

void usage(char * program);
void printLocalIPs();
void waitForConnections(int server_fd);
void attendRequest(int client_fd, int port);

/*   *   *   *   *   *   *   *   *   *   *   *   *   *   */
/*                 Main Entry Point                      */
/*   *   *   *   *   *   *   *   *   *   *   *   *   *   */

int main(int argc, char * argv[])
{
      int server_fd;

      printf("\n=== SERVER ===\n");

    /* Check the correct arguments */
      if (argc != 2)
      {
          usage(argv[0]);
          fatalErrorMsg("main","Incorrect Number of parameters");
      }

  	/* Show the IPs assigned to this computer */
  	  printLocalIPs();

    /* Start the server */
      server_fd = initServer(argv[1]);

  	/* Listen for connections from the clients */
      waitForConnections(server_fd);

    /* Close the socket */
      close(server_fd);

      return 0;
}

/*   *   *   *   *   *   *   *   *   *   *   *   *   *   */
/*               Function Definitions                    */
/*   *   *   *   *   *   *   *   *   *   *   *   *   *   */

/*
 *
 *  Function: usage
 *
 *  Purpose: This Function shows the explanation to the
 *           user of the parameters required to run
 *           the program.
 *
 *  Parameters:
 *           Input   Strings with the name of the program
 *
 *           Output  Doesn't return anything, but exits the
 *                   program.
 */
void usage(char * program)
{
    printf("Usage:\n");
    printf("\t%s {port_number}\n", program);
}

/*
	Show the local IP addresses, to allow testing
	Based on code from:
		https://stackoverflow.com/questions/20800319/how-do-i-get-my-ip-address-in-c-on-linux
*/
void printLocalIPs()
{
	 struct ifaddrs * addrs;
	 struct ifaddrs * tmp;

	// Get the list of IP addresses used by this machine
	 getifaddrs(&addrs);
	 tmp = addrs;

   printf("Server IP addresses:\n");

  	while (tmp)
  	{
  		if (tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_INET)
  		{
  		  // Get the address structure casting as IPv4
  			struct sockaddr_in *pAddr = (struct sockaddr_in *)tmp->ifa_addr;
        // Print the ip address of the local machine
  			printf("%s: %s\n", tmp->ifa_name, inet_ntoa(pAddr->sin_addr));
  		}
  		// Advance in the linked list
  		tmp = tmp->ifa_next;
  	}

	freeifaddrs(addrs);
}

/*
    Main loop to wait for incomming connections
*/
void waitForConnections(int server_fd)
{
    struct sockaddr_in client_address;
    socklen_t client_address_size;
    char client_presentation[INET_ADDRSTRLEN];
    int client_fd;
    pid_t new_pid;

    int children=0;

    // Get the size of the structure to store client information
    client_address_size = sizeof client_address;

    while (1)
    {
        // ACCEPT
        // Wait for a client connection
        client_fd = accept(server_fd, (struct sockaddr *) &client_address, &client_address_size);
        if (client_fd == -1)
        {
            fatalError("accept");
        }

        /* Cambiar por Threads*/
       if (children < MAX_QUEUE)
       {

           children++;

        /* Create a child to deal with the new client */
            new_pid = fork();

        /* Parent Process */
            if(new_pid > 0)
            {
              /* Close the socket to the new client */
                close(client_fd);
            }
            else
              /* Child process */
                if(new_pid == 0)
                {
                  /* Get the data from the client */
                    inet_ntop (client_address.sin_family, &client_address.sin_addr, client_presentation, sizeof client_presentation);
                    printf("Received incomming connection from %s on port %d\n", client_presentation, client_address.sin_port);

                  /* Deal with the client */
                    attendRequest(client_fd, client_address.sin_port);

                  /* Finish the child process */
                      children --;
                      close(client_fd);
                      exit(EXIT_SUCCESS);
                }
              /* Error */
                else
                {
                    fatalError("Unable to fork");
                }
      }
    }
}
