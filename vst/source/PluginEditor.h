#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class DrumAcademyAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit DrumAcademyAudioProcessorEditor (DrumAcademyAudioProcessor&);
    ~DrumAcademyAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    DrumAcademyAudioProcessor& audioProcessor;

    juce::Label titleLabel;
    juce::Label authorLabel;

    juce::ComboBox modeBox;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> modeAttachment;

    juce::Slider bpmSlider;
    juce::Label bpmLabel;

    juce::ToggleButton metOnButton;
    juce::ToggleButton backingOnButton;
    juce::ToggleButton autoFollowButton;
    juce::ToggleButton loopOnButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> metOnAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> backingOnAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> autoFollowAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> loopOnAttachment;

    juce::Slider loopStartSlider;
    juce::Slider loopEndSlider;
    juce::Label loopStartLabel;
    juce::Label loopEndLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> loopStartAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> loopEndAttachment;

    juce::Slider masterSlider;
    juce::Slider metVolSlider;
    juce::Slider backingVolSlider;
    juce::Label masterLabel;
    juce::Label metVolLabel;
    juce::Label backingVolLabel;

    juce::Slider kickSlider;
    juce::Slider snareSlider;
    juce::Slider hatClosedSlider;
    juce::Slider hatOpenSlider;
    juce::Slider hatPedalSlider;
    juce::Slider tomLowSlider;
    juce::Slider tomMidSlider;
    juce::Slider tomHighSlider;
    juce::Slider crashSlider;
    juce::Slider rideSlider;

    juce::Label kickLabel;
    juce::Label snareLabel;
    juce::Label hatClosedLabel;
    juce::Label hatOpenLabel;
    juce::Label hatPedalLabel;
    juce::Label tomLowLabel;
    juce::Label tomMidLabel;
    juce::Label tomHighLabel;
    juce::Label crashLabel;
    juce::Label rideLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bpmAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> masterAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> metVolAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> backingVolAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> kickAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> snareAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> hatClosedAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> hatOpenAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> hatPedalAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> tomLowAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> tomMidAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> tomHighAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> crashAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rideAttachment;

    void styleSlider (juce::Slider& slider);
    void styleLabel (juce::Label& label, const juce::String& text);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DrumAcademyAudioProcessorEditor)
};
