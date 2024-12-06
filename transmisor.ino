#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define LED_PIN 4         // Pin donde está conectado el LED
#define BIT_DURATION 500  // Duración de cada bit en milisegundos

// Configuración de Wi-Fi
const char* ssid = "No one";        // Reemplaza con tu SSID
const char* password = "password"; // Reemplaza con tu contraseña

ESP8266WebServer server(80); // Servidor web en el puerto 80

void setup() {
  pinMode(LED_PIN, OUTPUT);     // Configura el pin del LED como salida
  digitalWrite(LED_PIN, LOW);   // Asegúrate de que el LED esté apagado al inicio

  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.print("Conectando a Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConexión establecida. Dirección IP: " + WiFi.localIP().toString());

  // Ruta principal para ingresar palabras
  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html",
      "<form action=\"/send\" method=\"POST\">"
      "<label>Ingrese la palabra a enviar:</label><br>"
      "<input type=\"text\" name=\"word\"><br>"
      "<input type=\"submit\" value=\"Enviar\">"
      "</form>"
    );
  });

  // Ruta para procesar la palabra y enviarla
  server.on("/send", HTTP_POST, []() {
    if (server.hasArg("word")) {
      String palabra = server.arg("word"); // Guardar el argumento en una variable
      palabra.toUpperCase();              // Convertir a mayúsculas
      server.send(200, "text/plain", "Palabra enviada: " + palabra);
      Serial.println("Transmitiendo palabra: " + palabra);
      transmitirPalabra(palabra);
    } else {
      server.send(400, "text/plain", "No se ingresó ninguna palabra.");
    }
  });

  server.begin(); // Inicia el servidor web
}

void loop() {
  server.handleClient(); // Maneja las peticiones web
}

void transmitirPalabra(String palabra) {
  for (int i = 0; i < palabra.length(); i++) {
    char caracter = palabra[i];
    Serial.print("Transmitiendo carácter: ");
    Serial.println(caracter);

    // Transmitir el carácter como 5 bits con bit de sincronización
    transmitirCaracter(caracter);
    delay(BIT_DURATION * 4); // Pausa adicional entre caracteres
  }
}

void transmitirCaracter(char caracter) {
  // Validar que sea una letra mayúscula entre 'A' y 'Z'
  if (caracter < 'A' || caracter > 'Z') {
    Serial.println("Carácter inválido: " + String(caracter));
    return;
  }

  // Agregar un bit de inicio (1) para sincronización
  transmitirBit(1);

  // Convertir el carácter a su índice en el alfabeto (A=1, B=2, ..., Z=26)
  int valor = caracter - 'A' + 1;

  // Transmitir los 5 bits de la letra
  for (int i = 4; i >= 0; i--) {
    int bit = (valor >> i) & 1; // Extraer cada bit
    transmitirBit(bit);
  }
}

void transmitirBit(int bit) {
  if (bit == 1) {
    digitalWrite(LED_PIN, HIGH); 
  } else {
    digitalWrite(LED_PIN, LOW);  
  }

  delay(BIT_DURATION); // Esperar el tiempo del bit

  // Apagar el LED brevemente al final de cada bit
  digitalWrite(LED_PIN, LOW);
  //delay(50); // Pausa entre bits
}
