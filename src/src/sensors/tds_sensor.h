#ifndef TDS_SENSOR_H
#define TDS_SENSOR_H

#define TDS_PIN 34
float tdsValue = 0;
void initTDSSensor() {
  // Nothing needed for analog read
}
void readTDS() {

  int raw = analogRead(TDS_PIN);
  float voltage = (raw / 4095.0) * 5; //to volt 
  tdsValue = voltage * 500;             // to ppm ( neeed calibaration)
  Serial.print("TDS: ");               // for esp32 only this equation 
  Serial.println(tdsValue);
}

#endif