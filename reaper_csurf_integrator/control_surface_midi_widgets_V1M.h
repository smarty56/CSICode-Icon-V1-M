
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class V1MVUMeter_Midi_FeedbackProcessor : public Midi_FeedbackProcessor  // Linked to widget "FB_V1MVUMeter" Kev Smart 
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
protected:
    int displayType_;
    int channelNumber_;
    int lastMidiValue_;
    bool isClipOn_;

public:
    virtual ~V1MVUMeter_Midi_FeedbackProcessor() {}
    V1MVUMeter_Midi_FeedbackProcessor(CSurfIntegrator* const csi, Midi_ControlSurface* surface, Widget* widget, int displayType, int channelNumber) : Midi_FeedbackProcessor(csi, surface, widget), displayType_(displayType), channelNumber_(channelNumber)
    {
        lastMidiValue_ = 0;
        isClipOn_ = false;
    }

    virtual const char* GetName() override { return "V1MVUMeter_Midi_FeedbackProcessor"; }

    virtual void ForceClear() override
    {
        const PropertyList properties;
        ForceValue(properties, 0.0);
    }

    virtual void SetValue(const PropertyList& properties, double value) override
    {
        int midiValue = GetMidiValue(properties, value);
        if (midiValue != -1)
        {
            SendMidiMessage(0xd0, (channelNumber_ << 4) | midiValue, 0);
        }
    }

    virtual void ForceValue(const PropertyList& properties, double value) override
    {
        ForceMidiMessage(0xd0, (channelNumber_ << 4) | GetMidiValue(properties, value), 0);
    }

    int GetMidiValue(const PropertyList& properties, double value)
    {
        int    midiValue = -1;
        double   dbValue = VAL2DB(normalizedToVol(value));

        //---------------------------------------------------
        // HANDLE METER SPECIFIC SCALING, DEFAULT IS "XTOUCH"
        //---------------------------------------------------
        const char* meterMode = nullptr;

        //PropertyType propertyType = properties.prop_from_string("MeterMode");
        //if (propertyType)
        //    meterMode = (char*)properties.get_prop(propertyType);
        //if (!meterMode)
        //    meterMode = "XTOUCH";

        if (dbValue >= -100.1 && dbValue < -60.1)  midiValue = 0x00;  //No LEDs
        else if (dbValue >= -60.1 && dbValue < -48.1)  midiValue = 0x01; // LED 1 Green  (–60 dB)
        else if (dbValue >= -48.1 && dbValue < -42.1)  midiValue = 0x02; // LED 2 Green  (–48 dB)
        else if (dbValue >= -42.1 && dbValue < -36.1)  midiValue = 0x03; // LED 3 Green  (–42 dB)
        else if (dbValue >= -36.1 && dbValue < -30.1)  midiValue = 0x04; // LED 4 Green  (–36 dB)
        else if (dbValue >= -30.1 && dbValue < -24.1)  midiValue = 0x05; // LED 5 Green  (–30 dB)
        else if (dbValue >= -24.1 && dbValue < -18.1)  midiValue = 0x06; // LED 6 Green  (–24 dB)
        else if (dbValue >= -18.1 && dbValue < -12.1)  midiValue = 0x07; // LED 7 Green  (–18 dB)
        else if (dbValue >= -12.1 && dbValue < -9.1)  midiValue = 0x08; // LED 8 Orange (–12 dB)
        else if (dbValue >= -9.1 && dbValue < -6.1)  midiValue = 0x09; // LED 9 Orange (–9 dB)
        else if (dbValue >= -6.1 && dbValue < -3.1)  midiValue = 0x0A; // LED 10 Orange (–6 dB)
        else if (dbValue >= -3.1 && dbValue < 0.1)  midiValue = 0x0B; // LED 11 Orange (–3 dB)
        else if (dbValue >= 0.1)                      midiValue = 0x0E; // LED 12 Red    (> 0 dB)


        //---------------------------------------------------//
        // SCALING COMPLETED - RETURN VALUE                  //
        //---------------------------------------------------//

        if (midiValue > 0)
            WindowsOutputDebugString("MeterMode=%s   track=%d   midvalue=0x%02x\n", meterMode, channelNumber_, midiValue);

        return  midiValue;
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class V1MMasterVUMeter_Midi_FeedbackProcessor : public Midi_FeedbackProcessor
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
private:
    double minDB_;
    double maxDB_;
    int param_;

public:
    virtual ~V1MMasterVUMeter_Midi_FeedbackProcessor() {}
    V1MMasterVUMeter_Midi_FeedbackProcessor(CSurfIntegrator* const csi, Midi_ControlSurface* surface, Widget* widget, int param) : Midi_FeedbackProcessor(csi, surface, widget), param_(param)
    {
        minDB_ = 0.0;
        maxDB_ = 24.0;
    }

    virtual const char* GetName() override { return "V1MMasterVUMeter_Midi_FeedbackProcessor"; }

    virtual void ForceClear() override
    {
        const PropertyList properties;
        ForceValue(properties, 0.0);
    }

    int GetMidiMeterValue(double value)
    {
        int      midiValue = 0;
        double   dbValue = VAL2DB(normalizedToVol(value));
        if (dbValue >= -60.1 && dbValue < -48.1)  midiValue = 0x01; // LED  1 Green  (–60 dB)
        else if (dbValue >= -48.1 && dbValue < -42.1)  midiValue = 0x02; // LED  2 Green  (–48 dB)
        else if (dbValue >= -42.1 && dbValue < -36.1)  midiValue = 0x03; // LED  3 Green  (–42 dB)
        else if (dbValue >= -36.1 && dbValue < -30.1)  midiValue = 0x04; // LED  4 Green  (–36 dB)
        else if (dbValue >= -30.1 && dbValue < -24.1)  midiValue = 0x05; // LED  5 Green  (–30 dB)
        else if (dbValue >= -24.1 && dbValue < -18.1)  midiValue = 0x06; // LED  6 Green  (–24 dB)
        else if (dbValue >= -18.1 && dbValue < -12.1)  midiValue = 0x07; // LED  7 Green  (–18 dB)
        else if (dbValue >= -12.1 && dbValue < -9.1)  midiValue = 0x08; // LED  8 Orange (–12 dB)
        else if (dbValue >= -9.1 && dbValue < -6.1)  midiValue = 0x09; // LED  9 Orange (–9  dB)
        else if (dbValue >= -6.1 && dbValue < -3.1)  midiValue = 0x0A; // LED 10 Orange (–6  dB)
        else if (dbValue >= -3.1 && dbValue < 0.1)  midiValue = 0x0B; // LED 11 Orange (–3  dB)
        else if (dbValue >= 0.1)  midiValue = 0x0E; // LED 12 Red    (> 0 dB)
        return midiValue;
    }

    virtual void SetValue(const PropertyList& properties, double value) override
    {
        WindowsOutputDebugString("QConProXMasterVUMeter_Midi_FeedbackProcessor: 0xd1, 0x%02x\n", (param_ << 4) | GetMidiMeterValue(value));
        SendMidiMessage(0xd1, (param_ << 4) | GetMidiMeterValue(value), 0);
    }

    virtual void ForceValue(const PropertyList& properties, double value) override
    {
        ForceMidiMessage(0xd1, (param_ << 4) | GetMidiMeterValue(value), 0);
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class V1MDisplay_Midi_FeedbackProcessor : public Midi_FeedbackProcessor // Linked to widgets "FB_V1MDisplay1Upper" "FB_V1MDisplay1Lower" "FB_V1MDisplay2Upper" "FB_V1MDisplay2Lower"" Kev Smart
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
private:
    int sysExByte1_;
    int sysExByte2_;
    int offset_;
    int displayType_;
    int displayRow_;
    int channel_;
    int preventUpdateTrackColors_;
    string lastStringSent_;
    vector<rgba_color> currentTrackColors_;

public:
    virtual ~V1MDisplay_Midi_FeedbackProcessor() {}
    V1MDisplay_Midi_FeedbackProcessor(CSurfIntegrator* const csi, Midi_ControlSurface* surface, Widget* widget, int displayUpperLower, int displayType, int displayRow, int channel, int sysExByte1, int sysExByte2) : Midi_FeedbackProcessor(csi, surface, widget), offset_(displayUpperLower * 56), displayType_(displayType), displayRow_(displayRow), channel_(channel), sysExByte1_(sysExByte1), sysExByte2_(sysExByte2)
    {
        preventUpdateTrackColors_ = false;
        rgba_color color;
        for (int i = 0; i < surface_->GetNumChannels(); ++i)
            currentTrackColors_.push_back(color);
    }

    virtual const char* GetName() override { return "V1MDisplay_Midi_FeedbackProcessor"; }

    virtual void ForceClear() override
    {
        const PropertyList properties;
        ForceValue(properties, "");
    }

    virtual void SetValue(const PropertyList& properties, const char* const& inputText) override
    {
        if (strcmp(inputText, lastStringSent_.c_str())) // changes since last send
            ForceValue(properties, inputText);
    }

    virtual void ForceValue(const PropertyList& properties, const char* const& inputText) override
    {
        lastStringSent_ = inputText;

        char tmp[MEDBUF];
        const char* text = GetWidget()->GetSurface()->GetRestrictedLengthText(inputText, tmp, sizeof(tmp));

        if (!strcmp(text, "-150.00")) text = "-Inf";

        struct
        {
            MIDI_event_ex_t evt;
            char data[256];
        } midiSysExData;
        midiSysExData.evt.frame_offset = 0;
        midiSysExData.evt.size = 0;
        midiSysExData.evt.midi_message[midiSysExData.evt.size++] = 0xF0;
        midiSysExData.evt.midi_message[midiSysExData.evt.size++] = 0x00;

        midiSysExData.evt.midi_message[midiSysExData.evt.size++] = sysExByte1_;
        midiSysExData.evt.midi_message[midiSysExData.evt.size++] = sysExByte2_;
        midiSysExData.evt.midi_message[midiSysExData.evt.size++] = displayType_;
        midiSysExData.evt.midi_message[midiSysExData.evt.size++] = displayRow_;

        midiSysExData.evt.midi_message[midiSysExData.evt.size++] = channel_ * 7 + offset_;

        int cnt = 0;
        while (cnt++ < 7)
            midiSysExData.evt.midi_message[midiSysExData.evt.size++] = *text ? *text++ : ' ';

        midiSysExData.evt.midi_message[midiSysExData.evt.size++] = 0xF7;

        SendMidiSysExMessage(&midiSysExData.evt);

        ForceUpdateTrackColors(); // After updating text, update the track colors
    }

    //-----------Track Colour Code---------------
    virtual void ForceUpdateTrackColors() override
    {
        vector<rgba_color> trackColors;
        bool update = false;
        for (int i = 0; i < surface_->GetNumChannels(); ++i)
        {
            trackColors.push_back(surface_->GetTrackColorForChannel(i)); //get the track colours from reaper

            if (currentTrackColors_[i] != trackColors[i])
            {
                update = true;
                currentTrackColors_[i] = trackColors[i];
            }
        }

        if (preventUpdateTrackColors_ || update == false)
            return; //NO CHANGE TO COLORS SO RETURN
        else
            DoTheColourChange(trackColors);
    }

    int adjustTo7bit(int value) // Adjust RGB to 7-bit range as required by MIDI SysEx
    {
        // Convert from 8-bit to 7-bit (0-127)
        return (value >> 1) & 0x7F;
    }

    int adjustBlueValue(int blue, int green) // Apply the blue color correction as mentioned in the specs
    {
        float greenNormalized = green / 127.0f; // Normalize green to 0-1 range

        // Apply the correction formula: Blue = Blue * (0.70 + (0.30 * (Green / 128)))
        float blueAdjusted = blue * (0.70f + (0.30f * greenNormalized));

        // Ensure the value stays within 0-127 range
        int result = static_cast<int>(blueAdjusted);
        if (result < 0) result = 0;
        if (result > 127) result = 127;
        return result;
    }

    void DoTheColourChange(vector<rgba_color> trackColors)
    {
        //Continue with ForceUpdateTrackColors()
        struct
        {
            MIDI_event_ex_t evt;
            char data[256]; //this allocates the reuired data
        } midiSysExData;
        midiSysExData.evt.frame_offset = 0;
        midiSysExData.evt.size = 0;

        // iCON V1-M specific SysEx header for colors
        midiSysExData.evt.midi_message[midiSysExData.evt.size++] = 0xF0; // Start SysEx
        midiSysExData.evt.midi_message[midiSysExData.evt.size++] = 0x00; // Start of header
        midiSysExData.evt.midi_message[midiSysExData.evt.size++] = 0x02;
        midiSysExData.evt.midi_message[midiSysExData.evt.size++] = 0x4E;
        midiSysExData.evt.midi_message[midiSysExData.evt.size++] = 0x16;
        midiSysExData.evt.midi_message[midiSysExData.evt.size++] = 0x14; // End of header


        // Send all 8 channel colors at once (RGB triplets)
        for (int i = 0; i < trackColors.size(); ++i)
        {
            //update current colors because of change
            rgba_color color = trackColors[i];
            //currentTrackColors_[i] = color;

            // Convert from 8-bit to 7-bit values
            int r = adjustTo7bit(color.r);
            int g = adjustTo7bit(color.g);
            int b = adjustTo7bit(color.b);

            // Apply the blue correction for better color representation
            b = adjustBlueValue(b, g);

            // Add RGB values to SysEx message
            midiSysExData.evt.midi_message[midiSysExData.evt.size++] = r;
            midiSysExData.evt.midi_message[midiSysExData.evt.size++] = g;
            midiSysExData.evt.midi_message[midiSysExData.evt.size++] = b;

        }
        midiSysExData.evt.midi_message[midiSysExData.evt.size++] = 0xF7; // End SysEx

        SendMidiSysExMessage(&midiSysExData.evt);

    }

};


