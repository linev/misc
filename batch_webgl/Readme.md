# Test webgl drawing with browser in headless mode

This simple HTTML page can be used to test headless mode.
To use with chrome:

    chromium --headless --disable-gpu --print-to-pdf canvas.htm
    
    chromium --headless --screenshot webgl.htm

On OpenSUSE with chrome 68 PDF output does not work with webgl - canvas should be used 

    chromium --headless --print-to-pdf canvas.htm 

To use with firefox, one should create temporary profile first:

    /usr/bin/firefox --no-remote -CreateProfile "test_ff /tmp/test_ff"

After this one could start firefox with following commands:

    /usr/bin/firefox --no-remote -profile "/tmp/test_ff" -screenshot `pwd`/webgl.htm 

    /usr/bin/firefox --no-remote -profile "/tmp/test_ff" -screenshot `pwd`/canvas.htm 
    
Neither webgl no canvas renderer were working.     
    