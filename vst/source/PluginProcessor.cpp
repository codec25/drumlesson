#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cmath>

namespace
{
constexpr float twoPi = juce::MathConstants<float>::twoPi;

float midiVelocityToGain (float velocity)
{
    return juce::jlimit (0.05f, 1.0f, velocity);
}

const char* pieceParamId (DrumAcademyAudioProcessor::DrumPiece piece)
{
    using Piece = DrumAcademyAudioProcessor::DrumPiece;
    switch (piece)
    {
        case Piece::kick:      return "kickVol";
        case Piece::snare:     return "snareVol";
        case Piece::hatClosed: return "hatClosedVol";
        case Piece::hatOpen:   return "hatOpenVol";
        case Piece::hatPedal:  return "hatPedalVol";
        case Piece::tomLow:    return "tomLowVol";
        case Piece::tomMid:    return "tomMidVol";
        case Piece::tomHigh:   return "tomHighVol";
        case Piece::crash:     return "crashVol";
        case Piece::ride:      return "rideVol";
    }

    return "kickVol";
}

void addLevelParam (std::vector<std::unique_ptr<juce::RangedAudioParameter>>& params,
                    const juce::String& id,
                    const juce::String& name,
                    float defaultValue = 0.85f)
{
    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        id,
        name,
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f),
        defaultValue));
}

} // namespace

DrumAcademyAudioProcessor::DrumAcademyAudioProcessor()
    : AudioProcessor (BusesProperties().withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      apvts (*this, nullptr, "PARAMS", createParameterLayout())
{
}

DrumAcademyAudioProcessor::~DrumAcademyAudioProcessor() = default;

juce::AudioProcessorValueTreeState::ParameterLayout DrumAcademyAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back (std::make_unique<juce::AudioParameterChoice> (
        "mode",
        "Mode",
        juce::StringArray { "Free Play", "Training" },
        0));

    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        "bpm",
        "BPM",
        juce::NormalisableRange<float> (40.0f, 220.0f, 1.0f),
        100.0f));

    params.push_back (std::make_unique<juce::AudioParameterBool> ("metOn", "Metronome On", true));
    params.push_back (std::make_unique<juce::AudioParameterBool> ("backingOn", "Backing On", false));
    params.push_back (std::make_unique<juce::AudioParameterBool> ("autoFollow", "Auto Follow", true));
    params.push_back (std::make_unique<juce::AudioParameterBool> ("loopOn", "Loop On", false));

    params.push_back (std::make_unique<juce::AudioParameterInt> ("loopStart", "Loop Start Bar", 1, maxBars, 1));
    params.push_back (std::make_unique<juce::AudioParameterInt> ("loopEnd", "Loop End Bar", 1, maxBars, 2));

    addLevelParam (params, "masterVol", "Master", 0.9f);
    addLevelParam (params, "metVol", "Metronome", 0.55f);
    addLevelParam (params, "backingVol", "Backing", 0.55f);

    addLevelParam (params, "kickVol", "Kick", 0.9f);
    addLevelParam (params, "snareVol", "Snare", 0.9f);
    addLevelParam (params, "hatClosedVol", "Hat Closed", 0.8f);
    addLevelParam (params, "hatOpenVol", "Hat Open", 0.8f);
    addLevelParam (params, "hatPedalVol", "Hat Pedal", 0.7f);
    addLevelParam (params, "tomLowVol", "Tom Low", 0.85f);
    addLevelParam (params, "tomMidVol", "Tom Mid", 0.85f);
    addLevelParam (params, "tomHighVol", "Tom High", 0.85f);
    addLevelParam (params, "crashVol", "Crash", 0.8f);
    addLevelParam (params, "rideVol", "Ride", 0.8f);

    return { params.begin(), params.end() };
}

const juce::String DrumAcademyAudioProcessor::getName() const { return JucePlugin_Name; }
bool DrumAcademyAudioProcessor::acceptsMidi() const { return true; }
bool DrumAcademyAudioProcessor::producesMidi() const { return false; }
bool DrumAcademyAudioProcessor::isMidiEffect() const { return false; }
double DrumAcademyAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int DrumAcademyAudioProcessor::getNumPrograms() { return 1; }
int DrumAcademyAudioProcessor::getCurrentProgram() { return 0; }
void DrumAcademyAudioProcessor::setCurrentProgram (int) {}
const juce::String DrumAcademyAudioProcessor::getProgramName (int) { return {}; }
void DrumAcademyAudioProcessor::changeProgramName (int, const juce::String&) {}

void DrumAcademyAudioProcessor::prepareToPlay (double newSampleRate, int)
{
    sampleRateHz = static_cast<float> (juce::jmax (22050.0, newSampleRate));
    samplesUntilNextStep = getSamplesPerStep();
    currentStep = 0;
    hiHatPedalHeld = false;

    for (auto& voice : voices)
        voice.active = false;
}

void DrumAcademyAudioProcessor::releaseResources() {}

