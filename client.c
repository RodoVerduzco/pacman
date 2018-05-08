/*
 *    The main client program for a multiplayer Pacman game.
 *    Written by Ludovic Cyril Michel, Rodolfo Verduzco and Cynthia Castillo.
 */
#include "client.h"

thread_data_t * thread_data;
/*  *  *  *  *  *  *  */
/*  Main entry point  */
/*  *  *  *  *  *  *  */

int main(int argc, char *argv[]) {
  int connection_fd;

  // show program usage
  if (argc != 3) {
    print_usage_error(argv[0], "<address> <port>");
  }
  // start the server
  connection_fd = open_socket(argv[1], argv[2]);
  // play the game
  play(connection_fd);

  // close the socket
  close(connection_fd);

  return 0;
}
/*
  This Function opens the socket to the server.
*/
int open_socket(char *address, char *port) {
  struct addrinfo hints;
  struct addrinfo *server_info = NULL;
  int connection_fd;

  // prepare the hints structure
  bzero(&hints, sizeof hints);
  // set to use IPv4
  hints.ai_family = AF_INET;
  // set type of socket
  hints.ai_socktype = SOCK_STREAM;
  // set to look for the address automatically
  hints.ai_flags = AI_PASSIVE;

  // use the presets to get the actual information for the socket
  if (getaddrinfo(address, port, &hints, &server_info) == -1) {
    print_network_error("getaddrinfo", 1);
  }

  // open the socket using the information obtained
  connection_fd = socket(server_info->ai_family, server_info->ai_socktype,
                         server_info->ai_protocol);
  if (connection_fd == -1) {
    close(connection_fd);
    print_network_error("socket", 1);
  }

  // connect to the server
  if (connect(connection_fd, server_info->ai_addr, server_info->ai_addrlen) ==
      -1) {
    print_network_error("connect", 1);
  }

  // free the memory used for the address info
  freeaddrinfo(server_info);

  return connection_fd;
}

/*
  This function sends the data request though the sockets
  to the server
*/
void send_request(char *buffer, int connection_fd) {
  // send
  if (send(connection_fd, buffer, strlen(buffer) + 1, 0) == -1) {
    print_network_error("send", 0);
  }
  printf("sent %s\n", buffer);
}

/*
  This function handles the received response by the server
  and prints it.

      ** USED FOR DEBUG PURPOSES **
*/
void print_response(char *response, char *buffer, int connection_fd) {
  int response_size;
  bzero(buffer, BUFFER_SIZE);
  // clean the response string
  bzero(response, BUFFER_SIZE);
  // read from the buffer
  response_size = recv(connection_fd, buffer, BUFFER_SIZE, 0);
  if (response_size == -1) {
    print_network_error("recv", 0);
  }
  // write the response text
  printf("received %s\n", buffer);
}


/*
  This function is a game testing function
      ** USED FOR DEBUG PURPOSES **
*/
int play(int connection_fd) {
  char *buffer = (char *)malloc(BUFFER_SIZE);
  char *response = (char *)malloc(BUFFER_SIZE);
  char data[50];
  int code;

  // Get the name of the player
  printf("\nWhat is your name? ");
  scanf("%[^\t\n]", data);
  sprintf(buffer, "%d %s", INIT, data);

  // send the request
  send_request(buffer, connection_fd);
  parse_data(buffer, &code, data);

  if(code == WAIT){
    printf("Seguimos esperando conexiones, el juego iniciara pronto");
  }

  // print the response
  print_response(response, buffer, connection_fd);

  initscr();  // ncurses init
  noecho();   // Dont show keys pressed in console
   
  while (1)
  {
    timeout(100); // Requests every 100 miliseconds
    if (getch() == '\033') {
      getch(); // skip the [
      switch(getch()) { // the real value
          case 'A':
              // up
              printf("UP");
              //sprintf(buffer, "%d 0", MOVE); // move in y +1
              break;
          case 'B':
              // down
              printf("DOWN");
              //sprintf(buffer, "%d 0", MOVE); //move in y -1
              break;
          case 'C':
              // right
              //sprintf(buffer, "%d 0", MOVE);  //movoe in x +1
              break;
          case 'D':
              // left
              //sprintf(buffer, "%d 0", MOVE);  // move in x -1
              break;
            }
    }
    else {
      // No Keys pressed within the time
        sprintf(buffer, "%d 0", ACK);
    }
      // send the request
      send_request(buffer, connection_fd);
      // print the response
      print_response(response, buffer, connection_fd);
      parse_data(buffer, &code, data);
      bzero(buffer, BUFFER_SIZE);

      if(code == WAIT){
        // save positions to struct
        break;
      }
      if(code == ERROR){
        printf("\nServer was disconected");
        endwin();
        break;
      }
  }

  // free the memory
  free(buffer);
  free(response);

  return 0;
}

void parse_data(char * buffer, int *type, char * data)
{
  // parse the data
  sscanf(buffer, "%d %[^\t\n]", type, data);
}

void * handle_logic(void * arg)
{
  int connection_fd = thread_data->client_fd;

  play(connection_fd);
  return (void *)0;
}
