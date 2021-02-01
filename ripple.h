
//----------------- Ripple structure definition ----------------------------------------------------------------


struct ripple {                                                                 // Reko Meriö's structures

// Local definitions

// Persistent local variables

// Temporary local variables
  uint8_t brightness;                                   // 0 to 255
  int8_t color;                                         // 0 to 255
  int16_t pos;                                           // -1 to SEAT_LEDS  (up to 127 LED's)
  int8_t velocity;                                      // 1 or -1
  uint8_t life;                                         // 0 to 20
  uint8_t maxLife;                                      // 10. If it's too long, it just goes on and on. . .
  uint8_t fade;                                         // 192
  bool exist;                                           // 0 to 1


  void Move() {

    pos += velocity;
    life++;

    if (pos > SEAT_LEDS - 1) {                           // Bounce back from far end.
      velocity *= -1;
      pos = SEAT_LEDS - 1;
    }

    if (pos < 0) {                                      // Bounce back from 0.
      velocity *= -1;
      pos = 0;
    }

    brightness = scale8(brightness, fade);              // Adjust brightness accordingly to strip length

    if (life > maxLife || brightness <= 0) exist = false;                  // Kill it once it's expired.

  } // Move()



  void Init(uint8_t Fade, uint8_t MaxLife) {                 // Typically 216, 20

    pos = random8(SEAT_LEDS/8, SEAT_LEDS-SEAT_LEDS/8);     // Avoid spawning too close to edge.
    velocity = 1;                                       // -1 or 1
    life = 0;                                           // 0 to Maxlife
    maxLife = MaxLife;                                  // 10 called
    exist = true;                                       // 0, 1
    brightness = 255;                                   // 0 to 255
    color = millis();                                   // hue;
    fade = Fade;                                        // 192 called

  } // Init()

}; // struct ripple
