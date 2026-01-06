// Copyright 2025 Craig Corvin
//
// Author: Craig Corvin
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// See http://creativecommons.org/licenses/MIT/ for more information.
//
// -----------------------------------------------------------------------------

#include <math.h>
#include <vector>


class FmdbOsc
{
    public:
        FmdbOsc() {}
        ~FmdbOsc() {}

        /*
         @param float sample_rate the sample rate of the audio engine
         */
        void Init(float sample_rate);

        /*
        @param return output
        */
        float Process(float in);

        /*
        set the frequency in HZ of the sine wave that is added to the input buffer
        the inclusion of this into output is set by SetIncludeCenterFrequency
        @param float center_frequency
        */
        void SetCenterFrequency(float center_frequency);

        /*
        set the frequency in HZ that the input buffer will be modulated by
        @param float delay_buffer_modulation_frequency
        */
        void SetInputBufferModulationFrequency(float delay_buffer_modulation_frequency);

        /*
        set the range of the input buffer that is modulated
        @param float delay_buffer_modulation_range
        */
        void SetInputBufferModulationRange(float delay_buffer_modulation_range);

        /*
        set the center point of offset of the input buffer
        @param float delay_buffer_modulation_offset
        */
        void SetInputBufferModulationOffset(float delay_buffer_modulation_offset);

        /*
        resets the period of the beta_ oscillator when theta_ finishes a cycle
        @param bool sync
        */
        void SetSync(bool sync);

        /*
        include the center frequency in the output from Process()
        @param bool include_center_frequency
        */
        void SetIncludeCenterFrequency(bool include_center_frequency);

        /*
        use input into process instead of oscillator
        @param bool in
        */
        void SetInputIntoProcess(bool use_input);

        /*
        get value of in_, to check if use input into process has been set to true
        @return in_
        */
        bool GetInputIntoProcess();
        

    private:

        // number of samples in one second
        float sample_rate_;
        // vector for delay buffer
        std::vector<float> delay_buffer_;
        // location within vector to read out from
        unsigned int delay_read_pointer_;
        // location within vector to write input to
        unsigned int delay_write_pointer_;
        // the numeric location within the delay_buffer_ to read output from
        unsigned int current_pointer_;
        // the length of the buffer storing input
        unsigned int buffer_length_;
        // the delay buffer width to modulate through by beta frequency
        float delay_buffer_range_;
        // the delay buffer offset center point that is modulated around
        float delay_buffer_offset_;
        // store the incremental radian values of the center frequency
        float theta_;
        // store the incremental radian values of the coefficient modulation frequency
        float beta_;
         // theta frequency
        float center_frequency_;
        // beta frequency
        float delay_buffer_modulation_frequency_;
        // the sine of beta
        float delay_buffer_coeffcient_;
        // calculated in Process
        int offset_value_;
        // resets the period of the beta_ oscillator when theta_ finishes a cycle
        bool sync_;
        // use input into process instead of oscillator
        bool use_input_;
        // include the center frequency in the output from Process()
        bool include_center_frequency_;

};