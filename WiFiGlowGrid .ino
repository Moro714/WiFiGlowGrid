#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL4OYz8VaID"
#define BLYNK_TEMPLATE_NAME "Blinking app"
#define BLYNK_AUTH_TOKEN "N8mtEgD46wysJM0U4-n03RPeOvj6nLOb"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

char ssid[] = //"your wifi name";
char pass[] = //"your wifi password";

int anim = 0; // Current animation number
float maxBrightness = 0.4f; // Adjustable brightness

const uint16_t ledCount = 42;
const uint16_t ledPin = 2;

NeoPixelBus<NeoGrbFeature, NeoWs2812xMethod> strip(ledCount, ledPin);
NeoPixelAnimator animations(999); // Animator instance

struct AnimState {
  RgbColor StartingColor;
  RgbColor EndingColor;
};

AnimState animationState[ledCount];

// Helper function to set a random color with max brightness
RgbColor getRandomColor() {
  float hue = random(360) / 360.0f;
  return HslColor(hue, 1.0f, maxBrightness);
}

// Helper function to clear the LED strip
void resetStrip() {
  for (uint16_t i = 0; i < strip.PixelCount(); i++) {
    strip.SetPixelColor(i, RgbColor(0));
  }
  strip.Show();
}

// Function to stop all animations
void stopAnimations() {
  animations.StopAll(); // Stop all active animations
}

// Trail animation function
void trailAnimation() {
  resetStrip();
  uint16_t tailLength = 6;
  NeoGamma<NeoGammaTableMethod> colorGamma;

  AnimUpdateCallback animate = [=](const AnimationParam& param) {
    if (param.state == AnimationState_Completed) {
      animations.RestartAnimation(param.index);
      strip.RotateRight(1);
    }
  };

  float hue = random(360) / 360.0f;
  for (uint16_t index = 0; index < strip.PixelCount() && index <= tailLength; index++) {
    float brightness = index * maxBrightness / tailLength;
    RgbColor color = HslColor(hue, 1.0f, brightness);
    strip.SetPixelColor(index, colorGamma.Correct(color));
  }
  animations.StartAnimation(0, 150, animate);
}

// Blend animation function
void blendAnimation() {
  resetStrip();

  AnimUpdateCallback animate = [=](const AnimationParam& param) {
    // Calculate the blended color based on progress
    RgbColor updatedColor = RgbColor::LinearBlend(
        animationState[param.index].StartingColor,
        animationState[param.index].EndingColor,
        param.progress);

    // Set the updated color to the LED
    strip.SetPixelColor(param.index, updatedColor);

    // When animation completes, restart with a new target color
    if (param.state == AnimationState_Completed) {
      animationState[param.index].StartingColor = animationState[param.index].EndingColor;
      animationState[param.index].EndingColor = getRandomColor();
      animations.RestartAnimation(param.index); // Restart the animation for this LED
    }
  };

  // Start animations for all LEDs
  for (uint16_t i = 0; i < ledCount; i++) {
    animationState[i].StartingColor = strip.GetPixelColor<RgbColor>(i); // Initial color
    animationState[i].EndingColor = getRandomColor(); // Random target color
    animations.StartAnimation(i, random(100, 400), animate); // Set a random duration
  }
}



// Fade animation that loops and has a slower speed
void fadeAnimation() {
  resetStrip();

  AnimUpdateCallback animate = [=](const AnimationParam& param) {
    float progress = param.progress;
    RgbColor color = getRandomColor();
    RgbColor fadedColor = RgbColor::LinearBlend(RgbColor(0), color, progress);
    strip.SetPixelColor(param.index, fadedColor);

    if (param.state == AnimationState_Completed) {
      animations.RestartAnimation(param.index); // Loop the animation
    }
  };

  for (uint16_t i = 0; i < strip.PixelCount(); i++) {
    animations.StartAnimation(i, 1500, animate); // Slower duration of 1500ms
  }
}

// Wave animation that creates a smooth wave of colors
void waveAnimation() {
  resetStrip();

  float waveHue = 0.0f; // Initial hue for wave color

  AnimUpdateCallback animate = [=](const AnimationParam& param) mutable {
    float progress = param.progress;
    float wavePosition = (float)param.index / strip.PixelCount();
    RgbColor waveColor = HslColor(waveHue + wavePosition, 1.0f, maxBrightness * (0.5f + 0.5f * sin(progress * PI)));
    strip.SetPixelColor(param.index, waveColor);

    if (param.state == AnimationState_Completed) {
      animations.RestartAnimation(param.index); // Loop the wave animation
    }
  };

  for (uint16_t i = 0; i < strip.PixelCount(); i++) {
    animations.StartAnimation(i, 2000, animate); // Slower wave movement
  }
}

// Rainbow cycle animation that smoothly transitions through rainbow colors
void rainbowCycleAnimation() {
  resetStrip();

  AnimUpdateCallback animate = [=](const AnimationParam& param) mutable {
    float hueOffset = (float)param.index / strip.PixelCount();
    RgbColor rainbowColor = HslColor(hueOffset + param.progress, 1.0f, maxBrightness);
    strip.SetPixelColor(param.index, rainbowColor);

    if (param.state == AnimationState_Completed) {
      animations.RestartAnimation(param.index); // Loop the rainbow cycle animation
    }
  };

  for (uint16_t i = 0; i < strip.PixelCount(); i++) {
    animations.StartAnimation(i, 2500, animate); // Slower rainbow cycle speed
  }
}

// Blynk virtual pin handler for selecting animations
BLYNK_WRITE(V5) {
  anim = param.asInt();
  
  stopAnimations(); // Stop all running animations
  resetStrip();     // Clear the strip

  if (anim == 1) {
    trailAnimation();  
  } else if (anim == 2) {
    blendAnimation();
  } else if (anim == 3) {
    fadeAnimation();
  } else if (anim == 4) {
    waveAnimation();
  } else if (anim == 5) {
    rainbowCycleAnimation();
  }
}

// Blynk virtual pin handler for brightness
BLYNK_WRITE(V6) {
  maxBrightness = param.asFloat();
  maxBrightness = constrain(maxBrightness, 0.0, 1.0);  // Ensure valid range
}

void setup() {
  Serial.begin(9600);
  strip.Begin();
  strip.Show();
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  Blynk.run();

  if (animations.IsAnimating()) {
    animations.UpdateAnimations();
    strip.Show();
  } else if (anim == 0) {
    resetStrip();
  }
}
