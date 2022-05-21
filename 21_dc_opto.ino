// --------------------------------------------
// OPTO Coupler

/* ISR penghitung pulsa opto coupler
 * 
 */
void IRAM_ATTR optoISR() {
  portENTER_CRITICAL_ISR(&opto_mux);
  opto_counter++;
  portEXIT_CRITICAL_ISR(&opto_mux);
}


/* mengolah data optocoupler secara periodik
 * dengan moving average
 */
void optoMeasure() {
  OptoData d1, d2;

  // baca hasil hitungan interrupt
  portENTER_CRITICAL_ISR(&opto_mux);
  d1.count = opto_counter;
  opto_counter = 0;
  portEXIT_CRITICAL_ISR(&opto_mux);

  // baca interval waktu
  unsigned long now = micros();
  d1.interval = now - opto_last_time;
  opto_last_time = now;

  // hitung moving average
  if (obuff.count() >= N_WINDOW) {
    obuff.take(d2);
    opto_interval -= d2.interval;
    opto_sum -= d2.count;
  }
  opto_interval += d1.interval;
  opto_sum += d1.count;
  obuff.put(d1);
}


/* Menghitung RPM
 * Memakai data yg sudah disiapkan oleh optoMeasure
 */
unsigned optoGetRPM() {    
  // PERBAIKI SESUAI JUMLAH BLADE
  return (float) 30000000L * opto_sum / opto_interval;
}


/* Menyiapkan opto coupler
 *  
 */
void optoSetup() {
  obuff.reset();
  unsigned long opto_sum = 0;
  unsigned long opto_interval = 2;
  opto_counter=0;
  pinMode(pin_opto, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pin_opto), optoISR, FALLING); 
  opto_last_time = micros();
}

/*
 * Opto selesai bekerja
 */
void optoStop() {
  detachInterrupt(digitalPinToInterrupt(pin_opto)); 
}
