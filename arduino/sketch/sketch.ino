#include "settings.h"
#include "mp3player.h"
#include "ESP8266TrueRandom.h" // https://github.com/marvinroger/ESP8266TrueRandom
#include "CircularBuffer.h" // https://github.com/rlogiacco/CircularBuffer

CircularBuffer<int,3> lastPlayedFiles;

/**
 * Setup le great speaking present.
 */
void setup() {
  Serial.begin(115200);
  delay(300);
  splashScreen();

  //Init player 
  Serial.println("Setting up player...");
  setupPlayer();
  Serial.println("DONE.");
  delay(200);
}

/**
 * Looping Louie
 */
void loop() {
  if(millis() > msCount + 1000) {
    msCount = millis();
    handlePlayer();
  }
  mp3.loop();
}

/**
 * Play random file on random time
 */
void handlePlayer() {
  doPlay = random(1, 100);
  if(doPlay <= freq && !playing && coolingDownSecs == 0) {
    fileCount = mp3.getTotalTrackCount(DfMp3_PlaySource_Sd);
    randomize = true;
    while(randomize) { // to avoid playing same file from before again
      randInt = random(1, fileCount + 1);
      if(!isPlayedFile(randInt)) {
        lastPlayedFiles.unshift(randInt);
        randomize = false;
      }
    }

    Serial.print("Track ");
    Serial.println(randInt);
    playing = true;
    mp3.playMp3FolderTrack(randInt); 
    coolingDownSecs = cooldownSecs;
  }
  if (coolingDownSecs > 0) coolingDownSecs--;
}

/**
 * Check if file was played before by walking through buffer
 */
bool isPlayedFile(int fileNumber) {
  for (int i = 0; i < lastPlayedFiles.size(); i++) {
    if(lastPlayedFiles[i] == fileNumber) {
      return true;
    }
  }

  return false;
}

/**
 * Dump some information on startup.
 */
void splashScreen() {
  for (int i=0; i<=5; i++) Serial.println();
  Serial.println("#######################################");
  Serial.print("# Sprechgeschenk - CLient - v. ");
  Serial.println(clientVer);
  Serial.println("# -----------");
  Serial.println("# Chris Figge (flazer)");
  Serial.println("# Mail: info@flazer.net");
  Serial.println("#######################################");
}
