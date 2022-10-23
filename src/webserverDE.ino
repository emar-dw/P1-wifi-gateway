#ifdef GERMAN
void addIntro(String& str){
    char ipstr[20];
  IPAddress ip = WiFi.localIP();
  sprintf_P(ipstr, PSTR("%u.%u.%u.%u"), ip[0], ip[1], ip[2], ip[3]);
      str +=("<body><div style='text-align:left;display:inline-block;color:#000000;width:600px;'><div style='text-align:center;color:#000000;'><h2>P1 wifi-gateway</h2></div><br>");
      str += ("<p style='text-align:right;font-size:11px;color:#aaa'><a href='http://");
      str += ipstr;
      str += ("/Help' target='_blank'>Hilfe</a>");
}

void addUptime(String& str){
  char ipstr[20];
  IPAddress ip = WiFi.localIP();
  sprintf_P(ipstr, PSTR("%u.%u.%u.%u"), ip[0], ip[1], ip[2], ip[3]);
    
  str += ("<div style='text-align:left;font-size:11px;color:#aaa'><hr/>");
    char strUpTime[40];
    int minuten = millis() / 60000;
    int dagen = minuten / 1440;
    minuten = minuten % 1440;
    int uren = minuten / 60;
    minuten = minuten % 60;
    sprintf_P(strUpTime, PSTR("%d Tagen %d Uhren %d Minuten"), dagen, uren, minuten);
  str += strUpTime;
  str += ("<p style='text-align:right;font-size:11px;color:#aaa'>");
  str += ipstr;
  str += ("</div>");
}

void addFoot(String& str){
  str += F("<div style='text-align:right;font-size:11px;color:#aaa;'><hr/>version: ");
  str += version;
  str += F("<br><a href='http://esp8266thingies.nl' target='_blank' style='color:#aaa;'>esp8266thingies.nl</a>");
  str += F("</div></div></body></html>");
}

void setupSaved(String& str){
  str += F("<script>var countDownDate = new Date().getTime()+600000;var x = setInterval(function() {var now = new Date().getTime();var distance = countDownDate - now;var seconds = Math.floor((distance % (1000 * 60)) / 1000);document.getElementById(\"timer\").innerHTML = seconds + \" seconden tot module weer bereikbaar.\";if (seconds < 2) {location.replace(\"http://p1wifi.local\")}}, 1000);</script>");
  str += F("<fieldset>");
  str += F("<fieldset><legend><b>WLAN- und Modul-Setup</b></legend>");
   str += F("<p><b>Die Einstellungen wurden erfolgreich gespeichert</b><br><br>");
   str += F("<p>Das Modul wird jetzt neu gestartet. Dies dauert etwa eine Minute.<br>Die blaue LED leuchtet 2x auf, wenn das Modul den Bootvorgang beendet hat</p>");
   str += F("<p>Die LED blinkt langsam, während die Verbindung zu Ihrem WLAN-Netzwerk hergestellt wird.</p>");
   str += F("<p>Wenn die blaue LED an bleibt, ist die Einstellung fehlgeschlagen und Sie werden <br>");
   str += F("muss erneut mit dem WLAN-Netzwerk 'P1_Setup' verbunden werden .</p>");
   str += F("<br>");
  str += F("<p id=\"timer\"></p>");
  str += F("</fieldset></p>");
  str += F("<div style='text-align:right;font-size:11px;'><hr/><a href='http://eps8266thingies.nl' target='_blank' style='color:#aaa;'>eps8266thingies.nl</a></div></div></fieldset></body></html>");
}

void uploadDiag(String& str){
  addHead(str);
  addIntro(str);
  str += F("<fieldset><fieldset><legend><b>Update firmware</b></legend>");
  str += F("<form action='' method='post'><form method='POST' action='' enctype='multipart/form-data'><p>");
  str += F("<b>Firmware</b><input type='file' accept='.bin,.bin.gz' name='firmware'></p>");
  str += F("<button type='submit'>Update Firmware</button>");
  str += F("</form>");
  str += F("<form action='/' method='POST'><button class='button bhome'>Menu</button></form>");
  addFoot(str); 
}

