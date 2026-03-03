#include "PluginEditor.h"

DrumAcademyAudioProcessorEditor::DrumAcademyAudioProcessorEditor (DrumAcademyAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (980, 640);

    titleLabel.setText ("Drum Academy VST", juce::dontSendNotification);
    titleLabel.setJustificationType (juce::Justification::centredLeft);
    titleLabel.setFont (juce::Font (22.0f, juce::Font::bold));
    addAndMakeVisible (titleLabel);

    authorLabel.setText ("by Moise T. Sahouo", juce::dontSendNotification);
    authorLabel.setJustificationType (juce::Justification::centredLeft);
    authorLabel.setColour (juce::Label::textColourId, juce::Colours::white.withAlpha (0.78f));
    addAndMakeVisible (authorLabel);

    modeBox.addItemList (juce::StringArray { "Free Play", "Training" }, 1);
    addAndMakeVisible (modeBox);
    modeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (audioProcessor.apvts, "mode", modeBox);

    styleLabel (bpmLabel, "BPM");
    styleSlider (bpmSlider);
    bpmSlider.setRange (40.0, 220.0, 1.0);
    addAndMakeVisible (bpmSlider);

    metOnButton.setButtonText ("Metronome");
    backingOnButton.setButtonText ("Backing");
    autoFollowButton.setButtonText ("Auto Follow");
    loopOnButton.setButtonText ("Loop");
    addAndMakeVisible (metOnButton);
    addAndMakeVisible (backingOnButton);
    addAndMakeVisible (autoFollowButton);
    addAndMakeVisible (loopOnButton);

    styleLabel (loopStartLabel, "Loop Start");
    styleLabel (loopEndLabel, "Loop End");
    styleSlider (loopStartSlider);
    styleSlider (loopEndSlider);
    loopStartSlider.setRange (1.0, 8.0, 1.0);
    loopEndSlider.setRange (1.0, 8.0, 1.0);
    addAndMakeVisible (loopStartSlider);
    addAndMakeVisible (loopEndSlider);

    styleLabel (masterLabel, "Master");
    styleLabel (metVolLabel, "Metronome");
    styleLabel (backingVolLabel, "Backing");
    styleSlider (masterSlider);
    styleSlider (metVolSlider);
    styleSlider (backingVolSlider);
    addAndMakeVisible (masterSlider);
    addAndMakeVisible (metVolSlider);
    addAndMakeVisible (backingVolSlider);

    styleLabel (kickLabel, "Kick");
    styleLabel (snareLabel, "Snare");
    styleLabel (hatClosedLabel, "Hat Closed");
    styleLabel (hatOpenLabel, "Hat Open");
    styleLabel (hatPedalLabel, "Hat Pedal");
    styleLabel (tomLowLabel, "Tom Low");
    styleLabel (tomMidLabel, "Tom Mid");
    styleLabel (tomHighLabel, "Tom High");
    styleLabel (crashLabel, "Crash");
    styleLabel (rideLabel, "Ride");

    styleSlider (kickSlider);
    styleSlider (snareSlider);
    styleSlider (hatClosedSlider);
    styleSlider (hatOpenSlider);
    styleSlider (hatPedalSlider);
    styleSlider (tomLowSlider);
    styleSlider (tomMidSlider);
    styleSlider (tomHighSlider);
    styleSlider (crashSlider);
    styleSlider (rideSlider);

    addAndMakeVisible (kickSlider);
    addAndMakeVisible (snareSlider);
    addAndMakeVisible (hatClosedSlider);
    addAndMakeVisible (hatOpenSlider);
    addAndMakeVisible (hatPedalSlider);
    addAndMakeVisible (tomLowSlider);
    addAndMakeVisible (tomMidSlider);
    addAndMakeVisible (tomHighSlider);
    addAndMakeVisible (crashSlider);
    addAndMakeVisible (rideSlider);

    bpmAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "bpm", bpmSlider);
    metOnAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (audioProcessor.apvts, "metOn", metOnButton);
    backingOnAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (audioProcessor.apvts, "backingOn", backingOnButton);
    autoFollowAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (audioProcessor.apvts, "autoFollow", autoFollowButton);
    loopOnAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (audioProcessor.apvts, "loopOn", loopOnButton);
    loopStartAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "loopStart", loopStartSlider);
    loopEndAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "loopEnd", loopEndSlider);

    masterAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "masterVol", masterSlider);
    metVolAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "metVol", metVolSlider);
    backingVolAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "backingVol", backingVolSlider);

    kickAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "kickVol", kickSlider);
    snareAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "snareVol", snareSlider);
    hatClosedAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "hatClosedVol", hatClosedSlider);
    hatOpenAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "hatOpenVol", hatOpenSlider);
    hatPedalAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "hatPedalVol", hatPedalSlider);
    tomLowAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "tomLowVol", tomLowSlider);
    tomMidAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "tomMidVol", tomMidSlider);
    tomHighAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "tomHighVol", tomHighSlider);
    crashAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "crashVol", crashSlider);
    rideAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "rideVol", rideSlider);
}

DrumAcademyAudioProcessorEditor::~DrumAcademyAudioProcessorEditor() = default;

