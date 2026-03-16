#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

#define numberJokes 5
#define numberStates 6

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

SoftwareSerial mp3Serial(10,11); //rx, tx for ATMEGA
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
    ID_LOSE = 006
};

enum soundID_Jokes : uint8_t
{
    ID_JOKE1 = 001,
    ID_JOKE2 = 002,
    ID_JOKE3 = 003,
    ID_JOKE4 = 004,
    ID_JOKE5 = 005
};

void speakerInit()
{
    mp3Serial.begin(9600);
    if (!mp3.begin(mp3Serial)) {  //Use softwareSerial to communicate with mp3.
        //Serial.println(F("Unable to begin:"));
        //Serial.println(F("1.Please recheck the connection!"));
        //Serial.println(F("2.Please insert the SD card!"));
        while(true);
    }
    //Serial.println(F("DFPlayer Mini online."));
    
    mp3.setTimeOut(500); //Set serial communictaion time out 500ms

    mp3.volume(10);  //Set volume value (0~30).
    mp3.EQ(DFPLAYER_EQ_NORMAL);
    mp3.outputDevice(DFPLAYER_DEVICE_SD);
}

int random_joke()
{
    return (rand() % numberJokes) + 1; //returns a random joke ID between 1 and numberJokes
}

//function that handles the sound that is to be played depending on the state
void generateSound(uint8_t state)
{
    uint8_t jokeID = random_joke();
    switch (state)
    {
        case (mash) :
            mp3.play(ID_MASH);
            break;
        case (shake) :
            mp3.play(ID_SHAKE);
            break;
        case (hide) :
            mp3.play(ID_HIDE);
            break;
        case (joke) :
            mp3.play(soundID_Jokes(jokeID)); //playing random joke
            break;
        case (win) :
            mp3.play(ID_EGGCELLENT);
            break;
        case (lose) :
            mp3.play(ID_LOSE);
    }
}


