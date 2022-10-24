/*
  This example demonstrates compression and de-compression of Short Strings
  using Unishox compression utility found at https://github.com/siara-cc/Shox96

  How Unishox works:
  https://github.com/siara-cc/Unishox/blob/master/Unishox_Article_2.pdf?raw=true

  To store and retrieve compressed data in progmem, use the other library:
  https://github.com/siara-cc/Unishox_Arduino_Progmem_lib

*/
#include "unishox2.h"

void setup() {

  Serial.begin(115200);

}

void print_compressed(char *in, int len) {
  int l;
  byte bit;
  Serial.write("\nCompressed bits:");
  for (l=0; l<len*8; l++) {
    bit = (in[l/8]>>(7-l%8))&0x01;
    Serial.print((int)bit);
    if (l%8 == 7) Serial.print(" ");
  }
}

int input_string(char *str, int max_len) {
  max_len--;
  int ctr;
  Serial.flush();
  ctr = 0;
  str[ctr] = 0;
  while (true) {
    if (Serial.available() > 0) {
        char c = Serial.read();
        if (ctr == 0 && (c == '\n' || c == '\r'))
          continue;
        if (c >= 32 && c <= 126) {
          str[ctr] = c;
          ctr++;
        }
        if (c < 0 || c == '\n' || c == '\r' || ctr >= max_len)
          break;
    }
  }
  str[ctr] = 0;
  Serial.println(str);
  return ctr;
}


void loop() {

  char str[255];
  char cbuf[300];
  char dbuf[300];
  Serial.write("Welcome\n");
  Serial.write("-------\n");

  Serial.write("Enter string to compress (Max 255 letters):\n");
  int len = input_string(str, sizeof(str));
 
  if (len > 0) {
    memset(cbuf, 0, sizeof(cbuf));
    int ctot = unishox2_compress(str, len, cbuf, NULL);
    print_compressed(cbuf, ctot);
    memset(dbuf, 0, sizeof(dbuf));
    int dlen = unishox2_decompress(cbuf, ctot, dbuf, NULL);
    dbuf[dlen] = 0;
    Serial.write("\n\nDecompressed:");
    Serial.write(dbuf);
    float perc = (dlen-ctot);
    perc /= dlen;
    perc *= 100;
    Serial.write("\n\nRatio: ");
    Serial.print(ctot);
    Serial.write("/");
    Serial.print(dlen);
    Serial.write(" = ");
    Serial.print(perc);
    Serial.write("\n\n");
  }
  delay(500);

}

