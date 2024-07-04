#ifndef mesh_lib //Check if mesh_lib has not already been defined, if not then define it.
#define mesh_lib

#include "esp_wifi.h" //Requirement for mesh and mqtt protocol
#include "esp_mesh.h" //Required to use mesh functions
#include "mqtt_client.h" //Required to use mqtt library functions
#include "esp_err.h" //Libary used for error checking and handling functions
#include "esp_netif.h" //Some important network library I don't know its purpose
#include "esp_event.h" //Required for event handling functions -> Not used in my code 
#include "esp_log.h" //For using functions like LOGI, LOGD for data logging/Feedback
#include "nvs_flash.h" //Non Volatile Storage Library for storing important Mesh, MQTT and WiFiconfig data in Flash

namespace meshy //Define a custom namespace required to run all library functions
{
    static const char *Log_tag = "mesh_lib"; //For all log files
    esp_mqtt_client_handle_t client; //Define a client structure responsible for MQTT



    //Necessary for setting up WiFi connection
    void prereq_init()
    {
        ESP_ERROR_CHECK(esp_netif_init());
        /*  event initialization */
        ESP_ERROR_CHECK(esp_event_loop_create_default());
        /*  Wi-Fi initialization */
        wifi_init_config_t config = WIFI_INIT_CONFIG_DEFAULT();
        ESP_ERROR_CHECK(esp_wifi_init(&config));


        // Event handlers were removed as I couldn't understand how to register them by default
        /*  register IP events handler */
        //ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &ip_event_handler, NULL));
        ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_FLASH));
        ESP_ERROR_CHECK(esp_wifi_start()); //Start WiFi activity

    }

//----------------------------------------------------------------------------------------------------------------------------------------------------    
    // Code to initialize WiFi connection
    void pre_init_mesh()
    {
        /*  mesh initialization */
        ESP_ERROR_CHECK(esp_mesh_init());
        // Event handlers were removed as I couldn't understand how to register them by default
        /*  register mesh events handler */
        //ESP_ERROR_CHECK(esp_event_handler_register(MESH_EVENT, ESP_EVENT_ANY_ID, &mesh_event_handler, NULL));
    }

//----------------------------------------------------------------------------------------------------------------------------------------------------    
    //Initialize Mesh network
    void init_mesh(bool master, const char *MESH_ID, const char *router_ssid, const char *router_pass, const char *AP_pass, int channel_no )
    {

        /* Enable the Mesh IE encryption by default */
        mesh_cfg_t cfg = MESH_INIT_CONFIG_DEFAULT();


        //---------------------mesh ID and channel configuration---------------------------
        memcpy((uint8_t *) &cfg.mesh_id, MESH_ID, 6);
        /* channel (must match the router's channel) */
        cfg.channel = channel_no;


        //----------------------Router configuration---------------------------------------
        cfg.router.ssid_len = strlen(router_ssid); //length of SSID = length of string entered by user
        memcpy((uint8_t *) &cfg.router.ssid, router_ssid, cfg.router.ssid_len);
        memcpy((uint8_t *) &cfg.router.password, router_pass, strlen(router_pass));


        //----------------mesh softAccess point configuration------------------------------
        if(master)
        {
            cfg.mesh_ap.max_connection = 4; //Max connections for master node
        }
        else
            cfg.mesh_ap.max_connection = 1; //Max connections for slave node

        memcpy((uint8_t *) &cfg.mesh_ap.password, AP_pass, strlen(AP_pass));
        ESP_ERROR_CHECK(esp_mesh_set_config(&cfg));

        //Needs to be set after mesh config
        //----------------------------Set Master Slave relation----------------------------
        if(master)
        {
            ESP_LOGI(Log_tag, "Setting node as Master"); //Return log information
            ESP_ERROR_CHECK(esp_mesh_set_type(MESH_ROOT)); //Set as master_node
        }
        else
        {
            ESP_LOGI(Log_tag, "Setting node as Master"); //Return log information
            ESP_ERROR_CHECK(esp_mesh_set_type(MESH_LEAF)); //set as slave_node, cannot extend signals any further
        }
        ESP_ERROR_CHECK(esp_mesh_start()); // Start the esp WiFi mesh
    }
    
//----------------------------------------------------------------------------------------------------------------------------------------------------
    //Initialize MQTT Protocol
    void init_mqtt(const char *broker_uri, const char *username, const char *password)
    {
        esp_mqtt_client_config_t mqtt_cfg;
        //Access individual components of struct mqtt_cfg and define them
        mqtt_cfg.broker.address.uri = broker_uri; //User configurable broker URI
        mqtt_cfg.credentials.username = username; //User configurable Username
        mqtt_cfg.credentials.authentication.password = password; //User configurable Password

        //Creating a client instance
        client = esp_mqtt_client_init(&mqtt_cfg); // Sending struct as a parameter in init client function
        esp_mqtt_client_start(client); // Starting the process

        
    }

//-------------------------------------------Main library functions to be used------------------------------------
    //Master function with all relevant user configurable parameters
    void def_master(const char *MESH_ID, const char *router_ssid, const char *router_pass, const char *AP_pass, int channel_no, const char *broker_uri, const char *username,const char *password) //Define master function
    {
        nvs_flash_init(); //Stores important WiFi and Mesh configuration files
        //Setup prerequisits, start WiFi, initiate Mesh
        prereq_init(); 
        pre_init_mesh();
        init_mesh(true, MESH_ID, router_ssid, router_pass, AP_pass, channel_no);// Initialize Mesh network
        init_mqtt(broker_uri, username, password);//Initialize MQTT Protocol

    }
    //Slave function with all relevant user configurable parameters
    void def_slave(const char *MESH_ID, const char *router_ssid, const char *router_pass, const char *AP_pass, int channel_no, const char *broker_uri, const char *username,const char *password)
    {
        nvs_flash_init(); //Stores important WiFi and Mesh configuration files
        //Setup prerequisits, start WiFi, initiate Mesh
        prereq_init();
        pre_init_mesh();
        //Initialize mesh and mqtt network
        //Initialize mesh and mqtt network
        init_mesh(false,  MESH_ID, router_ssid, router_pass, AP_pass, channel_no); // Initialize Mesh network
        init_mqtt(broker_uri, username, password); //Initialize MQTT Protocol
    }
}

#endif