void successResponse(String& str){
  addHead(str);
  addIntro(str);
  str += F("<fieldset>");
  str += F("<fieldset><legend><b>Firmware update</b></legend>");
  str += F("<p><b>Die Firmware wurde erfolgreich aktualisiert</b><br><br>");
   str += F("<p>Das Modul wird jetzt neu gestartet. Dies dauert etwa eine Minute.</p><br>");
   str += F("<p>Die blaue LED leuchtet 2x auf, wenn das Modul den Bootvorgang beendet hat blinkt die LED langsam, während die Verbindung zu Ihrem WLAN-Netzwerk hergestellt wird.</p>");
   str += F("<p>Wenn die blaue LED an bleibt, ist die Einstellung fehlgeschlagen und Sie werden müssen erneut mit dem WLAN-Netzwerk 'P1_Setup' mit dem Passwort 'configP1' verbunden werden.</p>");
  str += F("</fieldset></p>");
  str += F("<div style='text-align:right;font-size:11px;'><hr/><a href='http://eps8266thingies.nl' target='_blank' style='color:#aaa;'>eps8266thingies.nl</a></div></div></fieldset></body></html>");
}

void handleRoot(){
    debugln("handleRoot");
  String str = ""; 
    addHead(str);
    addIntro(str);

    str += F("<main class='form-signin'>");
    str += F("<form action='/P1' method='post'><button type='p1' class='button bhome'>Messwerte</button></form>");
    str += F("<form action='/Setup' method='post'><button type='Setup'>Setup</button></form>");
    str += F("<form action='/update' method='GET'><button type='submit'>Update firmware</button></form>");
  addUptime(str);
  addFoot(str);
  server.send(200, "text/html", str);
}

void handleSetup(){
    debugln("handleSetup");
    
 String str = ""; 
      debugln("handleSetupForm");

    addHead(str);
    addIntro(str);
      str += F("<fieldset>");
       str += F("<fieldset><legend><b>&nbsp;Wifi parameters&nbsp;</b></legend>");
       str += F("<form action='/SetupSave' method='POST'><p><b>SSId</b><br>");
       str += F("<input type='text' class='form-control' name='ssid' value='");
       str+= user_data.ssid;
       str+=  F("'></p>");
       str += F("<p><label><b>Password</b></label><br><input type='password' class='form-control' name='password' value='");
       str += user_data.password;
       str += F("'></p>");
      str += F("</fieldset>");
      str += F("<fieldset><legend><b>&nbsp;Domoticz parameters&nbsp;</b></legend>");
      
      str += F("<p><b>An Domoticz melden?</b><input type='checkbox' class='form-control' name='domo' id='domo' ");
      
       if (user_data.domo[0] =='j') str += F(" checked></p>"); else str += F("></p>");
       str += F("<p><b>Domoticz IP address</b><input type='text' class='form-control' name='domoticzIP' value='");
       str += user_data.domoticzIP;
       str += F("'></p><p>");
       str += F("<b>Domoticz Port</b><input type='text' class='form-control' name='domoticzPort' value='");
       str += user_data.domoticzPort;
       str += F("'></p><p>");
       str += F("<b>Domoticz Gas Idx</b><input type='text' class='form-control' name='domoticzGasIdx' value='");
       str += user_data.domoticzGasIdx;
       str += F("'></p><p>");
       str += F("<b>Domoticz Energy Idx</b><input type='text' class='form-control' name='domoticzEnergyIdx' value='");
       str += user_data.domoticzEnergyIdx;
       str += F("'></p>");
      str += F("</fieldset>");

       str += F("<fieldset><legend><b>&nbsp;MQTT parameters&nbsp;</b></legend>");
      str += F("<p><b>An MQTT broker melden?</b><input type='checkbox' class='form-control' name='mqtt' id='mqtt' ");
       if (user_data.mqtt[0] =='j') str += F(" checked></p>"); else str += F("></p>");
     str += F("<p><b>MQTT broker IP address</b><input type='text' class='form-control' name='mqttIP' value='");
       str += user_data.mqttIP;
       str += F("'></p><p>");
       str += F("<b>MQTT broker Port</b><input type='text' class='form-control' name='mqttPort' value='");
       str += user_data.mqttPort;
       str += F("'></p><p>");
       str += F("<b>MQTT user</b><input type='text' class='form-control' name='mqttUser' value='");
       str += user_data.mqttUser;
       str += F("'></p><p>");
       str += F("<b>MQTT password</b><input type='text' class='form-control' name='mqttPass' value='");
       str += user_data.mqttPass;
       str += F("'></p>");
       str += F("<b>MQTT root topic</b><input type='text' class='form-control' name='mqttTopic' value='");
       str += user_data.mqttTopic;
       str += F("'></p>");
       str += F("</fieldset>");
       str += F("<fieldset>");
      str += F("<b>Messintervall in Sek. (> 10 Sek.)</b><input type='text' class='form-control' name='interval' value='");
      str += user_data.interval; 
       str += F("'></p><p>");
       str += F("<p><b>Angabe in Watt (statt in kWh) </b><input type='checkbox' class='form-control' name='watt' id='watt' ");
       if (user_data.watt[0] =='j') str += F(" checked></p>"); else str += F("></p>");
      str += F("</fieldset><div></div>");
      str += F("<p><button type='submit'>Speichern</button></form>");
      str += F("<form action='/' method='POST'><button class='button bhome'>Menu</button></form></p>");
     addFoot(str);
    server.send(200, "text/html", str);
}