bool DrumAcademyAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    return layouts.getMainOutputChannelSet() == juce::AudioChannelSet::mono()
        || layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo();
}

void DrumAcademyAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    buffer.clear();

    for (const auto metadata : midiMessages)
        handleMidiMessage (metadata.getMessage());

    const bool backingOn = apvts.getRawParameterValue ("backingOn")->load() > 0.5f;
    const bool metronomeOn = apvts.getRawParameterValue ("metOn")->load() > 0.5f;

    if (backingOn || metronomeOn)
    {
        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            samplesUntilNextStep -= 1.0;
            if (samplesUntilNextStep <= 0.0)
            {
                triggerFromStep (currentStep);
                advanceSequencerStep();
                samplesUntilNextStep += getSamplesPerStep();
            }
        }
    }

    renderVoices (buffer, 0, buffer.getNumSamples());
}

void DrumAcademyAudioProcessor::handleMidiMessage (const juce::MidiMessage& message)
{
    if (message.isController() && message.getControllerNumber() == 4)
    {
        hiHatPedalHeld = message.getControllerValue() >= 64;
        return;
    }

    if (! message.isNoteOn())
        return;

    const auto note = message.getNoteNumber();
    const auto velocity = static_cast<float> (message.getVelocity());

    switch (note)
    {
        case 36: triggerPiece (DrumPiece::kick, velocity); break;
        case 38:
        case 40: triggerPiece (DrumPiece::snare, velocity); break;
        case 42: triggerPiece (DrumPiece::hatClosed, velocity); break;
        case 46: triggerPiece (hiHatPedalHeld ? DrumPiece::hatClosed : DrumPiece::hatOpen, velocity); break;
        case 44: triggerPiece (DrumPiece::hatPedal, velocity); break;
        case 45: triggerPiece (DrumPiece::tomLow, velocity); break;
        case 47:
        case 48: triggerPiece (DrumPiece::tomMid, velocity); break;
        case 50: triggerPiece (DrumPiece::tomHigh, velocity); break;
        case 49:
        case 57: triggerPiece (DrumPiece::crash, velocity); break;
        case 51:
        case 59: triggerPiece (DrumPiece::ride, velocity); break;
        default: break;
    }
}

void DrumAcademyAudioProcessor::triggerPiece (DrumPiece piece, float velocity)
{
    const auto master = apvts.getRawParameterValue ("masterVol")->load();
    const auto pieceGain = getPieceLevel (piece);
    const auto gain = midiVelocityToGain (velocity) * master * pieceGain;

    if (gain <= 0.0001f)
        return;

    const auto spec = getHitSpec (piece);

    auto* selected = &voices.front();
    for (auto& voice : voices)
    {
        if (! voice.active)
        {
            selected = &voice;
            break;
        }
    }

    selected->active = true;
    selected->phase = 0.0f;
    selected->frequency = spec.frequency;
    selected->amplitude = gain;
    selected->decay = spec.decay;
    selected->noiseMix = spec.noiseMix;
    selected->toneMix = spec.toneMix;
    selected->pitchDrop = spec.pitchDrop;
    selected->pitchDropRatio = spec.pitchDropRatio;
    selected->pan = spec.pan;
}

DrumAcademyAudioProcessor::HitSpec DrumAcademyAudioProcessor::getHitSpec (DrumPiece piece) const
{
    using Piece = DrumPiece;

    switch (piece)
    {
        case Piece::kick:      return { 65.0f, 0.9992f, 0.08f, 0.92f, true, 0.9987f, 0.5f };
        case Piece::snare:     return { 180.0f, 0.9975f, 0.75f, 0.25f, false, 0.9995f, 0.5f };
        case Piece::hatClosed: return { 8000.0f, 0.9925f, 0.95f, 0.05f, false, 0.9998f, 0.67f };
        case Piece::hatOpen:   return { 7400.0f, 0.9975f, 0.92f, 0.08f, false, 0.9998f, 0.68f };
        case Piece::hatPedal:  return { 6200.0f, 0.9935f, 0.95f, 0.05f, false, 0.9998f, 0.63f };
        case Piece::tomLow:    return { 120.0f, 0.9986f, 0.25f, 0.75f, true, 0.9990f, 0.34f };
        case Piece::tomMid:    return { 165.0f, 0.9985f, 0.24f, 0.76f, true, 0.9990f, 0.5f };
        case Piece::tomHigh:   return { 220.0f, 0.9983f, 0.22f, 0.78f, true, 0.9990f, 0.64f };
        case Piece::crash:     return { 5600.0f, 0.9991f, 0.94f, 0.06f, false, 0.9999f, 0.76f };
        case Piece::ride:      return { 5200.0f, 0.9989f, 0.88f, 0.12f, false, 0.9999f, 0.78f };
    }

    return {};
}

float DrumAcademyAudioProcessor::getPieceLevel (DrumPiece piece) const
{
    if (const auto* value = apvts.getRawParameterValue (pieceParamId (piece)))
        return value->load();

    return 0.8f;
}

