#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class ParticulaImpactEngineAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit ParticulaImpactEngineAudioProcessorEditor(ParticulaImpactEngineAudioProcessor&);
    ~ParticulaImpactEngineAudioProcessorEditor() override = default;
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    ParticulaImpactEngineAudioProcessor& processor;
    struct Control
    {
        juce::Slider slider;
        juce::Label label;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachment;
    };
    std::vector<std::unique_ptr<Control>> controls;
    void addControl(const juce::String& id, const juce::String& label);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParticulaImpactEngineAudioProcessorEditor)
};
