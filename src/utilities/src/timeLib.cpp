/**
 * @file timeLib.cpp
 * @author Alessandro Benetton (aleben98@gmail.com)
 * @author Crespan Lorenzo (lorenzo.crespan@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-05-06
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "../timeLib.hpp"
#include "../../connections/WiFi.hpp"
#include "../loggerLib.hpp"

#define N_ATTEMPTS 10

Timezone Rome;
char tstamp[20];

/**
 * @brief Inizializzazione del fuso orario e set default Rome.
 *
 */
void setupTime() {
	Rome.setLocation(F("Europe/Rome"));
	Rome.setDefault();
}

/**
 * @brief Ritorno del Timestamp.
 *
 * @return char *\n
 * d-m-y H:i:s T -> Ora Sincronizzata\n
 * ~N~Sd-m-y H:i:s T~N~S -> Ora Non Sincronizzata\n
 * TIME ND -> Ora Non Presente
 */
char* getTimestamp() {
	char* ts = tstamp;
	if (timeStatus() == timeSet) {
		// Se l'ora è sincronizzata
		sprintf(ts, "%s", dateTime("y-m-d~TH:i:s").c_str());  // 17-05-21 11:12:55 CEST
	} else if (timeStatus() == timeNeedsSync) {
		// Se l'ora è da sincronizzare
		sprintf(ts, "%s", dateTime("~Ey-m-d~TH:i:s~E").c_str());  //~N~S17-05-21 11:12:55 CEST~N~S
	} else
		// Se l'ora non è MAI stata sincronizzata
		sprintf(ts, "TIME ND");

	return ts;
}

bool checkTimeServer(){
	if (timeStatus() != timeSet) {
		logWarning("EZTIME", "Orologio fuori sync");
		syncTimeServer();
	}
	return timeStatus() == timeSet;
}

/**
 * @brief Sincronizzo l'ora con il server NTP
 *
 */
void syncTimeServer() {
	if (wifiStatus(false)) {
		// Sync server eztime
		logDebugf("EZTIME", "Sincronizzazione TS, Stato sincronizzazione: %s\n", String(timeStatus()));

		waitForSync(60);

		if (timeStatus() != timeSet) {  // La sync non ha funzionato
			logWarningf("EZTIME", "Errore nella sincronizzazione del TS EZTIME, Stato sincronizzazione: %s\n", String(timeStatus()));
			ESP.restart();
		}
		logDebugf("EZTIME", "Sincronizzazione TS completata, Stato sincronizzazione: %s", String(timeStatus()));
	}
}