void handleSetupSave() {
  debugln("handleSetupSave");
  debug("Server args: ");
  debugln(server.args());
  debugln(server.arg("plain"));

  if (server.args() == 0) {
    debugln("lege submit, dus redirect naar handleRoot");
    handleRoot();
    debugln("En terug naar mainloop");
    return;
  }
  
  String str = ""; 
  if (server.method() == HTTP_POST) {

    if (server.arg("domo") == "on") user_data.domo[0] = 'j'; else user_data.domo[0] = 'n';
    strncpy(user_data.ssid,     server.arg("ssid").c_str(),     sizeof(user_data.ssid) );
    strncpy(user_data.password, server.arg("password").c_str(), sizeof(user_data.password) );
    strncpy(user_data.domoticzIP, server.arg("domoticzIP").c_str(), sizeof(user_data.domoticzIP) );
    strncpy(user_data.domoticzPort, server.arg("domoticzPort").c_str(), sizeof(user_data.domoticzPort) );
    strncpy(user_data.domoticzEnergyIdx, server.arg("domoticzEnergyIdx").c_str(), sizeof(user_data.domoticzEnergyIdx) );
    strncpy(user_data.domoticzGasIdx, server.arg("domoticzGasIdx").c_str(), sizeof(user_data.domoticzGasIdx) );

    if (server.arg("mqtt") == "on") user_data.mqtt[0] = 'j'; else user_data.mqtt[0] = 'n';
    strncpy(user_data.mqttIP, server.arg("mqttIP").c_str(), sizeof(user_data.mqttIP) );
    strncpy(user_data.mqttPort, server.arg("mqttPort").c_str(), sizeof(user_data.mqttPort) );
    strncpy(user_data.mqttUser, server.arg("mqttUser").c_str(), sizeof(user_data.mqttUser) );
    strncpy(user_data.mqttPass, server.arg("mqttPass").c_str(), sizeof(user_data.mqttPass) );
    strncpy(user_data.mqttTopic, server.arg("mqttTopic").c_str(), sizeof(user_data.mqttTopic) );
                
    strncpy(user_data.interval, server.arg("interval").c_str(), sizeof(user_data.interval) );

    if (server.arg("watt") == "on") user_data.watt[0] = 'j'; else user_data.watt[0] = 'n';
    

    user_data.ssid[server.arg("ssid").length()] = 
    user_data.password[server.arg("password").length()] = 
    user_data.domoticzIP[server.arg("domoticzIP").length()] = 
    user_data.domoticzPort[server.arg("domoticzPort").length()] = 
    user_data.domoticzGasIdx[server.arg("domoticzGasIdx").length()] = 
    user_data.domoticzEnergyIdx[server.arg("domoticzEnergyIdx").length()] = 
    user_data.interval[server.arg("interval").length()] =
    user_data.mqttIP[server.arg("mqttIP").length()] = 
    user_data.mqttPort[server.arg("mqttPort").length()] = 
    user_data.mqttUser[server.arg("mqttUser").length()] = 
    user_data.mqttPass[server.arg("mqttPass").length()] = 
    user_data.mqttTopic[server.arg("mqttTopic").length()] = 
    user_data.mqtt[1] = //server.arg("mqtt").length()] = 
    user_data.domo[1] = //server.arg("domo").length()] = 
    user_data.watt[1] = //server.arg("watt").length()] = 
    '\0';
    
    EEPROM.put(0, user_data);
    EEPROM.commit();

    addHead(str);
    addIntro(str);
    setupSaved(str);
    server.send(200, "text/html", str);    
    delay(500);
    ESP.reset();
  } 
}

