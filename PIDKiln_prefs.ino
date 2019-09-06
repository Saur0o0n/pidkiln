/*
** Preferences stuff
**
*/



// Read prefs from file
//
void Load_prefs(){
File prf;
String line,item,value;
int pos=0;

  DBG Serial.println(" Loading prefs from file");
  if(prf=SPIFFS.open(PREFS_FILE,"r"))
    while(prf.available()){
      line=prf.readStringUntil('\n');
      line.trim();
      if(!line.length()) continue;         // empty line - skip it
      if(line.startsWith("#")) continue;   // skip every comment line
      if(pos=line.indexOf("#")){
        line=line.substring(0,pos);
        line.trim(); // trim again after removing comment
      }
      // Now we have a clean line - parse it
      if((pos=line.indexOf("="))>2){          // we need to have = sign - if not, ignore
        item=line.substring(0,pos-1);
        item.trim();
        value=line.substring(pos+1);
        value.trim();
        //DBG Serial.printf("Preference (=@%d) item: '%s' = '%s'\n",pos,item.c_str(),value.c_str());
        
        if(item.length()>2 && value.length()>0){
          for(uint16_t a=0; a<PRF_end; a++){
            if(item.equalsIgnoreCase(String(PrefsName[a]))){  // we have found na maching prefs value
              if(Prefs[a].type==STRING){
                if(Prefs[a].value.str) free(Prefs[a].value.str);
                Prefs[a].value.str=strdup(value.c_str());
                DBG Serial.printf("  -> saved STRING item value:%s type:%d\n",Prefs[a].value.str,(int)Prefs[a].type);
              }else if(Prefs[a].type==UINT8){
                Prefs[a].value.uint8=(uint8_t)value.toInt();
                DBG Serial.printf("  -> saved UINT8 item value:%d type:%d\n",Prefs[a].value.uint8,(int)Prefs[a].type);
              }else if(Prefs[a].type==UINT16){
                Prefs[a].value.uint16=(uint16_t)value.toInt();
                DBG Serial.printf("  -> saved UINT16 item value:%d type:%d\n",Prefs[a].value.uint16,(int)Prefs[a].type);
              }
            }
          }
        }
      }
    }

    // For debuging only
    DBG Serial.println("-=-=-= PREFS DISPLAY =-=-=-");
    for(uint16_t a=0; a<PRF_end; a++){
      if(Prefs[a].type==STRING) DBG Serial.printf(" %d) '%s' = '%s'\t%d\n",a,PrefsName[a],Prefs[a].value.str,(int)Prefs[a].type);
      if(Prefs[a].type==UINT8 || Prefs[a].type==UINT16) DBG Serial.printf(" %d) '%s' = '%d'\t%d\n",a,PrefsName[a],Prefs[a].value.uint8,(int)Prefs[a].type);
    }
}

/* 
** Setup preferences for PIDKiln
**
*/
void setup_prefs(void){
char tmp[30];

  // Fill the preferences with default values - if there is such a need
  DBG Serial.println("Preference initizalization");
  for(uint16_t a=1; a<PRF_end; a++)
    switch(a){
      case PRF_WIFI_SSID:
        Prefs[PRF_WIFI_SSID].type=STRING;
        Prefs[PRF_WIFI_SSID].value.str=strdup("");
        break;
      case PRF_WIFI_PASS:
        Prefs[PRF_WIFI_PASS].type=STRING;
        Prefs[PRF_WIFI_PASS].value.str=strdup("");
        break;
      case PRF_WIFI_MODE:
        Prefs[PRF_WIFI_RETRY_CNT].type=UINT8;
        Prefs[PRF_WIFI_RETRY_CNT].value.uint8=1;
        break;
      case PRF_WIFI_RETRY_CNT:
        Prefs[PRF_WIFI_RETRY_CNT].type=UINT8;
        Prefs[PRF_WIFI_RETRY_CNT].value.uint8=5;
        break;

      case PRF_NTPSERVER1:
        Prefs[PRF_NTPSERVER1].type=STRING;
        Prefs[PRF_NTPSERVER1].value.str=strdup("pool.ntp.org");
        break;
      case PRF_NTPSERVER2:
        Prefs[PRF_NTPSERVER2].type=STRING;
        Prefs[PRF_NTPSERVER2].value.str=strdup("");
        break;
      case PRF_NTPSERVER3:
        Prefs[PRF_NTPSERVER3].type=STRING;
        Prefs[PRF_NTPSERVER3].value.str=strdup("");
        break;
      case PRF_GMT_OFFSET:
        Prefs[PRF_GMT_OFFSET].type=UINT16;
        Prefs[PRF_GMT_OFFSET].value.uint16=0;
        break;
      case PRF_DAYLIGHT_OFFSET:
        Prefs[PRF_DAYLIGHT_OFFSET].type=UINT16;
        Prefs[PRF_DAYLIGHT_OFFSET].value.uint16=0;
        break;
      default:
        break;
    }
  Load_prefs();
}