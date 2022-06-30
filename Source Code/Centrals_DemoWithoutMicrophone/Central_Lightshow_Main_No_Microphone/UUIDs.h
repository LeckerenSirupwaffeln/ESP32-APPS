/**
 * @file UUIDs.h
 * @author Julian Ganser (julian.ganser@smail.emt.h-brs.de)
 * @brief Contains all the needed UUIDs and the relevant enums. Same file for every program.
 * @version 0.1
 * @date 2022-05-19
 * 
 * @copyright Fully open-source
 * 
 */

enum Services {VoiceTrainer, Peltier, /* add new services if needed */ numServices};
enum Attributes {Service, FirstCharacteristic, SecondCharacteristic, ThirdCharacteristic, FourthCharacteristic, /* add further characteristics if needed */ numAttributes};

enum VoiceTrainerAttributes {VoiceStrength = 1, VoiceQuality, numVoiceCharacteristics};

char UUIDs[numServices][numAttributes][37] = {
        //VoiceTrainer:
        {"19B10000-E8F2-537E-4F6C-000000000000", //Service UUID
         "19B10000-E8F2-537E-4F6C-111111111111", "19B10000-E8F2-537E-4F6C-222222222222", //UUIDs of characteristics
         "19B10000-E8F2-537E-4F6C-333333333333", "19B10000-E8F2-537E-4F6C-444444444444"},
         
         //Peltier:
        {"19B10000-E8F2-537E-1111-000000000000", //Service UUID
         "19B10000-E8F2-537E-1111-111111111111", "19B10000-E8F2-537E-1111-222222222222", //UUIDs of characteristics
         "19B10000-E8F2-537E-1111-333333333333", "19B10000-E8F2-537E-1111-444444444444"},
};
