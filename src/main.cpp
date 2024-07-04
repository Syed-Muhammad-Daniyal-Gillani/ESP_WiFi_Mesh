#include "esp_wifi.h"
#include "wifi_mesh.h"


extern "C" 
/* Extern C used to support method overloading as C language does not support it <---- Name Mangling
Reference video: https://www.youtube.com/watch?v=K-9oDE0-XsE&t=54s */
{
    void app_main() 
    {
        //User configurable settings
        const char *MESH_ID = "Insert mesh ID";
        const char *router_ssid = "Insert Router SSID";
        const char *router_pass= "Insert Router Password";;
        const char *AP_pass = "Insert Access point password";
        int channel_no = 6;
        const char *broker_uri = "Insert MQTT Broker URI";
        const char *username = "Insert MQTT Username";
        const char *password = "Insert MQTT Password";
       
       //Create Master or Slave node
       //Master node -------- Remove "//" to set device as master
        meshy::def_master(MESH_ID, router_ssid, router_pass, AP_pass, channel_no,broker_uri, username, password);
       //Slave node -------- Remove "//" to set device as slave
        //meshy::def_slave(MESH_ID, router_ssid, router_pass, AP_pass, channel_no,broker_uri, username, password);

    }
}