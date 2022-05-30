/**
 * Copyright (c) 2022 WIZnet Co.,Ltd
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*----------------------------------------------------------------------------------------------------
 * Includes
 * ---------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>

#include "port_common.h"
#include "mqtt_transport_interface.h"
#include "ssl_transport_interface.h"
#include "timer_interface.h"
#include "iot_socket.h"
#include "cmsis_os2.h"
#include "WiFi_WizFi360.h"


/* ----------------------------------------------------------------------------------------------------
*  WIFI TCP AP Thread
*/

/* Port */
#define TARGET_PORT 6000


/* ----------------------------------------------------------------------------------------------------
 * Variables
 * --------------------------------------------------------------------------------------------------*/
const osThreadAttr_t client_thread_attr = {
  .stack_size = 4096U,
  .priority = osPriorityBelowNormal
  //.attr_bits = osThreadJoinable
};

/* the argument of ThreadHandleClient */
struct ThreadArgs
{
	int clientSock;       //socket descriptor for client
};

/* global varialbe for client socket (index), shared with Ethernet MQTT buffer */\
int g_clientSock = 0;

/* Max Client Connection Limitaion is under 5 */
static const int MAXPENDING = 4; 

/* IOT device Data: only use TCP not MQTT */
static uint8_t g_iot_rcv_buf[MQTT_BUF_MAX_SIZE] = { 0, };
static uint8_t g_iot_trs_buf[MQTT_BUF_MAX_SIZE] = { 0, };

/* External varialbles */
extern WIFI_SOCKET Socket[];    //declared WiFi_WizFi360.c
extern uint8_t Server_ip[4];    //declared wifi_socket_startup.c
extern uint8_t g_mqtt_pub_msg_buf[MQTT_BUF_COUNT][MQTT_BUF_MAX_SIZE];    //declared aws_wifi_ethernet_mqtt.c
extern uint8_t g_conn_mac[6];    // Station's mac addr connected to AP
extern uint8_t g_link_mac[5][6]; //Client's mac addr connected to Server, [clients are max 4]x[mac address size]


/* ----------------------------------------------------------------------------------------------------
 * Access Point Thread and Function
/* ----------------------------------------------------------------------------------------------------*/
static void *ThreadHandleClient(void *arg);
static void Handle_TCP_Client(int clientSock);
static int Accept_TCP_Connection();
static int Setup_TCPServer_Socket(const char *service);
extern update_linked_mac(int socket);

void * WIFI_TCP_AP_thread()
{
  int servSock, clientSock, ret, sockDsc;
  int arrThreadId[5]={0,};
  osThreadId_t id;

  servSock = Setup_TCPServer_Socket(TARGET_PORT);
  if(servSock < 0)
  {
  	printf("Setup_TCPServer_Socket failed\r\n");
  }
  
  while(1)
  {
    osDelay(1000);
    WIFI_SOCKET *sock = &Socket[servSock];
    int n = sock->backlog;

    ret = iotSocketListen(servSock, MAXPENDING);
    if (ret < 0)
    {
      printf("Server: iotSocketListen() doesn't exist, ret = %d\r\n", ret);
    }

    /* WiFi_Wait(), until Someone Connects */
    clientSock = Accept_TCP_Connection(servSock);
    if(clientSock >= 0)
    {
      printf("Accept_TCP_Connection(): clientSock is %d\r\n", clientSock);

      /*	How to manange each socket's Recv and Send:
       *	1. Connection Check each loop, Make thread Client Socket Id order
      /*	2. Running time base, Not bad but needs tradeoff time and task priority */
      //if clientSock is 3, clients exist 1 2 3 socket
      for(sockDsc = 1; sockDsc < clientSock+1; sockDsc++)
      {
      	/* allocation memory for client process */
      	struct ThreadArgs *threadArgs = (struct ThreadArgs *)malloc(sizeof(struct ThreadArgs));
      	threadArgs->clientSock = sockDsc;			      

      	id = NULL;
      	g_clientSock = sockDsc;
        
      	/* create client thread for each Number of client socket descriptor */
      	id = osThreadNew(ThreadHandleClient(threadArgs), NULL, &client_thread_attr);
      	if(id == NULL)
      	{
      		printf("osThreadNew() Failed, client socket is %d\r\n", sockDsc);
      	}
      }
      clientSock = -1;
      //osThreadTerminate(id); //OS terminate this Thread when AT_Notify <link_id>, DISCONNECT
    }		
  }
	//iotSocketClose(clientSock); /*close when STA_DISCONNECTED, DISCONNECT */
}

