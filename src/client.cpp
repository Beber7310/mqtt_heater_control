/*
 * This example shows how to publish messages from outside of the Mosquitto network loop.
 */

#include "mosquitto.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>

#include "SerialManagement.h"

extern sem_t semaphore;
int cmd_radiateur = 0xEF;

/* Callback called when the client receives a CONNACK message from the broker. */
void
on_connect (struct mosquitto *mosq, void *obj, int reason_code)
{
  int rc;
  char topic[100];

  /* Print out the connection result. mosquitto_connack_string() produces an
   * appropriate string for MQTT v3.x clients, the equivalent for MQTT v5.0
   * clients is mosquitto_reason_string().
   */

  printf ("on_connect: %s\n", mosquitto_connack_string (reason_code));
  if (reason_code != 0)
    {
      /* If the connection fails for any reason, we don't want to keep on
       * retrying in this example, so disconnect. Without this, the client
       * will attempt to reconnect. */
      mosquitto_disconnect (mosq);
    }

  for (int ii = 0; ii < 5; ii++)
    {

      sprintf (topic, "radiateur/power_%i/cmnd", ii);
      rc = mosquitto_subscribe (mosq, NULL, topic, 1);
      if (rc != MOSQ_ERR_SUCCESS)
	{
	  fprintf (stderr, "Error subscribing: %s\n", mosquitto_strerror (rc));
	  /* We might as well disconnect if we were unable to subscribe */
	  mosquitto_disconnect (mosq);
	}

    }

}

/* Callback called when the client knows to the best of its abilities that a
 * PUBLISH has been successfully sent. For QoS 0 this means the message has
 * been completely written to the operating system. For QoS 1 this means we
 * have received a PUBACK from the broker. For QoS 2 this means we have
 * received a PUBCOMP from the broker. */
void
on_publish (struct mosquitto *mosq, void *obj, int mid)
{
  printf ("Message with mid %d has been published.\n", mid);
}

void
publish_power_data (struct mosquitto *mosq, int power)
{
  char payload[20];

  int rc;
  static int sampleCount = 0;
  static float powerAcc = 0;
  static float shared_power = 0;

  /* Get our pretend data */

  powerAcc += power;
  sampleCount++;

  /* Print it to a string for easy human reading - payload format is highly
   * application dependent. */
  snprintf (payload, sizeof(payload), "%d", power);

  rc = mosquitto_publish (mosq, NULL, "power/watt", strlen (payload), payload, 2, false);
  if (rc != MOSQ_ERR_SUCCESS)
    {
      fprintf (stderr, "Error publishing: %s\n", mosquitto_strerror (rc));
    }

  if (sampleCount > 60)
    {
      sampleCount = 0;
      shared_power += powerAcc / 3600000.0f;
      powerAcc = 0;

      snprintf (payload, sizeof(payload), "%f", shared_power);

      rc = mosquitto_publish (mosq, NULL, "power/kwh", strlen (payload), payload, 2, false);
      if (rc != MOSQ_ERR_SUCCESS)
	{
	  fprintf (stderr, "Error publishing: %s\n", mosquitto_strerror (rc));
	}
    }

}

void
publish_temperature_data (struct mosquitto *mosq, char *thermometre, char *temp)
{
  char topic[100];

  int rc;

  /* Get our pretend data */

  /* Print it to a string for easy human reading - payload format is highly
   * application dependent. */
  sprintf (topic, "thermometre/%s", thermometre);

  rc = mosquitto_publish (mosq, NULL, topic, strlen (temp), temp, 2, false);
  if (rc != MOSQ_ERR_SUCCESS)
    {
      fprintf (stderr, "Error publishing: %s\n", mosquitto_strerror (rc));
    }

}

void
publish_switch_data (struct mosquitto *mosq, char *interupteur, char *temp)
{
  char topic[100];

  int rc;

  /* Get our pretend data */

  /* Print it to a string for easy human reading - payload format is highly
   * application dependent. */
  sprintf (topic, "switch/%s", interupteur);

  rc = mosquitto_publish (mosq, NULL, topic, strlen (temp), temp, 2, false);
  if (rc != MOSQ_ERR_SUCCESS)
    {
      fprintf (stderr, "Error publishing: %s\n", mosquitto_strerror (rc));
    }

}

void
publish_relay_data (struct mosquitto *mosq)
{
  int rc;
  char topic[100];
  char payload[20];

  for (int ii = 0; ii < 5; ii++)
    {

      sprintf (topic, "radiateur/power_%i/state", ii);
      if (ii == 4)
	{
	  if (cmd_radiateur & (0x01 << ii))
	    {
	      sprintf (payload, "ON");
	    }
	  else
	    {
	      sprintf (payload, "OFF");
	    }
	}
      else
	{
	  if (cmd_radiateur & (0x01 << ii))
	    {
	      sprintf (payload, "OFF");
	    }
	  else
	    {
	      sprintf (payload, "ON");
	    }
	}
      rc = mosquitto_publish (mosq, NULL, topic, strlen (payload), payload, 2, false);
      if (rc != MOSQ_ERR_SUCCESS)
	{
	  fprintf (stderr, "Error publishing: %s\n", mosquitto_strerror (rc));
	}
    }
}

