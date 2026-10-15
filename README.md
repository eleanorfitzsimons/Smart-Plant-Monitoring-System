# Smart Plant Monitoring System

Third-year project, Trinity College Dublin (Computer and Electronic Engineering).
[Group project with 2 teammates]

## What is does
A smart plant pot that monitors soil moisture and waters the plant automatically.
It also detects when the plant is leaning using four pressure sensors under a rotating base, and turns the base with a servo to correct the imbalance and optimize plant growth using the principal of photosynthesis.
It detects environmental conditions of the plant and displays the on a monitor

## Hardware
- 2x ESP32
- Soil moisture, humidity, light, water level and pressure sensors (x4 pressure sensors under the base)
- Water pump
- Servo motor 
- 3D-printed parts, designed in OnShape

## How it works
**Watering:** If soil moisture drops below the threshold (12%), the pump runs
for 5000 ms, then a 10 s lockout stops it re-triggering while water drains.
The pump is disabled if the water level sensor reads low, to avoid dry running.

**Tilt correction:** The four pressure sensors give the weight distribution and
centre of mass. If the imbalance passes the threshold (2.0 kPa), the ESP32
calculates an angle in the opposite direction and the servo moves the base to
that position. The baseline is recalibrated after each watering, since adding
water changes the weight distribution.

The main loop runs every 100 ms.