/* Thread or Time Division Polling: Receive IoT Data and Send to Ethernet */
static void *ThreadHandleClient(void *threadArgs)
{
  char txBuffer[20]="ACK\r\n";
  int numBytesRcv, numBytesSnd;
  int clientSock;
  int n = g_clientSock;
  int pub_cnt = 0;

  /* give back thread resouces when run finishes */
  //pthread_detach(pthread_self());

  clientSock = ((struct ThreadArgs *)threadArgs)->clientSock;

  /* freeze memory by malloc() */
  //free(threadArgs); 

  memset(g_iot_rcv_buf, 0x00, MQTT_BUF_MAX_SIZE);

  /* recieve client message */
  numBytesRcv = iotSocketRecv ((uint8_t)clientSock, g_iot_rcv_buf, MQTT_BUF_MAX_SIZE);
  if (numBytesRcv > 0)
  {    
    printf("iotSocketRecv data is %s \r\n", g_iot_rcv_buf);

    /* Ethernet MQTT */
    sprintf(&g_mqtt_pub_msg_buf[n], "{\"mac\":\"%X:%X:%X:%X:%X:%X\", \"publish message\":\"%s\"}\n",\
      g_link_mac[clientSock][0],g_link_mac[clientSock][1],g_link_mac[clientSock][2],\
      g_link_mac[clientSock][3],g_link_mac[clientSock][4],g_link_mac[clientSock][5], g_iot_rcv_buf);
  }
  
  while(numBytesRcv > 0)
  {	    
    /* Send ACK for Test while recieving end of data stream */
    numBytesSnd = iotSocketSend(clientSock, txBuffer, sizeof(txBuffer));

    /* check remained recieved data */
    numBytesRcv = iotSocketRecv((uint8_t)clientSock, g_iot_rcv_buf, MQTT_BUF_MAX_SIZE);
    if(numBytesRcv > 0)
    {			
      printf("iotSocketRecv data is %s \r\n", g_iot_rcv_buf);

      /* Ethernet MQTT */
      sprintf(&g_mqtt_pub_msg_buf[n], "{\"mac\":\"%X:%X:%X:%X:%X:%X\", \"publish message\":\"%s\"}\n",\
      	g_link_mac[clientSock][0],g_link_mac[clientSock][1],g_link_mac[clientSock][2],\
      	g_link_mac[clientSock][3],g_link_mac[clientSock][4],g_link_mac[clientSock][5], g_iot_rcv_buf);
    }
  }
  /*close client socketwhen AT_Notify STA_DISCONNECTED and DISCONNECT */
  //iotSocketClose(clientSock); 
  //osThreadExit();

  return 0;
}


/* Socket control block: Client Socket Accept by client's link_id */
static int Accept_TCP_Connection(int servSock)
{
	int clientSock, clientsockD, ret;
	int timeOut = 10000;

	/* referencing extern WiFi_WizFi360 driver */
	WIFI_SOCKET *sock = &Socket[servSock];
	int n = sock->backlog;
	int sockSize = sizeof(Socket[n].r_ip);
	uint16_t localPort = Socket[n].l_port;
  
	//g_clientSock = n;   //for Ethenet Tx buffer index using socket number

	printf("Accept_TCP_Connection() wait someone connect: backlog is %d\r\n", n);
	
	/* Wait client connection, get socket's link_id, ip and port */
	clientSock = iotSocketAccept (servSock, (uint8_t *)sock->r_ip, &sockSize, &localPort);
	if (clientSock < 0)
	{
		printf("iotSocketAccept failed\r\n");
	}
	else
	{
	  for(clientsockD = 1; clientsockD < clientSock+1; clientsockD++)
		{
		  ret = iotSocketSetOpt ((uint8_t)clientsockD, IOT_SOCKET_SO_RCVTIMEO, &timeOut, sizeof(timeOut));
      update_linked_mac(clientsockD);    //need to sophisticate mapping mac address to client socket
    }

    /* print Server and Client Socket information */
   #if 0 
		printf("-------------------------------------------------------\r\n");
		printf("#  Accept_TCP_Connection() client connection success  #\r\n");
		printf("Socket Backlog = %d \r\n",sock->backlog);
		printf("Socket[backlog].state = %d \r\n", Socket[n].state);
		printf("Socket[ 0 ].state = %d \r\n", Socket[0].state);
		printf("Socket[ 1 ].state = %d \r\n", Socket[1].state);
		printf("Socket[ 2 ].state = %d \r\n", Socket[2].state);
		printf("Socket[ 3 ].state = %d \r\n", Socket[3].state);
		printf("Socket[ 4 ].state = %d \r\n", Socket[4].state);
    printf("Connect(Link) ID = %d \r\n",Socket[n].conn_id);
		printf("Remote IP = %d.%d.%d.%d \r\n",Socket[n].r_ip[0],Socket[n].r_ip[1],Socket[n].r_ip[2],Socket[n].r_ip[3]);
		printf("Local IP = %d.%d.%d.%d \r\n",Socket[n].l_ip[0],Socket[n].l_ip[1],Socket[n].l_ip[2],Socket[n].l_ip[3]);
		printf("Server Socket at WIFI_DRIVER= %d \r\n",Socket[n].server);
		printf("Server IP setting = %d.%d.%d.%d \r\n",Server_ip[0],Server_ip[1],Server_ip[2],Server_ip[3]);
		printf("-------------------------------------------------------\r\n");
    #endif
	}
	return clientSock; //connected client identifier
}

/* Server Socket Create, Bind, Listen */
static int Setup_TCPServer_Socket(const char *portNumber)
{
	int servSock =  -1;
	int ret;

	/* Socket Create when Station Connected to AP */
 	servSock = iotSocketCreate (IOT_SOCKET_AF_INET, IOT_SOCKET_SOCK_STREAM, IOT_SOCKET_IPPROTO_TCP);
	if(servSock <  0)
	{
		printf("iotSocketCreate() failed %d\r\n", servSock);
 	}
	else
	{
		printf("Server Socket is %d: iotSocketCreate() success \r\n", servSock);
	}

	ret = iotSocketBind(servSock, Server_ip, sizeof(Server_ip), portNumber);
	if (ret < 0)
	{
		printf("Server: iotSocketBind() failed, ret = %d\r\n", ret);
	}
	else 
	{
	  //Under line comments are normally used technique
	  
		/* success of bind and wait */
    //Realistically, wait client connection and connected inform at iotSocketAccept()
    
		/* Socket Close Only Station Disconnect AP connection */
		//iotSocketClose(servSock);	//acquire socket and try others
		//servSock = -1; 

    /* Give Back Address Memory List Only Station Disconnect AP connection */
    //example: freeAddrXXX(servSock);

    printf("Setup_TCPServer_Socket() success\r\n");
	}
	
	return servSock;
}

/*-----------------------------------------------------------*/
