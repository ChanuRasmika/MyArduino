#include <SoftwareSerial.h>

SoftwareSerial gsm(7, 8);  // Define the GSM module's serial connections
SoftwareSerial gps(10 , 11);  // Define the GPS module's serial connections

String latitude = "";
String longitude = "";

void setup() {
  Serial.begin(9600);
  gsm.begin(9600);
  gps.begin(9600);

  // Initialize GSM module
  gsm.println("AT");
  delay(1000);
  gsm.println("AT+CREG?");
  delay(1000);
  gsm.println("AT+CGATT?");
  delay(1000);
  gsm.println("AT+CIPSHUT");
  delay(1000);
  gsm.println("AT+CIPMUX=0");
  delay(1000);
  gsm.println("AT+CSTT=\"your-apn\",\"\",\"\"");
  delay(1000);
  gsm.println("AT+CIICR");
  delay(1000);
  gsm.println("AT+CIFSR");
  delay(1000);
  gsm.println("AT+CIPSTART=\"TCP\",\"BusWise.firebaseio.com\",\"443\"");
  delay(1000);
}

void loop() {
  // Read GPS data
  if (gps.available()) {
    char c = gps.read();
    if (c == '$') {
      String sentence = gps.readStringUntil('\n');
      if (sentence.startsWith("$GPRMC")) {
        // Parse the GPRMC sentence to get latitude and longitude
        String parts[12];
        int index = 0;
        int startIndex = 0;
        for (int i = 0; i < sentence.length(); i++) {
          if (sentence[i] == ',') {
            parts[index++] = sentence.substring(startIndex, i);
            startIndex = i + 1;
          }
        }
        latitude = parts[3];
        longitude = parts[5];
      }
    }
  }

  if (latitude != "" && longitude != "") {
    // Construct the data,  want to send to Firestore
    String dataToSend = "{\"latitude\": " + latitude + ", \"longitude\": " + longitude + "}";

    // Send data to Firestore via HTTPS POST request
    String postRequest = "POST /v1/projects/BusWise/databases/(default)/documents/gps-data?key="-----BEGIN PRIVATE KEY-----\nMIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKcwggSjAgEAAoIBAQDaE6l3LX+H7smn\nai93Y4btJIlDykM6Vcw6Fnwqj9/LOyD3bkBxDwnfDKPJW1i7unrcnddum+EeG8/R\nNOS56bfhHpITTFF2T9VMP80WPdQS9F8LJS0np616wDwGrOgKoDvdFc2ylJtK8QlB\net6iVqFak4fRTeNdJmm9nJSddll0XH90YyVxDMkNdUE4+WMNT1AjqRQrqr5ZYtqh\naHeSOPVinbM/Gbzb4aQS+9C36e6Cax8+HpKTYGScNU7f5R/1cWt43gyt4iQlHhcV\nSqXoe0dRS+4W6tU8f18GaJwKoUruXpqETRbr0w17juac7sAWInLjittcy3LFpiqr\n8sUq7oEVAgMBAAECggEAIxumvO3JegIWm1n0NdGDU5K9+XiqzY2qAREFINoEQqTs\niAELJVhxM6RGiwBmfuSE6pcLq34zZgQ+tvS/vPF0Pa0NAcfX1ocANRQRaZr/FSUb\n1wC/bgKn4lrn3wA/pBRLRuq/cJhNjulSnuTPyk1NkTYul6LlL2YjgFfgsh/vQ1Tj\njLnqMfF79VeSpX5IhWFyrZkLLumRamQ42bKtxFm4l+K+HaqyOBKuw+SeW6p2urJA\nZV4xeHQanLqoPWWaoJRnDN68KMVjXzSeCRsYl525w0ZmfChEvifXQlRwaVbViQme\nlIV9hodshHRLZBqExw+sdSvw7mQm+uL3uBWP/ittWQKBgQD/oKVNko/8VVjPJ5DI\n6DyYtvaD5qryqwm/rNkJo/KvcCoFWXp547T0xT8iWAxlSNHn+DIOg5O+jT98f+AP\nczwEnZ11QYKbutUUV7ru25pvjTULrxlWwCw0Ieyq7Hf5K8fIAjuqPvqv/6O1tnbX\nQMYokwImqRYbDLPzB1/rRfQhfQKBgQDaZQJSxJ/HNaBZahy8PlH5/zhxltQTn7Ob\n6BT7zkuWnXp3jflhMgie6WW9AS/Phm4N/d1a4GqtKNx6p42KgH9OSGnq+zGwcq5B\n1rziFTYSatZqYkSXXSNQycoPGdlAlaSbdmbOqFI8A+i4SboroSDIfU2N+aPsqgHP\nbdoqklTxeQKBgGYdgmtOlP40CdoW8BuRykMnM6rVRozSDn7nSXw0Zgn3QJIKgIaY\nk/0DnojVq336bP/Bgh2SKj8Apc/SNWt2mQ8u92uT09zp8ZzCvwYT68mMGxijxfeF\npSLeOnC9Ld/3wUMAwVzs86/C6173mNm8Imyq7T0MZqw/ktMOqx6NP0zxAoGAYzmh\nZJmRPqFOyVE83QozlLIwous0T/YvVJaNchq4nSXPjqIiu9ZPtGO2pbj0UAVz20eM\nWs27qhD2m4Ok+1iGfrrH5MtcOB7irWczSkT43EVORed/4oWelgYYl+PWoVFcCIYz\nD5HKSxo7kf9tZoU+r9i+c9MId26GhAgVJ2J01YECgYEAzvPeJPjOK5Ln+d5dvgom\nn7nx1IMaNuVvvurC7cwV4h5a6imyF3NV5KM2rTCpbZ8oM389fhHfpclKuC8yYRY9\nlYRKhnBdI5bhhwqOzkeI46F7QrrkObl30xmzlUfpkLvFD0swIXlA8RJr0IPdDhwq\nijVUiEmbLBiu2OWGSpfHuSs=\n-----END PRIVATE KEY-----\n", HTTP/1.1\r\n";
    postRequest += "Host: BusWise.firebaseio.com\r\n";
    postRequest += "Content-Type: application/json\r\n";
    postRequest += "Content-Length: " + String(dataToSend.length()) + "\r\n\r\n";
    postRequest += dataToSend;

    gsm.println("AT+CIPSEND=" + String(postRequest.length()));
    delay(1000);
    gsm.println(postRequest);
    
    // Reset the GPS data
    latitude = "";
    longitude = "";

    delay(10000);  // Send data every 10 seconds
  }
}