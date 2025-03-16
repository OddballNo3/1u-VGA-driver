#define H_SYNC 1  
#define V_SYNC 3
#define VGA_R  2
#define VGA_G  4
#define VGA_B  0

#define TOTAL_LINES 750
#define BACK_PORCH_LINES 20

int V_Sync_Line;

void setup() {
  // Configure Output I/O pins
  PORTB = 0;
  DDRB = (1 << H_SYNC) | (1 << V_SYNC) | (1 << VGA_R) | (1 << VGA_G) | (1 << VGA_B);

  // Run a timer interrupt off a 74.25 MHz clock
  PLLCSR |= (1 << PLLE);            // Enable 74.25 MHz PLL clock
  while (!(PLLCSR & (1 << PLOCK))); // Wait for the PLL lock
  PLLCSR |= (1 << PCKE);            // Enable PLL as source for Timer 1

  // Timer 1 for CTC mode and set on compare with OCR1A and overflow interrupt
  TCCR1 = (1 << CTC1) |              // Reset on match with OCR1C
          (1 << CS12) |              // Prescale /8
          (1 << PWM1A) |             // PWM mode
          (1 << COM1A1) | (1 << COM1A0); // Set OC1A on compare; clear on rest
  GTCCR = 0;
  OCR1A = 15;
  OCR1C = 148;  // Adjusted value for the new resolution

  // Enable Overflow Interrupt
  TIMSK = (1 << TOIE1);

  V_Sync_Line = 0;
}

void loop() {
  
}

// Timer 1 Match with OCR1A interrupt
ISR(TIMER1_OVF_vect) {
  // Triggered every H_Sync line
  //
  // It automatically outputs the H_Sync pulse, so need to complete the line:
  // <---- H line Pixel output ---->
  //
  // From timings, is approx 22.2 μs
  // So read the timer register and ensure it is > 22
  //
  while (TCNT1 < 23) {};

  if (V_Sync_Line <= (TOTAL_LINES - 1 - BACK_PORCH_LINES)) {
    // Visible line output
    PORTB |= (1 << VGA_R) | (1 << VGA_G) | (1 << VGA_B);
  } else if (V_Sync_Line == TOTAL_LINES - BACK_PORCH_LINES) {
    // V Front Porch
  } else if ((V_Sync_Line >= (TOTAL_LINES - BACK_PORCH_LINES + 1)) && (V_Sync_Line <= (TOTAL_LINES - 1))) {
    // V Sync Pulse
    PORTB &= ~(1 << V_SYNC);
  } else { // Beyond V Back Porch
    PORTB |= (1 << V_SYNC);
    PORTB &= ~((1 << VGA_R) | (1 << VGA_G) | (1 << VGA_B));
  }


  V_Sync_Line++;
  if (V_Sync_Line >= 770) {
    V_Sync_Line = 0;
  }
}
