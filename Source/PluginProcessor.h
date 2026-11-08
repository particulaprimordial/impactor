#pragma once
#include <JuceHeader.h>

class ParticulaImpactEngineAudioProcessor : public juce::AudioProcessor
{
public:
    ParticulaImpactEngineAudioProcessor();
    ~ParticulaImpactEngineAudioProcessor() override = default;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override {}
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "Particula Impact Engine"; }
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 3.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}

    void getStateInformation(juce::MemoryBlock&) override;
    void setStateInformation(const void*, int) override;

    juce::AudioProcessorValueTreeState parameters;

private:
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    double sr = 44100.0;
    double phase = 0.0;
    double subPhase = 0.0;
    double pitchAge = 1.0;
    double ampAge = 1.0;
    float currentVelocity = 0.0f;
    bool active = false;
    juce::Random random;

    float readParam(const char* id) const;
    float renderSample();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParticulaImpactEngineAudioProcessor)
};
