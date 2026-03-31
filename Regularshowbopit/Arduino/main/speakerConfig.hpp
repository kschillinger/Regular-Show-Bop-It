#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

#define numberJokes 5
#define numberStates 6
#define folderActions 1
#define folderJokes 2

//creating enums fore the speaker outputs, note that file structure 
//is as follows: 
/*
/MP3
    /1 -> Actions
        001.mp3
        002.mp3
        003.mp3
        004.mp3
        005.mp3
        006.mp3
    /2 -> Jokes
        001.mp3
        002.mp3
        003.mp3
        004.mp3
        005.mp3
        ....mp3
*/

//SoftwareSerial mp3Serial(0,1); //rx, tx for ATMEGA
DFRobotDFPlayerMini mp3;

enum state : uint8_t
{
     mash,
     shake,
     hide,
     joke,
     prestart,
     win,
     lose
};

enum soundID_Actions : uint8_t 
{
    ID_MASH  = 001,
    ID_SHAKE = 002, 
    ID_HIDE  = 003,
    ID_JOKE = 004,
    ID_EGGCELLENT = 005,
    ID_YOURFIRED = 006,
    ID_COUNTDOWN = 007
};

enum soundID_Jokes : uint8_t
{
    ID_NULL = 000,
    ID_JOKE1 = 001,
    ID_JOKE2 = 002,
    ID_JOKE3 = 003,
    ID_JOKE4 = 004,
    ID_JOKE5 = 005
};

void speakerInit()
{
    Serial.begin(9600);
    delay(1500);
    if (!mp3.begin(Serial,true,false)) {  //Use softwareSerial to communicate with mp3.
        Serial.println(F("Unable to begin:"));
        Serial.println(F("1.Please recheck the connection!"));
        Serial.println(F("2.Please insert the SD card!"));
        while(true);
    }
    Serial.println(F("DFPlayer Mini online."));
    
    
    mp3.setTimeOut(500); //Set serial communictaion time out 500ms

    mp3.volume(20);  //Set volume value (0~30).
    mp3.EQ(DFPLAYER_EQ_NORMAL);
    mp3.outputDevice(DFPLAYER_DEVICE_SD);
}

int random_joke()
{
    return (rand() % numberJokes) + 1; //returns a random joke ID between 1 and numberJokes
}

soundID_Jokes randomJokeID()
{
    switch (random_joke())
    {
        case 1:
            return ID_JOKE1;
            break;
        case 2:
            return ID_JOKE2;
            break;
        case 3:
            return ID_JOKE3;
            break;
        case 4:
            return ID_JOKE4;
            break;
        case 5:
            return ID_JOKE5;
            break;
        default:
            return ID_NULL; //should never happen
    }
}

void playAndWait(uint8_t folderID, uint8_t state)
{
    //function that handles the playing of the audio and waiting for it to finish before execution is continued, takes in the folder ID and state ID to determine which audio to play
    mp3.playFolder(folderID,state);
     unsigned start = millis();         
    while(true)
    {
           
            if(mp3.available() && mp3.readType() == DFPlayerPlayFinished)
            {
                break;
            }
            unsigned long now = millis();
            if (now - start > 6000) 
                break;
    }
}

//function that handles the sound that is to be played depending on the state
void generateSound(uint8_t state)
{
    uint8_t jokeID = randomJokeID(); //get a random joke ID to be used if the state is joke
    switch (state)
    {
        case (mash) :
            playAndWait(folderActions, ID_MASH);
            break;
        case (shake) :
            playAndWait(folderActions, ID_SHAKE);
            break;
        case (hide) :
            playAndWait(folderActions, ID_HIDE);
            break;
        case (joke) :
            playAndWait(folderJokes, jokeID);
            break;
        case (win) :
            playAndWait(folderActions, ID_EGGCELLENT);
            break;
        case (lose) :
            playAndWait(folderActions, ID_YOURFIRED);
            break;
        case (prestart) :
            playAndWait(folderActions, ID_COUNTDOWN);
            break;
    }
}