void DrumAcademyAudioProcessor::triggerFromStep (int step)
{
    const int stepInBar = step % stepsPerBar;

    const bool metronomeOn = apvts.getRawParameterValue ("metOn")->load() > 0.5f;
    const bool backingOn = apvts.getRawParameterValue ("backingOn")->load() > 0.5f;

    if (metronomeOn && (stepInBar % 4 == 0))
    {
        const float metGain = apvts.getRawParameterValue ("metVol")->load();
        triggerPiece (stepInBar == 0 ? DrumPiece::crash : DrumPiece::hatClosed, 0.35f * metGain);
    }

    if (! backingOn)
        return;

    const float backingGain = apvts.getRawParameterValue ("backingVol")->load();

    const bool kickPattern = (stepInBar == 0 || stepInBar == 10);
    const bool snarePattern = (stepInBar == 4 || stepInBar == 12);
    const bool hatPattern = (stepInBar % 2 == 0);

    if (kickPattern) triggerPiece (DrumPiece::kick, 0.62f * backingGain);
    if (snarePattern) triggerPiece (DrumPiece::snare, 0.56f * backingGain);
    if (hatPattern) triggerPiece (DrumPiece::hatClosed, 0.35f * backingGain);
}

void DrumAcademyAudioProcessor::advanceSequencerStep()
{
    const bool loopOn = apvts.getRawParameterValue ("loopOn")->load() > 0.5f;

    if (! loopOn)
    {
        currentStep = (currentStep + 1) % totalSteps;
        return;
    }

    const int startStep = getLoopStartStep();
    const int endStep = getLoopEndStep();

    if (currentStep < startStep || currentStep >= endStep)
        currentStep = startStep;
    else
        ++currentStep;

    if (currentStep >= endStep)
        currentStep = startStep;
}

void DrumAcademyAudioProcessor::renderVoices (juce::AudioBuffer<float>& buffer, int startSample, int numSamples)
{
    auto* left = buffer.getWritePointer (0, startSample);
    auto* right = buffer.getNumChannels() > 1 ? buffer.getWritePointer (1, startSample) : nullptr;

    for (auto& voice : voices)
    {
        if (! voice.active)
            continue;

        for (int i = 0; i < numSamples; ++i)
        {
            const float noise = (random.nextFloat() * 2.0f - 1.0f);
            const float tone = std::sin (voice.phase);
            const float sample = (tone * voice.toneMix + noise * voice.noiseMix) * voice.amplitude;

            const float leftMix = sample * (1.0f - voice.pan * 0.45f);
            const float rightMix = sample * (0.55f + voice.pan * 0.45f);

            left[i] += leftMix;
            if (right != nullptr)
                right[i] += rightMix;

            voice.phase += twoPi * voice.frequency / sampleRateHz;
            if (voice.phase > twoPi)
                voice.phase -= twoPi;

            if (voice.pitchDrop)
                voice.frequency *= voice.pitchDropRatio;

            voice.amplitude *= voice.decay;
            if (voice.amplitude < 0.0001f)
            {
                voice.active = false;
                break;
            }
        }
    }

    for (int i = 0; i < numSamples; ++i)
    {
        left[i] = std::tanh (left[i] * 1.2f);
        if (right != nullptr)
            right[i] = std::tanh (right[i] * 1.2f);
    }
}

double DrumAcademyAudioProcessor::getSamplesPerStep() const
{
    const auto* bpmParam = apvts.getRawParameterValue ("bpm");
    const float bpm = bpmParam != nullptr ? bpmParam->load() : 100.0f;
    return (60.0 / juce::jlimit (40.0, 220.0, static_cast<double> (bpm))) * static_cast<double> (sampleRateHz) / 4.0;
}

int DrumAcademyAudioProcessor::getLoopStartStep() const
{
    auto startBar = static_cast<int> (apvts.getRawParameterValue ("loopStart")->load());
    startBar = juce::jlimit (1, maxBars, startBar);
    return (startBar - 1) * stepsPerBar;
}

int DrumAcademyAudioProcessor::getLoopEndStep() const
{
    auto startBar = static_cast<int> (apvts.getRawParameterValue ("loopStart")->load());
    auto endBar = static_cast<int> (apvts.getRawParameterValue ("loopEnd")->load());
    startBar = juce::jlimit (1, maxBars, startBar);
    endBar = juce::jlimit (startBar, maxBars, endBar);
    return endBar * stepsPerBar;
}

bool DrumAcademyAudioProcessor::hasEditor() const { return true; }
juce::AudioProcessorEditor* DrumAcademyAudioProcessor::createEditor() { return new DrumAcademyAudioProcessorEditor (*this); }

void DrumAcademyAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void DrumAcademyAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState != nullptr && xmlState->hasTagName (apvts.state.getType()))
        apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DrumAcademyAudioProcessor();
}
