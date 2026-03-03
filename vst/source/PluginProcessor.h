#pragma once

#include <JuceHeader.h>

class DrumAcademyAudioProcessor : public juce::AudioProcessor
{
public:
    DrumAcademyAudioProcessor();
    ~DrumAcademyAudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState apvts;

    enum class DrumPiece
    {
        kick = 0,
        snare,
        hatClosed,
        hatOpen,
        hatPedal,
        tomLow,
        tomMid,
        tomHigh,
        crash,
        ride
    };

    void triggerPiece (DrumPiece piece, float velocity);

private:
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    struct DrumVoice
    {
        bool active = false;
        float phase = 0.0f;
        float frequency = 200.0f;
        float amplitude = 0.0f;
        float decay = 0.999f;
        float noiseMix = 0.0f;
        float toneMix = 1.0f;
        bool pitchDrop = false;
        float pitchDropRatio = 0.9995f;
        float pan = 0.5f;
    };

    struct HitSpec
    {
        float frequency = 200.0f;
        float decay = 0.999f;
        float noiseMix = 0.3f;
        float toneMix = 0.7f;
        bool pitchDrop = false;
        float pitchDropRatio = 0.9995f;
        float pan = 0.5f;
    };

    float sampleRateHz = 44100.0f;
    double samplesUntilNextStep = 0.0;
    int currentStep = 0;
    bool hiHatPedalHeld = false;

    std::array<DrumVoice, 32> voices {};
    juce::Random random;

    static constexpr int stepsPerBar = 16;
    static constexpr int maxBars = 8;
    static constexpr int totalSteps = stepsPerBar * maxBars;

    double getSamplesPerStep() const;
    HitSpec getHitSpec (DrumPiece piece) const;
    float getPieceLevel (DrumPiece piece) const;
    void triggerFromStep (int step);
    void advanceSequencerStep();
    void renderVoices (juce::AudioBuffer<float>& buffer, int startSample, int numSamples);
    void handleMidiMessage (const juce::MidiMessage& message);
    int getLoopStartStep() const;
    int getLoopEndStep() const;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DrumAcademyAudioProcessor)
};
