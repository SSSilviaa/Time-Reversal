#include <MCP492X.h> // Include the library

#define PIN_SPI_CHIP_SELECT_DAC 4 // Or any pin you'd like to use
const int syncPin = 7;

MCP492X myDac(PIN_SPI_CHIP_SELECT_DAC);

const PROGMEM uint16_t DACLookup_FullSine_5Bit[32] =
{
  2048, 2447, 2831, 3185, 3495, 3750, 3939, 4056,
  4095, 4056, 3939, 3750, 3495, 3185, 2831, 2447,
  2048, 1648, 1264,  910,  600,  345,  156,   39,
     0,   39,  156,  345,  600,  910, 1264, 1648
};

const PROGMEM uint16_t DACLookup_FullSine_nBit[300] = 
{843, 837, 829, 821, 816, 814, 817, 821, 833, 840, 847, 851, 854, 855, 850, 846, 842, 843, 846, 853, 861, 868, 874, 881, 884, 889, 898, 906, 914, 919, 916, 910, 903, 893, 883, 874, 867, 860, 857, 858, 861, 865, 867, 866, 861, 855, 850, 847, 841, 836, 827, 821, 813, 811, 814, 820, 828, 840, 851, 858, 862, 864, 864, 863, 859, 857, 859, 864, 873, 878, 880, 881, 882, 883, 887, 896, 906, 914, 917, 915, 910, 902, 893, 884, 877, 869, 865, 867, 869, 875, 879, 882, 882, 879, 875, 873, 868, 865, 851, 838, 827, 821, 820, 823, 832, 842, 852, 862, 868, 870, 865, 860, 853, 848, 847, 850, 861, 868, 877, 882, 887, 894, 902, 911, 916, 922, 920, 915, 908, 898, 884, 874, 866, 862, 859, 859, 861, 865, 868, 867, 865, 860, 857, 854, 853, 851, 847, 840, 834, 824, 821, 820, 824, 829, 836, 842, 846, 849, 846, 843, 838, 836, 835, 839, 843, 850, 858, 864, 867, 869, 873, 881, 890, 899, 901, 897, 888, 880, 870, 861, 850, 841, 838, 837, 838, 843, 851, 856, 858, 856, 854, 853, 852, 851, 844, 833, 822, 811, 805, 803, 808, 816, 826, 836, 844, 848, 849, 843, 836, 834, 834, 837, 845, 856, 866, 875, 880, 884, 890, 896, 911, 918, 922, 921, 917, 909, 901, 889, 881, 872, 867, 865, 867, 871, 876, 879, 881, 879, 876, 873, 872, 870, 867, 861, 855, 848, 844, 844, 850, 858, 866, 872, 879, 882, 884, 879, 874, 866, 860, 859, 864, 872, 880, 889, 898, 906, 912, 919, 929, 935, 939, 935, 929, 920, 912, 901, 887, 879, 873, 871, 872, 873, 877, 881, 884, 884, 877, 873, 870, 867, 861, 851, 841, 832, 822, 822, 824, 834, 844, 854};
  

// const PROGMEM uint16_t DACLookup_FullSine_8Bit[256] =
// {
//   2048, 2098, 2148, 2198, 2248, 2298, 2348, 2398,
//   2447, 2496, 2545, 2594, 2642, 2690, 2737, 2784,
//   2831, 2877, 2923, 2968, 3013, 3057, 3100, 3143,
//   3185, 3226, 3267, 3307, 3346, 3385, 3423, 3459,
//   3495, 3530, 3565, 3598, 3630, 3662, 3692, 3722,
//   3750, 3777, 3804, 3829, 3853, 3876, 3898, 3919,
//   3939, 3958, 3975, 3992, 4007, 4021, 4034, 4045,
//   4056, 4065, 4073, 4080, 4085, 4089, 4093, 4094,
//   4095, 4094, 4093, 4089, 4085, 4080, 4073, 4065,
//   4056, 4045, 4034, 4021, 4007, 3992, 3975, 3958,
//   3939, 3919, 3898, 3876, 3853, 3829, 3804, 3777,
//   3750, 3722, 3692, 3662, 3630, 3598, 3565, 3530,
//   3495, 3459, 3423, 3385, 3346, 3307, 3267, 3226,
//   3185, 3143, 3100, 3057, 3013, 2968, 2923, 2877,
//   2831, 2784, 2737, 2690, 2642, 2594, 2545, 2496,
//   2447, 2398, 2348, 2298, 2248, 2198, 2148, 2098,
//   2048, 1997, 1947, 1897, 1847, 1797, 1747, 1697,
//   1648, 1599, 1550, 1501, 1453, 1405, 1358, 1311,
//   1264, 1218, 1172, 1127, 1082, 1038,  995,  952,
//    910,  869,  828,  788,  749,  710,  672,  636,
//    600,  565,  530,  497,  465,  433,  403,  373,
//    345,  318,  291,  266,  242,  219,  197,  176,
//    156,  137,  120,  103,   88,   74,   61,   50,
//     39,   30,   22,   15,   10,    6,    2,    1,
//      0,    1,    2,    6,   10,   15,   22,   30,
//     39,   50,   61,   74,   88,  103,  120,  137,
//    156,  176,  197,  219,  242,  266,  291,  318,
//    345,  373,  403,  433,  465,  497,  530,  565,
//    600,  636,  672,  710,  749,  788,  828,  869,
//    910,  952,  995, 1038, 1082, 1127, 1172, 1218,
//   1264, 1311, 1358, 1405, 1453, 1501, 1550, 1599,
//   1648, 1697, 1747, 1797, 1847, 1897, 1947, 1997
// };

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  myDac.begin();
  //Slave
  pinMode(syncPin, INPUT);
  // wait until mater signal
  Serial.println("INPUT - Receiving synchronization signal...");
  while (digitalRead(syncPin) != HIGH) {};
  Serial.println("Received master signal...");

  //Master
  // pinMode(syncPin, OUTPUT);
  // Serial.println("Master - Sending synchronization signal...");
  // for(int i = 0; i<70; i++){
  //   digitalWrite(syncPin, LOW);
  //   delay(200);
  // }
  // digitalWrite(syncPin, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < 300; i++)
  {
    //Serial.println(pgm_read_word(&(DACLookup_FullSine_nBit[i])));
    int value = ((pgm_read_word(&(DACLookup_FullSine_nBit[i])))*4096)/1024;
    myDac.analogWrite(value);
    delayMicroseconds(10);
  }

  // Write any value from 0-4095
  // myDac.analogWrite(4095);

  // If using an MCP4922, write a value to DAC B
  //myDac.analogWrite(1, 3172);
}