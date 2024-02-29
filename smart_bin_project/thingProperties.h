
#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>

const char DEVICE_LOGIN_NAME[]  = "9b4f710c-4b9b-4a9a-8843-9213499abcf3";

const char SSID[]               = SECRET_SSID;    
const char PASS[]               = SECRET_OPTIONAL_PASS;    
const char DEVICE_KEY[]  = SECRET_DEVICE_KEY;    


CloudColor led_status;
float occupancy_rate;
CloudColoredLight LedStatus;
int noUsage;

void initProperties(){

  ArduinoCloud.setBoardId(DEVICE_LOGIN_NAME);
  ArduinoCloud.setSecretDeviceKey(DEVICE_KEY);
  ArduinoCloud.addProperty(led_status, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(occupancy_rate, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(LedStatus, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(noUsage, READ, ON_CHANGE, NULL);

}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);
