#define LDR_PIN A0  // Pin ADC del ESP8266
#define THRESHOLD 500  // Umbral para distinguir entre 1 (luz presente) y 0 (luz ausente)

void setup() {
  Serial.begin(115200);
}




void loop() {
  Serial.println("Esperando bit de inicio...");
  
  // Esperar hasta detectar el bit de inicio (1)
  while (analogRead(LDR_PIN) < THRESHOLD) {
    Serial.println(analogRead(LDR_PIN));
    delay(1);  // Esperar mientras no haya suficiente luz
  }
  
  Serial.println("Bit de inicio detectado. Recibiendo bits...");
  delay(100);  // Pausa para estabilizar después del bit de inicio
  
  // Leer los siguientes 5 bits
  String bits = "";
  for (int i = 0; i < 5; i++) {
    int ldrValue = analogRead(LDR_PIN);
    int bit = (ldrValue >= THRESHOLD) ? 1 : 0;  // Comparar con el umbral
    bits += String(bit);
    Serial.println("Bit recibido: " + String(bit));
    delay(500);  // Ajusta según la duración de cada bit
  }
  
  // Convertir los bits a una letra
  char letter = bitsToChar(bits);
  Serial.println("Letra recibida: " + String(letter));
  
  delay(1000);  // Esperar antes de recibir un nuevo mensaje
}

// Función para convertir una cadena de bits a un carácter ASCII
char bitsToChar(String bits) {
  int value = strtol(bits.c_str(), NULL, 2);  // Convertir bits binarios a decimal
  return char(value + 64);  // Sumar 64 para mapear a letras mayúsculas (A=65 en ASCII)
}