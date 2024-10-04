#define sensor 25
#define Led1 19

int filteredValue = 0;  // Initialize filtered value
float alpha = 0.1;  // Filter coefficient (0 < alpha < 1)
int odl = 0;
void setup() {
  Serial.begin(115200);
  Serial.println("Test czujnika odbiciowego"); 
  pinMode(sensor, INPUT);
  pinMode(Led1, OUTPUT);
  digitalWrite(Led1, LOW);
}

void loop() {
  odl = analogRead(sensor);      //odczytanie wartości z czujnika
  filteredValue = alpha * odl + (1 - alpha) * filteredValue;  // Filter equation
  if(filteredValue>506){
  digitalWrite(Led1, LOW);
  Serial.println(filteredValue);                //wyświetlenie jej na monitorze
  }else{
    Serial.println(filteredValue); 
    digitalWrite(Led1, HIGH);
  }
  delay(20);                         //opóźnienie między kolejnymi odczytami
}
