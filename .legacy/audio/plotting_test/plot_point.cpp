#include <X11/Xlib.h>
#include <unistd.h> // for sleep
#include <cstdlib>  // for exit
#include <iostream>

// Window dimensions
const int WIDTH = 800;
const int HEIGHT = 600;

// Graph margins
const int MARGIN_LEFT = 50;
const int MARGIN_BOTTOM = 50;
const int MARGIN_TOP = 50;
const int MARGIN_RIGHT = 50;

// Graph range
const double X_MAX = 10.0; // Time axis from 0 to 10
const double Y_MAX = 10.0; // Data axis from 0 to 10

// Function to map graph coordinates to window pixels
void mapCoordinates(double x, double y, int &px, int &py) {
    // Scale factors
    double scaleX = (WIDTH - MARGIN_LEFT - MARGIN_RIGHT) / X_MAX;
    double scaleY = (HEIGHT - MARGIN_BOTTOM - MARGIN_TOP) / Y_MAX;

    px = MARGIN_LEFT + static_cast<int>(x * scaleX);
    // In X11, y=0 is at the top, so we invert the y-axis
    py = HEIGHT - MARGIN_BOTTOM - static_cast<int>(y * scaleY);
}

int main() {
    // Open connection to the X server
    Display *display = XOpenDisplay(NULL);
    if (display == NULL) {
        std::cerr << "Unable to open X display.\n";
        exit(1);
    }

    // Get the default screen
    int screen = DefaultScreen(display);

    // Create a window
    Window win = XCreateSimpleWindow(display, RootWindow(display, screen),
                                     10, 10, WIDTH, HEIGHT, 1,
                                     BlackPixel(display, screen),
                                     WhitePixel(display, screen));

    // Select input events
    XSelectInput(display, win, ExposureMask | KeyPressMask | StructureNotifyMask);

    // Map (show) the window
    XMapWindow(display, win);

    // Create a Graphics Context
    GC gc = XCreateGC(display, win, 0, NULL);

    // Set foreground color to black
    XSetForeground(display, gc, BlackPixel(display, screen));

    // Event loop
    bool running = true;
    bool point_drawn = false; // Flag to ensure the point is drawn only once
    while (running) {
        XEvent event;
        XNextEvent(display, &event);

        if (event.type == Expose) {
            // Draw axes
            // X-axis
            int x0, y0, x1, y1;
            mapCoordinates(0, 0, x0, y0);
            mapCoordinates(X_MAX, 0, x1, y1);
            XDrawLine(display, win, gc, x0, y0, x1, y1);

            // Y-axis
            mapCoordinates(0, 0, x0, y0);
            mapCoordinates(0, Y_MAX, x1, y1);
            XDrawLine(display, win, gc, x0, y0, x1, y1);

            // Draw the point only once after axes are drawn
            if (!point_drawn) {
                double pointX = 1.0;
                double pointY = 1.0;
                int px, py;
                mapCoordinates(pointX, pointY, px, py);

                // Draw a small filled circle to represent the point
                int radius = 5;
                XFillArc(display, win, gc, px - radius, py - radius, 2 * radius, 2 * radius, 0, 360 * 64);

                point_drawn = true;
            }
        }

        if (event.type == KeyPress) {
            // Exit on any key press
            running = false;
        }

        if (event.type == DestroyNotify) {
            running = false;
        }
    }

    // Cleanup
    XFreeGC(display, gc);
    XDestroyWindow(display, win);
    XCloseDisplay(display);

    return 0;
}

