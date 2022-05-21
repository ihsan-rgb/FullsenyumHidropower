/*
 * Bagian PV menampilkan hasil pengukuran ke OLED
 * dan ke Serial plotter
 */

void pvMeasure(unsigned ts) {
  v = ina219.getBusVoltage_V();
  ma = ina219.getCurrent_mA();
  mw = ina219.getPower_mW();

  //Masukan ke array untuk di publish
  inputs[V] = v*1000; //dikirim ke dashboard dalam satuan mV
  inputs[I] = ma;
  inputs[P] = mw;

  //mengukur tegangan V1 dan V2 serta masukan ke data untuk publisher
  load_V1 = analogReadMv(DI0);
  load_V2 = analogReadMv(DI1);
  inputs[V1] = load_V1;s
  inputs[V2] = load_V2;

  // hitung total energi
  joule += mw * (0.001 * ts); 
}


//-----------------------------
byte chart[D_WIDTH];

// konstanta display OLED
#define CHART_MIN  0
#define CHART_MAX  20
#define CHART_SPAN (CHART_MAX-CHART_MIN)

void pvDisplay() {
  char buff[60];

  // masukkan data ke chart
  for(int i=D_WIDTH-1; i>0;i--) {
    chart[i]= chart[i-1];
  }
  
  // tambah data baru ke chart, konversi
  chart[0] = pv_mw * CHART_SPAN / PV_MW_SPAN;
  if (chart[0] > CHART_SPAN)
    chart[0] = CHART_SPAN;

  //import RPM
  int motor_rpm = optoGetRPM();

  // format tampilan text
  sprintf(buff, "V:%04.2f I:%04.2f P:%05.1f RPM:%d",
    v, ma, mw, motor_rpm);

  oled.clearDisplay();
  oled.setCursor(0,0);
  oled.print(buff);

  if (pv_display_mode == 0) {
    // plot chart
    for(int i=1; i<D_WIDTH; i++) {
      oled.drawLine(i, D_HEIGHT-chart[i-1], i+1, D_HEIGHT-chart[i], WHITE);
    }
  }
  else {
    // print total energi
    sprintf(buff, "E:%8.1f", pv_joule);
    oled.setTextSize(2);
    oled.setCursor(0, 12);
    oled.print(buff);
    oled.setTextSize(1);
  }  
  oled.display();


  // tampil ke Serial
  // header
  //sprintf(buff, "V I P E:%8.2f", pv_joule);
  Serial.print(buff);
  // data
  sprintf(buff, " E=%.2f", pv_joule);
  //sprintf(buff, "%5.3f %5.2f %5.2f", pv_v, pv_ma, pv_mw);
  Serial.println(buff);
}
