# led-drivers
A collection of utilities to drive the current ambient LED setup.

### Serial command case table
| CMD (string literals) | Animation Type |
|-|-|
| '0' | Static line segment |
| '1' | 2D circular cycling |
| '2' | 2D planar animation |

## audioCapture.py
Dependancies:
Mainly fetched with `apt-get install []`
    - libasound2-dev
    - `sudo -H pip3 install pyalsaaudio`