void handleP1(){
  debugln("handleP1");
  String str = "";
  String eenheid, eenheid2, eenheid3;
  if (reportInDecimals) eenheid = " kWh'></div>"; else eenheid = " Wh'></div>";
 //if (reportInDecimals) eenheid = " kWh</div></div>"; else eenheid = " Wh</div></div>";
  if (reportInDecimals) eenheid2 = " kW'></div></p>"; else eenheid2 = " W'></div></p>";
  
  char str2[10];
  int temp;
    addHead(str);
    addIntro(str);
  //  str += ("<p>");
  //  str += P1timestamp;
   // str += P1timestamp[7];
   // str += P1timestamp[8];
   // str += P1timestamp[9];
 // str += ("</p>");
  str += F("<form ><fieldset><legend><b>Messwerte</b></legend>");
 // str += F("<form action='/' method='post'>");

  str += "<p><div class='row'><div class='column'><b>Verbrauch niedriger Satz: <br>total</b><input type='text' class='form-control c6' value='";
  str += electricityUsedTariff1;
  str += eenheid;
  str += "<div class='column' style='text-align:right'><br><b>Heute</b><input type='text' class='form-control c7' value='";
  str += atof(electricityUsedTariff1) - atof(dayStartUsedT1);
  str += eenheid;
  str += "</div></p>";

  str += "<p><div class='row'><div class='column'><b>Verbrauchshöchstsatz:<br> total</b><input type='text' class='form-control c6' value='";
  str += electricityUsedTariff2;
  str += eenheid;
  str += "<div class='column' style='text-align:right'><br><b>Heute</b><input type='text' class='form-control c7' value='";
  str += atof(electricityUsedTariff2) - atof(dayStartUsedT2);
  str += eenheid;
  str += "</div></p>";

  str += "<p><div class='row'><div class='column'><b>Niedrige Rücklaufquote: total</b><input type='text' class='form-control c6' value='";
  str += electricityReturnedTariff1;
  str += eenheid;
  str += "<div class='column' style='text-align:right'><b>Heute</b><input type='text' class='form-control c7' value='";
  str += atof(electricityReturnedTariff1) - atof(dayStartReturnedT1);
  str += eenheid;
  str += "</div></p>";
  
  str += "<p><div class='row'><div class='column'><b>Hohe Rücklaufquote:<br>insgesamt</b><input type='text' class='form-control c6' value='";
  str += electricityReturnedTariff2;
  str += eenheid;
  str += "<div class='column' style='text-align:right'><br><b>Heute</b><input type='text' class='form-control c7' value='";
  str += atof(electricityReturnedTariff2) - atof(dayStartReturnedT2);
  str += eenheid;
  str += "</div></p>";
  

  str += "<p><div class='row'><b>Aktuelle Nutzung</b><input type='text' class='form-control c6' value='";
  str += actualElectricityPowerDelivered;
  str += eenheid2;

  str += "<p><div class='row'><b>Aktuelle Einspeisung</b><input type='text' class='form-control c6' value='";
  str += actualElectricityPowerReturned;
  str += eenheid2;

  str += "<p><div class='row'><div class='column3'><b>Stromspannung:<br>L1</b><input type='text' class='form-control c6' value='";
  str += instantaneousVoltageL1;
  str += " V'></div>";
  str += "<div class='column3' style='text-align:right'><br><b>L2</b><input type='text' class='form-control c7' value='";
  str += instantaneousVoltageL2;
  str += " V'></div>";
  str += "<div class='column3' style='text-align:right'><br><b>L3</b><input type='text' class='form-control c7' value='";
   str += instantaneousVoltageL2;
    str += " V'></div></div></p>";

       str += "<p><div class='row'><div class='column3'><b>Stromstärke: <br>L1</b><input type='text' class='form-control c6' value='";
  str += instantaneousCurrentL1;
  str += " A'></div>";
  str += "<div class='column3' style='text-align:right'><br><b>L2</b><input type='text' class='form-control c7' value='";
  str += instantaneousCurrentL2;
  str += " A'></div>";
  str += "<div class='column3' style='text-align:right'><br><b>L3</b><input type='text' class='form-control c7' value='";
   str += instantaneousCurrentL3;
    str += " A'></div></div></p>";
/*

  str += F("<p><b>Voltage dips</b><input type='text' class='form-control' style='text-align:right' value='");
  str += numberVoltageSagsL1;
  str += F("'></p>");
  str += F("<p><b>Voltage pieken</b><input type='text' class='form-control' style='text-align:right' value='");
  str += numberVoltageSwellsL1;
  str += F("'></p>");
  str += F("<p><b>Stroomonderbrekingen</b><input type='text' class='form-control' style='text-align:right' value='");
  str += numberPowerFailuresAny;
  str += F("'></p>");
  */
  str += "<p><div class='row'><div class='column'><b>Gasverbrauch: totaal</b><input type='text' class='form-control c6' value='";
  str += gasReceived5min;
  str += F(" m3'></div>");
  str += "<div class='column' style='text-align:right'><b>Heute</b><input type='text' class='form-control c7' value='";
  str += atof(gasReceived5min) - atof(dayStartGaz);
  str += " m3'></div></div></p>";
  str += F("</fieldset></form>");
  str += F("<form action='/' method='POST'><button class='button bhome'>Menu</button></form>");
  addUptime(str);
  addFoot(str);
  server.send(200, "text/html", str);
}

