#include "OSC/callbacks.h"
#include "logging/Logger.h"

namespace Haptics {
namespace Wireless
{
    Logging::Logger logger("Wireless");


    void printRawPacket() {
        printRaw();
    }
    
    /// @brief start mDNS and OSC
    void Start(Config *conf) {
    
        WiFi.mode(WIFI_STA);
        WiFi.setSleep(false); // thanks Daky
    
        // Start WiFi connection
        WiFi.begin(conf->wifi_ssid, conf->wifi_password);
        logger.debug("WIFI: Connecting");
        
        // Wait for connection
        while (WiFi.status() != WL_CONNECTED) {
            delay(1); // needed for esp8266
        }
    
        // Print the IP address
        selfIP = WiFi.localIP().toString();
        logger.debug("Wifi Connected:", selfIP);
    
        // Start listening for OSC server
        OscWiFi.subscribe(RECIEVE_PORT, PING_ADDRESS, &handlePing);
        logger.debug("WIFI: OSC: server started on port:", RECIEVE_PORT);
    
        StartMDNS(conf);// NEEDS TO BE AFTER SUBSCRIBER SET UP
        
    }
    
    void StartMDNS(Config *conf) {
        //close any previous entries
        MDNS.end();
        delay(100);
    
        // Start mDNS service
        if (MDNS.begin(conf->mdns_name)) {
            MDNS.addService(MDNS_SERVICE_NAME, MDNS_SERVICE_PROTOCOL, RECIEVE_PORT);  // Advertise the service
            MDNS.addServiceTxt(MDNS_SERVICE_NAME, MDNS_SERVICE_PROTOCOL, "MAC", WiFi.macAddress());
            logger.debug("WIFI: mDNS service started");
    
        } else {
            logger.error("WIFI: Error starting mDNS");
        }
    
    }
    
    bool WiFiConnected(){
        return WiFi.status() == WL_CONNECTED;
    }
    
    void StartHeartBeat() {
        if (hostIP.isEmpty() | !sendPort) {
            logger.debug("HeartBeat set up before ip filled!");
        } else if (heartbeatPublisher){
            logger.debug("Heartbeat already set up");
        }
    
        // Publish heart beat on one second intervals
        OscWiFi.publish(hostIP, sendPort, HEARTBEAT_ADDRESS, &millis, selfIP, selfMac) 
            -> setFrameRate(1);
        heartbeatPublisher = OscWiFi.getPublishElementRef(hostIP, sendPort, HEARTBEAT_ADDRESS);
    
        if (!heartbeatPublisher) {
            logger.warn("Heartbeat wasn't established");
        }
    }
    
    void handlePing(const OscMessage& message){
        //if we recieve a ping and we were already setup, it is likely a server restart. 
        //In that case the port, ip, and other values should be reinited
    
        sendPort = message.arg<uint16_t>(0);  // Get the host's port from the message
        hostIP = message.remoteIP();  // Get the host's IP address
    
        //create our own recieving server
        OscWiFi.subscribe(RECIEVE_PORT, MOTOR_ADDRESS, &motorMessage_callback);
    
        logger.debug("Received ping from:", hostIP);
    
        //sending client
        oscClient = OscWiFi.getClient();
    
        // Respond to ping
        OscMessage pingResponse(PING_ADDRESS);
        pingResponse.pushInt32(RECIEVE_PORT);
        pingResponse.pushString(WiFi.macAddress());
        oscClient.send(hostIP, sendPort, pingResponse);
    
        if (!heartbeatPublisher){
            StartHeartBeat();
        }
    
    }
    
    /// @brief Push and pull OSC updates
    void Tick() {
        OscWiFi.update(); // should be called to subscribe + publish osc
        
    }



} // namespace Wireless
} // namespace Haptics
