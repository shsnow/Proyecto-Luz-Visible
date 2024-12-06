#define LDR_PIN A0        // Pin ADC del ESP8266
#define THRESHOLD 500     // Umbral para distinguir entre 1 (luz presente) y 0 (luz ausente)
#define BIT_DURATION 500  // Duración esperada de cada bit en milisegundos

// Arreglo con las 26 letras del abecedario
const char alphabet[26] = { 
  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 
  'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 
  'U', 'V', 'W', 'X', 'Y', 'Z' 
};

void setup() {
  Serial.begin(115200);
}

void loop() {
  Serial.println("Esperando bit de inicio...");
  
  // Esperar hasta detectar el bit de inicio (1)
  while (analogRead(LDR_PIN) < THRESHOLD) {
    delay(1); // Esperar mientras no haya suficiente luz
  }

  Serial.println("Bit de inicio detectado. Recibiendo bits...");
  delay(BIT_DURATION+BIT_DURATION/2); // Ajuste inicial para estabilizar lectura

  // Leer los siguientes 5 bits
  String bits = "";
  for (int i = 0; i < 5; i++) {
    int ldrValue = analogRead(LDR_PIN);
    int bit = (ldrValue >= THRESHOLD) ? 1 : 0; // Comparar con el umbral
    bits += String(bit);
    Serial.println("Bit recibido: " + String(bit));
    delay(BIT_DURATION); // Esperar el tiempo del siguiente bit
  }

  // Convertir los bits a un índice y obtener la letra
  char letter = bitsToChar(bits);
  if (letter != '\0') {
    Serial.println("Letra recibida: " + String(letter));
  } else {
    Serial.println("Bits inválidos: " + bits);
  }

  delay(1000); // Pausa antes de procesar un nuevo mensaje
}

char bitsToChar(String bits) {
  if (bits.length() != 5) return '\0'; // Validar longitud de bits
  int index = strtol(bits.c_str(), NULL, 2) - 1; // Convertir bits binarios a índice (0-25)
  
  // Validar índice dentro del rango del alfabeto
  if (index < 0 || index >= 26) return '\0'; 
  return alphabet[index]; // Retornar la letra correspondiente
}