void handleRawData(){
  String str;
  debugln("Raw data requested. Here you go.");
  str= "<html><head></head><body>";
  str += datagram;
  str += "</body></html>";
    if (dataEnd){   
    server.send(200, "text/html", str);
    debugln(datagram);
  }
}

void handleHelp(){
    char ipstr[20];
  IPAddress ip = WiFi.localIP();
  sprintf_P(ipstr, PSTR("%u.%u.%u.%u"), ip[0], ip[1], ip[2], ip[3]);
  
 String str;
 addHead(str);
  addIntro(str);
  str += F("<fieldset>");
  str += F("<fieldset><legend><b>Hilfe</b></legend>");
  str += F("<p><b>Das P1 WiFi-Gateway kann Daten auf 4 Arten liefern.</b><br><br>");
  str += F("<p>Die Kernwerte sind immer über das P1-Menü verfügbar (entweder über");
  str += ipstr;
  str += F("/P1, oder ¨ber p1wifi.local/P1 )</p><br>");
  
  str += F("<p>Die Rohdaten sind über ");
  str += ipstr;
  str += F("/Data, oder über p1wifi.local/Data verfügbar</p><br><br>");
  str += F("<p>Häufiger ist die Verwendung des Gateways mit einem Hausautomationssystem.</p>");
  str += F("<p><b>Domoticz</b> akzeptiert json-Nachrichten. Geben Sie dazu die IP-Adresse von Ihren Domoticz-Server und die Portnummer, über die darauf zugegriffen werden kann ein (normalerweise 8080).</p>");
  str += F("Die notwendigen Idx für Gas und Strom erhalten Sie, indem Sie zunächst virtuelle Sensoren in Domoticz für beide erstellen.");
  str += F("Nach der Erstellung erscheinen die Idxes auf der Registerkarte 'Geräte'.</p><br>");
  str +=F("");
  str +=F("Über Port 23 des Moduls (p1wifi.local:23) können Daten auch gelesen werden (z.B. von Domoticz).");
  str +=F("Auf diese Weise kann die Domoticz mittels Hardware einstellung [P1 Smart Meter mit LAN-Schnittstelle] Daten abrufen.");
  str +=F("Sie brauchen am p1wifi seite nichts einzustellen (Nr Häkchen bei json und mqtt).");

  str += F("Für andere Systeme können Sie einen MQTT-Broker verwenden. Füllen Sie die Details der Broker aus und legen Sie das Stammthema fest. Für Home Assistant ist dies „sensors/power/p1meter“.</p>");
  str += F("Daniel de Jong beschreibt auf seinem <a href='https://github.com/daniel-jong/esp8266_p1meter'>github</a> wie man HA weiter konfiguriert.</p>");
  str += F("Verwenden Sie die Kontrollkästchen, um anzugeben, welche Berichtsmethode(n) Sie verwenden möchten.</p>");

  str += F("</fieldset></p>");
  str += F("<div style='text-align:right;font-size:11px;'><hr/><a href='http://eps8266thingies.nl' target='_blank' style='color:#aaa;'>eps8266thingies.nl</a></div></div></fieldset></body></html>");
server.send(200, "text/html", str);
}
#endif
