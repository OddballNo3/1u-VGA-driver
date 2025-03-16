# 1u-VGA-driver
A 1u cv / audio to vga converter

Ever wonder what audio looks like?  This is a simple H-sync, V-sync, and blanking gater to allow ramming just about anything down the RGB lines of your standard VGA cable to make interesting patterns.  This module's core is a simple Arduino script that runs on an attiny 85.  it can accept dc to about 3mhz or up to the pixel clock.  Base code was adapted from Kevin (last name unknown) to produce a higher resolution, and add 3 outputs to gate incoming signals for proper signal drop out during the v-sync and h-sync porches.  If you’re interested in how this code works the original code can be seen here: https://emalliab.wordpress.com/2020/05/03/attiny85-vga-sync/

