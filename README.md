Secure Vehicle Access System

A simple but effective Arduino-based access control system that mimics how a car's keyless entry might work. Punch in the right code on a keypad, and the system "unlocks" the vehicle by rotating a servo — get it wrong too many times, and it locks you out for half a minute.

I built this mainly to get hands-on with combining a few common Arduino modules — keypad, LCD, servo, EEPROM — into something that actually feels like a real-world security project rather than just blinking an LED.

How it works


Enter your password on the keypad — each digit shows up as a * on the LCD so nobody can read it over your shoulder.
Press # to submit. If it matches, the servo rotates to 90° (simulating the vehicle unlocking), the green LED lights up, and the screen lets you know the vehicle's ready.
Get it wrong, and the red LED lights up with a buzzer beep, and you've used up one of your three attempts.
Three wrong attempts in a row, and the system locks itself for 30 seconds — buzzer sounds, screen shows "SYSTEM LOCKED," and the keypad is ignored until the timeout passes.
Press * anytime to backspace/clear what you've typed.
Once the vehicle is unlocked, pressing D locks it back up and resets the servo and LED.
Press A to start a password change. You'll be asked to verify your current password first — only after that does it let you set a new one. This was important to me, since allowing anyone to change the password without verification would defeat the whole point of having one.


Hardware


Arduino Uno
4x4 Matrix Keypad
16x2 I2C LCD Display
Servo Motor (for the unlock simulation)
Piezo Buzzer
Green LED (success)
Red LED (failure)

Default Password

The default password is 1234, set automatically the first time the board runs. Once changed, the new password is saved to EEPROM and persists across power cycles on real hardware.


Screenshots

circuit
correctpassword
login_screen
Passwordchange
System_locked