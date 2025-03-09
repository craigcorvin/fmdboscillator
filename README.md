<h2>Frequency Modulated Delay Buffer Oscillator</h2>
The oscillating values of a sine wave are inputed into a delay buffer, and the output is a modulated range of locations at a specific offset from the current input location... For example, if the total delay buffer is 48,000 samples, and the offset is 24,000, with a range of 6,000, then the output would be locations between 18,000 (3,000 x -1.0) and 30,000 (3,000 x 1.0) as the modulation sine wave oscillates.
<pre>
                     modulation
                        of
                    |  range  |
sine wave   =>  | | | | | | | | | | | | delay buffer | | |
                        \ /
                       output
</pre>
