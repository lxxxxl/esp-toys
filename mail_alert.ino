#include <ESP8266WiFi.h>
#include <ESP_Mail_Client.h>

/* Wi-Fi connection settings */
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASS"

/* SMPT server address and port */
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465

/* Email credentials */
#define AUTHOR_EMAIL "FROM@EMAIL.COM"
#define AUTHOR_PASSWORD "PASSWORD"

/* Recipient's email */
#define RECIPIENT_EMAIL "TO@EMAIL.COM"

/* SMTP Session object used for Email sending */
SMTPSession smtp;

/* Callback function to get Email sending status */
void smtpCallback(SMTP_Status status){
  // Print the current status
  Serial.println(status.info());

  // Print the sending result
  if (status.success()){
    ESP_MAIL_PRINTF("Mesasges sent: %d\n", status.completedCount());
    ESP_MAIL_PRINTF("Messages failed: %d\n", status.failedCount());
  }
}

/* Connects to specified Wi-Fi network */
void wifiConnect(){
  Serial.print("Connecting to AP");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(200);
  }
  Serial.println();
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

/* Sends email with specified subject and data to recipient */
void sendEmail(char* recipient, char* subject, char* data){
  // Enable debug via Serial port
  smtp.debug(1);

  // Set the callback function to get the sending results
  smtp.callback(smtpCallback);

  // Declare the session config data
  ESP_Mail_Session session;

  // Set the session config
  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;
  session.login.user_domain = "";

  // Declare the message class
  SMTP_Message message;

  // Set the message headers
  message.sender.email = AUTHOR_EMAIL;
  message.subject = subject;
  message.addRecipient("", recipient);

  // Set message contents
  message.html.content = data;
  message.text.charSet = "utf8";
  message.html.transfer_encoding = Content_Transfer_Encoding::enc_base64;

  // Connect to SMTP server with the session config
  if (!smtp.connect(&session))
    return;

  // Start sending Email and close the session
  if (!MailClient.sendMail(&smtp, &message))
    Serial.println("Error sending Email, " + smtp.errorReason());
}



void setup(){
  Serial.begin(115200);
  // Connect to Wi-Fi
  wifiConnect();
  // Setup email contents
  char* notification  = "Notification contents";
  // Send email
  sendEmail(RECIPIENT_EMAIL, "ESP notify", notification);

  Serial.println("Going to sleep...");
  
  // Sleep while there is HIGH on RST pin
  // Connect alert signal source to RST pin. Source should emit LOW signal on some event.
  ESP.deepSleep(0);
}

void loop(){

}
