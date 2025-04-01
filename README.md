# WiFiGlowGrid

This project is a WiFi-enabled RGB matrix powered by an ESP8266 microcontroller, designed to display customizable LED animations and effects. The system connects to a local host and integrates with the Blynk platform, allowing remote control from both PC and mobile devices.
Features

    ESP8266-based RGB matrix with wireless connectivity.
    Blynk integration using <BlynkSimpleEsp8266.h>, enabling remote control via a digital platform on PC and phone.
    Dynamic LED animations using <NeoPixelBus.h> and <NeoPixelAnimator.h>.
    Customizable lighting effects, adjustable from the Blynk app or local controls.
    Local host connectivity for real-time interaction.

Libraries Used

    <BlynkSimpleEsp8266.h> – Handles WiFi connectivity and communication with the Blynk platform.
    <NeoPixelBus.h> – Provides efficient control over NeoPixel (WS2812, WS2811, etc.) LEDs.
    <NeoPixelAnimator.h> – Enables smooth and complex LED animations.

How It Works

    The ESP8266 connects to a WiFi network and establishes a connection with Blynk.
    LED animations are managed using NeoPixelBus and NeoPixelAnimator libraries.
    Users can control LED presets, brightness, and effects remotely through the Blynk app on their PC or smartphone.
    The system can be expanded with additional features like sensor input or interactive modes.

Setup & Installation

    Install the required Arduino libraries (Blynk, NeoPixelBus, NeoPixelAnimator).
    Flash the provided Arduino script to the ESP8266.
    Configure the Blynk app to connect to the device.
    Power the RGB matrix and start customizing the animations!

Demo
[![Demo Video](https://github.com/Moro714/WiFiGlowGrid/blob/main/1.mp4)
