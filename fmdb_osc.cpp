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

#include "fmdb_osc.h"
#include <math.h>
#include <vector>

#define PI_DSF 3.1415927410125732421875f
#define TWOPI_DSF (2.0f * PI_DSF)

/*
Frequency Modulated Delay Buffer Oscillator
*/
void FmdbOsc::Init(float sample_rate)
{
    sample_rate_ = sample_rate;
    buffer_length_ = sample_rate_;
	// resize the length of the vector that stores input 
    delay_buffer_.resize(buffer_length_);
    delay_buffer_range_ = buffer_length_ / 4;
    delay_buffer_offset_ = 1;
    delay_write_pointer_ = 0;
    delay_read_pointer_ = 1;
    offset_value_ = 0;
    current_pointer_ = 0;
    center_frequency_ = 110.0f;
    delay_buffer_modulation_frequency_ = 110.0f;
    delay_buffer_coeffcient_ = 0.0f;
    theta_ = 0.0f;
    beta_ = 0.0f;
    sync_ = true;
    include_center_frequency_ = false;
}

/*
@param return output 
*/
float FmdbOsc::Process()
{

    // sine wave to add into input/delay buffer
    theta_ += TWOPI_DSF * center_frequency_ / sample_rate_;

    // sine wave to modulate the buffer location at width delay_buffer_range_ around
    // an offset location delay_buffer_offset_
    beta_ += TWOPI_DSF * delay_buffer_modulation_frequency_ / sample_rate_;

    if (theta_ >= TWOPI_DSF)
    {
        theta_ -= TWOPI_DSF;

        // resets the period of the beta_ oscillator when theta_ finishes a cycle
        if (sync_)
        {
            beta_ = TWOPI_DSF * delay_buffer_modulation_frequency_ / sample_rate_;
        }
    }

    // reset beta_ when cyle finishes, independent of theta_
    if (!sync_)
    {
        if (beta_ >= TWOPI_DSF)
        {
            beta_ -= TWOPI_DSF;
        }
    }

    // input to add to delay buffer, which is a sine wave at center_frequency_
    float input = sin(theta_);

	// coefficient value between 1 and -1 oscillating at delay_buffer_modulation_frequency_ 
    delay_buffer_coeffcient_ = sin(beta_);

	// the current offset value within the width delay_buffer_range_ at frequency of delay_buffer_coeffcient_
    offset_value_ = (delay_buffer_coeffcient_ * delay_buffer_range_) + delay_buffer_offset_;

	// calculate the numeric location within the delay_buffer_
    current_pointer_ = (delay_read_pointer_ - offset_value_  + delay_buffer_.size()) % delay_buffer_.size();

	// get value from delay_buffer_
    float delay_buffer_value = delay_buffer_[current_pointer_];

	// advance the read pointer
    delay_read_pointer_++;
    if (delay_read_pointer_ >= delay_buffer_.size()) 
    {
        delay_read_pointer_ = 0;
    }

    float output = delay_buffer_value;

	// add center frequency to audio output
    if (include_center_frequency_)
    {
        output += input;
    }

    // write to input buffer
    delay_buffer_[delay_write_pointer_] = input;

    delay_write_pointer_++;
    if (delay_write_pointer_ >= delay_buffer_.size())
    {
        delay_write_pointer_ = 0;
    }

    return output;

}

void FmdbOsc::SetCenterFrequency(float center_frequency) {
    center_frequency_ = center_frequency;
}

void FmdbOsc::SetInputBufferModulationFrequency(float delay_buffer_modulation_frequency){
    delay_buffer_modulation_frequency_ = delay_buffer_modulation_frequency;
}

void FmdbOsc::SetInputBufferModulationRange(float delay_buffer_modulation_range) {

    if (delay_buffer_modulation_range < 0.0f)
    {
        delay_buffer_modulation_range = 0.0f;
    }

    if (delay_buffer_modulation_range > 1.0f)
    {
        delay_buffer_modulation_range = 1.0f;
    }

    // expecting a value between 0.0f and 1.0f
    // delay_buffer_modulation_range * buffer_length = range of values that the buffer will be modulated through
    delay_buffer_range_ = delay_buffer_modulation_range * buffer_length_;

}

void FmdbOsc::SetInputBufferModulationOffset(float delay_buffer_modulation_offset) {

    if (delay_buffer_modulation_offset < 0.0f)
    {
        delay_buffer_modulation_offset = 0.0f;
    }

    if (delay_buffer_modulation_offset > 1.0f)
    {
        delay_buffer_modulation_offset = 1.0f;
    }

    // center point in the range that the delay_buffer_range_ modulate around
    delay_buffer_offset_ = delay_buffer_modulation_offset * buffer_length_;

}

void FmdbOsc::SetSync(bool sync)
{
    sync_ = sync;
}


void FmdbOsc::SetIncludeCenterFrequency(bool include_center_frequency)
{
    include_center_frequency_ = include_center_frequency;
}