/* This function pretends to read some data from a sensor and publish it.*/
void
publish_sensor_data (struct mosquitto *mosq, int pipe)
{
  char topic[100];
  char payload[100];
  char payload2[100];
  read (pipe, payload, 2);

  if (strncmp (payload, "A:", 2) == 0)
    {
      read (pipe, payload, 6);
      publish_power_data (mosq, atoi (payload));
    }
  else if (strncmp (payload, "C:", 2) == 0)
    {
      read (pipe, topic, 7);
      topic[7] = 0;
      printf ("%s\n", topic);

      if (strncmp (topic, "65", 2) == 0)
	{
	  read (pipe, payload, 3); // drop the space
	  read (pipe, payload, 3);
	  payload[3] = 0;
	  float temperature = strtol (payload, 0, 16) / 10.0f;
	  sprintf (payload2, "%2.2f\n", temperature);
	  publish_temperature_data (mosq, topic, payload2);
	}
      else if (strncmp (topic, "FE", 2) == 0)
	{
	  read (pipe, payload, 1); // drop the space
	  read (pipe, payload, 1);
	  payload[1] = 0;
	  printf ("%s", payload);
	  publish_switch_data (mosq, topic, payload);
	  read (pipe, payload, 4);
	}
    }
  else
    {
      printf ("Error while reading pipe");
    }

}

// C:65051012D00CD

/* Callback called when the broker sends a SUBACK in response to a SUBSCRIBE. */
void
on_subscribe (struct mosquitto *mosq, void *obj, int mid, int qos_count, const int *granted_qos)
{
  int i;
  bool have_subscription = false;

  /* In this example we only subscribe to a single topic at once, but a
   * SUBSCRIBE can contain many topics at once, so this is one way to check
   * them all. */
  for (i = 0; i < qos_count; i++)
    {
      // printf("on_subscribe: %d:granted qos = %d\n", i, granted_qos[i]);
      if (granted_qos[i] <= 2)
	{
	  have_subscription = true;
	}
    }
  if (have_subscription == false)
    {
      /* The broker rejected all of our subscriptions, we know we only sent
       * the one SUBSCRIBE, so there is no point remaining connected. */
      fprintf (stderr, "Error: All subscriptions rejected.\n");
      mosquitto_disconnect (mosq);
    }
}

/* Callback called when the client receives a message. */
void
on_message (struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
{
  /* This blindly prints the payload, but the payload can be anything so take care. */
  printf ("%s %d %s\n", msg->topic, msg->qos, (char*) msg->payload);
  int idx = -1;

  if (strncmp (msg->topic, "radiateur/power", strlen ("radiateur/power")) == 0)
    {
      idx = msg->topic[strlen ("radiateur/power_")] - '0';

      if ((idx >= 0) && (idx <= 3))
	{
	  if (strncmp ((char*) msg->payload, "ON", 2) == 0)
	    {
	      //Inverted on purpose as a tension "freeze" the heater
	      cmd_radiateur &= ~(0x01 << idx);
	    }
	  else
	    {
	      //Inverted on purpose as a tension "freeze" the heater
	      cmd_radiateur |= 0x01 << idx;
	    }
	}
      if (idx == 4)
	{
	  if (strncmp ((char*) msg->payload, "ON", 2) == 0)
	    {
	      cmd_radiateur |= 0x01 << idx;
	    }
	  else
	    {
	      cmd_radiateur &= ~(0x01 << idx);
	    }
	}

    }
  SerialFilPiloteSendCommande (cmd_radiateur);
  publish_relay_data (mosq);
}

int
main_mqtt (int pipe)
{
  struct mosquitto *mosq;
  int rc;

  SerialFilPiloteSendCommande (cmd_radiateur);

  /* Required before calling other mosquitto functions */
  mosquitto_lib_init ();

  /* Create a new client instance.
   * id = NULL -> ask the broker to generate a client id for us
   * clean session = true -> the broker should remove old sessions when we connect
   * obj = NULL -> we aren't passing any of our private data for callbacks
   */
  mosq = mosquitto_new (NULL, true, NULL);
  if (mosq == NULL)
    {
      fprintf (stderr, "Error: Out of memory.\n");
      return 1;
    }

  /* Configure callbacks. This should be done before connecting ideally. */
  mosquitto_connect_callback_set (mosq, on_connect);
  mosquitto_publish_callback_set (mosq, on_publish);
  mosquitto_subscribe_callback_set (mosq, on_subscribe);
  mosquitto_message_callback_set (mosq, on_message);

  rc = mosquitto_username_pw_set (mosq, "mqttuser", "mqttuser");
  if (rc != MOSQ_ERR_SUCCESS)
    {
      mosquitto_destroy (mosq);
      fprintf (stderr, "Error: %s\n", mosquitto_strerror (rc));
      return 1;
    }

  /* Connect to test.mosquitto.org on port 1883, with a keepalive of 60 seconds.
   * This call makes the socket connection only, it does not complete the MQTT
   * CONNECT/CONNACK flow, you should use mosquitto_loop_start() or
   * mosquitto_loop_forever() for processing net traffic. */
  rc = mosquitto_connect (mosq, "homeassistant", 1883, 60);
  if (rc != MOSQ_ERR_SUCCESS)
    {
      mosquitto_destroy (mosq);
      fprintf (stderr, "Error: %s\n", mosquitto_strerror (rc));
      return 1;
    }

  /* Run the network loop in a background thread, this call returns quickly. */
  rc = mosquitto_loop_start (mosq);
  if (rc != MOSQ_ERR_SUCCESS)
    {
      mosquitto_destroy (mosq);
      fprintf (stderr, "Error: %s\n", mosquitto_strerror (rc));
      return 1;
    }

  publish_relay_data (mosq);
  /* At this point the client is connected to the network socket, but may not
   * have completed CONNECT/CONNACK.
   * It is fairly safe to start queuing messages at this point, but if you
   * want to be really sure you should wait until after a successful call to
   * the connect callback.
   * In this case we know it is 1 second before we start publishing.
   */
  while (1)
    {
      publish_sensor_data (mosq, pipe);
    }

  mosquitto_lib_cleanup ();
  return 0;
}