void DrumAcademyAudioProcessorEditor::styleSlider (juce::Slider& slider)
{
    slider.setSliderStyle (juce::Slider::LinearHorizontal);
    slider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 56, 20);
}

void DrumAcademyAudioProcessorEditor::styleLabel (juce::Label& label, const juce::String& text)
{
    label.setText (text, juce::dontSendNotification);
    label.setColour (juce::Label::textColourId, juce::Colours::white.withAlpha (0.86f));
    label.setJustificationType (juce::Justification::centredLeft);
    addAndMakeVisible (label);
}

void DrumAcademyAudioProcessorEditor::paint (juce::Graphics& g)
{
    const auto bgTop = juce::Colour::fromRGB (8, 19, 43);
    const auto bgBottom = juce::Colour::fromRGB (6, 10, 26);

    g.setGradientFill ({ bgTop, 0.0f, 0.0f, bgBottom, 0.0f, static_cast<float> (getHeight()), false });
    g.fillAll();

    g.setColour (juce::Colours::white.withAlpha (0.10f));
    g.fillRoundedRectangle (16.0f, 76.0f, static_cast<float> (getWidth() - 32), static_cast<float> (getHeight() - 92), 12.0f);
}

void DrumAcademyAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced (16);
    auto header = area.removeFromTop (52);

    titleLabel.setBounds (header.removeFromTop (28));
    authorLabel.setBounds (header);

    area.removeFromTop (8);

    auto topRow = area.removeFromTop (120);
    auto leftTop = topRow.removeFromLeft (420);
    modeBox.setBounds (leftTop.removeFromTop (28));
    leftTop.removeFromTop (8);
    bpmLabel.setBounds (leftTop.removeFromTop (20));
    bpmSlider.setBounds (leftTop.removeFromTop (30));

    auto rightTop = topRow;
    auto buttonRow = rightTop.removeFromTop (28);
    metOnButton.setBounds (buttonRow.removeFromLeft (100));
    buttonRow.removeFromLeft (8);
    backingOnButton.setBounds (buttonRow.removeFromLeft (92));
    buttonRow.removeFromLeft (8);
    autoFollowButton.setBounds (buttonRow.removeFromLeft (110));
    buttonRow.removeFromLeft (8);
    loopOnButton.setBounds (buttonRow.removeFromLeft (80));

    rightTop.removeFromTop (12);
    auto loopRow = rightTop.removeFromTop (58);
    auto startZone = loopRow.removeFromLeft (210);
    loopStartLabel.setBounds (startZone.removeFromTop (20));
    loopStartSlider.setBounds (startZone.removeFromTop (28));

    loopRow.removeFromLeft (14);
    auto endZone = loopRow.removeFromLeft (210);
    loopEndLabel.setBounds (endZone.removeFromTop (20));
    loopEndSlider.setBounds (endZone.removeFromTop (28));

    area.removeFromTop (12);

    auto mixRow = area.removeFromTop (94);
    auto m1 = mixRow.removeFromLeft (300);
    masterLabel.setBounds (m1.removeFromTop (20));
    masterSlider.setBounds (m1.removeFromTop (30));

    mixRow.removeFromLeft (12);
    auto m2 = mixRow.removeFromLeft (300);
    metVolLabel.setBounds (m2.removeFromTop (20));
    metVolSlider.setBounds (m2.removeFromTop (30));

    mixRow.removeFromLeft (12);
    backingVolLabel.setBounds (mixRow.removeFromTop (20));
    backingVolSlider.setBounds (mixRow.removeFromTop (30));

    area.removeFromTop (12);

    auto row1 = area.removeFromTop (66);
    auto row2 = area.removeFromTop (66);
    auto row3 = area.removeFromTop (66);
    auto row4 = area.removeFromTop (66);

    auto bindRow = [] (juce::Rectangle<int> row,
                       juce::Label& l1, juce::Slider& s1,
                       juce::Label& l2, juce::Slider& s2,
                       juce::Label& l3, juce::Slider& s3)
    {
        auto c1 = row.removeFromLeft (320);
        auto c2 = row.removeFromLeft (320);
        auto c3 = row;

        l1.setBounds (c1.removeFromTop (18));
        s1.setBounds (c1.removeFromTop (28));
        l2.setBounds (c2.removeFromTop (18));
        s2.setBounds (c2.removeFromTop (28));
        l3.setBounds (c3.removeFromTop (18));
        s3.setBounds (c3.removeFromTop (28));
    };

    bindRow (row1, kickLabel, kickSlider, snareLabel, snareSlider, hatClosedLabel, hatClosedSlider);
    bindRow (row2, hatOpenLabel, hatOpenSlider, hatPedalLabel, hatPedalSlider, tomLowLabel, tomLowSlider);
    bindRow (row3, tomMidLabel, tomMidSlider, tomHighLabel, tomHighSlider, crashLabel, crashSlider);

    auto c1 = row4.removeFromLeft (320);
    rideLabel.setBounds (c1.removeFromTop (18));
    rideSlider.setBounds (c1.removeFromTop (28));
}
