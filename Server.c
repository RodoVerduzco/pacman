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

/*   *   *   *   *   *   *   *   *   *   *   *   *   *   */
/*               Function Declarations                   */
/*   *   *   *   *   *   *   *   *   *   *   *   *   *   */

void usage(char * program);
void printLocalIPs();
void waitForConnections(int server_fd, game_t * game, locks_t * data_locks);
void attendRequest(int client_fd, int port);

/*   *   *   *   *   *   *   *   *   *   *   *   *   *   */
/*                 Main Entry Point                      */
/*   *   *   *   *   *   *   *   *   *   *   *   *   *   */

int main(int argc, char * argv[])
{
      int server_fd;
      game_t game;
      locks_t data_locks;

      printf("\n=== SERVER ===\n");

    /* Check the correct arguments */
      if (argc != 2)
      {
          usage(argv[0]);
          fatalErrorMsg("main","Incorrect Number of parameters");
      }

    /* Initialize the data structures */
        initGame(&game, &data_locks);

  	/* Show the IPs assigned to this computer */
  	  printLocalIPs();

    /* Start the server */
      server_fd = initServer(argv[1]);

  	/* Listen for connections from the clients */
      waitForConnections(server_fd, &game, &data_locks);

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
void waitForConnections(int server_fd, game_t * game, locks_t * data_locks)
{
    pthread_t new_tid;
    thread_data_t * connection_data = NULL;
    socklen_t client_address_size;

    struct sockaddr_in client_address;
    char   client_presentation[INET_ADDRSTRLEN];
    int    client_fd;

    while (1)
    {

      /* CREATE A THREAD */
        if(checkNumPlayers(game, data_locks) == 1)
        {
            // Get the size of the structure to store client information
              client_address_size = sizeof client_address;

            // Accept
              client_fd = accept(server_fd, (struct sockaddr *)&client_address, &client_address_size);

              if (client_fd == -1){
                fatalError("ERROR: accept");
              }

          // Get the data from the client
            inet_ntop(client_address.sin_family, &client_address.sin_addr, client_presentation, sizeof client_presentation);
            printf("\nReceived incomming connection from %s on port %d\n", client_presentation, client_address.sin_port);

            updateRunningThreads(1, game, data_locks);

          // Allocate thread's structure
            connection_data = (thread_data_t *) malloc(sizeof (thread_data_t ));

          // Prepare the structure to send to the thread
            initializeStruct(connection_data, client_fd, game, data_locks);

            int status = pthread_create(&new_tid, NULL, attentionThread, (void *)connection_data);
            checkThreadStatus(status, new_tid);

          }
      }
}
