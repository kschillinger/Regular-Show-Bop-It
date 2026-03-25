#include <Arduino.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

#define numberJokes 5
#define numberStates 7
#define folderActions 1
#define folderJokes 2

/*
SD card structure:

/MP3
    /1 -> Actions
        001.mp3
        002.mp3
        003.mp3
        004.mp3
        005.mp3
        006.mp3
        007.mp3
    /2 -> Jokes
        001.mp3
        002.mp3
        003.mp3
        004.mp3
        005.mp3
*/

SoftwareSerial mp3Serial(10, 11);  // RX, TX for ATmega328P
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
    ID_MASH       = 1,
    ID_SHAKE      = 2,
    ID_HIDE       = 3,
    ID_JOKE       = 4,
    ID_EGGCELLENT = 5,
    ID_YOURFIRED  = 6,
    ID_COUNTDOWN  = 7
};

enum soundID_Jokes : uint8_t
{
    ID_NULL  = 0,
    ID_JOKE1 = 1,
    ID_JOKE2 = 2,
    ID_JOKE3 = 3,
    ID_JOKE4 = 4,
    ID_JOKE5 = 5
};

void speakerInit()
{
    mp3Serial.begin(9600);

    if (!mp3.begin(mp3Serial)) {
        while (true) {
        }
    }

    mp3.setTimeOut(500);
    mp3.volume(10);                 // 0 to 30
    mp3.EQ(DFPLAYER_EQ_NORMAL);
    mp3.outputDevice(DFPLAYER_DEVICE_SD);

    randomSeed(micros());
}

uint8_t random_joke()
{
    return random(1, numberJokes + 1);   // 1 through 5
}

soundID_Jokes randomJokeID()
{
    switch (random_joke())
    {
        case 1: return ID_JOKE1;
        case 2: return ID_JOKE2;
        case 3: return ID_JOKE3;
        case 4: return ID_JOKE4;
        case 5: return ID_JOKE5;
        default: return ID_NULL;
    }
}

bool playAndWait(uint8_t folderID, uint8_t trackID, unsigned long timeoutMs = 10000)
{
    mp3.playFolder(folderID, trackID);

    unsigned long startTime = millis();

    while (millis() - startTime < timeoutMs)
    {
        if (mp3.available())
        {
            uint8_t eventType = mp3.readType();

            if (eventType == DFPlayerPlayFinished)
            {
                return true;
            }
        }
    }

    return false;
}

void generateSound(uint8_t currentState)
{
    uint8_t jokeID = randomJokeID();

    switch (currentState)
    {
        case mash:
            playAndWait(folderActions, ID_MASH);
            break;

        case shake:
            playAndWait(folderActions, ID_SHAKE);
            break;

        case hide:
            playAndWait(folderActions, ID_HIDE);
            break;

        case joke:
            playAndWait(folderJokes, jokeID);
            break;

        case win:
            playAndWait(folderActions, ID_EGGCELLENT);
            break;

        case lose:
            playAndWait(folderActions, ID_YOURFIRED);
            break;

        case prestart:
            playAndWait(folderActions, ID_COUNTDOWN);
            break;

        default:
            break;
    }
}