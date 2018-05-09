/*
  A server program for a multiplayer Pacman game.
  Written by Ludovic Cyril Michel, Rodolfo Verduzco and Cynthia Castillo.
*/

#include "server.h"

int main(int argc, char *argv[]) {
  int server_fd;

  setup_handlers();

  // check the correct arguments
  if (argc != 3) {
    print_usage_error(argv[0], "<port> <player_num>");
  }

  // show the IPs assigned to this computer
  print_ips();

  // start the server
  server_fd = init_server(argv[1]);

  // listen for connections from the clients
  create_games(server_fd, atoi(argv[2]));

  // close the socket
  close(server_fd);

  // finish the main thread
  pthread_exit(NULL);

  return 0;
}

// Modify the signal handlers for specific events
void setup_handlers() {
  struct sigaction new_action;

  // Change the action for the Ctrl-C input (SIGINT)
  new_action.sa_handler = on_interrupt;
  // Set the mask to the empty set
  if (sigemptyset(&new_action.sa_mask) == -1) {
    perror("ERROR: sigemptyset");
    exit(EXIT_FAILURE);
  }
  if (sigaction(SIGINT, &new_action, NULL) == -1) {
    perror("ERROR: sigaction");
    exit(EXIT_FAILURE);
  }
}

void print_ips() {
  struct ifaddrs *addrs;
  struct ifaddrs *tmp;

  // get the list of IP addresses used by this machine
  getifaddrs(&addrs);
  tmp = addrs;

  printf("Server IP addresses:\n");

  while (tmp) {
    if (tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_INET) {
      // get the address structure casting as IPv4
      struct sockaddr_in *pAddr = (struct sockaddr_in *)tmp->ifa_addr;
      // print the IP address of the local machine
      printf("%s: %s\n", tmp->ifa_name, inet_ntoa(pAddr->sin_addr));
    }
    // advance in the linked list
    tmp = tmp->ifa_next;
  }

  freeifaddrs(addrs);
}

int init_server(char *port) {
  struct addrinfo hints;
  struct addrinfo *server_info = NULL;
  int server_fd;
  int reuse = 1;

  // Prepare the hints structure
  bzero(&hints, sizeof hints);
  // Set to use IPv4
  hints.ai_family = AF_INET;
  // Set type of socket
  hints.ai_socktype = SOCK_STREAM;
  // Set to look for the address automatically
  hints.ai_flags = AI_PASSIVE;

  // Use the presets to get the actual information for the socket
  if (getaddrinfo(NULL, port, &hints, &server_info) == -1) {
    print_network_error("getaddrinfo", 1);
  }

  // Open the socket using the information obtained
  server_fd = socket(server_info->ai_family, server_info->ai_socktype,
                     server_info->ai_protocol);
  if (server_fd == -1) {
    close(server_fd);
    print_network_error("socket", 1);
  }

  // Allow reuse of the same port even when the server does not close correctly
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) ==
      -1) {
    print_network_error("setsockopt", 1);
  }

  // Connect the port with the desired port
  if (bind(server_fd, server_info->ai_addr, server_info->ai_addrlen) == -1) {
    print_network_error("bind", 1);
  }

  // Start listening for incoming connections
  if (listen(server_fd, MAX_QUEUE) == -1) {
    print_network_error("listen", 1);
  }

  // Free the memory used for the address info
  freeaddrinfo(server_info);

  printf("Server ready\n");

  return server_fd;
}

void create_games(int server_fd, int player_num) {
  struct sockaddr_in client_address;
  socklen_t client_address_size = sizeof(client_address);
  char client_presentation[INET_ADDRSTRLEN];

  int client_fd;
  pthread_t tid;

  game_state_t *game_state = init_game_state(player_num);
  int player_id = 0;

  while (1) {

    // wait for a client connection
    client_fd = accept(server_fd, (struct sockaddr *)&client_address,
                       &client_address_size);
    if (client_fd == -1) {
      print_network_error("accept", 0);
    }

    // get the data from the client
    inet_ntop(client_address.sin_family, &client_address.sin_addr,
              client_presentation, sizeof client_presentation);
    printf("Received incoming connection from %s on port %d\n",
           client_presentation, client_address.sin_port);

    thread_data_t *thread_data = (thread_data_t *)malloc(sizeof(thread_data_t));
    thread_data->connection_fd = client_fd;
    thread_data->player_num = player_num;
    thread_data->player_id = player_id;
    thread_data->game_state = game_state;

    pthread_create(&tid, NULL, &handle_players, thread_data);

    player_id++;

    if (player_id >= player_num) {
      game_state = init_game_state(player_num);
      player_id = 0;
    }
  }
}

void *handle_players(void *arg) {
  thread_data_t *thread_data = (thread_data_t *)arg;
  int client_fd = thread_data->connection_fd;
  int player_num = thread_data->player_num;
  int player_id = thread_data->player_id;
  game_state_t *game_state = thread_data->game_state;

  serve(client_fd, player_num, player_id, game_state);

  // close the connection and free the memory
  close(client_fd);
  free(thread_data);
  if (player_id == 0) {
    free(game_state);
  }

  printf("thread ending");

  fflush(stdout);

  // exit
  pthread_exit(NULL);
}
