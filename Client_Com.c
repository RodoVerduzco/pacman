/*
 *
 *   Program :   Client_Com.c
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
 *              This program handles the clients communication with
 *              the server.
 *
 *   Error handling:
 *               On any unrecoverable error, the program shows an error
 *               message, and exits.
 */

/* Custom Libraries */
  #include "Client_Com.h"

 /*  *  *  *  *  SEND  *  *  *  *  */
 /*         Send the response      */
 void sendStringClient(int connection_fd, char * buffer)
 {
   if (send(connection_fd, buffer, strlen(buffer) + 1, 0) == -1 )
   {
     fatalError("send");
     fatalErrorMsg("sendData/send", "Couldn't send the response to the Server");
   }
 }

 /*  *  *  *  *  RECV  *  *  *  *  */
 /* Receive the request from the server */
 void recvStringClient(int connection_fd, char * buffer, char * receivedData)
 {
     int chars_read;

     chars_read = recv(connection_fd, buffer, BUFFER_SIZE, 0);
     if (chars_read == -1)
     {
       fatalError("recv");
       fatalErrorMsg("sendData/recv", "Couldn't receive the request from the server");
     }

     /* Scans everything including spaces */
       sscanf(buffer, "%[^\t]", receivedData);
 }
