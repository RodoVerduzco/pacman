/*
*
*   Program :   Client.c
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
*   Purpose :
*              This program is the player (client) to a simple online
*              pacman style game using sockets.
*
*   Usage  :
*              The program connects to the server with sockets, shows
*              the map with the current positions of the other players
*              to play the game correctly.
*
*   Error handling:
*               On any unrecoverable error, the program shows an error
*               message, and exits.
*
*/

/* Custom libraries */
  #include "error.h"
  #include "Client_Helper.h"

  #include <ifaddrs.h>

/*   *   *   *   *   *   *   *   *   *   *   *   *   *   */
/*               Function Declarations                   */
/*   *   *   *   *   *   *   *   *   *   *   *   *   *   */

void usage(char * program);
int openSocket(char * address, char * port);
void play(int connection_fd);

/*   *   *   *   *   *   *   *   *   *   *   *   *   *   */
/*                 Main Entry Point                      */
/*   *   *   *   *   *   *   *   *   *   *   *   *   *   */
int main(int argc, char * argv[])
{
    /* File Descriptor */
      int connection_fd;

      printf("\n=== PACMAN ===\n");

    /* Check the correct arguments */
      if (argc != 3)
      {
          usage(argv[0]);
          fatalErrorMsg("main","Incorrect Number of parameters");
      }

    /* Start the server */
      connection_fd = openSocket(argv[1], argv[2]);

  	/* Listen for connections from the server */
      play(connection_fd);

    /* Close the socket */
      close(connection_fd);

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
      printf("\t%s {server_address} {port_number}\n\n", program);
}


/*
 *
 *  Function: openSocket
 *
 *  Purpose: This Function opens the socket to the server.
 *
 *  Parameters:
 *           Input   Receives the addres on which the socket
 *                   will be open, and the port to make the
 *                   bind.
 *
 *           Output  Returns the file descriptor for the
 *                   socket.
 */

int openSocket(char * address, char * port)
{
      struct addrinfo hints;
      struct addrinfo * server_info = NULL;
      int connection_fd;

    /* Prepare the hints structure */
      bzero (&hints, sizeof hints);
    /* Set to use IPv4 */
      hints.ai_family = AF_INET;
    /* Set type of socket */
      hints.ai_socktype = SOCK_STREAM;
    /* Set to look for the address automatically */
      hints.ai_flags = AI_PASSIVE;

    /*  *  *  *  *  GETADDRINFO  *  *  *  *  */
    /* Use the presets to get the actual information for the socket */
      if (getaddrinfo(address, port, &hints, &server_info) == -1)
          fatalError("getaddrinfo");

    /*  *  *  *  *  SOCKET  *  *  *  *  */
    /* Open the socket using the information obtained */
      connection_fd = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
      if (connection_fd == -1)
      {
          close(connection_fd);
          fatalError("socket");
      }

    /*  *  *  *  *  CONNECT  *  *  *  *  */
    /* Connect to the server */
      if (connect(connection_fd, server_info->ai_addr, server_info->ai_addrlen) == -1)
          fatalError("connect");

    /*  *  *  *  *  FREEADDRINFO  *  *  *  *  */
    /* Free the memory used for the address info */
      freeaddrinfo(server_info);

      return connection_fd;
}

/*
 *
 *  Function: play
 *
 *  Purpose: This Function handles the main game funcionality
 *
 *  Parameters:
 *           Input   The file descriptor.
 *
 *           Output  Doesn't return anything, but exits the
 *                   program.
 */
void play(int connection_fd)
{
    char data[BUFFER_SIZE]; /* Stores the data received from server */

    while(1)
    {

      /* Prepare and send the data to the server*/
        sprintf(data, "0_Hi");
        sendData(connection_fd, data);

        printf("THE PROGRAM WILL TERMINATE");
        break;

    }
}
