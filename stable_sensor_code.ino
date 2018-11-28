                                       //this is proper working code written by poorenedra
                                              
                                              #include <SD.h>               // for Sd card communication
                                              //#include <OneWire.h>
                                              //#include <DallasTemperature.h>
                                              //#include <MemoryFree.h>
                                              
                                              // state = sensor ON/OFF
                                              int stateTemp = 1, stateLdr = 1, stateGas = 1, stateFire = 1, stateMotion = 1;
                                              // sensitivity = difference in analog value
                                              int sensitivityTemp = 5, sensitivityLdr = 5, sensitivityGas = 5, sensitivityFire = 5, sensitivityMotion = 5;
                                              // interval = sensor reading after milisecond
                                              unsigned long currentMillis=0,intervalTemp = 10000, intervalLdr = 10000, intervalGas = 10000, intervalFire = 10000, intervalMotion = 10000;
                                              unsigned long waitCounter=0;
                                              unsigned long waitCounterTemp=0, waitCounterLdr=0, waitCounterGas=0 ,waitCounterFire=0;
                                              int prevValTemp=0, prevValLdr=0, prevValGas=0, prevValFire=0;
                                                    
                                              // analog/Digital pins for temperature sensor
                                              int pinGas = 0, pinFire = 1, pinTemp = 2, pinLdr=3, pinMotion=6, pinLed=9;
                                              int pirState = 0;             // start, assuming no motion detected
                                              int currentMt=0;
                                              void initializeSDCard(){
                                              //  Serial.println(F("Initializing SD Card..."));
                                                if (!SD.begin(4)) {
                                              //    Serial.println(F("initialization failed!"));
                                                  return;
                                                }
                                              //  Serial.println(F("initialization of SD Card Done."));
                                              }
                                              
                                              void retriveSensitivityVal(String sensorName, String remoteId){
                                                File myFile;
                                                if (!SD.exists(sensorName + "/" + remoteId)) {
                                                  Serial.println("file does Not Exist: " + (sensorName + "/" + remoteId));
                                                  return;
                                                }
                                              
                                              // stateString + "/" + sensitivityString + "/" + intervalString
                                                myFile = SD.open(sensorName + "/" + remoteId, FILE_READ);
                                                if(sensorName == "TM"){
                                                  stateTemp = (myFile.readStringUntil(',')).toInt();
                                                  sensitivityTemp = (myFile.readStringUntil(',')).toInt();
                                                  intervalTemp = (myFile.readStringUntil(',')).toInt();
                                              //    Serial.print("values saved: " + (sensorName + "/" + remoteId));
                                              //    Serial.println("values saved: " + (String(stateTemp) + "/" + String(sensitivityTemp) + "/" + String(intervalTemp))); 
                                                }else if(sensorName == "LD"){
                                                  stateLdr = (myFile.readStringUntil(',')).toInt();
                                                  sensitivityLdr =(myFile.readStringUntil(',')).toInt();
                                                  intervalLdr = (myFile.readStringUntil(',')).toInt(); 
                                              //    Serial.print("values saved: " + (sensorName + "/" + remoteId));
                                              //    Serial.println("values saved: " + (String(stateLdr) + "/" + String(sensitivityLdr) + "/" + String(intervalLdr)));
                                                }else if(sensorName == "GS"){
                                                  stateGas = (myFile.readStringUntil(',')).toInt();
                                                  sensitivityGas =(myFile.readStringUntil(',')).toInt();
                                                  intervalGas = (myFile.readStringUntil(',')).toInt();
                                              //    Serial.print("values saved: " + (sensorName + "/" + remoteId));
                                              //    Serial.println("values saved: " + (String(stateGas) + "/" + String(sensitivityGas) + "/" + String(intervalGas)));
                                                }else if(sensorName == "SM"){
                                                  stateFire = (myFile.readStringUntil(',')).toInt();
                                                  sensitivityFire =(myFile.readStringUntil(',')).toInt();
                                                  intervalFire = (myFile.readStringUntil(',')).toInt();
                                              //    Serial.print("values saved: " + (sensorName + "/" + remoteId));
                                              //    Serial.println("values saved: " + (String(stateFire) + "/" + String(sensitivityFire) + "/" + String(intervalFire)));
                                                }else if(sensorName == "MT"){
                                                  stateMotion = (myFile.readStringUntil(',')).toInt();
                                                  sensitivityMotion =(myFile.readStringUntil(',')).toInt();
                                                  intervalMotion = (myFile.readStringUntil(',')).toInt();
                                              //    Serial.print("values saved: " + (sensorName + "/" + remoteId));
                                              //    Serial.println("values saved: " + (String(stateFire) + "/" + String(sensitivityFire) + "/" + String(intervalFire)));
                                                }
                                              //  myFile.flush();
                                                myFile.close();
                                              }
                                              
                                              void saveSensitivity(String sensorName, String remoteId, String stateString, String sensitivityString, String intervalString){
                                                File myFile;
                                                SD.mkdir(sensorName);
                                              
                                              //Remove already paired file and create new
                                                if (SD.exists(sensorName + "/" + remoteId)) {
                                                  SD.remove(sensorName + "/" + remoteId);
                                              //    Serial.println("Removed file : " + (sensorName + "/" + remoteId));
                                                }
                                              
                                                myFile = SD.open(sensorName + "/" + remoteId, FILE_WRITE);
                                                if (!SD.exists(sensorName + "/" + remoteId)) {
                                                  Serial.println("Unable to create file " + (sensorName + "/" + remoteId));
                                                  return;
                                                }
                                                myFile.print(stateString);
                                                myFile.print(F(","));
                                                myFile.print(sensitivityString);
                                                myFile.print(F(","));
                                                myFile.print(intervalString);
                                                myFile.print(F(","));
                                                myFile.flush();
                                                myFile.close();
                                              //  Serial.println("values saved: " + (sensorName + "/" + remoteId));
                                              //  Serial.println("values saved: " + (stateString + "/" + sensitivityString + "/" + intervalString));
                                              }
                                              
                                              void setup() {
                                                // put your setup code here, to run once:
                                                Serial.begin(9600);
                                                //  Serial.println(F("Initializing Code."));
                                                initializeSDCard();
                                                pinMode(pinLed, OUTPUT);
                                                digitalWrite(pinLed, HIGH);
                                                retriveSensitivityVal("GS", "1");
                                                retriveSensitivityVal("SM", "2");
                                                retriveSensitivityVal("TM", "3");
                                                retriveSensitivityVal("MT", "4");
                                                retriveSensitivityVal("LD", "5");
                                                digitalWrite(pinLed, LOW);
                                              }
                                              
                                              void loop() {
                                               
                                                digitalWrite(pinLed, HIGH);
                                                // put your main code here, to run repeatedly:
                                              //  Serial.println("start : "+String(freeMemory()));
                                                while(Serial.available()){
                                                  String rawInput;
                                                  //IR.10.12.9.P|IR.8.14.7.P|GS.1.ACT.<state>.<sensitivity>.<time_interval>
                                                  rawInput = Serial.readStringUntil('|');   //IR.10.12.9.Pd
                                              //    Serial.println("rawInput: " + rawInput);
                                                  if(rawInput != ""){
                                              //      Serial.println("start frame : "+String(freeMemory()));
                                                    String moduleName = "", remoteId = "", action = "", stateString = "" , sensitivityString = "" , intervalString = ""; 
                                                    char* command;
                                                    char* commandPart;
                                                    command = strdup(rawInput.c_str());
                                                    commandPart = strtok(command, ".");
                                                    moduleName = String(commandPart);
                                              //      Serial.println("module Name : " + moduleName);
                                                    
                                                    int partCounter = 0;
                                                    if(moduleName == "TM" || moduleName == "GS" || moduleName == "LD" || moduleName == "SM" || moduleName == "MT")
                                                    {
                                                      while( commandPart != NULL ) 
                                                      {
                                                        partCounter++;
                                                        commandPart = strtok(NULL, ".");
                                              //          Serial.println("commandPart ("+String(partCounter)+") : " +  commandPart );
                                                        switch(partCounter){
                                                          case 1: 
                                              //                  Serial.println("commandPart ("+String(partCounter)+") : " +  commandPart );
                                                                remoteId = String(commandPart);
                                                                break;
                                                          case 3:
                                              //                  Serial.println("commandPart ("+String(partCounter)+") : " +  commandPart );
                                                                stateString = String(commandPart);
                                                                break;
                                                          case 4:
                                              //                  Serial.println("commandPart ("+String(partCounter)+") : " +  commandPart );
                                                                sensitivityString = String(commandPart);
                                                                break;
                                                          case 5:
                                              //                  Serial.println("commandPart ("+String(partCounter)+") : " +  commandPart );
                                                                intervalString = String(commandPart);
                                                                break;
                                                          default:
                                                                break;
                                                        }
                                                      }
                                                     saveSensitivity(moduleName, remoteId, stateString, sensitivityString, intervalString);
                                                     retriveSensitivityVal(moduleName, remoteId);
                                              //       GS.<device_number>.ACTACK.<status>.<value>.<sensitivity>.<time_interval>
                                                     int currentVal;
                                                     if(moduleName == "TM"){
                                                      currentVal=temperatureSensor();
                                                     }else if(moduleName == "LD"){
                                                      currentVal=ldrSensor();;
                                                     }else if(moduleName == "GS"){
                                                      currentVal=gasSensor();;
                                                     }else if(moduleName == "SM"){
                                                      currentVal=fireSensor();;
                                                     }else if(moduleName == "MT"){
                                                      currentVal=motionSensor(1);
                                                     }else{
                                                      Serial.println("False Frame");
                                                     }
                                                     Serial.println(moduleName + "." + remoteId + ".ACTACK." + stateString + "."  + String(currentVal) + "." + sensitivityString + "." + intervalString + "|");
                                                    }      
                                                    rawInput = "";
                                                    free(command);
                                                    free(commandPart);
                                              //      Serial.println("End Frame: "+String(freeMemory()));
                                                    delay(200);
                                                  }
                                                }
                                              sensitivityTemp = 5, sensitivityLdr = 100, sensitivityGas = 10, sensitivityFire = 10, sensitivityMotion = 5; 
                                                currentMillis = millis();
                                                if(currentMillis - waitCounterGas  >= intervalGas && stateGas==1) {
                                                  int currentGas=gasSensor();
                                                  int diffGas = abs(currentGas - prevValGas);
                                                  if(diffGas > sensitivityGas || diffGas <-sensitivityGas){
                                                    prevValGas = currentGas;
                                                    Serial.println("GS.1.READ." + String(currentGas)+ "|");
                                                    }        
                                                  waitCounter = currentMillis;
                                                }
                                                if(currentMillis - waitCounterFire  >= intervalFire && stateFire==1) {
                                                  int currentFire=fireSensor();
                                             //     Serial.println("currentFire: "+String(currentFire));
                                                  int diffFire = currentFire - prevValFire;
                                               //   Serial.println("diffFire: "+String(diffFire));
                                              //    Serial.println("sensitivityFire: "+String(sensitivityFire));
                                                  if(diffFire > sensitivityFire || diffFire <-sensitivityFire){
                                                    prevValFire = currentFire;
                                              //      Serial.println("prevValFire: "+String(prevValFire));
                                                    Serial.println("SM.2.READ." + String(currentFire)+ "|");
                                                    }    
                                                  waitCounterFire = currentMillis;   
                                                }
                                                if(currentMillis - waitCounterTemp  >= intervalTemp && stateTemp==1) {
                                                  int currentTemp=temperatureSensor();
                                                  int diffTemp = currentTemp - prevValTemp;
                                                  if(diffTemp > sensitivityTemp || diffTemp <-sensitivityTemp){
                                                    prevValTemp = currentTemp;
                                                    Serial.println("TM.3.READ." + String(currentTemp)+ "|");
                                                    }
                                                  waitCounterTemp = currentMillis;   
                                                }
                                                if(currentMillis - waitCounterLdr  >= intervalLdr && stateLdr==1) {
                                                  int currentLdr=ldrSensor();
                                                  int diffLdr = currentLdr - prevValLdr;
                                                  if(diffLdr > sensitivityLdr || diffLdr <-sensitivityLdr){
                                                    prevValLdr = currentLdr;
                                                    Serial.println("LD.5.READ." + String(currentLdr)+ "|");
                                                    }
                                                 waitCounterLdr = currentMillis;
                                                }
                                                if(stateMotion==1)
                                                currentMt=motionSensor(0); 
                                              //  if(currentMt==1 && pirState==1){
                                              //    Serial.println("MT.4.READ.1|");
                                              //  }else if(currentMt==0 && pirState==0){
                                              //    Serial.println("MT.4.READ.0|");
                                              //  }
                                              //  Serial.println("End : "+String(freeMemory()));  
                                              digitalWrite(pinLed, LOW);
                                              }
                                              
                                              
                                              int temperatureSensor(){
                                                int currentTemp=0;
                                                for(int i=0; i<5; i++)
                                                  currentTemp += analogRead(pinTemp);
                                                currentTemp = (currentTemp / 5);
                                                return currentTemp;
                                              }
                                              
                                              int ldrSensor(){
                                                int currentLdr=0;
                                                for(int i=0; i<5; i++)
                                                  currentLdr += analogRead(pinLdr);
                                                currentLdr = (currentLdr / 5);
                                                return currentLdr;
                                              }
                                              
                                              int gasSensor(){
                                                int currentGas=0;
                                                for(int i=0; i<5; i++)
                                                  currentGas += analogRead(pinGas);
                                                currentGas = (currentGas / 5);
                                                return currentGas;
                                              }
                                              
                                              int fireSensor(){
                                                int currentFire=0;
                                                for(int i=0; i<5; i++)
                                                  currentFire += analogRead(pinFire);
                                                currentFire = (currentFire / 5);
                                                return currentFire;
                                              }
                                              
                                              
                                              int motionSensor(int frameORvalue){
                                                int currentMotion = digitalRead(pinMotion);  // read input value
                                                if (currentMotion == HIGH && pirState==0) {
                                                  pirState = 1;
                                                  if(frameORvalue==0)
                                                    Serial.println("MT.4.READ.1|");
                                                  return 1;
                                                }else if (currentMotion == LOW && pirState==1){
                                                  pirState = 0;
                                                  if(frameORvalue==0)
                                                    Serial.println("MT.4.READ.0|");
                                                  return 0;
                                                }
                                                return 0;
                                              }

