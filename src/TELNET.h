/*
 * Copyright (c) 2022 Ronald Leenes
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * @file TELNET.ino
 * @author Ronald Leenes
 * @date 28.12.2022
 *
 * @brief This file contains the TELNET interface functions
 *
 * @see http://esp8266thingies.nl
 */

void setupTelnet() {
  telnet.setNoDelay(true);
  telnet.begin();
}

void telnetloop() {
  int i;
  Log.verboseln("telnetloop()");
  if (telnet.hasClient()) {
    Log.verboseln("telnet.hasClient()");
    // find free/disconnected spot
    for (i = 0; i < MAX_SRV_CLIENTS; i++)
      if (!telnetClients[i]) { // equivalent to !serverClients[i].connected()
        telnetClients[i] = telnet.available(); // used to be telnet.accept() which was not found
        Log.verbose("New client: index ");
        Log.verbose("%d\n", i);
        telnetConnected = true;
        break;
      } else
        telnetConnected = false;
    // no free/disconnected spot so reject
    if (i == MAX_SRV_CLIENTS) {
      telnet.available().println("busy"); // used to be telnet.accept() which was not found
      // hints: server.accept() is a WiFiClient with short-term scope
      // when out of scope, a WiFiClient will
      // - flush() - all data will be sent
      // - stop() - automatically too
      Log.verbose("server is busy with %d active connections\n", MAX_SRV_CLIENTS);
      // something is wrong. We should not reach MAX_SRV_CLIENTS
      // reboot to clean up things.
      ESP.restart();
    }
  }
  for (i = 0; i < MAX_SRV_CLIENTS; i++) {
    while (telnetClients[i].available() &&
           telnetClients[i].availableForWrite() > 0) {
      char buf[50];
      telnetClients[i].read(buf, 100);
      if (strncmp(buf, "debug", 5) == 0) {
        telnetDebugClient = i; // we have found a debug client
        Log.verboseln("debug port request");
        telnetDebugConnected = true;
        telnetClients[i].flush();
      }
      if (strncmp(buf, "quit", 4) ==
          0) { // we have found a debug client dropping debug connection
        Log.verboseln("debug port request dropped");
        telnetDebugConnected = false;
        telnetClients[i].flush();
        telnetClients[i].stop();
      }
    }
  }
}

bool telnetStillRunning() {
  // test for running server
  if (telnetConnected &&
      !telnet.hasClient()) { // module thinks it has connections, but actually
                             // doesn't have one
    telnet.stop();
    setupTelnet();
    return false;
  } else
    return true;
}

void TelnetReporter() {
  Log.verboseln("TelnetReporter()");
  activetelnets = 0;
  int maxToTcp = 0;
  int i;
  for (i = 0; i < MAX_SRV_CLIENTS; i++)
    if (telnetClients[i]) {
      int afw = telnetClients[i].availableForWrite();
      if (afw) {
        if (!maxToTcp) {
          maxToTcp = afw;
        } else {
          maxToTcp = std::min(maxToTcp, afw);
        }
        Log.verbose("[]client ");
        Log.verbose("%d", i);
        Log.verbose(" maxtcp ");
        Log.verbose("%d", maxToTcp);
        activetelnets++;
      } else {
        // warn but ignore congested clients
        Log.verboseln("one client is congested");
        telnetClients[i].flush();
        telnetClients[i].stop();
      }
    }
  unsigned int len = datagram.length();
  for (i = 0; i < MAX_SRV_CLIENTS; i++) {
    Log.verbose(">>>>> Client ");
    Log.verbose("%d", i);
    if (telnetClients[i].availableForWrite() >= 1) {
      Log.verboseln(" is available for writing");
      size_t tcp_sent = telnetClients[i].write(datagram.c_str(), len);
      if (tcp_sent != len) {
        statusMsg = String("len mismatch for : " + i);
      }
      Log.verbose("Raw Data Published to Telnet connection: ");
      Log.verbose("%d\n", i);
      LastTReport = timestampkaal();
    } else
      Log.verboseln(" is not available for writing");
  }
  yield();
}
